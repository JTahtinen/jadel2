#pragma once

#include "jadel_defs.h"
#include <stdio.h>
#include <string.h>

/*
    IMPORTANT: Pass by value, not by address.
*/
#define JADEL_SWAP(val0, val1) \
    {                          \
        auto temp = val0;      \
        val0 = val1;           \
        val1 = temp;           \
    }

/*
    IMPORTANT: Pass by value, not by address.
*/
#define JADEL_SWAP_IF_COMPARISON_TRUE(val0, val1, comparison) \
    {                                                         \
        if ((val0)comparison(val1))                           \
        {                                                     \
            JADEL_SWAP(val0, val1);                           \
        }                                                     \
    }

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

    inline void flipBytes(void *buffer, size_t numBytes)
    {
        uint8 *pointer = (uint8 *)buffer;
        for (size_t i = 0; i < numBytes / 2; ++i)
        {
            uint8 startByte = *(pointer + i);
            *(pointer + i) = *(pointer + numBytes - 1 - i);
            *(pointer + numBytes - 1 - i) = startByte;
        }
    }
}