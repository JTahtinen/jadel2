#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#include "defs.h"
#include <stdlib.h>
#include <stdio.h>

namespace jadel
{
    template <typename T>
    struct Node
    {
        T data;
        Node<T> *prev;
        Node<T> *next;

        Node(T &data)
            : data(data), prev(NULL), next(NULL)
        {
        }

        void insertNext(Node<T> &node)
        {
            Node<T> *newPrev = this;
            Node<T> *newNext;
            if (next)
                newNext = next->next;
            else
                newNext = NULL;

            node.prev = newPrev;
            node.next = newNext;
            this->next = &node;
        }

        void insertPrev(Node<T> &node)
        {
            Node<T> *newNext = this;
            Node<T> *newPrev;
            if (prev)
                newPrev = prev->prev;
            else
                newPrev = NULL;

            node.prev = newPrev;
            node.next = newNext;
            this->prev = node;
        }
    };

    template <typename T>
    struct LinkedList
    {
        Node<T> *head;

        LinkedList()
            : head(NULL)
        {
        }

        LinkedList(const T &data)
        {
            head = createNode(data);
        }

        T *getHead()
        {
            if (!head)
                return NULL;
            T *result = &head->data;
            return result;
        }

        void append(Node<T> *node)
        {
            if (!head)
            {
                head = node;
                head->prev = NULL;
                head->next = NULL;
                return;
            }
            Node<T> *current = head;
            while (current->next)
            {
                current = current->next;
            }
            current->next = node;
            node->prev = current;
        }

        void append(const T &data)
        {
            Node<T> *newNode = createNode(data);
            append(newNode);
        }

        void prepend(Node<T> *node)
        {
            if (!head)
            {
                head = node;
                head->prev = NULL;
                head->next = NULL;
                return;
            }

            head->prev = node;
            node->next = head;
            head = node;
        }

        void prepend(const T &data)
        {
            Node<T> *newNode = createNode(data);
            prepend(newNode);
        }

        bool insertNext(size_t index, Node<T> *node)
        {
            Node<T> *current = head;
            size_t i = 0;
            while (i < index)
            {
                current = current->next;
                if (!current)
                    return false;
                ++i;
            }
            if (!current)
                return false;
            current->insertNext(*node);
            return true;
        }

        bool insertNext(size_t index, const T &data)
        {
            Node<T> *newNode = createNode(data);
            bool result = insertNext(index, newNode);
            return result;
        }

        bool deleteWithValue(const T &value)
        {
            if (!head)
                return false;
            Node<T> *current = head;

            while (current->data != value)
            {
                if (!current->next)
                    return false;
                current = current->next;
            }
            if (current == head)
                head = current->next;
            else
            {
                current->prev->next = current->next;
                if (current->next)
                    current->next->prev = current->prev;
            }
            free(current);
            return true;
        }

        bool deleteByIndex(size_t index)
        {
            if (!head)
                return false;
            Node<T> *current = head;
            for (size_t i = 0; i < index; ++i)
            {
                current = current->next;
                if (!current)
                    return false;
            }
            if (current == head)
            {
                head = current->next;
                free(current);
                return true;
            }
            current->prev->next = current->next;
            if (current->next)
            {
                current->next->prev = current->prev;
            }
            free(current);
            return true;
        }

        T *get(size_t index)
        {
            if (!head)
                return NULL;
            Node<T> *current = head;
            for (int i = 0; i < index; ++i)
            {
                if (current->next)
                    current = current->next;
                else
                {
                    return NULL;
                }
            }
            return &current->data;
        }

        T *getByValue(const T &value)
        {
            T *currentData = getHead();
            size_t i = 0;
            while (*currentData != value)
            {
                currentData = get(++i);
                if (!currentData)
                    return NULL;
            }
            return currentData;
        }

        static Node<T> *createNode(const T &data)
        {
            Node<T> *result = (Node<T> *)malloc(sizeof(Node<T>));
            result->prev = NULL;
            result->next = NULL;
            result->data = data;
            return result;
        }
    };
}

#endif