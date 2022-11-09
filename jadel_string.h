#pragma once

#include <stdio.h>
#include "jadel_defs.h"

#define JADEL_SMALL_STRING_SIZE (50)
namespace jadel
{
    struct DECLSPEC String
    {
        char smallString[JADEL_SMALL_STRING_SIZE + 1];
        char *content;
        size_t size;
        bool isSmallString;

        // String(const char *content);
        // String(const String& other);
        // String();
        //~String();
        String();
        String(const char* content);
        static bool init(String *string, size_t length);
        static bool init(String *string, const char *content);
        void free();
        const char *c_str() const;
        char *c_str();
        char operator[](size_t index) const;
        String &operator=(const char *other);
        bool operator==(const String &other) const;
        bool operator==(const char *other) const;
        bool operator!=(const String &other) const;
        bool operator!=(const char *other) const;
        String operator+(const String &other) const;
        String operator+(const char* other) const;
    };
    
    static char stringBuffer[50];
    
    inline String toString(int val)
    {
        sprintf(stringBuffer, "%d", val);
        // message("%s\n", stringBuffer);
        String result;
        String::init(&result, stringBuffer);
        return result;
    }

    
    inline String toString(size_t val)
    {
        sprintf(stringBuffer, "%zd", val);
        // message("%s\n", stringBuffer);
        String result;
        String::init(&result, stringBuffer);
        return result;
    }

    inline String toString(unsigned int val)
    {
        sprintf(stringBuffer, "%d", val);
        // message("%s\n", stringBuffer);
        String result;
        String::init(&result, stringBuffer);
        return result;
    }

    inline String toString(float val)
    {
        sprintf(stringBuffer, "%f", val);
        // message("%s\n", stringBuffer);
        String result;
        String::init(&result, stringBuffer);
        return result;
    }

    inline String toString(double val)
    {
        sprintf(stringBuffer, "%f", val);
        // message("%s\n", stringBuffer);
        String result;
        String::init(&result, stringBuffer);
        return result;
    }
}