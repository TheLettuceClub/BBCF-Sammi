#include "pch.h"
#include "dinput8.h"

DirectInput8Create_t OriginalFunction = nullptr;
HMODULE DInput8DLL = nullptr;

BBFRAMEWORK_API HRESULT WINAPI DirectInput8Create(HINSTANCE hinst, DWORD dwVersion, REFIID riidltf, LPVOID * ppvOut, LPUNKNOWN punkOuter)
{
#pragma comment(linker, "/EXPORT:" __FUNCTION__"=" __FUNCDNAME__)

    if (OriginalFunction)
    {
        return OriginalFunction(hinst, dwVersion, riidltf, ppvOut, punkOuter);
    }
    
    return S_FALSE;
}