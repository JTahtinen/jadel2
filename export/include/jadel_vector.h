#pragma once
#include "jadel_defs.h"
#include "jadel_memory.h"
#include "jadel_iterator.h"

namespace jadel
{
    template <typename T>
    struct Vector
    {
        T *data;
        size_t size;
        size_t capacity;
        /*
                Vector(size_t capacity)
                    : size(0)
                    , capacity(capacity)
                {
                    data = (T *)memoryReserve(sizeof(T) * capacity);
                }

                Vector()
                {
                }

                /*~Vector()
                {
                    free(data);
                    data = NULL;
                }
                */

        void freeVector()
        {
            jadel::memoryFree(data);
            data = NULL;
        }

        bool push(const T &elem)
        {
            if (size == capacity)
            {
                if (!resize(capacity * 2))
                {
                    return false;
                }
            }
            data[size++] = elem;
            return true;
        }

        bool pop()
        {
            if (size > 0)
            {
                --size;
                return true;
            }
            return false;
        }

        T &get(size_t index)
        {
            // It is the caller's responsibility to pass a valid index
            T &result = data[index];
            return result;
        }

        T &back()
        {
            return data[size - 1];
        }

        const T& back() const
        {
            return data[size - 1];
        }

        void clear()
        {
            size = 0;
        }

        const T &get(size_t index) const
        {
            // It is the caller's responsibility to pass a valid index
            const T &result = data[index];
            return result;
        }

        bool resize(size_t newCap)
        {
            if (newCap == capacity)
                return true;
            T *newData = (T *)jadel::memoryReserve(sizeof(T) * newCap);
            if (!newData)
                return false;
            memmove(newData, data, size * sizeof(T));
            capacity = newCap;
            jadel::memoryFree(data);
            data = newData;
            return true;
        }

        T &operator[](size_t index)
        {
            T &result = get(index);
            return result;
        }

        const T &operator[](size_t index) const
        {
            const T &result = get(index);
            return result;
        }

        Iterator<T> begin()
        {
            return Iterator<T>(&data[0]);
        }

        Iterator<T> end()
        {
            return Iterator<T>(&data[size]);
        }
    };

    template <typename T>
    inline bool vectorInit(size_t capacity, Vector<T> *target)
    {
        if (!target)
            return false;
        target->data = (T *)jadel::memoryReserve(sizeof(T) * capacity);
        if (!target->data)
            return false;
        target->capacity = capacity;
        target->size = 0;
        return true;
    }
}