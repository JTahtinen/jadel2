#pragma once

#include <windows.h>
#include <tchar.h>
#include "jadel_defs.h"

namespace jadel
{
    enum Endianness
    {
        LITTLE_ENDIAN,
        BIG_ENDIAN
    };

    extern DECLSPEC HINSTANCE myHInstance;
    extern DECLSPEC Endianness endianness;
    extern DECLSPEC TCHAR className[];
}

extern DECLSPEC bool JadelInit();
extern DECLSPEC void JadelUpdate();