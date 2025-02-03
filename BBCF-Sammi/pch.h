#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files
#include <windows.h>
#include <winternl.h>
#include <imagehlp.h>


#ifdef DLL_EXPORTS
#define BBFRAMEWORK_API __declspec(dllexport)
#else
#define DINPUT8_API __declspec(dllimport)
#endif
