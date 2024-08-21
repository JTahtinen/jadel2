#include <stdlib.h>
#include <stdio.h>
#include "jadel_linkedlist.h"
#include "jadel_memory.h"
#include "jadel_message.h"

namespace jadel
{
    struct MemoryBlock
    {
        void *pointer;
        size_t size;
    };

    static void *memoryPool = NULL;
    static size_t memoryPoolSize = 0;
    static jadel::LinkedList<MemoryBlock> freeList;
    static jadel::LinkedList<MemoryBlock> allocationList;

    static void mergeBlockIfNecessary(jadel::Node<MemoryBlock> *block)
    {
        if (!block)
        {
            JADEL_DEBUGMSG("[ERROR] Tried to merge nonexistent block");
            return;
        }
        jadel::Node<MemoryBlock> *prevBlock = block->prev;
        jadel::Node<MemoryBlock> *nextBlock = block->next;
        if (prevBlock)
        {
            if ((uint8 *)prevBlock->data.pointer + prevBlock->data.size == block->data.pointer)
            {
                prevBlock->data.size += block->data.size;
                block->remove();
                block = prevBlock;
            }
        }
        if (nextBlock)
        {
            if ((uint8 *)block->data.pointer + block->data.size == nextBlock->data.pointer)
            {
                block->data.size += nextBlock->data.size;
                nextBlock->remove();
            }
        }
    }

    static void insertToFreeList(void *pointer, size_t size)
    {
        MemoryBlock block;
        block.pointer = pointer;
        block.size = size;
        jadel::Node<MemoryBlock> *headNode = freeList.getHead();
        // if free list is empty, we can just append and return.
        if (!headNode)
        {
            freeList.append(block);
            return;
        }
        jadel::Node<MemoryBlock> *blockNode = headNode;
        while (blockNode)
        {
            if (blockNode->data.pointer > pointer)
            {
                blockNode->insertPrev(block);
                if (blockNode == headNode)
                {
                    freeList.head = blockNode->prev;
                }
                mergeBlockIfNecessary(blockNode->prev);
                return;
            }
            if (blockNode->next)
            {
                blockNode = blockNode->next;
            }
            else
            {
                blockNode->insertNext(block);
                mergeBlockIfNecessary(blockNode->next);
                break;
            }
        }
    };

    static void removeFromFreeList(void *pointer)
    {
        jadel::Node<MemoryBlock> *headNode = freeList.getHead();
        jadel::Node<MemoryBlock> *blockNode = headNode;
        while (blockNode)
        {
            if (blockNode->data.pointer == pointer)
            {
                if (blockNode == headNode)
                {
                    freeList.head = blockNode->next;
                }
                blockNode->remove();
                return;
            }
            blockNode = blockNode->next;
        }
    }

    static void insertToAllocationList(void *pointer, size_t size)
    {
        MemoryBlock block;
        block.pointer = pointer;
        block.size = size;
        allocationList.append(block);
    }

    static bool removeFromAllocationList(void *pointer, MemoryBlock *blockInfo)
    {
        JADEL_DEBUGMSG("DELETING BLOCK %p ... ", pointer);
        jadel::Node<MemoryBlock> *headNode = allocationList.getHead();
        jadel::Node<MemoryBlock> *blockNode = headNode;
        while (blockNode)
        {
            if (blockNode->data.pointer == pointer)
            {
                if (blockNode == headNode)
                {
                    allocationList.head = headNode->next;
                }
                if (blockInfo)
                {
                    *blockInfo = blockNode->data;
                }
                JADEL_DEBUGMSG("done! Block size: %zd\n\n", blockNode->data.size);
                blockNode->remove();
                return true;
            }
            blockNode = blockNode->next;
        }
        jadel::message("failed!\n\n");
        return false;
    }

    bool memoryInit(size_t bytes)
    {
        memoryPool = malloc(bytes);
        if (!memoryPool)
        {
            JADEL_DEBUGMSG("[ERROR] Could not init memory pool!\n");
            return false;
        }
        memoryPoolSize = bytes;
        insertToFreeList(memoryPool, memoryPoolSize);
        return true;
    }

    void *memoryAllocate(size_t bytes)
    {
        int offsetFromAlignment = bytes % 4;
        int requiredAlignment = offsetFromAlignment > 0 ? 4 - offsetFromAlignment : 0;
        int finalBytes = bytes + requiredAlignment;
        JADEL_DEBUGMSG("Aligned allocation by %d bytes - Requested: %zd, Allocated: %d\n", requiredAlignment, bytes, finalBytes);
        jadel::Node<MemoryBlock> *blockNode = freeList.getHead();
        while (blockNode)
        {
            if (finalBytes <= blockNode->data.size)
            {
                void *allocationPointer = blockNode->data.pointer;
                int remainingBytes = blockNode->data.size - finalBytes;
                if (remainingBytes == 0)
                {
                    removeFromFreeList(blockNode->data.pointer);
                }
                else
                {
                    blockNode->data.pointer = (uint8 *)blockNode->data.pointer + finalBytes;
                    blockNode->data.size -= finalBytes;
                }
                insertToAllocationList(allocationPointer, finalBytes);
                return allocationPointer;
            }
            blockNode = blockNode->next;
        }
        JADEL_DEBUGMSG("[ERROR] Could not reserve %zd bytes!\n", bytes);
        return NULL;
    }

    bool memoryFree(void *block)
    {
        MemoryBlock blockInfo;
        bool blockDeleted = removeFromAllocationList(block, &blockInfo);
        if (!blockDeleted)
        {
            jadel::message("[WARNING] Tried to delete nonexistent block: %p\n", block);
            return false;
        }
        insertToFreeList(blockInfo.pointer, blockInfo.size);
        return true;
    }

    void memoryPrintDebugData()
    {
        int numBlocks = 0;
        jadel::message("***MEMORY DEBUG DATA***\nFree list contents:\n--------------------\n");
        jadel::Node<MemoryBlock> *currentBlock = freeList.getHead();
        while (currentBlock)
        {
            jadel::message("Block %d: Pointer: %p - size: %zd\n", numBlocks++, currentBlock->data.pointer, currentBlock->data.size);
            currentBlock = currentBlock->next;
        }

        jadel::message("Allocation list contents:\n--------------------\n");
        numBlocks = 0;
        currentBlock = allocationList.getHead();
        while (currentBlock)
        {
            jadel::message("Block %d: Pointer: %p - size: %zd\n", numBlocks++, currentBlock->data.pointer, currentBlock->data.size);
            currentBlock = currentBlock->next;
        }
        jadel::message("\n");
    };

    size_t memoryGetTotalAllocationSize()
    {
        return memoryPoolSize;
    }

    size_t memoryGetNumAllocatedBlocks()
    {
        int numAllocs = 0;
        jadel::Node<MemoryBlock> *currentBlock = allocationList.getHead();
        while (currentBlock)
        {
            ++numAllocs;
            currentBlock = currentBlock->next;
        }
        return numAllocs;
    }

    size_t memoryGetNumAllocatedBytes()
    {
        size_t numAllocatedBytes = 0;
        jadel::Node<MemoryBlock> *currentBlock = allocationList.getHead();
        while (currentBlock)
        {
            numAllocatedBytes += currentBlock->data.size;
            currentBlock = currentBlock->next;
        }
        return numAllocatedBytes;
    }

    size_t memoryGetFreeBytes()
    {
        size_t result = memoryPoolSize - memoryGetNumAllocatedBytes();
        return result;
    }

#ifdef DEBUG
    MemoryDebugData memoryGetDebugData()
    {
        MemoryDebugData result;
        result.memoryPool = memoryPool;
        result.memoryPoolSize = memoryPoolSize;
        int numAllocations = 0;
        jadel::Node<MemoryBlock> *currentBlock = allocationList.getHead();
        while (currentBlock)
        {
            MemoryBlockDebugData *blockData = &result.blockData[numAllocations++];
            blockData->pointer = currentBlock->data.pointer;
            blockData->pointerOffset = (uint8 *)currentBlock->data.pointer - (uint8 *)memoryPool;
            blockData->size = currentBlock->data.size;
            currentBlock = currentBlock->next;
        }
        result.numAllocations = numAllocations;
        return result;
    }
#endif
}
