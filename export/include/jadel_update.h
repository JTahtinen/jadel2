#pragma once

#include <windows.h>
#include <tchar.h>
#include "jadel_defs.h"

namespace jadel
{
    extern DECLSPEC HINSTANCE myHInstance;
    extern DECLSPEC TCHAR className[];
}

extern DECLSPEC bool JadelInit();
extern DECLSPEC void JadelUpdate();