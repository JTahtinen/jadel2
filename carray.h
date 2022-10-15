#ifndef CARRAY_H
#define CARRAY_H
#include "defs.h"

namespace jadel
{
    struct cArray
    {
        void *data;
        size_t size;
        size_t capacity;
        size_t elemSize;

        cArray(size_t cap, size_t elemSize);
        cArray initArray(size_t cap, size_t elemSize);
        bool push(const void *elem);
        bool pop();
        void *get(size_t index);
        const void *get(size_t index) const;
        void *back();
        const void *back() const;
        void *front();
        const void *front() const;

        inline void *operator[](size_t index)
        {
            return get(index);
        }

        inline const void *operator[](size_t index) const
        {
            return get(index);
        }
    };
}

#endif