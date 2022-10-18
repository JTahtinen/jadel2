#pragma once

#include "jadel_defs.h"

namespace jadel
{
    struct DECLSPEC String
    {
        char *content;
        size_t size;

        String(const char *content);
        ~String();
        const char* c_str() const;
        char* c_str();
        char operator[](size_t index) const;
        bool operator==(const String& other) const;
        bool operator==(const char* other) const;
        bool operator!=(const String& other) const;
        bool operator!=(const char* other) const;
        void print() const;
    };

}