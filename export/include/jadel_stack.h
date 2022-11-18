#pragma once
#include "jadel_memory.h"

namespace jadel
{
    template <typename T>
    class Stack
    {
        T *_data;
        size_t _size;
        size_t _capacity;

    public:

        bool init(size_t cap)
        {
            _data = (T*)memoryReserve(cap * sizeof(T));
            if (!_data)
            {
                return false;
            }
            _size = 0;
            _capacity = cap;
            return true;
        }

        size_t size()
        {
            return _size;
        }

        T& top()
        {
            return _data[_size - 1];
        }

        const T &top() const
        {
            return _data[_size - 1];
        }

        void push(const T &element)
        {
            if (_size == _capacity)
            {
                if (!resize(_capacity * 2))
                {
                    return;
                }
            }
            _data[_size++] = element;
        }

        void pop()
        {
            if (_size > 0)
            {
                --_size;
            }
        }

        bool resize(size_t cap)
        {
            T* newData = (T*)memoryReserve(cap * sizeof(T));
            if (!newData)
            {
                return false;
            }
            if (_data)
            {
                memoryFree(_data);
            } 
            _data = newData;
            _capacity = cap;
            return true;
        }
    };
}
