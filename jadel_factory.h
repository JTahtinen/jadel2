#pragma once
#include "jadel_memory.h"
#include "jadel_linkedlist.h"
namespace jadel
{
    template <typename T>
    class Factory
    {
        T *_data;
        // Store data about free entities in 2 ways in order to access data faster
        bool *_freeIndices;
        LinkedList<T *> _freeList;
        size_t _numReserved;
        size_t _capacity;

    public:
        bool init(size_t size)
        {
            if (size == 0)
            {
                return false;
            }
            _data = (T *)memoryReserve(size * sizeof(T));
            if (!_data)
            {
                return false;
            }
            _numReserved = 0;
            _freeIndices = (bool *)memoryReserve(size * sizeof(bool));
            if (!_freeIndices)
            {
                memoryFree(_data);
                _data = NULL;
                return false;
            }
            _capacity = size;
            _freeList.head = NULL;
            for (int i = 0; i < _capacity; ++i)
            {
                _freeIndices[i] = true;
                _freeList.append(&_data[i]);
            }
            return true;
        }

        T *get()
        {
            Node<T*> *head = _freeList.head;
            if (head)
            {
                T **element = &head->data;
                _freeList.deleteByIndex(0);
                size_t index = getIndexOfElement(*element);
                _freeIndices[index] = false;
                ++_numReserved;
                return *element;
            }
            return NULL;
        }

        void release(T *element)
        {
            if (element < _data || element >= _data + _capacity)
            {
                jadel::message("[WARNING] Factory: Tried to release out of bounds element\n");
                return;
            }
            size_t index = getIndexOfElement(element);
            if (!_freeIndices[index])
            {
                _freeIndices[index] = true;
                _freeList.append(element);
                --_numReserved;
            }
        }

        size_t getReservedCount()
        {
            return _numReserved;
        }

        size_t getCapacity()
        {
            return _capacity;
        }

    private:
        size_t getIndexOfElement(T *element)
        {
            size_t result = element - _data;
            return result;
        }
    };
}
