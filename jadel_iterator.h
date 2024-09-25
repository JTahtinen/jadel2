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

        T &operator*()
        {
            return *pointer;
        }

        const T &operator*() const
        {
            return *pointer;
        }

        T *operator->()
        {
            return pointer;
        }

        const T *operator->() const
        {
            return pointer;
        }

        Iterator &operator++()
        {
            pointer++;
            return *this;
        }

        Iterator &operator++() const
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

        Iterator operator++(int) const
        {
            Iterator tmp = *this;
            ++(*this);
            return tmp;
        }

        bool operator==(const Iterator other) const
        {
            bool result = (this->pointer == other.pointer);
            return result;
        }

        bool operator!=(const Iterator other) const
        {
            bool result = !(*this == other);
            return result;
        }
    };
}