#include "jadel_color.h"
#include "jadel_endian.h"

namespace jadel
{

    Color::Color(float a, float r, float g, float b)
        : a(a), r(r), g(g), b(b)
    {
    }

    Color::Color(uint32 color)
        : Color((float)((color & ALPHA_MASK) >> ALPHA_SHIFT) / 255.0f,
                (float)((color & RED_MASK) >> RED_SHIFT) / 255.0f,
                (float)((color & GREEN_MASK) >> GREEN_SHIFT) / 255.0f,
                (float)((color & BLUE_MASK) >> BLUE_SHIFT) / 255.0f)
    {
    }

    Color::Color()
    {
    }

    uint32 Color::toU32() const
    {
        uint32 result =
            (uint32)(this->a * 255.0f) << ALPHA_SHIFT & ALPHA_MASK |
            (uint32)(this->r * 255.0f) << RED_SHIFT & RED_MASK |
            (uint32)(this->g * 255.0f) << GREEN_SHIFT & GREEN_MASK |
            (uint32)(this->b * 255.0f) << BLUE_SHIFT & BLUE_MASK;
        return result;
    }

    Color Color::addTo(Color background) const
    {
        float r = clampf(background.r + this->r * this->a, 0, 1.0f);
        float g = clampf(background.g + this->g * this->a, 0, 1.0f);
        float b = clampf(background.b + this->b * this->a, 0, 1.0f);
        return Color(1.0f, r, g, b);
    }

    Color Color::addTo(uint32 background) const
    {
        Color bgColor(background);
        Color result = this->addTo(bgColor);
        return result;
    }

    uint32 Color::addToU32(Color background) const
    {
        uint32 result = (this->addTo(background)).toU32();
        return result;
    }

    uint32 Color::addToU32(uint32 background) const
    {
        uint32 result = this->addToU32(Color(background));
        return result;
    }

    Color Color::blend(Color background) const
    {
        float r = lerp(this->r, background.r, 1.0f - this->a);
        float g = lerp(this->g, background.g, 1.0f - this->a);
        float b = lerp(this->b, background.b, 1.0f - this->a);
        Color result(1.0f, r, g, b);
        return result;
    }

    Color Color::blend(uint32 background) const
    {
        Color result = this->blend(Color(background));
        return result;
    }

    uint32 Color::blendU32(Color background) const
    {
        Color color = this->blend(background);
        uint32 result = (uint8)0xff << ALPHA_SHIFT | (uint8)(color.r * 255.0f) << RED_SHIFT | (uint8)(color.g * 255.0f) << GREEN_SHIFT | (uint8)(color.b * 255.0f) << BLUE_SHIFT;
        return result;
    }

    uint32 Color::blendU32(uint32 background) const
    {
        uint32 result = this->blendU32(Color(background));
        return result;
    }

    Color Color::blend(uint32 foreground, uint32 background)
    {
        Color result = Color(foreground).blend(background);
        return result;
    }

    Color Color::blend(uint32 foreground, Color background)
    {
        Color result = Color(foreground).blend(background);
        return result;
    }

    Color Color::addTo(uint32 foreground, uint32 background)
    {
        Color result = Color(foreground).addTo(background);
        return result;
    }

    Color Color::addTo(uint32 foreground, Color background)
    {
        Color result = Color(foreground).blend(background);
        return result;
    }

    uint32 Color::blendU32(uint32 foreground, uint32 background)
    {
        Color fgColor(foreground);
        uint32 result = fgColor.blendU32(background);
        return result;
    }

    uint32 Color::blendU32(uint32 foreground, Color background)
    {
        uint32 result = Color(foreground).blendU32(Color(background));
        return result;
    }

    uint32 Color::addToU32(uint32 foreground, uint32 background)
    {
        uint32 result = Color(foreground).addToU32(background);
        return result;
    }

    uint32 Color::addToU32(uint32 foreground, Color background)
    {
        uint32 result = Color(foreground).addToU32(Color(background));
        return result;
    }
}