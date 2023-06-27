#include "jadel_vec3.h"
#include <math.h>

namespace jadel
{
    Vec3::Vec3(float x, float y, float z)
        : x(x)
        , y(y)
        , z(z)
    {
    }
    Vec3::Vec3()
        : Vec3(0, 0, 0)
    {
    }
        
    Vec3 Vec3::add(Vec3 v1) const
    {
        Vec3 result(x + v1.x, y + v1.y, z + v1.z);
        return result;
    }

    Vec3 Vec3::subst(Vec3 v1) const
    {
        Vec3 result(x - v1.x, y - v1.y, z - v1.z);
        return result;
    }

    Vec3 Vec3::mul(float val) const
    {
        Vec3 result(x * val, y * val, z * val);
        return result;
    }

    Vec3 Vec3::cross(Vec3 other) const
    {
        Vec3 result;
        result.x = y * other.z - z * other.y;
        result.y = z * other.x - x * other.z;
        result.z = x * other.y - y * other.x;
        return result;
    }

    float Vec3::length() const
    {
        float result = sqrtf(x * x + y * y + z * z);
        return result;
    }

    Vec3 Vec3::normalize() const
    {
        float len = length();
        Vec3 result(x / len, y / len, z / len);
        return result;
    }
}