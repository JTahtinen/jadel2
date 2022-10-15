#ifndef MEMORY_H
#define MEMORY_H
#include "defs.h"
namespace jadel
{

#define KB(n) (n * 1024)
#define MB(n) KB(n * 1024)
#define GB(n) MB(n * 1024)

    extern DLLIMPORT bool initMemory(size_t bytes);
    extern DLLIMPORT void *memoryReserve(size_t bytes);
    extern DLLIMPORT bool memoryFree(void *block);
    extern DLLIMPORT void memoryPrintDebugData();
}
#endif