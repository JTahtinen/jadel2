#ifndef VECTOR_H
#define VECTOR_H
#include "defs.h"
#include <string.h>
namespace jadel
{
    template <typename T>
    struct Vector
    {
        T *data;
        size_t size;
        size_t capacity;

        Vector(size_t capacity)
            : size(0), capacity(capacity)
        {
            data = (T *)malloc(sizeof(T) * capacity);
        }

        Vector()
            : Vector(10)
        {
        }

        ~Vector()
        {
            free(data);
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

        T *back()
        {
            T *result;
            if (size > 0)
            {
                result = &data[size - 1];
            }
            else
                result = NULL;
            return result;
        }

        const T *back() const
        {
            const T *result;
            if (size > 0)
            {
                result = &data[size - 1];
            }
            else
                result = NULL;
            return result;
        }

        const T &get(size_t index) const
        {
            // It is the caller's responsibility to pass a valid index
            const T &result = data[index];
            return result;
        }

        bool resize(size_t newCap)
        {
            T *newData = (T *)malloc(sizeof(T) * newCap);
            if (!newData)
                return false;
            memmove(newData, data, size * sizeof(T));
            capacity = newCap;
            free(data);
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
    };
}

#endif