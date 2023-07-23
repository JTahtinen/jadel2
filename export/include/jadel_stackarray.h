#pragma once
#include <memory.h>

namespace jadel
{
    template <typename T, int cap>
    struct StackArray
    {
        T data[cap];
        size_t size;
        size_t capacity{cap};

        inline StackArray()
            : size(0)
        {
        }

        inline StackArray(const StackArray<T, cap> &other)
        {
            if (other.size > cap)
            {
                return;
            }
            memmove(this->data, other.data, other.size * sizeof(T));
            this->size = other.size;
        }

        inline bool push(const T &val)
        {
            if (this->size >= this->capacity)
            {
                return false;
            }
            this->data[this->size++] = val;
            return true;
        }

        inline bool pop()
        {
            if (this->size == 0)
            {
                return false;
            }
            --this->size;
            return true;
        }

        inline T &back()
        {
            T &result = this->data[this->size - 1];
            return result;
        }

        const inline T &back() const
        {
            const T &result = this->data[this->size - 1];
            return result;
        }

        inline void clear()
        {
            this->size = 0;
        }

        inline bool empty() const
        {
            bool result = this->size == 0;
            return result;
        }

        inline T &operator[](size_t index)
        {
            T &result = this->data[index];
            return result;
        }

        inline const T &operator[](size_t index) const
        {
            const T &result = this->data[index];
            return result;
        }

        inline Iterator<T> begin()
        {
            return Iterator<T>(&data[0]);
        }

        inline Iterator<T> end()
        {
            return Iterator<T>(&data[size]);
        }
    };
}