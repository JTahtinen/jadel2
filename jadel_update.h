#pragma once

#include <windows.h>
#include <tchar.h>
#include "jadel_defs.h"

namespace jadel
{
    struct Window;
    
    extern DECLSPEC HINSTANCE myHInstance;
    extern DECLSPEC TCHAR className[];

    int DECLSPEC allocateConsole();
    void DECLSPEC writeToConsole(const char* text, size_t numChars);

    extern Window* windowList[10];
    extern int numWindows;
}

extern DECLSPEC bool JadelInit(size_t bytesOfMemoryToReserve);
extern DECLSPEC void JadelUpdate();
extern DECLSPEC bool JadelIsInitialized();