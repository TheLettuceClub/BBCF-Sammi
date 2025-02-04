#pragma once
#include "safetyhook.hpp"
#include <mutex>

class BBFramework {
private:
	BBFramework() {
		initalize();
	}

	static BBFramework* instance;
	static std::mutex mtx;

	//safetyhooks here
	SafetyHookMid FrameStep_Hook;
	SafetyHookMid CreateObject_Hook;
	SafetyHookMid AttackHit_Hook;
	SafetyHookMid RoundStart_Hook;
	SafetyHookMid RoundEnd_Hook;
	SafetyHookMid SpriteUpdate_Hook;
	SafetyHookMid TitleScreen_Hook;
	SafetyHookMid MenuScreen_Hook;
	SafetyHookMid LobbyState_Hook;
	SafetyHookMid MatchVariables_Hook;
	auto initalize() -> void;

public:
	BBFramework(const BBFramework&) = delete;

	static auto get_instance() -> BBFramework*;
};