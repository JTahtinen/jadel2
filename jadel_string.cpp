#include "jadel_string.h"
#include <string.h>
//#include <stdlib.h>
#include <stdio.h>
#include "jadel_memory.h"

namespace jadel
{/**
    String::String(const char *content)
    {
        if (!content)
        {
            this->content = NULL,
            size = 0;
            return;
        }
        size_t len = strlen(content);
        this->content = (char *)jadel::memoryReserve(len + 1);
        memmove(this->content, content, len);
        this->size = len;
        this->content[this->size] = 0;
    }

    String::String(const String& other)
        : String(other.content)
    { 
    }

    String::String()
        : content(NULL)
        , size(0)
    {
    }

    String::~String()
    {
        jadel::memoryFree(content);
        content = NULL;
        size = 0;
    }
*/
    String::String()
    {
    }

    String::String(const char* content)
    {
        init(this, content);
    }

    String::String(size_t length)
    {
        init(this, length);
    }

    bool String::init(String *string, size_t length)
    {
        if (!string || length == 0)
        {
            return false;
        }
/*        if (length <= JADEL_SMALL_STRING_SIZE)
        {
            string->isSmallString = true;
            string->smallString[length] = '\0';
        }*/
        //else
        //{
        //    string->isSmallString = false;
            string->content = (char *)jadel::memoryReserve(length + 1);
            string->content[length] = '\0';
        //}
        string->size = length;
        return true;
    }

    bool String::init(String *string, const char* content)
    {
        if (!string || !content || content[0] == '\0')
        {
            return false;
        }
        size_t len = strlen(content);
        init(string, len);
        /*if (string->isSmallString)
        {
            memmove(string->smallString, content, len);
        }
        else*/
        {
            memmove(string->content, content, len);
        }
        return true;
    }

    bool String::set(const char* content)
    {
        size_t len = strlen(content);
        if (len > this->size)
        {
            char* newContent = (char*)jadel::memoryReserve(len + 1); 
            if (!newContent) return false;
            jadel::memoryFree(this->content);
            this->content = newContent;
            size = len;
        }
        memmove(this->content, content, len);
        this->content[len] = '\0';
        return true;
    }

    void String::free()
    {
        //if (!this->isSmallString)
        {
            jadel::memoryFree(content);
        }
        content = NULL;
        size = 0;
    }

    const char *String::c_str() const
    {
        if (size == 0) return NULL;
        const char* result;
        /*if (this->isSmallString)
        {
            result = smallString;
        }
        else*/
        {
            result = content;
        }
        return result;    }

    char *String::c_str()
    {
        if (size == 0) return NULL;
        char* result;
        /*if (this->isSmallString)
        {
            result = smallString;
        }
        else*/
        {
            result = content;
        }
        return result;
    }

    char String::operator[](size_t index) const
    {
        char result;
        /*if (this->isSmallString)
        {
            result = smallString[index];
        }
        else*/
        {
            result = content[index];
        }
        return result;
    }

    String& String::operator=(const char* other)
    {
        String::init(this, other);
        return *this;
    }

    bool String::operator==(const String &other) const
    {
        bool result = *this == other.c_str();
        return result;
    }

    bool String::operator==(const char *other) const
    {
        bool result;
        /*if (this->isSmallString)
        {
            result = strncmp(this->smallString, other, this->size) == 0;
        }
        else*/
        { 
            result = strncmp(this->content, other, this->size) == 0;
        }
        return result;
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

    String String::operator+(const String& b) const
    {
        String result;
        const char* aContent = this->c_str();
        const char* bContent = b.c_str();
        size_t aLen = strlen(aContent);
        size_t bLen = strlen(bContent);
        size_t length =  aLen + bLen;
        String::init(&result, length);
        char* resultContent = result.c_str();
        memmove(resultContent, aContent, aLen);
        memmove(resultContent + aLen, bContent, bLen);
        return result;
    }

    String String::operator+(const char* other) const
    {
        String result = *this + String(other);
        return result;
    }
}