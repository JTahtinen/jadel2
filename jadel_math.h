#pragma once

#include <stddef.h>
#include "jadel_defs.h"

namespace jadel
{

    struct Point2i
    {
        int x;
        int y;
    };

    struct Recti
    {
        int x;
        int y;
        int w;
        int h;
        
    };

    inline float clampf(float val, float min, float max)
    {
        float result;
        if (val < min)
            result = min;
        else if (val > max)
            result = max;
        else
            result = val;
        return result;
    }

    inline float lerp(float v0, float v1, float val)
    {
        float result = v0 + (v1 - v0) * val;
        return result;
    }

    inline int roundToInt(float val)
    {
        int result = val > 0 ? (int)(val + 0.5f) : (int)(val - 0.5f);
        return result;
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

    inline int absInt(int val)
    {
        int result = val < 0 ? -val : val;
        return result;
    }
}