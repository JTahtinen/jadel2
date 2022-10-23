#pragma once

#include "jadel_defs.h"

namespace jadel
{
    struct DECLSPEC cArray
    {
        void *data;
        size_t size;
        size_t capacity;
        size_t elemSize;

        //cArray(size_t cap, size_t elemSize);
        void init(size_t cap, size_t elemSize);
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

    DECLSPEC bool cArrayInit(size_t cap, size_t elemSize, cArray* target);
}