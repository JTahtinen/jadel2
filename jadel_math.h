#pragma once

#include <stddef.h>
#include "jadel_defs.h"
#include "jadel_util.h"


#define SQRT2 (1.4142135624)

#define JADEL_CLAMP_MIN(val, min) \
    ((val) < (min) ? (min) : (val))

#define JADEL_CLAMP_MAX(val, max) \
    ((val) > (max) ? (max) : (val))

#define JADEL_CLAMP(val, min, max) \
    ((val) >= (min) ? ((val) <= (max) ? (val) : (max)) : (min))

/*
    min inclusive, max exclusive.
*/
#define JADEL_IS_VAL_BETWEEN(val, min, max) \
    (((val) >= (min)) && ((val) < (max)))

namespace jadel
{
    //**** DECLARATIONS **** //

    float square(float val);

    float clampf(float val, float min, float max);

    float lerp(float v0, float v1, float val);

    int roundToInt(float val);

    int absInt(int val);

    float absFloat(float val);

    bool compareFloat(float f0, float f1, float epsilon);

    bool compareFloat(float f0, float f1);

    //********************************************//

    //**** DEFINITIONS ****//


    inline float square(float val)
    {
        float result = val * val;
        return result;
    }

    inline float clampf(float val, float min, float max)
    {
        float result = JADEL_CLAMP(val, min, max);
        return result;
    }

    inline float lerp(float v0, float v1, float factor)
    {
        float result = v0 + (v1 - v0) * factor;
        return result;
    }

    inline int roundToInt(float val)
    {
        int result = val >= 0.0f ? (int)(val + 0.5f) : (int)(val - 0.5f);
        return result;
    }

    inline int absInt(int val)
    {
        int result = val < 0 ? -val : val;
        return result;
    }

    inline float absFloat(float val)
    {
        float result = val < 0.0f ? -val : val;
        return result;
    }

    inline bool compareFloat(float f0, float f1, float epsilon)
    {
        bool result = absFloat(f1 - f0) < epsilon;
        return result;
    }

    inline bool compareFloat(float f0, float f1)
    {
        bool result = compareFloat(f0, f1, 0.001f);
        return result;
    }
}