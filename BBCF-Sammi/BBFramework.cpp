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
	// should be called when object created. ebx/ecx here is ptr to object?? TODO: expand!
	CreateObject co{};
	co.frameCount = frameCounter;
	json j = co;
	std::thread(sendEvent, "bbcf_objectCreatedEvent", j.dump()).detach();
}

static void hook_FrameStep(SafetyHookContext& ctx) {
	// runs once per frame, use as time reference
	state.frameCount = frameCounter;
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
	he.damage = attacker->damageScaled; // this is VERY inconsistent! For p1, it only updates to the new scaled value after! the first hit, though it usually work fine in a combo. It does not work at all for p2!!!! TODO: find a new value
	he.defender = defender->side ? "Player 2" : "Player 1";
	he.defenderAction = &defender->currAction[0];
	he.defenderPrevAction = &defender->prevAction[0];
	he.hitstopOverride = attacker->hitstopOverride;
	he.untechTime = attacker->untechTime;
	json j = he;
	std::thread(sendEvent, "bbcf_hitEvent", j.dump()).detach();
}

static void hook_RoundStart(SafetyHookContext& ctx) {
	// called multiple times per real round start, locked out after first, cancelled by round end
	if (!bRoundStarted) {
		frameCounter = 0;
		RoundStart rs{};
		rs.frameCount = frameCounter;
		json j = rs;
		std::thread(sendEvent, "bbcf_roundStartEvent", j.dump()).detach();
		bRoundStarted = true;
		bRoundEnded = false;
	}
}

static void hook_RoundEnd(SafetyHookContext& ctx) {
	// runs twice on every round end, incl skipped outros. locked out after first, cancelled by round end
	if (!bRoundEnded) {
		RoundEnd re{};
		re.frameCount = frameCounter;
		json j = re;
		std::thread(sendEvent, "bbcf_roundEndEvent", j.dump()).detach();
		bRoundEnded = true;
		bRoundStarted = false;
	}
}

static void hook_SpriteUpdate(SafetyHookContext& ctx) {
	// called every sprite update, ebx/ecx hold ptr to BCOM of updating character
	// use to update external data thing, mutex
	const Battle_CObject* updater = reinterpret_cast<Battle_CObject*>(ctx.ebx);
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
	ps->currAction = &updater->currAction[0];
	ps->drive = updater->drive;
	ps->health = updater->health;
	ps->heat = updater->heat;
	ps->maxDrive = updater->maxDrive;
	ps->Overdrive = updater->OverdriveAmnt;
	ps->posx = updater->posx;
	ps->prevAction = &updater->prevAction[0];
	ps->side = updater->side;
}


auto BBFramework::initalize() -> void {
	base = GetModuleHandle(NULL);
	// set function and data hooks here
	CreateObject_Hook = safetyhook::create_mid(reinterpret_cast<uintptr_t>(base) + 0x191720, hook_CreateObject);
	FrameStep_Hook = safetyhook::create_mid(reinterpret_cast<uintptr_t>(base) + 0x16B01B, hook_FrameStep);
	AttackHit_Hook = safetyhook::create_mid(reinterpret_cast<uintptr_t>(base) + 0x18C3C0, hook_AttackHit);
	RoundStart_Hook = safetyhook::create_mid(reinterpret_cast<uintptr_t>(base) + 0x1584EF, hook_RoundStart);
	RoundEnd_Hook = safetyhook::create_mid(reinterpret_cast<uintptr_t>(base) + 0x1C83C4, hook_RoundEnd);
	SpriteUpdate_Hook = safetyhook::create_mid(reinterpret_cast<uintptr_t>(base) + 0x182655, hook_SpriteUpdate);
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