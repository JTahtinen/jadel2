#pragma once
#include "jadel_vec4.h"
#include "jadel_vec3.h"
#include "jadel_vec2.h"

namespace jadel
{
    struct DECLSPEC Mat4
    {
        float elements[4 * 4];
        Mat4 mul(Mat4 other);
        Vec4 mul(Vec4 vec);
        Vec3 mul(Vec3 vec);
        Vec2 mul(Vec2 vec);
        Mat4(float v0, float v1, float v2, float v3, float v4, float v5, float v6, float v7, float v8, float v9, float v10, float v11, float v12, float v13, float v14, float v15);
        Mat4();
        static Mat4 identity();
    };
}