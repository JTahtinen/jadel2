#pragma once

namespace jadel
{
    template <typename T>
    struct Iterator
    {
        T *pointer;
        Iterator(T *data)
            : pointer(data)
        {
        }

        T &operator*() const
        {
            return *pointer;
        }

        T *operator->()
        {
            return pointer;
        }

        Iterator &operator++()
        {
            pointer++;
            return *this;
        }

        Iterator operator++(int)
        {
            Iterator tmp = *this;
            ++(*this);
            return tmp;
        }

        bool operator==(const Iterator other)
        {
            bool result = (this->pointer == other.pointer);
            return result;
        }

        bool operator!=(const Iterator other)
        {
            bool result = !(*this == other);
            return result;
        }
    };
}