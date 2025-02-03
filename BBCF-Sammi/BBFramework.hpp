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
	SafetyHookMid CreateObject_hook;
	SafetyHookMid AttackHit_Hook;
	SafetyHookMid RoundStart_Hook;
	SafetyHookMid RoundEnd_Hook;
	auto initalize() -> void;

public:
	BBFramework(const BBFramework&) = delete;

	static auto get_instance() -> BBFramework*;
};