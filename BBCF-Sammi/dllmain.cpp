#include "pch.h"
#include "Print.h"
#include "dinput8.h"
#include "BBFramework.hpp"
#include <iostream>


auto init() -> void
{
	// Load the original dinput8.dll from the system directory
	char DInputDllName[MAX_PATH];
	GetSystemDirectoryA(DInputDllName, MAX_PATH);
	strcat_s(DInputDllName, "\\dinput8.dll");
	DInput8DLL = LoadLibraryA(DInputDllName);
	if (DInput8DLL > reinterpret_cast<HMODULE>(31))
	{
		OriginalFunction = reinterpret_cast<DirectInput8Create_t>(GetProcAddress(DInput8DLL, "DirectInput8Create"));
	}
	CreateConsole();
	std::cout << "test text!" << std::endl;
	std::wcout << L"wide test text!" << std::endl;
	BBFramework* framework = BBFramework::get_instance();
}

BOOL APIENTRY DllMain([[maybe_unused]] HMODULE Module,
					  DWORD  ReasonForCall,
					  [[maybe_unused]] LPVOID Reserved)
{
	switch (ReasonForCall)
	{
	case DLL_PROCESS_ATTACH:
		init();
		break;
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
	default:
		break;
	}
	return TRUE;
}
