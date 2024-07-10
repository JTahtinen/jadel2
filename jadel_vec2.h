#pragma once
#include "jadel_defs.h"
#include "jadel_point2i.h"
#include "jadel_math.h"

namespace jadel
{
    struct DECLSPEC Vec2
    {
        union
        {
            struct
            {
                float x, y;
            };
            float elements[2];
        };

        Vec2();
        Vec2(float x, float y);
        Vec2 add(Vec2 v1) const;
        Vec2 subst(Vec2 v1) const;
        Vec2 mul(float scalar) const;
        float length() const;
        float dot(Vec2 b) const;
        Vec2 normalize() const;

        inline operator Point2i() const
        {
            Point2i result((int)this->x, (int)this->y);
            return result;
        }
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

    inline Vec2 operator*(Vec2 left, float scalar)
    {
        Vec2 result = left.mul(scalar);
        return result;
    }

    inline float operator*(Vec2 left, Vec2 right)
    {
        float result = left.dot(right);
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

    inline Vec2 &operator*=(Vec2 &left, float scalar)
    {
        left = left.mul(scalar);
        return left;
    }

    inline bool operator==(Vec2 left, Vec2 right)
    {
        bool result = (compareFloat(left.x, right.x) && compareFloat(left.y, right.y));
        return result;
    }

    inline bool operator!=(Vec2 left, Vec2 right)
    {
        bool result = !(left == right);
        return result;
    }
}