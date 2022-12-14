#pragma once

#include "jadel_defs.h"
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
            data = (T *)malloc(sizeof(T) * capacity);
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

        inline void push(T elem)
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

        inline T &ArraygetElement(size_t index)
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