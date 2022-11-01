#pragma once
#include "jadel_defs.h"

namespace jadel
{
    struct DECLSPEC Vec3
    {
        float x, y, z;

        Vec3(float x, float y, float z);
        Vec3();
        Vec3 add(Vec3 v1) const;
        Vec3 subst(Vec3 v1) const;
        Vec3 mul(float val) const;
        float length() const;
        Vec3 normalize() const;
    };

    inline Vec3 operator+(Vec3 left, Vec3 right)
    {
        Vec3 result = left.add(right);
        return result;
    }

    inline Vec3 operator-(Vec3 left, Vec3 right)
    {
        Vec3 result = left.subst(right);
        return result;
    }

    inline Vec3 operator*(Vec3 left, float right)
    {
        Vec3 result = left.mul(right);
        return result;
    }

    inline Vec3 &operator+=(Vec3 &left, Vec3 right)
    {
        left = left.add(right);
        return left;
    }

    inline Vec3 &operator-=(Vec3 &left, Vec3 right)
    {
        left = left.subst(right);
        return left;
    }

    inline Vec3 &operator*=(Vec3 &left, float right)
    {
        left = left.mul(right);
        return left;
    }

    inline bool operator==(Vec3 left, Vec3 right)
    {
        bool result = (left.x == right.x && left.y == right.y);
        return result;
    }

    inline bool operator!=(Vec3 left, Vec3 right)
    {
        bool result = !(left == right);
        return result;
    }
}