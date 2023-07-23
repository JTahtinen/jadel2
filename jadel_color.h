#pragma once
#include "jadel_math.h"
#include "jadel_defs.h"

namespace jadel
{
    struct DECLSPEC Color
    {
        float a;
        float r;
        float g;
        float b;

        Color(float a, float r, float g, float b);
        Color(uint32 color);
        Color();

        uint32 toU32() const;
        Color addTo(Color background) const;
        Color addTo(uint32 background) const;
        uint32 addToU32(Color background) const;
        uint32 addToU32(uint32 background) const;
        Color blend(Color background) const;
        Color blend(uint32 background) const;
        uint32 blendU32(Color background) const;
        uint32 blendU32(uint32 background) const;

        static Color blend(uint32 foreground, uint32 background);
        static Color blend(uint32 foreground, Color background);
        static Color addTo(uint32 foreground, uint32 background);
        static Color addTo(uint32 foreground, Color background);

        static uint32 blendU32(uint32 foreground, uint32 background);
        static uint32 blendU32(uint32 foreground, Color background);
        static uint32 addToU32(uint32 foreground, uint32 background);
        static uint32 addToU32(uint32 foreground, Color background);
    };

    inline Color operator+(Color left, Color right)
    {
        Color result = left.addTo(right);
        return result;
    }

    inline Color operator+(Color left, uint32 right)
    {
        Color result = left.addTo(right);
        return result;
    }

    inline Color operator+(uint32 left, Color right)
    {
        Color result = Color::addTo(left, right);
        return result;
    }

    inline Color &operator+=(Color &left, Color right)
    {
        left = left.addTo(right);
        return left;
    }

    inline Color &operator+=(Color &left, uint32 right)
    {
        left = left.addTo(right);
        return left;
    }

    inline uint32 &operator+=(uint32 &left, Color right)
    {
        uint32 result = Color::addToU32(left, right);
        return result;
    }

    inline Color operator*(Color left, float right)
    {
        Color result;
        result.r = clampf(left.r * right, 0, 1.0f);
        result.g = clampf(left.g * right, 0, 1.0f);
        result.b = clampf(left.b * right, 0, 1.0f);
        return result;
    }

    inline Color operator*(Color left, Color right)
    {
        Color result = left.blend(right);
        return result;
    }

    inline Color operator*(Color left, uint32 right)
    {
        Color result = left.blend(right);
        return result;
    }

    inline Color operator*(uint32 left, Color right)
    {
        Color result = Color::blend(left, right);
        return result;
    }

    inline Color &operator*=(Color &left, uint32 right)
    {
        left = left.blend(right);
        return left;
    }

    inline Color &operator*=(Color &left, Color right)
    {
        left = left.blend(right);
        return left;
    }

    inline uint32 &operator*=(uint32 &left, Color right)
    {
        left = Color::blendU32(left, right);
        return left;
    }
}