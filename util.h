#ifndef UTIL_H
#define UTIL_H
#include "defs.h"
#include <stdio.h>
#include <string.h>

namespace jadel
{
    inline void printHexAsString(uint8 *hex, size_t length, bool flipHexBytes)
    {
        for (int i = 0; i < length; ++i)
        {
            if (flipHexBytes)
            {
                printf("%c", (char)hex[length - 1 - i]);
            }
            else
            {
                printf("%c", (char)hex[i]);
            }
        }
        printf("\n");
    }

    inline bool compareHexToString(uint8 *hex, const char *str, bool flipHexBytes)
    {
        size_t len = strlen(str);
        for (int i = 0; i < len; ++i)
        {
            if (flipHexBytes)
            {
                if ((const char)hex[i] != str[len - 1 - i])
                    return false;
            }
            else
            {
                if ((const char)hex[i] != str[i])
                    return false;
            }
        }
        return true;
    }
}

#endif