#pragma once

#include "jadel_defs.h"
#include "jadel_memory.h"
#include "jadel_iterator.h"

namespace jadel
{
    template <typename T>
    struct Array
    {
        T *data;
        size_t size;
        size_t capacity;

        Array(size_t capacity)
            : capacity(capacity), size(0)
        {
            data = (T *)jadel::memoryReserve(sizeof(T) * capacity);
        }

        Array()
            : Array(0)
        {
        }

        ~Array()
        {
            for (size_t i = 0; i < size; ++i)
                data[i].~T();
            free(data);
        }

        inline void push(const T& elem)
        {
            if (size < capacity)
            {
                data[size++] = elem;
            }
        }

        inline bool pop()
        {
            if (size > 0)
            {
                --size;
                return true;
            }
            return false;
        }

        inline T &get(size_t index)
        {
            if (index < size)
            {
                return data[index];
            }
        }

        inline T &back()
        {
            return data[size - 1];
        }

        inline const T &back() const
        {
            return data[size - 1];
        }

        Iterator<T> begin()
        {
            return Iterator<T>(&data[0]);
        }

        Iterator<T> end()
        {
            return Iterator<T>(&data[size]);
        }

        inline const T &operator[](size_t index) const
        {
            return data[index];
        }

        inline T &operator[](size_t index)
        {
            return data[index];
        }

    };
}