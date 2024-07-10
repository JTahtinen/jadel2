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

    inline Mat3 &operator*=(Mat3 &left, const Mat3 right)
    {
        left = left.mul(right);
        return left;
    }

    inline Vec3 operator*(const Mat3 left, const Vec3 right)
    {
        Vec3 result;

        for (int i = 0; i < 3; ++i)
        {
            for (int j = 0; j < 3; ++j)
            {
                result.elements[i] += left.elements[3 * i + j] * right.elements[j];
            }
        }
        return result;
    }

    inline Vec2 operator*(Mat3 left, Vec2 right)
    {
        Vec3 intermediate(right.x, right.y, 1);
        intermediate = left * intermediate;
        Vec2 result(intermediate.x, intermediate.y);
        return result;
    }

    inline Vec3 operator*(Vec3 left, Mat3 right)
    {
        Vec3 result;

        for (int i = 0; i < 3; ++i)
        {
            for (int j = 0; j < 3; ++j)
            {
                result.elements[i] += left.elements[j] * right.elements[4 * j + i];
            }
        }
        return result;
    }

    inline Vec2 operator*(Vec2 left, Mat3 right)
    {
        Vec3 intermediate(left.x, left.y, 1.0f);
        intermediate = intermediate * right;
        Vec2 result(intermediate.x, intermediate.y);
        return result;
    }

    inline Vec3 &operator*=(Vec3 &left, Mat3 right)
    {
        left = left * right;
        return left;
    }

    inline Vec2 &operator*=(Vec2 &left, Mat3 right)
    {
        left = left * right;
        return left;
    }

}