#ifndef WINAPPTEST_H
#define WINAPPTEST_H
#include <windows.h>
#include <tchar.h>
#include "defs.h"

namespace jadel
{
    enum Endianness
    {
        LITTLE_ENDIAN,
        BIG_ENDIAN
    };

    extern DLLIMPORT HINSTANCE myHInstance;
    extern DLLIMPORT Endianness endianness;
    extern DLLIMPORT TCHAR className[];
}

extern DLLIMPORT bool JadelInit();
extern DLLIMPORT void JadelUpdate();

#endif