#pragma once
#include "jadel_defs.h"
#include "jadel_vec3.h"

namespace jadel
{
    struct DECLSPEC Vec4
    {
        float x, y, z, w;

        Vec4(float x, float y, float z, float w);
        Vec4(float value);
        Vec4();
        Vec4(Vec3 vec);
        Vec4 add(Vec4 v1) const;
        Vec4 subst(Vec4 v1) const;
        Vec4 mul(float val) const;
        float length() const;
        Vec4 normalize() const;
    };

    inline Vec4 operator+(Vec4 left, Vec4 right)
    {
        Vec4 result = left.add(right);
        return result;
    }

    inline Vec4 operator-(Vec4 left, Vec4 right)
    {
        Vec4 result = left.subst(right);
        return result;
    }

    inline Vec4 operator*(Vec4 left, float right)
    {
        Vec4 result = left.mul(right);
        return result;
    }

    inline Vec4 &operator+=(Vec4 &left, Vec4 right)
    {
        left = left.add(right);
        return left;
    }

    inline Vec4 &operator-=(Vec4 &left, Vec4 right)
    {
        left = left.subst(right);
        return left;
    }

    inline Vec4 &operator*=(Vec4 &left, float right)
    {
        left = left.mul(right);
        return left;
    }

    inline bool operator==(Vec4 left, Vec4 right)
    {
        bool result = (left.x == right.x && left.y == right.y && left.z == right.z && left.w == right.w);
        return result;
    }

    inline bool operator!=(Vec4 left, Vec4 right)
    {
        bool result = !(left == right);
        return result;
    }
}