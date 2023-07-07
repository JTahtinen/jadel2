#include "jadel_vec2.h"
#include <math.h>

namespace jadel
{
    Vec2::Vec2(float x, float y)
        : x(x)
        , y(y)
    {
    }

    Vec2::Vec2()
        : Vec2(0, 0)
    {
    }
        
    Vec2 Vec2::add(Vec2 v1) const
    {
        Vec2 result(x + v1.x, y + v1.y);
        return result;
    }

    Vec2 Vec2::subst(Vec2 v1) const
    {
        Vec2 result(x - v1.x, y - v1.y);
        return result;
    }

    Vec2 Vec2::mul(float val) const
    {
        Vec2 result(x * val, y * val);
        return result;
    }

    float Vec2::length() const
    {
        float result = sqrtf(x * x + y * y);
        return result;
    }

    float Vec2::dot(Vec2 b) const
    {
        float result = x * b.x + y * b.y;
        return result;
    }

    Vec2 Vec2::normalize() const
    {
        float len = length();
        Vec2 result(x / len, y / len);
        return result;
    }
}