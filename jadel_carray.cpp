#include "jadel_carray.h"
#include <stdlib.h>
#include <memory.h>
#include "jadel_math.h"

namespace jadel
{
    /*cArray::cArray(size_t cap, size_t elemSize)
        : size(0), capacity(cap), elemSize(elemSize)
    {
        data = malloc(cap * elemSize);
    }*/

    void cArray::init(size_t cap, size_t elemSize)
    {
        this->size = 0;
        this->capacity = cap;
        this->elemSize = elemSize;
        this->data = malloc(elemSize * cap);
    }

    bool cArray::push(const void *elem)
    {
        if (size < capacity)
        {
            uint8 *pointer = (uint8 *)data + size * elemSize;
            memmove(pointer, elem, elemSize);
            ++size;
            return true;
        }
        return false;
    }

    bool cArray::pop()
    {
        if (size > 0)
        {
            --size;
            return true;
        }
        return false;
    }

    void *cArray::get(size_t index)
    {
        if (index < size)
        {
            uint8 *result = (uint8 *)data + index * elemSize;
            return result;
        }
        return NULL;
    }

    const void *cArray::get(size_t index) const
    {
        if (index < size)
        {
            const uint8 *result = (const uint8 *)data + index * elemSize;
            return result;
        }
        return NULL;
    }

    void *cArray::back()
    {
        if (size > 0)
        {
            void *result = get(size - 1);
            return result;
        }
        return NULL;
    }

    const void *cArray::back() const
    {
        if (size > 0)
        {
            const void *result = get(size - 1);
            return result;
        }
        return NULL;
    }

    void *cArray::front()
    {
        if (size > 0)
        {
            void *result = get(0);
            return result;
        }
        return NULL;
    }

    const void *cArray::front() const
    {
        if (size > 0)
        {
            const void *result = get(0);
            return result;
        }
        return NULL;
    }

    bool cArrayInit(size_t cap, size_t elemSize, cArray* target)
    {
        if (!target) return false;
        void* data = malloc(cap * elemSize);
        if (!data) return false;
        target->capacity = cap;
        target->size = 0;
        target->elemSize = elemSize;
        target->data = data;
        return true;
    }
}