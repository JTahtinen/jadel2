#pragma once

#include "jadel_defs.h"
namespace jadel
{

#define KB(n) (n * 1024)
#define MB(n) KB(n * 1024)
#define GB(n) MB(n * 1024)

    extern DECLSPEC bool memoryInit(size_t bytes);
    extern DECLSPEC void *memoryReserve(size_t bytes);
    extern DECLSPEC bool memoryFree(void *block);
    extern DECLSPEC void memoryPrintDebugData();
}