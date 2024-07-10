#include "jadel_graphics_internal.h"

namespace jadel
{
    int pixelAlphaIndex = 0;
    int pixelRedIndex = 0;
    int pixelGreenIndex = 0;
    int pixelBlueIndex = 0;

    void Graphics::drawPixelFast(int x, int y, uint32 color)
    {
        JADEL_GRAPHICS_DRAW_PIXEL_FAST(x, y, color);
    }

    void Graphics::drawPixelFast(int x, int y, float a, float r, float g, float b)
    {
        Pixel pixel;
        Color finalColor;
        if (a < 1.0f)
        {
            drawPixelFast(x, y, Color(a, r, g, b) * (JADEL_GRAPHICS_GET_BACKGROUND_PIXEL(x, y)));
        }

        pixel.elems[pixelAlphaIndex] = (uint8)(a * 255.0f);
        pixel.elems[pixelRedIndex] = (uint8)(r * 255.0f);
        pixel.elems[pixelGreenIndex] = (uint8)(g * 255.0f);
        pixel.elems[pixelBlueIndex] = (uint8)(b * 255.0f);
        drawPixelFast(x, y, pixel.content);
    }

    void Graphics::drawPixelFast(int x, int y, Color color)
    {
        drawPixelFast(x, y, color.a, color.r, color.g, color.b);
    }

    void Graphics::drawPixelRelativeFast(int x, int y, Color color)
    {
        drawPixelFast(x + targetSurface->halfWidth, y + targetSurface->halfHeight, color);
    }

     void Graphics::drawPixelRelativeFast(int x, int y, uint32 color)
    {
        drawPixelFast(x + targetSurface->halfWidth, y + targetSurface->halfHeight, color);
    }
}