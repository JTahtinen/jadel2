#pragma once

#define JADEL_MALLOC( T ) \
((T*)jadel::memoryReserve(sizeof(T)))

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


    extern DECLSPEC size_t memoryGetTotalAllocationSize();
    extern DECLSPEC size_t memoryGetNumAllocatedBlocks();
    extern DECLSPEC size_t memoryGetNumAllocatedBytes();
    extern DECLSPEC size_t memoryGetFreeBytes();
    
    template<typename T, typename... Args>
    T* memoryNew(Args... args)
    {
        T* result = JADEL_MALLOC(T);
        if (!result) return NULL;
        *result = T(args...);
        return result;
    }

    template<typename T>
    void memoryDelete(T *t)
    {
        if (!t) return;
        t->~T();
        jadel::memoryFree(t);
    }
}