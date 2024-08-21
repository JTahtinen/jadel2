#pragma once
#include <new>
#include "jadel_defs.h"
#define JADEL_GET_MALLOC_MACRO(_1, _2, NAME, ...) NAME

#define JADEL_MALLOC_2(type, elementCount) (type *)jadel::memoryAllocate(elementCount * sizeof(type))
#define JADEL_MALLOC_1(type) JADEL_MALLOC_2(type, 1)
#define JADEL_MALLOC(...) JADEL_EXPAND_MACRO(JADEL_GET_MALLOC_MACRO(__VA_ARGS__, JADEL_MALLOC_2, JADEL_MALLOC_1)(__VA_ARGS__))

namespace jadel
{

#define KB(n) ((n) * 1024)
#define MB(n) KB((n) * 1024)
#define GB(n) MB((n) * 1024)

    extern DECLSPEC bool memoryInit(size_t bytes);
    extern DECLSPEC void *memoryAllocate(size_t bytes);
    extern DECLSPEC bool memoryFree(void *block);
    extern DECLSPEC void memoryPrintDebugData();

    extern DECLSPEC size_t memoryGetTotalAllocationSize();
    extern DECLSPEC size_t memoryGetNumAllocatedBlocks();
    extern DECLSPEC size_t memoryGetNumAllocatedBytes();
    extern DECLSPEC size_t memoryGetFreeBytes();

#ifdef DEBUG
    struct MemoryBlockDebugData
    {
        void *pointer;
        ptrdiff_t pointerOffset;
        size_t size;
    };
    
    struct MemoryDebugData
    {
        void *memoryPool;
        size_t memoryPoolSize;
        int numAllocations;
        MemoryBlockDebugData blockData[1000];
    };

    extern DECLSPEC MemoryDebugData memoryGetDebugData();
#endif

    template <typename T, typename... Args>
    T *memoryNew(Args... args)
    {
        void *block = jadel::memoryAllocate(sizeof(T));
        if (!block)
        {
            return NULL;
        }
        T *result = new (block) T(args);
        return result;
    }
    template <typename T, typename... Args>
    T *memoryNewArray(size_t arrayLength)
    {
        void block = jadel::memoryAllocate(arrayLength * sizeof(T));
        if (!block)
        {
            return NULL;
        }
        T *result = new (block) T[arrayLength];
        return result;
    }

    template <typename T>
    void memoryDelete(T *t)
    {
        if (!t)
            return;
        t->~T();
        jadel::memoryFree(t);
    }
}