#include "jadel_graphics.h"
#include "jadel_math.h"
#include "jadel_endian.h"
#include <stdlib.h>
#include <stdio.h>
#include <xmmintrin.h>
#include <memory.h>
#include "jadel_array.h"

namespace jadel
{

#define GET_PIXEL(x, y, surface) (((uint32*)surface->pixels)[(x) + (y) * surface->width])
#define TARGET_SURFACE_STACK_SIZE (20)
    int aIndex;
    int rIndex;
    int gIndex;
    int bIndex;

    struct TargetSurface
    {
        Surface *surface;
        int width;
        int height;
        int halfWidth;
        int halfHeight;
    };

    static TargetSurface *targetSurface;

    static Surface *targetSurfaceData;

    static Array<TargetSurface> targetSurfaceStack(TARGET_SURFACE_STACK_SIZE);

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

    int getRelativeX(float x)
    {
        int result = targetSurface->halfWidth + roundToInt(x * (float)targetSurface->halfWidth);
        return result;
    }

    int getRelativeY(float y)
    {
        int result = targetSurface->halfHeight + roundToInt(y * (float)targetSurface->halfHeight);
        return result;
    }

    Point2i getRelativePixel(float x, float y)
    {
        Point2i result = {getRelativeX(x), getRelativeY(y)};
        return result;
    }

    bool graphicsPushTargetSurface(Surface *target)
    {
        if (!target || targetSurfaceStack.size == TARGET_SURFACE_STACK_SIZE)
            return false;
        TargetSurface t;
        t.surface = target;
        t.width = target->width;
        t.height = target->height;
        t.halfWidth = target->width / 2;
        t.halfHeight = target->height / 2;
        targetSurfaceStack.push(t);
        targetSurface = &targetSurfaceStack.back();
        targetSurfaceData = targetSurface->surface;
        return true;
    }

    bool graphicsPopTargetSurface()
    {
        if (!targetSurfaceStack.pop())
            return false;
        if (targetSurfaceStack.size > 0)
        {
            targetSurface = &targetSurfaceStack.back();
            targetSurfaceData = targetSurface->surface;
        }
        else
        {
            targetSurface = NULL;
            targetSurfaceData = NULL;
        }
        return true;
    }

    void graphicsDrawPixelFast(int x, int y, uint32 color)
    {
        uint32 *pixels = (uint32 *)targetSurfaceData->pixels;
        pixels[x + y * targetSurface->width] = color;
    }

    void graphicsDrawPixelFast(int x, int y, float a, float r, float g, float b)
    {
        a = clampf(a, 0, 1.0f);
        r = clampf(r, 0, 1.0f);
        g = clampf(g, 0, 1.0f);
        b = clampf(b, 0, 1.0f);

        Pixel pixel;
        pixel.elems[aIndex] = (uint8)(a * 255.0f);
        pixel.elems[rIndex] = (uint8)(r * 255.0f);
        pixel.elems[gIndex] = (uint8)(g * 255.0f);
        pixel.elems[bIndex] = (uint8)(b * 255.0f);

        graphicsDrawPixelFast(x, y, pixel.content);
    }

    void graphicsDrawPixelFast(int x, int y, Color color)
    {
        graphicsDrawPixelFast(x, y, color.a, color.r, color.g, color.b);
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

        uint32 *targetPixels = (uint32 *)targetSurfaceData->pixels;
        for (int y = yStart; y < yEnd; ++y)
        {
            for (int x = xStart; x < xEnd; ++x)
            {
                uint32 finalColor = graphicsBlendColors(color, targetPixels[x + y * targetSurface->width]);
                graphicsDrawPixelFast(x, y, finalColor);
            }
        }
    }

    void graphicsDrawRect(int xStart, int xEnd, int yStart, int yEnd, Color color)
    {
        graphicsDrawRect(xStart, xEnd, yStart, yEnd, graphicsConvertColorToU32(color));
    }

    void graphicsDrawRect(Recti rect, uint32 color)
    {
        graphicsDrawRect(rect.x0, rect.x0 + rect.x1, rect.y0, rect.y0 + rect.y1, color);
    }

    void graphicsDrawRect(Recti rect, Color color)
    {
        graphicsDrawRect(rect.x0, rect.x0 + rect.x1, rect.y0, rect.y0 + rect.y1, color);
    }

    void graphicsDrawRectRelative(float xStart, float yStart, float xEnd, float yEnd, Color color)
    {
        Point2i start = getRelativePixel(xStart, yStart);
        Point2i end = getRelativePixel(xEnd, yEnd);
        Recti finalRect = {start.x, start.y, end.x - start.x, end.y - start.y};

        graphicsDrawRect(finalRect, color);
    }

    void graphicsDrawRectRelative(Rectf rect, Color color)
    {
        graphicsDrawRectRelative(rect.x0, rect.y0, rect.x1, rect.y1, color);
    }

    uint32 graphicsConvertColorToU32(Color color)
    {
        color.a = clampf(color.a, 0, 1.0f);
        color.r = clampf(color.r, 0, 1.0f);
        color.g = clampf(color.g, 0, 1.0f);
        color.b = clampf(color.b, 0, 1.0f);

        uint32 result =
            (uint32)(color.a * 255.0f) << ALPHA_SHIFT & ALPHA_MASK |
            (uint32)(color.r * 255.0f) << RED_SHIFT & RED_MASK |
            (uint32)(color.g * 255.0f) << GREEN_SHIFT & GREEN_MASK |
            (uint32)(color.b * 255.0f) << BLUE_SHIFT & BLUE_MASK;
        return result;
    }

    Color graphicsConvertU32ToColor(unsigned int color)
    {
        Color result;
        result.a = (float)((color & ALPHA_MASK) >> ALPHA_SHIFT) / 255.0f;
        result.r = (float)((color & RED_MASK) >> RED_SHIFT) / 255.0f;
        result.g = (float)((color & GREEN_MASK) >> GREEN_SHIFT) / 255.0f;
        result.b = (float)((color & BLUE_MASK) >> BLUE_SHIFT) / 255.0f;

        return result;
    }

    uint32 graphicsBlendColors(Color foreground, Color background)
    {
        float r = lerp(foreground.r, background.r, 1.0f - foreground.a);
        float g = lerp(foreground.g, background.g, 1.0f - foreground.a);
        float b = lerp(foreground.b, background.b, 1.0f - foreground.a);
        unsigned int result = 0xff << ALPHA_SHIFT | (uint8)(r * 255.0f) << RED_SHIFT | (uint8)(g * 255.0f) << GREEN_SHIFT | (uint8)(b * 255.0f) << BLUE_SHIFT;
        return result;
    }

    uint32 graphicsBlendColors(uint32 foreground, uint32 background)
    {
        uint32 result = graphicsBlendColors(graphicsConvertU32ToColor(foreground), graphicsConvertU32ToColor(background));
        return result;
    }

    void graphicsBlitFast(const Surface *source, int x, int y, int w, int h)
    {
        if (x == 0 && y == 0 && w == source->width && h == source->height)
        {
            // The method will check if both surfaces are of equal size before copying
            if (graphicsCopyEqualSizeSurface(source))
                return;
        }

        float xStep = (float)source->width / (float)w;
        float yStep = (float)source->height / (float)h;
        // printf("x: %d, y: %d, w: %d, h: %d\n ", x, y, w, h);
        uint32 *sourcePixels = (uint32 *)source->pixels;
        uint32 *targetPixels = (uint32 *)targetSurfaceData->pixels;
        for (int yIndex = 0; yIndex < h; ++yIndex)
        {
            for (int xIndex = 0; xIndex < w; ++xIndex)
            {
                unsigned int finalPixel = graphicsBlendColors(
                    sourcePixels[roundToInt((float)(xIndex)*xStep) + roundToInt((float)(yIndex)*yStep) * source->width], targetPixels[(x + xIndex) + (y + yIndex) * targetSurface->width]);
                graphicsDrawPixelFast(x + xIndex, y + yIndex, finalPixel);
            }
        }
    }

    void graphicsBlitFast(const Surface* source, int targetX, int targetY, int targetW, int targetH, int sourceX, int sourceY, int sourceW, int sourceH, float sourceXStep, float sourceYStep)
    {
        for (int y = 0; y < targetH; ++y)
        {
            for (int x = 0; x < targetW; ++x)
            {
                uint32 finalPixel = graphicsBlendColors(GET_PIXEL(sourceX + (int)((float)x * sourceXStep), 
                                                                  sourceY + (int)((float)y * sourceYStep), source), 
                    GET_PIXEL(targetX + x, targetY + y, targetSurface->surface));
                graphicsDrawPixelFast(targetX + x, targetY + y, finalPixel);
            }
        }
    }

    void graphicsBlit(const Surface *source, int targetX, int targetY, int targetW, int targetH, int sourceX, int sourceY, int sourceW, int sourceH)
    {
        if (targetX >= targetSurface->width || targetY >= targetSurface->height)
        {
            return;
        }

        if (targetW < 1 || targetH < 1)
            return;

        float sourceXStep = ((float)sourceW / (float)targetW);
        float sourceYStep = ((float)sourceH / (float)targetH);

        int xOffset = 0;
        int yOffset = 0;
        if (targetX < 0)
        {
            xOffset = -targetX;
            targetW += targetX;
            targetX = 0;
        }
        if (targetY < 0)
        {
            yOffset = -targetY;
            targetH += targetY;
            targetY = 0;
        }
        if (targetX + targetW > targetSurface->width)
        {
            targetW = targetSurface->width - targetX;
        }
        if (targetY + targetH > targetSurface->height)
        {
            targetH = targetSurface->height - targetY;
        }

        if (sourceX < 0)
        {
            sourceX = 0;
        }
        if (sourceX + sourceW > source->width)
        {
            sourceW = source->width - sourceX;
        }
        if (sourceY < 0)
        {
            sourceY = 0;
        }
        if (sourceY + sourceH > source->height)
        {
            sourceH = source->height - sourceY;
        }

        graphicsBlitFast(source, targetX, targetY, targetW, targetH, 
                        sourceX + (float)xOffset * sourceXStep, sourceY + (float)yOffset * sourceYStep, sourceW, sourceH, 
                        sourceXStep, sourceYStep);
    }

    void graphicsBlit(const Surface *source, int x, int y, int w, int h)
    {
        graphicsBlit(source, x, y, w, h, 0, 0, source->width, source->height);
    }

    void graphicsBlit(const Surface *source, int x, int y)
    {
        graphicsBlit(source, x, y, source->width, source->height);
    }
    
    void graphicsBlit(const Surface *source, Recti dimensions)
    {
        graphicsBlit(source, dimensions.x0, dimensions.y0, dimensions.x1 - dimensions.x0, dimensions.y1 - dimensions.y0);
    }

    void graphicsBlitRelative(const Surface *source, Rectf dimensions)
    {
        Point2i start = getRelativePixel(dimensions.x0, dimensions.y0);
        Point2i end = getRelativePixel(dimensions.x1, dimensions.y1);
        graphicsBlit(source, {start.x, start.y, end.x, end.y});
    }

    void graphicsBlitFast(const Surface *source, Recti rect)
    {
        graphicsBlitFast(source, rect.x0, rect.y0, rect.x1, rect.y1);
    }

    bool graphicsCopyEqualSizeSurface(const Surface *source)
    {
        if (source->width != targetSurface->width || source->height != targetSurface->height)
            return false;
        bool result = memcpy(targetSurfaceData->pixels, source->pixels, source->width * source->height * 4);
        return result != NULL;
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

    void graphicsMultiplyPixelValues(float val)
    {
        //__vector unsigned char cs = (__vector unsigned char){1, 2, 3, 4, 2, 4, 5, 5, 1, 2, 3, 4, 2, 4, 5, 5};
        uint8 *pixelElements = (uint8 *)targetSurfaceData->pixels;
        for (int i = 0; i < targetSurface->width * targetSurface->width; ++i)
        {
            unsigned int temp = pixelElements[i * 4 + rIndex] * val;
            if (temp > 255)
                temp = 255;
            pixelElements[i * 4 + rIndex] = temp;
            temp = pixelElements[i * 4 + gIndex] * val;
            if (temp > 255)
                temp = 255;
            pixelElements[i * 4 + gIndex] = temp;
            temp = pixelElements[i * 4 + bIndex] * val;
            if (temp > 255)
                temp = 255;
            pixelElements[i * 4 + bIndex] = temp;
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