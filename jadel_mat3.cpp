#include "jadel_mat3.h"
#include <memory.h>

#define GET_FROM(mat, col, row) ((mat).elements[col + row * 3])
#define GET(col, row) (GET_FROM(*this, col, row))
#define OTHER_GET(col, row) (GET_FROM(other, col, row))

namespace jadel
{
    Mat3::Mat3()
    {
        memset(&elements[0], 0, 9 * sizeof(float));
    }

    Mat3::Mat3(float v0, float v1, float v2, float v3, float v4, float v5, float v6, float v7, float v8)
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
    }

    Mat3 Mat3::mul(const Mat3 other) const
    {
        Mat3 result;
        for (int row = 0; row < 3; ++row)
        {
            for (int col = 0; col < 3; ++col)
            {
                result.elements[col + row * 3] =
                    GET(0, row) * OTHER_GET(col, 0) +
                    GET(1, row) * OTHER_GET(col, 1) +
                    GET(2, row) * OTHER_GET(col, 2);
            }
        }
        return result;
    }

    Vec3 Mat3::mul(const Vec3 vec) const
    {
        Vec3 result(vec.x * GET(0, 0) + vec.y * GET(0, 1) + vec.z * GET(0, 2),
                    vec.x * GET(1, 0) + vec.y * GET(1, 1) + vec.z * GET(1, 2),
                    vec.x * GET(2, 0) + vec.y * GET(2, 1) + vec.z * GET(2, 2));
        return result;
    }

    Vec2 Mat3::mul(const Vec2 vec) const
    {
        Vec3 intermediate = mul(Vec3(vec.x, vec.y, 1));
        Vec2 result(intermediate.x, intermediate.y);
        return result;
    }

    Mat3 Mat3::identity()
    {
        Mat3 result;
        GET_FROM(result, 0, 0) = 1.0f;
        GET_FROM(result, 1, 1) = 1.0f;
        GET_FROM(result, 2, 2) = 1.0f;
        return result;
    }
}