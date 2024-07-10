#pragma once

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

        void insertNext(const T &data)
        {
            Node<T> *node = createNode(data);
            node->prev = this;
            node->next = this->next;
            this->next = node;
        }

        void insertPrev(const T &data)
        {
            Node<T> *node = createNode(data);
            node->next = this;
            node->prev = this->prev;
            this->prev = node;
        }

        void remove()
        {
            if (this->prev)
            {
                this->prev->next = this->next;
            }
            if (this->next)
            {
                this->next->prev = this->prev;
            }

            this->prev = NULL;
            this->next = NULL;
            free(this);
        }

        static Node<T> *createNode(const T &data)
        {
            Node<T> *result = (Node<T> *)malloc(sizeof(Node<T>));
            result->data = data;
            result->prev = NULL;
            result->next = NULL;
            return result;
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
            head = Node<T>::createNode(data);
        }

        Node<T> *getHead() const
        {
            return head;
        }

        void append(const T &data)
        {
            Node<T> *newNode = Node<T>::createNode(data);

            if (!head)
            {
                head = newNode;
                return;
            }
            Node<T> *current = head;
            while (current->next)
            {
                current = current->next;
            }
            current->insertNext(data);
        }

        void prepend(const T &data)
        {
            Node<T> *newNode = createNode(data);
            if (!head)
            {
                head = newNode;
                head->prev = NULL;
                head->next = NULL;
                return;
            }
            head->insertPrev(data);
        }
    };
}
