#pragma once
#include "jadel_vec3.h"
#include "jadel_vec2.h"

namespace jadel
{
    struct DECLSPEC Mat3
    {
        float elements[3 * 3];

        Mat3(float v0, float v1, float v2, float v3, float v4, float v5, float v6, float v7, float v8);
        Mat3();

        Mat3 mul(const Mat3 other) const;
        Vec3 mul(const Vec3 vec) const;
        Vec2 mul(const Vec2 vec) const;
        static Mat3 identity();
    };
    inline Mat3 operator*(const Mat3 left, const Mat3 right)
    {
        Mat3 result = left.mul(right);
        return result;
    }

    inline Mat3 &operator*=(Mat3& left, const Mat3 right)
    {
        left = left.mul(right);
        return left;
    }

    inline Vec3 operator*(const Mat3 left, const Vec3 right)
    {
        Vec3 result = left.mul(right);
        return result;
    }

    inline Vec2 operator*(const Mat3 left, const Vec2 right)
    {
        Vec2 result = left.mul(right);
        return result;
    }
}