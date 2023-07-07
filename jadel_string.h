#pragma once

#include <stdio.h>
#include "jadel_defs.h"

//#define JADEL_SMALL_STRING_SIZE (50)
namespace jadel
{
    struct DECLSPEC String
    {
        //char smallString[JADEL_SMALL_STRING_SIZE + 1];
        char *content;
        size_t size;
  //      bool isSmallString;

        // String(const char *content);
        // String(const String& other);
        // String();
        //~String();
        String();
        String(const char* content);
        String(size_t length);
        static bool init(String *string, size_t length);
        static bool init(String *string, const char *content);
        bool set(const char* content);
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
    
    #define JADEL_VAL_TO_STRING_BUFFER_SIZE (50)
    
    static char valueToStringBuffer[JADEL_VAL_TO_STRING_BUFFER_SIZE] = {0};
    
    inline String toString(int val)
    {
        snprintf(valueToStringBuffer, JADEL_VAL_TO_STRING_BUFFER_SIZE, "%d", val);
        // message("%s\n", stringBuffer);
        String result;
        String::init(&result, valueToStringBuffer);
        return result;
    }
    
    inline String toString(size_t val)
    {
        snprintf(valueToStringBuffer, JADEL_VAL_TO_STRING_BUFFER_SIZE, "%zd", val);
        // message("%s\n", stringBuffer);
        String result;
        String::init(&result, valueToStringBuffer);
        return result;
    }

    inline String toString(unsigned int val)
    {
        snprintf(valueToStringBuffer, JADEL_VAL_TO_STRING_BUFFER_SIZE - 1, "%d", val);
        // message("%s\n", stringBuffer);
        String result;
        String::init(&result, valueToStringBuffer);
        return result;
    }

    inline String toString(float val)
    {
        snprintf(valueToStringBuffer, JADEL_VAL_TO_STRING_BUFFER_SIZE - 1, "%f", val);
        // message("%s\n", stringBuffer);
        String result;
        String::init(&result, valueToStringBuffer);
        return result;
    }

    inline String toString(double val)
    {
        snprintf(valueToStringBuffer, JADEL_VAL_TO_STRING_BUFFER_SIZE - 1, "%f", val);
        // message("%s\n", stringBuffer);
        String result;
        String::init(&result, valueToStringBuffer);
        return result;
    }
}