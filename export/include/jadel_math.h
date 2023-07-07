#pragma once

#include <stddef.h>
#include "jadel_defs.h"
#include "jadel_vec2.h"
#include "jadel_vec3.h"
#include "jadel_mat3.h"

namespace jadel
{

    struct Point2i
    {
        int x;
        int y;

        Point2i operator+(Point2i other) const
        {
            Point2i result = {x + other.x, y + other.y};
            return result;
        }

        Point2i operator-(Point2i other) const
        {
            Point2i result = {x - other.x, y - other.y};
            return result;
        }
        
        Point2i& operator+=(Point2i other)
        {
            *this = {x + other.x, y + other.y};
            return *this;
        }

        Point2i operator-=(Point2i other)
        {
            *this = {x - other.x, y - other.y};
            return *this;        
        }

        bool operator==(Point2i other) const
        {
            bool result = (this->x == other.x && this->y == other.y);
            return result;
        }

        bool operator!=(Point2i other) const
        {
            bool result = !(*this == other);
            return result;
        }

    };

    struct Recti
    {
        int x0;
        int y0;
        int x1;
        int y1;
        
    };

    struct Rectf
    {
        float x0;
        float y0;
        float x1;
        float y1;

        Rectf(float x0, float y0, float x1, float y1)
            : x0(x0)
            , y0(y0)
            , x1(x1)
            , y1(y1)
        {
        }
        
        Rectf(Vec2 p0, Vec2 p1)
            : Rectf(p0.x, p0.y, p1.x, p1.y)
        {
        }

        Rectf()
        {
        }

        Vec2 getPoint0() const
        {
            Vec2 result(x0, y0);
            return result;
        }

        Vec2 getPoint1() const
        {
            Vec2 result(x1, y1);
            return result;
        }
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

    inline float absFloat(float val)
    {
        float result = val < 0 ? -val : val;
        return result;
    }
}