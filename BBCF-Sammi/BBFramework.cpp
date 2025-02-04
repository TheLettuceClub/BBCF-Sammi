#include "pch.h"
#include "BB.h"
#include "BBFramework.hpp"
#include "SAMMITypes.hpp"
#include "WebSockets.hpp"

BBFramework* BBFramework::instance = nullptr;
std::mutex BBFramework::mtx{};
using json = nlohmann::json;

HMODULE base;
unsigned long int frameCounter = 0;
std::thread messageHandler;
worker_t worker{};
StateUpdate state{};
bool bRoundStarted = false;
int* pGameState{};
int* pGameMode{};
int* pMatchState{};
int* pMatchTimer{};
int* pMatchRounds{};

static void initalizeWSServer() {
	worker.thread = std::make_shared<std::thread>([]() {
		/* create uWebSocket worker and capture uWS::Loop, uWS::App objects. */
		worker.work();
		});
	std::cout << "BBCF-Sammi: WS: server started" << std::endl;
}

static void sendEvent(std::string eventName, std::string customData) {
	std::string body = std::format("{{\"event\": \"{}\", \"eventInfo\": {}}}", eventName, customData);
	//send to all opened clients
	worker.loop->defer([body]() {
		worker.app->publish("broadcast", body, uWS::TEXT);
		});
}

// function hooks here
static void hook_CreateObject(SafetyHookContext& ctx) {
	// should be called when object created. ebx/ecx here is ptr to object TODO: expand!
	const Battle_CObject* obj = reinterpret_cast<Battle_CObject*>(ctx.ebx);
	CreateObject co{};
	co.frameCount = frameCounter;
	co.currAction = &obj->currAction[0];
	co.sprite = &obj->sprite[0];
	json j = co;
	std::thread(sendEvent, "bbcf_objectCreatedEvent", j.dump()).detach();
}

static void hook_FrameStep(SafetyHookContext& ctx) {
	// runs once per frame, use as time reference
	state.frameCount = frameCounter;
	state.gameMode = *pGameMode;
	state.gameState = *pGameState;
	state.inGameTimer = (*pMatchTimer)/60; // to convert frames to seconds
	state.matchState = *pMatchState;
	json j = state;
	std::thread(sendEvent, "bbcf_stateUpdate", j.dump()).detach();
	frameCounter++;
}

static void hook_AttackHit(SafetyHookContext& ctx) {
	// called on hit and block. eax is ptr attacker, esi ptr defender. also called for projectiles
	const Battle_CObject* attacker = reinterpret_cast<Battle_CObject*>(ctx.eax);
	const Battle_CObject* defender = reinterpret_cast<Battle_CObject*>(ctx.esi);
	HitEvent he{};
	he.frameCount = frameCounter;
	he.attacker = attacker->side ? "Player 2" : "Player 1";
	he.airPushbackX = defender->airPushbackX;
	he.airPushbackY = defender->airPushbackY;
	he.attackerAction = &attacker->currAction[0];
	he.attackFlag = attacker->attackFlag;
	he.attackLevel = attacker->attackLevel;
	he.damage = defender->incomingDamage; // actually the damage of the whole combo, not individual move damage
	he.defender = defender->side ? "Player 2" : "Player 1";
	he.defenderAction = &defender->currAction[0];
	he.defenderPrevAction = &defender->prevAction[0];
	he.hitstopOverride = attacker->hitstopOverride;
	he.untechTime = attacker->untechTime;
	he.scalingTicks = defender->incomingScalingTicks; // ticks of combo scaling, i'm sure dustloop has info about this
	json j = he;
	std::thread(sendEvent, "bbcf_hitEvent", j.dump()).detach();
}

static void roundStart() {
	// called when either player has "entry" in their action name. likely called twice even if intros skipped so it still locks
	if (!bRoundStarted) {
		frameCounter = 0;
		RoundStart rs{};
		rs.rounds = *pMatchRounds;
		rs.frameCount = frameCounter;
		json j = rs;
		std::thread(sendEvent, "bbcf_roundStartEvent", j.dump()).detach();
		bRoundStarted = true;
	}
}

static void roundEnd(const Battle_CObject* winner) {
	// called whenever either player has "CmnAct???Win" in their action name
	RoundEnd re{};
	re.winner = winner->side ? "Player 2" : "Player 1";
	re.winType = (*pMatchTimer) ? "KO" : "Timeout";
	re.frameCount = frameCounter;
	json j = re;
	std::thread(sendEvent, "bbcf_roundEndEvent", j.dump()).detach();
	bRoundStarted = false;
}

static void hook_SpriteUpdate(SafetyHookContext& ctx) {
	// called every sprite update, ebx/ecx hold ptr to BCOM of updating character
	const Battle_CObject* updater = reinterpret_cast<Battle_CObject*>(ctx.ebx);
	// detect round start and end via stringchecks
	std::string currAct = &updater->currAction[0];
	if (currAct.contains("CmnActEntry")) { // might filter out special intros, idk
		roundStart();
	}
	if (currAct.contains("CmnAct") && currAct.contains("Win")) { // should match against "CmnActRoundWin", "CmnActMatchWin" and other variants
		roundEnd(updater); //TODO: likely doesn't work on double ko!
	}
	// determine which player is being updated and update their slot in the StateUpdate
	PlayerState* ps;
	if (updater->side == 0) {
		ps = &state.p1;
	}
	else if (updater->side == 1) {
		ps = &state.p2;
	}
	else {
		return;
	}
	ps->barrierGauge = updater->barrierGauge;
	ps->character = std::string(&updater->sprite[0]).substr(0, 2);
	ps->currAction = currAct;
	ps->drive = updater->drive;
	ps->health = updater->health;
	ps->heat = updater->heat;
	ps->maxDrive = updater->maxDrive;
	ps->Overdrive = updater->OverdriveAmnt;
	ps->posx = updater->posx;
	ps->prevAction = &updater->prevAction[0];
	ps->side = updater->side;
}

void hook_TitleScreen(SafetyHookContext& ctx) {
	// here edi+0x108 is the game mode, get a ptr to it. edi+0x10C is game state, also ptr it.
	pGameState = (int*)(ctx.edi + 0x10C);
	pGameMode = (int*)(ctx.edi + 0x108);
}

void hook_Timeout(SafetyHookContext& ctx) {
	// called by various functions that generally signal that a running game is over
	std::thread(sendEvent, "bbcf_Timeout", "{}").detach();
}

void hook_MatchVars(SafetyHookContext& ctx) {
	// here ecx+0x30 is match state, +0x18 is match timer, +0x4 is match rounds
	pMatchState = (int*)(ctx.ecx + 0x30);
	pMatchRounds = (int*)(ctx.ecx + 0x4);
	pMatchTimer = (int*)(ctx.ecx + 0x18);
}


auto BBFramework::initalize() -> void {
	base = GetModuleHandle(NULL);
	// set function and data hooks here
	CreateObject_Hook = safetyhook::create_mid(reinterpret_cast<uintptr_t>(base) + 0x191720, hook_CreateObject);
	FrameStep_Hook = safetyhook::create_mid(reinterpret_cast<uintptr_t>(base) + 0x16B01B, hook_FrameStep);
	AttackHit_Hook = safetyhook::create_mid(reinterpret_cast<uintptr_t>(base) + 0x18C3C0, hook_AttackHit);
	//RoundStart_Hook = safetyhook::create_mid(reinterpret_cast<uintptr_t>(base) + 0x1584EF, hook_RoundStart);
	//RoundEnd_Hook = safetyhook::create_mid(reinterpret_cast<uintptr_t>(base) + 0x1C83C4, hook_RoundEnd);
	SpriteUpdate_Hook = safetyhook::create_mid(reinterpret_cast<uintptr_t>(base) + 0x182655, hook_SpriteUpdate);
	TitleScreen_Hook = safetyhook::create_mid(reinterpret_cast<uintptr_t>(base) + 0x1F9AEE, hook_TitleScreen);
	MenuScreen_Hook = safetyhook::create_mid(reinterpret_cast<uintptr_t>(base) + 0x32DD33, hook_Timeout);
	LobbyState_Hook = safetyhook::create_mid(reinterpret_cast<uintptr_t>(base) + 0x34B811, hook_Timeout);
	MatchVariables_Hook = safetyhook::create_mid(reinterpret_cast<uintptr_t>(base) + 0x15F67F, hook_MatchVars);
}

auto BBFramework::get_instance() -> BBFramework*
{
	if (instance == nullptr)
	{
		std::lock_guard lock(mtx);
		if (instance == nullptr)
		{
			instance = new BBFramework();
			initalizeWSServer();
		}
	}
	return instance;
}