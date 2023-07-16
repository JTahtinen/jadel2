#include <stdlib.h>
#include <stdio.h>
#include "jadel_linkedlist.h"
#include "jadel_memory.h"
#include "jadel_message.h"

namespace jadel
{
    struct MemoryBlock
    {
        uint8 *pointer;
        size_t size;
        bool reserved;

        inline bool operator==(const MemoryBlock &other)
        {
            bool result = this->pointer == other.pointer;
            return result;
        }

        inline bool operator!=(const MemoryBlock &other)
        {
            bool result = !(*this == other);
            return result;
        }
    };

    static uint8 *memoryPool;
    static size_t memoryPoolSize;
    static size_t numAllocatedBlocks = 0;
    static size_t numAllocatedBytes = 0;
    static LinkedList<MemoryBlock> freeList;

    static uint8 *getEndOfBlock(const MemoryBlock *block)
    {
        uint8 *result = block->pointer + block->size;
        return result;
    }

    static uint8 *getMemoryPoolEnd()
    {
        return memoryPool + memoryPoolSize;
    }

    static size_t getMemoryOffset(uint8 *pointer)
    {
        size_t result = pointer - memoryPool;
        return result;
    }

    static MemoryBlock createMemoryBlock(uint8 *pointer, size_t size)
    {
        MemoryBlock result;
        result.pointer = pointer;
        result.size = size;
        result.reserved = false;
        return result;
    }

    static void createMemoryBlockAndAppendFreeList(uint8 *pointer, size_t size)
    {
        MemoryBlock block = createMemoryBlock(pointer, size);
        freeList.append(block);
    }

    static void createMemoryBlockAndInsertFreeList(uint8 *pointer, size_t size, size_t index)
    {
        MemoryBlock block = createMemoryBlock(pointer, size);
        freeList.insertNext(index, block);
    }

    static MemoryBlock *findBlockByPointer(void *pointer, size_t *index)
    {
        if (!pointer)
            return false;
        MemoryBlock *result = freeList.getHead();

        // Look for the correct block in the free list
        size_t i = 0;
        while (result->pointer != (uint8 *)pointer)
        {
            ++i;
            result = freeList.get(i);
            if (!result)
                return false;
        }
        *index = i;
        return result;
    }

    bool memoryInit(size_t bytes)
    {
        memoryPool = (uint8 *)malloc(bytes);

        if (!memoryPool)
            return false;

        memoryPoolSize = bytes;
        createMemoryBlockAndAppendFreeList(memoryPool, memoryPoolSize);
        return true;
    }

    void *memoryReserve(const size_t bytes)
    {
        bool blockFound = false;
        MemoryBlock *block;
        size_t blockIndex = 0;
        while (!blockFound)
        {
            block = freeList.get(blockIndex);
            if (!block)
            {
                jadel::message("[ERROR] Cannot reserve %d bytes!\n", bytes);
                return NULL;
            }
            if (!block->reserved && bytes <= block->size)
                blockFound = true;
            else
                ++blockIndex;
        }

        block->reserved = true;
        // jadel::message("Reserved %d bytes\n", bytes);
        numAllocatedBytes += bytes;
        ++numAllocatedBlocks;
        // No need to do more processing if the block is already precisely the correct size
        if (bytes == block->size)
            return block->pointer;

        size_t oldBlockSize = block->size;
        block->size = bytes;
        uint8 *expectedNextBlockStart = getEndOfBlock(block);
        MemoryBlock *nextBlockInList = freeList.get(blockIndex + 1);
        if (!nextBlockInList)
        {
            // If current block ends at the edge of the memory pool
            // we do not add another block at the end of the freeList
            createMemoryBlockAndAppendFreeList(expectedNextBlockStart, getMemoryPoolEnd() - expectedNextBlockStart);
        }
        return block->pointer;
    }

    bool memoryFree(void *block)
    {
        size_t index;
        MemoryBlock *memBlock = findBlockByPointer(block, &index);
        if (!memBlock)
        {
            return false;
        }
        // jadel::message("Freed %d bytes\n", memBlock->size);
        numAllocatedBytes -= memBlock->size;
        --numAllocatedBlocks;
        memBlock->reserved = false;

        // Merge the block with the surrounding blocks next if they're not reserved
        MemoryBlock *nextBlock = freeList.get(index + 1);
        if (nextBlock && !nextBlock->reserved)
        {
            memBlock->size += nextBlock->size;
            freeList.deleteByIndex(index + 1);
        }
        MemoryBlock *prevBlock = freeList.get(index - 1);
        if (prevBlock && !prevBlock->reserved)
        {
            memBlock->pointer = prevBlock->pointer;
            memBlock->size += prevBlock->size;
            freeList.deleteByIndex(index - 1);
        }
        return true;
    }

    void memoryPrintDebugData()
    {
        jadel::message("FreeList contents:\n");

        MemoryBlock *block = freeList.getHead();
        size_t i = 0;
        while (block)
        {
            jadel::message("Block index: %d, pointer offset: %d, size: %d, reserved: %d\n", i, getMemoryOffset(block->pointer), block->size, (int)block->reserved);
            block = freeList.get(++i);
        }
    }

    size_t memoryGetTotalAllocationSize()
    {
        return memoryPoolSize;
    }

    size_t memoryGetNumAllocatedBlocks()
    {
        return numAllocatedBlocks;
    }

    size_t memoryGetNumAllocatedBytes()
    {
        return numAllocatedBytes;
    }

    size_t memoryGetFreeBytes()
    {
        return memoryPoolSize - numAllocatedBytes;
    }
}