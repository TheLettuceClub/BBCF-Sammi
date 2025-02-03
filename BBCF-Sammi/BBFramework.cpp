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
Battle_CObject* p1 = nullptr;
Battle_CObject* p2 = nullptr;
int p1Chara = 0;
int p2Chara = 0;
int time = 0;

void initalizeWSServer() {
	worker.thread = std::make_shared<std::thread>([]() {
		/* create uWebSocket worker and capture uWS::Loop, uWS::App objects. */
		worker.work();
		});
	std::cout << "BBCF-Sammi: WS: server started" << std::endl;
}

void sendEvent(std::string eventName, std::string customData) {
	std::string body = std::format("{{\"event\": \"{}\", \"eventInfo\": {}}}", eventName, customData);
	//send to all opened clients
	worker.loop->defer([body]() {
		worker.app->publish("broadcast", body, uWS::TEXT);
		});
}

void MessageHandler() {
	// do message stuff here
}

// function hooks here
void hook_CreateObject(SafetyHookContext& ctx) {
	// should be called when object created
}


auto BBFramework::initalize() -> void {
	base = GetModuleHandle(NULL);
	// set function and data hooks here
	CreateObject_hook = safetyhook::create_mid(reinterpret_cast<uintptr_t>(base) + 0x191720, hook_CreateObject);
	p1 = reinterpret_cast<Battle_CObject*>(base) + 0x892998;
	p2 = reinterpret_cast<Battle_CObject*>(base) + 0x89299C;
	p1Chara = reinterpret_cast<int>(base) + 0x8919F8;
	p2Chara = reinterpret_cast<int>(base) + 0x891A18;
	time = reinterpret_cast<int>(base) + 0xE19674;
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
			std::cout << "Starting message handler" << std::endl;
			std::thread(MessageHandler).detach();
		}
	}
	return instance;
}