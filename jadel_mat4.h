#pragma once
#include "jadel_vec4.h"
#include "jadel_vec3.h"
#include "jadel_vec2.h"

namespace jadel
{
    struct DECLSPEC Mat4
    {
        float elements[4 * 4];

        Mat4(float v0, float v1, float v2, float v3, float v4, float v5, float v6, float v7, float v8, float v9, float v10, float v11, float v12, float v13, float v14, float v15);
        Mat4();

        Mat4 mul(Mat4 other) const;
        Vec4 mul(Vec4 vec) const;
        Vec3 mul(Vec3 vec) const;
        Vec2 mul(Vec2 vec) const;
        static Mat4 identity();
    };

    inline Mat4 operator*(Mat4 left, Mat4 right)
    {
        Mat4 result = left.mul(right);
        return result;
    }

    inline Mat4 &operator*=(Mat4& left, Mat4 right)
    {
        left = left.mul(right);
        return left;
    }

    inline Vec4 operator*(Mat4 left, Vec4 right)
    {
        Vec4 result = left.mul(right);
        return result;
    }

    inline Vec3 operator*(Mat4 left, Vec3 right)
    {
        Vec3 result = left.mul(right);
        return result;
    }

    inline Vec2 operator*(Mat4 left, Vec2 other)
    {
        Vec2 result = left.mul(other);
        return result;
    }
}