#include "jadel_string.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

namespace jadel
{
    String::String(const char *content)
    {
        size_t len = strlen(content);
        this->content = (char *)malloc(len + 1);
        memmove(this->content, content, len);
        this->size = len;
        this->content[this->size] = 0;
    }

    String::String(const String& other)
    {
        size_t len = strlen(other.content);
        this->content = (char *)malloc(len + 1);
        memmove(this->content, other.content, len);
        this->size = len;
        this->content[this->size] = 0;        
    }

    String::~String()
    {
        //free(content);
        //size = 0;
    }

    const char *String::c_str() const
    {
        return content;
    }

    char *String::c_str()
    {
        return content;
    }

    char String::operator[](size_t index) const
    {
        char result = content[index];
        return result;
    }

    bool String::operator==(const String &other) const
    {
        bool result = *this == other.content;
        return result;
    }
    bool String::operator==(const char *other) const
    {
        bool result = strncmp(this->content, other, this->size);
        return result == 0;
    }

    bool String::operator!=(const String &other) const
    {
        bool result = !(*this == other);
        return result;
    }

    bool String::operator!=(const char *other) const
    {
        bool result = !(*this == other);
        return result;
    }

    void String::print() const
    {
        for (size_t i = 0; i < size; ++i)
        {
            putchar(content[i]);
        }
    }
}