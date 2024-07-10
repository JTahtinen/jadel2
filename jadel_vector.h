#pragma once
#include "jadel_defs.h"
#include "jadel_memory.h"
#include "jadel_iterator.h"

namespace jadel
{
    template <typename T>
    struct Vector
    {
        T *m_data;
        size_t m_size;
        size_t m_capacity;

    public:
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

        Vector()
            : m_data(NULL), m_size(0), m_capacity(0)
        {
        }

        bool init(size_t capacity)
        {
            m_data = (T *)jadel::memoryReserve(sizeof(T) * capacity);
            if (!m_data)
            {
                return false;
            }
            m_capacity = capacity;
            m_size = 0;
            return true;
        }

        void destroy()
        {
            jadel::memoryFree(m_data);
            m_data = NULL;
        }

        size_t size() const
        {
            return m_size;
        }

        bool push(const T &elem)
        {
            if (m_size == m_capacity)
            {
                if (!resize(m_capacity * 2))
                {
                    return false;
                }
            }
            m_data[m_size++] = elem;
            return true;
        }

        bool pop()
        {
            if (m_size > 0)
            {
                --m_size;
                return true;
            }
            return false;
        }

        T &get(size_t index)
        {
            // It is the caller's responsibility to pass a valid index
            T &result = m_data[index];
            return result;
        }

        T &back()
        {
            return m_data[m_size - 1];
        }

        const T &back() const
        {
            return m_data[m_size - 1];
        }

        void clear()
        {
            m_size = 0;
        }

        const T &get(size_t index) const
        {
            // It is the caller's responsibility to pass a valid index
            const T &result = m_data[index];
            return result;
        }

        bool resize(size_t newCap)
        {
            if (newCap == m_capacity)
                return true;
            T *newData = (T *)jadel::memoryReserve(sizeof(T) * newCap);
            if (!newData)
                return false;
            memmove(newData, m_data, m_size * sizeof(T));
            m_capacity = newCap;
            jadel::memoryFree(m_data);
            m_data = newData;
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
            return Iterator<T>(&m_data[0]);
        }

        Iterator<T> end()
        {
            return Iterator<T>(&m_data[m_size]);
        }
    };

}