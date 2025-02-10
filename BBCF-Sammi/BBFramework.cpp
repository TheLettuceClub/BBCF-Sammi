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
bool bRoundEnded = false;
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
	const BATTLE_CObjectManager* obj = reinterpret_cast<BATTLE_CObjectManager*>(ctx.ebx);
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

static void hook_AttackHitCheck(SafetyHookContext& ctx) {
	// called on hit and block but damage isn't there so just use for block. called before AttackHit on hit
	// eax is ptr attacker, esi ptr defender. also called for projectiles
	// if defender's stateflag2 and stateflag4 are set, they're blocking, else do nothing
	const BATTLE_CObjectManager* attacker = reinterpret_cast<BATTLE_CObjectManager*>(ctx.eax);
	const BATTLE_CObjectManager* defender = reinterpret_cast<BATTLE_CObjectManager*>(ctx.esi);
	if (defender->stateFlag2) {
		GuardEvent ge{};
		ge.frameCount = frameCounter;
		ge.attacker = attacker->side ? "Player 2" : "Player 1";
		ge.defender = defender->side ? "Player 2" : "Player 1";
		ge.attackerAction = &attacker->currAction[0];
		ge.attackLevel = attacker->attackLevel;
		ge.chipDamage = defender->health - defender->prevHealth;
		ge.defenderAction = &defender->currAction[0];
		ge.blockDir = getBlockDir(defender->stateFlag, defender->stateFlag4);
		ge.blockMethod = getBlockMeth(defender->stateFlag4);
		ge.moveType = attacker->moveType;
		ge.attackerActiveFlow = attacker->activeFlow;
		ge.defenderActiveFlow = defender->activeFlow;
		json j = ge;
		std::thread(sendEvent, "bbcf_guardEvent", j.dump()).detach();
	}
}

static void hook_AttackHit(SafetyHookContext& ctx) {
	// called on hit ONLY. ebx is ptr defender, edi ptr attacker. also called for projectiles
	const BATTLE_CObjectManager* attacker = reinterpret_cast<BATTLE_CObjectManager*>(ctx.edi);
	const BATTLE_CObjectManager* defender = reinterpret_cast<BATTLE_CObjectManager*>(ctx.ebx);
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
	he.comboCount = defender->incomingComboCount;
	he.moveType = attacker->moveType;
	he.attackerActiveFlow = attacker->activeFlow;
	he.defenderActiveFlow = defender->activeFlow;
	json j = he;
	std::thread(sendEvent, "bbcf_hitEvent", j.dump()).detach();
}

static void hook_RoundTransition(SafetyHookContext& ctx) {
	// here ebx/ecx is ptr to one of player structs
	RoundTransition rt{};
	rt.frameCount = frameCounter;
	rt.p1Act = state.p1.currAction;
	rt.p2Act = state.p2.currAction;
	rt.p1Health = state.p1.health;
	rt.p2Health = state.p2.health;
	if (rt.p1Act == "" && rt.p2Act == "") {
		rt.likelyNext = "RoundStartNew";
	}
	if ((rt.p1Act.contains("RoundWin") || rt.p2Act.contains("RoundWin")) && (rt.p1Act.contains("DownLoop") || rt.p2Act.contains("DownLoop"))) {
		rt.likelyNext = "RoundEndIntoStart";
	}
	if (rt.p1Act.contains("DownLoop") && rt.p2Act.contains("DownLoop")) {
		rt.likelyNext = "RoundEndByDoubleKO";
	}
	if (rt.p1Act.contains("MatchWin") || rt.p2Act.contains("MatchWin")) {
		rt.likelyNext = "RoundEndByKO";
	}
	if (rt.p1Act.contains("CmnActLose") || rt.p2Act.contains("CmnActLose")) {
		rt.likelyNext = "RoundEndByTimeout";
	}
	json j = rt;
	std::thread(sendEvent, "bbcf_roundTransitionEvent", j.dump()).detach();
	frameCounter = 0;
}

static void hook_SpriteUpdate(SafetyHookContext& ctx) {
	// called every sprite update, ebx/ecx hold ptr to BCOM of updating character
	const BATTLE_CObjectManager* updater = reinterpret_cast<BATTLE_CObjectManager*>(ctx.ebx);
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
	ps->character = std::string(&updater->sprite[0]).substr(0,2);
	ps->currAction = &updater->currAction[0];
	ps->drive = updater->drive; //TODO: expand?
	ps->health = updater->health;
	ps->heat = updater->heat;
	ps->maxDrive = updater->maxDrive;
	ps->Overdrive = updater->OverdriveAmnt;
	ps->posx = updater->posx;
	ps->prevAction = &updater->prevAction[0];
	ps->side = updater->side;
	ps->ODTimeRemaining = updater->overdriveTimeleft;
	ps->posy = updater->posy;
	ps->moveType = updater->moveType;
	ps->activeFlow = updater->activeFlow;
}

static void hook_TitleScreen(SafetyHookContext& ctx) {
	// here edi+0x108 is the game mode, get a ptr to it. edi+0x10C is game state, also ptr it.
	pGameState = (int*)(ctx.edi + 0x10C);
	pGameMode = (int*)(ctx.edi + 0x108);
}

static void hook_Timeout(SafetyHookContext& ctx) {
	// called by various functions that generally signal that a running game is over
	state.p1.currAction = "";
	state.p2.currAction = ""; // stupid stinky hack to get transition detector working right
	std::thread(sendEvent, "bbcf_Timeout", "{}").detach();
}

static void hook_MatchVars(SafetyHookContext& ctx) {
	// here ecx+0x30 is match state, +0x18 is match timer, +0x4 is match rounds
	pMatchState = (int*)(ctx.ecx + 0x30);
	pMatchRounds = (int*)(ctx.ecx + 0x4);
	pMatchTimer = (int*)(ctx.ecx + 0x18);
}


auto BBFramework::initalize() -> void {
	base = GetModuleHandle(NULL);
	// set function and data hooks here
	CreateObject_Hook = safetyhook::create_mid(reinterpret_cast<uintptr_t>(base) + 0x191720, hook_CreateObject); //func: 0x191720
	FrameStep_Hook = safetyhook::create_mid(reinterpret_cast<uintptr_t>(base) + 0x16B01B, hook_FrameStep); //func: 16aff0
	AttackHitCheck_Hook = safetyhook::create_mid(reinterpret_cast<uintptr_t>(base) + 0x18C3C0, hook_AttackHitCheck); //func: 18c377
	AttackHit_Hook = safetyhook::create_mid(reinterpret_cast<uintptr_t>(base) + 0x1C1D93, hook_AttackHit); //func: 0x1c17f0
	//RoundStart_Hook = safetyhook::create_mid(reinterpret_cast<uintptr_t>(base) + 0x1584EF, hook_RoundStart);
	RoundTransition_Hook = safetyhook::create_mid(reinterpret_cast<uintptr_t>(base) + 0x1C83C4, hook_RoundTransition); //func: 1c7fa2
	SpriteUpdate_Hook = safetyhook::create_mid(reinterpret_cast<uintptr_t>(base) + 0x182655, hook_SpriteUpdate); //func: 182620
	TitleScreen_Hook = safetyhook::create_mid(reinterpret_cast<uintptr_t>(base) + 0x1F9AEE, hook_TitleScreen); //func: 1f9a40
	MenuScreen_Hook = safetyhook::create_mid(reinterpret_cast<uintptr_t>(base) + 0x32DD33, hook_Timeout); //func: 32dd10
	LobbyState_Hook = safetyhook::create_mid(reinterpret_cast<uintptr_t>(base) + 0x34B811, hook_Timeout); //func: 34b7f0
	MatchVariables_Hook = safetyhook::create_mid(reinterpret_cast<uintptr_t>(base) + 0x15F67F, hook_MatchVars); //func: 15f5c0
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