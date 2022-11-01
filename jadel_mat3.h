#pragma once
#include "jadel_vec3.h"
#include "jadel_vec2.h"

namespace jadel
{
    struct DECLSPEC Mat3
    {
        float elements[3 * 3];
        Mat3 mul(Mat3 other);
        Vec3 mul(Vec3 vec);
        Vec2 mul(Vec2 vec);
        Mat3(float v0, float v1, float v2, float v3, float v4, float v5, float v6, float v7, float v8);
        Mat3();
        static Mat3 identity();
    };
}