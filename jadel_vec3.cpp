#include "jadel_vec3.h"
#include <math.h>

namespace jadel
{
    Vec3::Vec3(float x, float y, float z)
        : x(x), y(y), z(z)
    {
    }
    Vec3::Vec3()
        : Vec3(0, 0, 0)
    {
    }

    Vec3::Vec3(Vec2 vec, float z)
        : Vec3(vec.x, vec.y, z)
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

    Vec3 Vec3::mul(float scalar) const
    {
        Vec3 result(x * scalar, y * scalar, z * scalar);
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

    float Vec3::dot(Vec3 other) const
    {
        float result = this->x * other.x + this->y * other.y + this->z * other.z;
        return result;
    }

    float Vec3::length() const
    {
        float result = sqrtf(x * x + y * y + z * z);
        return result;
    }

    Vec3 Vec3::normalize() const
    {
        Vec3 result;
        float len = length();
        if (len == 0)
        {
            result = Vec3(0, 0, 0);
        }
        else
        {
            result = Vec3(x / len, y / len, z / len);
        }
        return result;
    }
}