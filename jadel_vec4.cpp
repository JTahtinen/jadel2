#include "jadel_vec4.h"
#include <math.h>

namespace jadel
{
    Vec4::Vec4(float x, float y, float z, float w)
        : x(x)
        , y(y)
        , z(z)
        , w(w)
    {
    }

    Vec4::Vec4(float value)
        : Vec4(value, value, value, value)
    {
    }

    Vec4::Vec4()
        : Vec4(0)
    {
    }

    Vec4::Vec4(Vec3 vec)
        : Vec4(vec.x, vec.y, vec.z, 1.0f)
    {
    }
        
    Vec4 Vec4::add(Vec4 v1) const
    {
        Vec4 result(x + v1.x, y + v1.y, z + v1.z, w + v1.w);
        return result;
    }

    Vec4 Vec4::subst(Vec4 v1) const
    {
        Vec4 result(x - v1.x, y - v1.y, z - v1.z, w - v1.w);
        return result;
    }

    Vec4 Vec4::mul(float scalar) const
    {
        Vec4 result(x * scalar, y * scalar, z * scalar, w * scalar);
        return result;
    }

    float Vec4::length() const
    {
        float result = sqrtf(x * x + y * y + z * z + w * w);
        return result;
    }

    Vec4 Vec4::normalize() const
    {
        Vec4 result;
        float len = length();
        if (len == 0)
        {
            result = Vec4(0, 0, 0 ,0);
        }
        else
        {
            result = Vec4(x / len, y / len, z / len, w / len);
        }
        return result;
    }
}