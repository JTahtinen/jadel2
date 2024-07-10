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

    inline Mat4 &operator*=(Mat4 &left, Mat4 right)
    {
        left = left.mul(right);
        return left;
    }

    inline Vec4 operator*(Mat4 left, Vec4 right)
    {
        Vec4 result;

        for (int i = 0; i < 4; ++i)
        {
            for (int j = 0; j < 4; ++j)
            {
                result.elements[i] += left.elements[4 * i + j] * right.elements[j];
            }
        }
        return result;
    }

    inline Vec3 operator*(Mat4 left, Vec3 right)
    {
        Vec4 intermediate(right.x, right.y, right.z, 1);
        intermediate = left * intermediate;
        Vec3 result(intermediate.x, intermediate.y, intermediate.z); 
        return result;
    }

    inline Vec2 operator*(Mat4 left, Vec2 right)
    {
        Vec4 intermediate(right.x, right.y, 1, 1);
        intermediate = left * intermediate;
        Vec2 result(intermediate.x, intermediate.y); 
        return result;
    }

    inline Vec4 operator*(Vec4 left, Mat4 right)
    {
        Vec4 result;

        for (int i = 0; i < 4; ++i)
        {
            for (int j = 0; j < 4; ++j)
            {
                result.elements[i] += left.elements[j] * right.elements[4 * j + i];
            }
        }
        return result;
    }

    inline Vec3 operator*(Vec3 left, Mat4 right)
    {
        Vec4 intermediate(left.x, left.y, left.z, 1.0f);
        intermediate = intermediate * right;
        Vec3 result(intermediate.x, intermediate.y, intermediate.z);
        return result;
    }

    inline Vec2 operator*(Vec2 left, Mat4 right)
    {
        Vec4 intermediate(left.x, left.y, 1.0f, 1.0f);
        intermediate = intermediate * right;
        Vec2 result(intermediate.x, intermediate.y);
        return result;
    }

    inline Vec4 &operator*=(Vec4 &left, Mat4 right)
    {
        left = left * right;
        return left;
    }

    inline Vec3 &operator*=(Vec3 &left, Mat4 right)
    {
        left = left * right;
        return left;
    }

    inline Vec2 &operator*=(Vec2 &left, Mat4 right)
    {
        left = left * right;
        return left;
    }

}