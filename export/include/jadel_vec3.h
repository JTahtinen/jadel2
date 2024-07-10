#pragma once
#include "jadel_defs.h"
#include "jadel_vec2.h"
#include "jadel_math.h"

namespace jadel
{
    struct DECLSPEC Vec3
    {
        union
        {
            struct
            {
                float x, y, z;
            };
            float elements[3];
        };

        Vec3(float x, float y, float z);
        Vec3();
        Vec3 add(Vec3 v1) const;
        Vec3 subst(Vec3 v1) const;
        Vec3 mul(float val) const;
        Vec3 cross(Vec3 other) const;
        float length() const;
        Vec3 normalize() const;

        inline operator Vec2() const
        {
            Vec2 result(this->x, this->y);
            return result;
        }
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

    inline Vec3 operator*(Vec3 left, float scalar)
    {
        Vec3 result = left.mul(scalar);
        return result;
    }

    inline Vec3 operator*(Vec3 left, Vec3 right)
    {
        Vec3 result = left.cross(right);
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

    inline Vec3 &operator*=(Vec3 &left, float scalar)
    {
        left = left.mul(scalar);
        return left;
    }

    inline Vec3 &operator*=(Vec3 &left, Vec3 right)
    {
        left = left.cross(right);
        return left;
    }

    inline bool operator==(Vec3 left, Vec3 right)
    {
        bool result = (compareFloat(left.x, right.x) && compareFloat(left.y, right.y) && compareFloat(left.z, right.z));
        return result;
    }

    inline bool operator!=(Vec3 left, Vec3 right)
    {
        bool result = !(left == right);
        return result;
    }
}