#pragma once
#include "jadel_memory.h"

namespace jadel
{
    template <typename T>
    class Stack
    {
        T *m_data;
        size_t m_size;
        size_t m_capacity;

    public:

        Stack()
            : m_data(NULL), m_size(0), m_capacity(0)
        {
        }

        bool init(size_t cap)
        {
            m_data = (T*)memoryReserve(cap * sizeof(T));
            if (!m_data)
            {
                return false;
            }
            m_size = 0;
            m_capacity = cap;
            return true;
        }

        void destroy()
        {
            memoryFree(m_data);
            m_data = NULL;
        }

        size_t size()
        {
            return m_size;
        }

        T& top()
        {
            return m_data[m_size - 1];
        }

        const T &top() const
        {
            return m_data[m_size - 1];
        }

        void push(const T &element)
        {
            if (m_size == m_capacity)
            {
                if (!resize(m_capacity * 2))
                {
                    return;
                }
            }
            m_data[m_size++] = element;
        }

        void pop()
        {
            if (m_size > 0)
            {
                --m_size;
            }
        }

        bool resize(size_t cap)
        {
            T* newData = (T*)memoryReserve(cap * sizeof(T));
            if (!newData)
            {
                return false;
            }
            if (m_data)
            {
                memoryFree(m_data);
            } 
            m_data = newData;
            m_capacity = cap;
            return true;
        }
    };
}
