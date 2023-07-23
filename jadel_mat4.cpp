#include "jadel_mat4.h"
#include <memory.h>

#define GET_FROM(mat, col, row) ((mat).elements[col + row * 4])
#define GET(col, row) (GET_FROM(*this, col, row))
#define OTHER_GET(col, row) (GET_FROM(other, col, row))

namespace jadel
{
    Mat4::Mat4()
    {
        memset(&elements[0], 0, 16 * sizeof(float));
    }

    Mat4::Mat4(float v0, float v1, float v2, float v3, float v4, float v5, float v6, float v7, float v8, float v9, float v10, float v11, float v12, float v13, float v14, float v15)
    {
        elements[0] = v0;
        elements[1] = v1;
        elements[2] = v2;
        elements[3] = v3;
        elements[4] = v4;
        elements[5] = v5;
        elements[6] = v6;
        elements[7] = v7;
        elements[8] = v8;
        elements[9] = v9;
        elements[10] = v10;
        elements[11] = v11;
        elements[12] = v12;
        elements[13] = v13;
        elements[14] = v14;
        elements[15] = v15;
    }

    Mat4 Mat4::mul(Mat4 other) const
    {

        Mat4 result;
        for (int row = 0; row < 4; ++row)
        {
            for (int col = 0; col < 4; ++col)
            {
                result.elements[col + row * 4] =
                    GET(0, row) * OTHER_GET(col, 0) +
                    GET(1, row) * OTHER_GET(col, 1) +
                    GET(2, row) * OTHER_GET(col, 2) +
                    GET(3, row) * OTHER_GET(col, 3);
            }
        }
        return result;
    }

    Vec4 Mat4::mul(Vec4 vec) const
    {
        Vec4 result(vec.x * GET(0, 0) + vec.y * GET(0, 1) + vec.z * GET(0, 2) + vec.w * GET(0, 3),
                    vec.x * GET(1, 0) + vec.y * GET(1, 1) + vec.z * GET(1, 2) + vec.w * GET(1, 3),
                    vec.x * GET(2, 0) + vec.y * GET(2, 1) + vec.z * GET(2, 2) + vec.w * GET(2, 3),
                    vec.x * GET(3, 0) + vec.y * GET(3, 1) + vec.z * GET(3, 2) + vec.w * GET(3, 3));
        return result;
    }

    Vec3 Mat4::mul(Vec3 vec) const
    {
        Vec4 intermediate = mul(Vec4(vec.x, vec.y, vec.z, 1));
        Vec3 result(intermediate.x, intermediate.y, intermediate.z);
        return result;
    }

    Vec2 Mat4::mul(Vec2 vec) const
    {
        Vec3 intermediate = mul(Vec3(vec.x, vec.y, 1));
        Vec2 result(intermediate.x, intermediate.y);
        return result;
    }

    Mat4 Mat4::identity()
    {
        Mat4 result;
        GET_FROM(result, 0, 0) = 1.0f;
        GET_FROM(result, 1, 1) = 1.0f;
        GET_FROM(result, 2, 2) = 1.0f;
        GET_FROM(result, 3, 3) = 1.0f;
        return result;
    }
}