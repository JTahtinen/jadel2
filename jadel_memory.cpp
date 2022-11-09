#include <stdlib.h>
#include <stdio.h>
#include "jadel_linkedlist.h"
#include "jadel_memory.h"
#include "jadel_message.h"

namespace jadel
{

    static size_t totalAllocationSize = 0;
    static size_t numAllocatedBlocks = 0;
    static size_t numAllocatedBytes = 0;

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

    static uint8 *memoryAllocation;
    static size_t numBytesAllocated;

    static LinkedList<MemoryBlock> freeList;

    static size_t getMemoryOffset(uint8 *pointer)
    {
        size_t result = pointer - memoryAllocation;
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

    bool memoryInit(size_t bytes)
    {
        memoryAllocation = (uint8 *)malloc(bytes);

        if (!memoryAllocation)
            return false;

        numBytesAllocated = bytes;
        freeList.append(createMemoryBlock(memoryAllocation, numBytesAllocated));
        totalAllocationSize = bytes;
        return true;
    }

    void *memoryReserve(size_t bytes)
    {
        bool blockFound = false;
        MemoryBlock *block;
        size_t i = 0;
        while (!blockFound)
        {
            block = freeList.get(i);
            if (!block)
            {
                printf("[ERROR] Cannot reserve %zd bytes!\n", bytes);
                return NULL;
            }
            if (!block->reserved && bytes <= block->size)
                blockFound = true;
            else
                ++i;
        }

        block->reserved = true;
        jadel::message("Reserved %d bytes\n", bytes);
        numAllocatedBytes += bytes;
        ++numAllocatedBlocks;
        // No need to do more processing if the block is already precisely the correct size
        if (bytes == block->size)
            return block->pointer;

        size_t oldBlockSize = block->size;
        block->size = bytes;
        MemoryBlock *nextBlockInList = freeList.get(i + 1);
        uint8 *nextBlockStart = (uint8 *)block->pointer + bytes;
        if (!nextBlockInList)
        {
            // If current block ends at the edge of the total memory allocation
            // we do not add another block at the end of the freeList
            if ((uint8 *)block->pointer + bytes - (uint8 *)memoryAllocation == numBytesAllocated)
                return block->pointer;

            freeList.append(createMemoryBlock(nextBlockStart, numBytesAllocated - getMemoryOffset(nextBlockStart)));
            return block->pointer;
        }

        // Skip if next block pointer is already at current block's edge
        if (nextBlockInList->pointer != block->pointer + block->size)
        {
            // Move the next block's pointer to the edge of current block unless next block is reserved
            if (!nextBlockInList->reserved)
            {
                nextBlockInList->pointer = nextBlockStart;
                nextBlockInList->size += oldBlockSize - block->size;
            }
            else // Otherwise insert a new block between the current and the next one
            {
                freeList.insertNext(i, createMemoryBlock(nextBlockStart, nextBlockInList->pointer - nextBlockStart));
            }
        }
        return block->pointer;
    }

    bool memoryFree(void *block)
    {
        if (block == NULL) return false;
        MemoryBlock *memBlock = freeList.getHead();
        size_t i = 0;
        while (memBlock->pointer != (uint8 *)block)
        {
            ++i;
            memBlock = freeList.get(i);
            if (!memBlock)
                return false;
        }
        jadel::message("Freed %d bytes\n", memBlock->size);
        numAllocatedBytes -= memBlock->size;
        --numAllocatedBlocks;
        memBlock->reserved = false;

        // Merge the block with the next if it's not reserved
        MemoryBlock *nextBlock = freeList.get(i + 1);
        if (nextBlock && !nextBlock->reserved)
        {
            memBlock->size += nextBlock->size;
            freeList.deleteByIndex(i + 1);
        }
        return true;
    }

    void memoryPrintDebugData()
    {
        printf("FreeList contents:\n");

        MemoryBlock *block = freeList.getHead();
        size_t i = 0;
        while (block)
        {
            printf("Block index: %zd, pointer %p, size: %zd, reserved: %d\n", i, block->pointer, block->size, (int)block->reserved);
            block = freeList.get(++i);
        }
    }

    size_t memoryGetTotalAllocationSize()
    {
        return totalAllocationSize;
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
        return totalAllocationSize - numAllocatedBytes;
    }
}