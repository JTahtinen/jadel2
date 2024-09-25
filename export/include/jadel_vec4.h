#pragma once
#include "jadel_defs.h"
#include "jadel_vec3.h"
#include "jadel_vec2.h"
#include "jadel_math.h"

namespace jadel
{
    struct DECLSPEC Vec4
    {
        union
        {
            struct
            {
                float x, y, z, w;
            }; 
            float elements[4];
        };

        Vec4(float x, float y, float z, float w);
        Vec4(float value);
        Vec4();
        Vec4(Vec3 vec, float w);
        Vec4 add(Vec4 v1) const;
        Vec4 subst(Vec4 v1) const;
        Vec4 mul(float val) const;
        Vec4 div(float val) const;
        float length() const;
        Vec4 normalize() const;
        inline operator Vec3() const
        {
            Vec3 result(this->x, this->y, this->z);
            return result;
        }
        inline operator Vec2() const
        {
            Vec2 result(this->x, this->y);
            return result;
        }
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
    
    inline Vec4 operator/(Vec4 left, float right)
    {
        Vec4 result = left.div(right);
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

    inline Vec4 &operator*=(Vec4 &left, float scalar)
    {
        left = left.mul(scalar);
        return left;
    }

    inline Vec4 &operator/=(Vec4& left, float val)
    {
        left = left.div(val);
        return left;
    }

    inline bool operator==(Vec4 left, Vec4 right)
    {
        bool result = (compareFloat(left.x, right.x) && compareFloat(left.y, right.y) && compareFloat(left.z, right.z) && compareFloat(left.w, right.w));
        return result;
    }

    inline bool operator!=(Vec4 left, Vec4 right)
    {
        bool result = !(left == right);
        return result;
    }
}