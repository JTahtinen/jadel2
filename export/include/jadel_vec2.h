#pragma once
#include "jadel_defs.h"

namespace jadel
{
    struct DECLSPEC Vec2
    {
        float x, y;
        Vec2();
        Vec2(float x, float y);
        Vec2 add(Vec2 v1) const;
        Vec2 subst(Vec2 v1) const;
        Vec2 mul(float val) const;
        float length() const;
        Vec2 normalize() const;
    };

    inline Vec2 operator+(Vec2 left, Vec2 right)
    {
        Vec2 result = left.add(right);
        return result;
    }

    inline Vec2 operator-(Vec2 left, Vec2 right)
    {
        Vec2 result = left.subst(right);
        return result;
    }

    inline Vec2 operator*(Vec2 left, float right)
    {
        Vec2 result = left.mul(right);
        return result;
    }

    inline Vec2 &operator+=(Vec2 &left, Vec2 right)
    {
        left = left.add(right);
        return left;
    }

    inline Vec2 &operator-=(Vec2 &left, Vec2 right)
    {
        left = left.subst(right);
        return left;
    }

    inline Vec2 &operator*=(Vec2 &left, float right)
    {
        left = left.mul(right);
        return left;
    }

    inline bool operator==(Vec2 left, Vec2 right)
    {
        bool result = (left.x == right.x && left.y == right.y);
        return result;
    }

    inline bool operator!=(Vec2 left, Vec2 right)
    {
        bool result = !(left == right);
        return result;
    }
}