#include "jadel_graphics.h"
#include "jadel_math.h"
#include "jadel_update.h"
#include <stdlib.h>
#include <stdio.h>
#include "jadel_vector.h"

namespace jadel
{
    int aIndex;
    int rIndex;
    int gIndex;
    int bIndex;

#define TARGET_SURFACE_STACK_SIZE (20)

    static Surface *targetSurface;

    static Vector<Surface *> targetSurfaceStack(TARGET_SURFACE_STACK_SIZE);

    static uint32 clearColor = 0;

    void graphicsInit()
    {
        if (endianness == LITTLE_ENDIAN)
        {
            aIndex = 3;
            rIndex = 2;
            gIndex = 1;
            bIndex = 0;
        }
        else
        {
            aIndex = 0;
            rIndex = 1;
            gIndex = 2;
            bIndex = 3;
        }
    }

    bool graphicsPushTargetSurface(Surface *target)
    {
        if (!target || targetSurfaceStack.size == TARGET_SURFACE_STACK_SIZE)
            return false;
        targetSurfaceStack.push(target);
        targetSurface = target;
        return true;
    }

    bool graphicsPopTargetSurface()
    {
        if (!targetSurfaceStack.pop())
            return false;
        if (targetSurfaceStack.size > 0)
        {
            targetSurface = *targetSurfaceStack.back();
        }
        else
            targetSurface = NULL;

        return true;
    }

    void graphicsDrawPixelFast(int x, int y, uint32 color)
    {
        uint32 *pixels = (uint32 *)targetSurface->pixels;
        pixels[x + y * targetSurface->width] = color;
    }

    void graphicsDrawPixelFast(int x, int y, float a, float r, float g, float b)
    {
        a = clampf(a, 0, 1.0f);
        r = clampf(r, 0, 1.0f);
        g = clampf(g, 0, 1.0f);
        b = clampf(b, 0, 1.0f);

        Pixel pixel;
        pixel.elems[aIndex] = a * 255;
        pixel.elems[rIndex] = r * 255;
        pixel.elems[gIndex] = g * 255;
        pixel.elems[bIndex] = b * 255;

        graphicsDrawPixelFast(x, y, pixel.content);
    }

    void graphicsDrawRect(int xStart, int xEnd, int yStart, int yEnd, uint32 color)
    {
        if (xStart >= targetSurface->width ||
            yStart >= targetSurface->height ||
            xEnd <= 0 ||
            yEnd <= 0)
            return;
        if (xStart < 0)
            xStart = 0;
        if (yStart < 0)
            yStart = 0;
        if (xEnd >= targetSurface->width)
            xEnd = targetSurface->width;
        if (yEnd >= targetSurface->height)
            yEnd = targetSurface->height;

        uint32 *targetPixels = (uint32 *)targetSurface->pixels;
        for (int y = yStart; y < yEnd; ++y)
        {
            for (int x = xStart; x < xEnd; ++x)
            {
                uint32 finalColor = graphicsBlendColors(color, targetPixels[x + y * targetSurface->width]);
                graphicsDrawPixelFast(x, y, finalColor);
            }
        }
    }

    void graphicsBlit(const Surface *source, Recti dimensions)
    {
        graphicsBlit(source, dimensions.x, dimensions.y, dimensions.w, dimensions.h);
    }

    void graphicsDrawRect(int xStart, int xEnd, int yStart, int yEnd, Color color)
    {
        graphicsDrawRect(xStart, xEnd, yStart, yEnd, graphicsConvertColorToU32(color));
    }

    uint32 graphicsConvertColorToU32(Color color)
    {
        uint32 result =
            (uint8)(color.a * 255.0f) << 24 |
            (uint8)(color.r * 255.0f) << 16 |
            (uint8)(color.g * 255.0f) << 8 |
            (uint8)(color.b * 255.0f);
        return result;
    }

    Color graphicsConvertU32ToColor(unsigned int color)
    {
        Color result;
        result.a = (float)(color >> 24 & 0xff) / 255.0f;
        result.r = (float)(color >> 16 & 0xff) / 255.0f;
        result.g = (float)(color >> 8 & 0xff) / 255.0f;
        result.b = (float)(color & 0xff) / 255.0f;

        return result;
    }

    uint32 graphicsBlendColors(Color foreground, Color background)
    {
        float r = lerp(foreground.r, background.r, 1.0f - foreground.a);
        float g = lerp(foreground.g, background.g, 1.0f - foreground.a);
        float b = lerp(foreground.b, background.b, 1.0f - foreground.a);
        unsigned int result = 0xff << 24 | (uint8)(r * 255.0f) << 16 | (uint8)(g * 255.0f) << 8 | (uint8)(b * 255.0f);
        return result;
    }

    uint32 graphicsBlendColors(uint32 foreground, uint32 background)
    {
        uint32 result = graphicsBlendColors(graphicsConvertU32ToColor(foreground), graphicsConvertU32ToColor(background));
        return result;
    }

    void graphicsBlit(const Surface *source, int x, int y)
    {
        if (x >= targetSurface->width || y >= targetSurface->height)
        {
            return;
        }

        int w = source->width;
        int h = source->height;

        if (x + w >= targetSurface->width)
            w = targetSurface->width - x;
        if (y + h >= targetSurface->height)
            h = targetSurface->height - y;

        int xOffset = 0;
        int yOffset = 0;
        if (x < 0)
        {
            xOffset = -x;
            w += x;
            x = 0;
        }
        if (y < 0)
        {
            yOffset = -y;
            h += y;
            y = 0;
        }
        // printf("x: %d, y: %d, w: %d, h: %d\n ", x, y, w, h);
        uint32 *sourcePixels = (uint32 *)source->pixels;
        uint32 *targetPixels = (uint32 *)targetSurface->pixels;
        for (int yIndex = 0; yIndex < h; ++yIndex)
        {
            for (int xIndex = 0; xIndex < w; ++xIndex)
            {
                unsigned int finalPixel = graphicsBlendColors(sourcePixels[(xIndex + xOffset) + (yIndex + yOffset) * source->width], targetPixels[(x + xIndex) + (y + yIndex) * targetSurface->width]);
                graphicsDrawPixelFast(x + xIndex, y + yIndex, finalPixel);
            }
        }
    }

    void graphicsBlit(const Surface *source, int x, int y, int w, int h)
    {
        if (x >= targetSurface->width || y >= targetSurface->height)
        {
            return;
        }

        if (w < 1 || h < 1) return;

        int xOffset = 0;
        int yOffset = 0;
        if (x < 0)
        {
            xOffset = -x;
            w += x;
            x = 0;
        }
        if (y < 0)
        {
            yOffset = -y;
            h += y;
            y = 0;
        }
        float xStep = (float)source->width / (float)w;
        float yStep = (float)source->height / (float)h;
        // printf("x: %d, y: %d, w: %d, h: %d\n ", x, y, w, h);
        uint32 *sourcePixels = (uint32 *)source->pixels;
        uint32 *targetPixels = (uint32 *)targetSurface->pixels;
        for (int yIndex = 0; yIndex < h; ++yIndex)
        {
            for (int xIndex = 0; xIndex < w; ++xIndex)
            {
                unsigned int finalPixel 
                = graphicsBlendColors(
                    sourcePixels[roundToInt((float)(xIndex + xOffset) * xStep) 
                                + roundToInt((float)(yIndex + yOffset) * yStep) * source->width], targetPixels[(x + xIndex) + (y + yIndex) * targetSurface->width]);
                graphicsDrawPixelFast(x + xIndex, y + yIndex, finalPixel);
            }
        }
    }

    void graphicsFill(unsigned int color)
    {
        for (int y = 0; y < targetSurface->height; ++y)
        {
            for (int x = 0; x < targetSurface->width; ++x)
            {
                graphicsDrawPixelFast(x, y, color);
            }
        }
    }

    bool graphicsCreateSurface(int width, int height, Surface *target)
    {
        if (width < 0 || height < 0)
        {
            return false;
        }

        target->width = width;
        target->height = height;
        target->pixels = malloc(width * height * sizeof(uint32));

        if (!target->pixels)
            return false;
        return true;
    }

    uint32 graphicsCreateColor(uint8 a, uint8 r, uint8 g, uint8 b)
    {
        uint32 result = a << 24 | r << 16 | b << 8 | g;
        return result;
    }

    void graphicsSetClearColor(uint32 color)
    {
        clearColor = color;
    }

    void graphicsClearTargetSurface()
    {
        graphicsFill(clearColor);
    }
}