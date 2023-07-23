#include "jadel_graphics.h"
#include "jadel_math.h"
#include "jadel_endian.h"
#include <stdlib.h>
#include <stdio.h>
#include <xmmintrin.h>
#include <memory.h>
#include "jadel_array.h"
#include "jadel_stack.h"
#include "jadel_util.h"

namespace jadel
{

#define GET_PIXEL(x, y, surface) (((uint32 *)surface->pixels)[(x) + (y)*surface->width])
#define GET_BACKGROUND_PIXEL(x, y) GET_PIXEL(x, y, targetSurfaceData)
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
        Recti rect;
    };

    static TargetSurface *targetSurface;

    static Surface *targetSurfaceData;

    //    static Array<TargetSurface> targetSurfaceStack(TARGET_SURFACE_STACK_SIZE);
    static Stack<TargetSurface> targetSurfaceStack;

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
        targetSurfaceStack.init(TARGET_SURFACE_STACK_SIZE);
    }

    int getTextureCoordPixelX(float x, const Surface *surface)
    {
        int result = roundToInt(x * (float)surface->width);
        return result;
    }

    int getTextureCoordPixelY(float y, const Surface *surface)
    {
        int result = roundToInt(y * (float)surface->height);
        return result;
    }

    Point2i getTextureCoordPixelPos(float x, float y, const Surface *surface)
    {
        Point2i result = {getTextureCoordPixelX(x, surface), getTextureCoordPixelY(y, surface)};
        return result;
    }

    int getRelativeX(float x, Recti rect)
    {
        int halfWidth = (rect.x1 - rect.x0) / 2;
        int result = halfWidth + roundToInt(x * (float)halfWidth);
        return result;
    }

    int getRelativeY(float y, Recti rect)
    {
        int halfHeight = (rect.y1 - rect.y0) / 2;
        int result = halfHeight + roundToInt(y * (float)halfHeight);
        return result;
    }

    Point2i getRelativePoint(float x, float y, Recti rect)
    {
        Point2i result = {getRelativeX(x, rect), getRelativeY(y, rect)};
        return result;
    }

    Point2i getRelativePoint(Vec2 point, Recti rect)
    {
        Point2i result = getRelativePoint(point.x, point.y, rect);
        return result;
    }

    bool graphicsPushTargetSurface(Surface *target)
    {
        if (!target || targetSurfaceStack.size() == TARGET_SURFACE_STACK_SIZE)
            return false;
        TargetSurface t;
        t.surface = target;
        t.width = target->width;
        t.height = target->height;
        t.halfWidth = target->width / 2;
        t.halfHeight = target->height / 2;
        t.rect = {0, 0, target->width, target->height};
        targetSurfaceStack.push(t);
        targetSurface = &targetSurfaceStack.top();
        targetSurfaceData = targetSurface->surface;
        return true;
    }

    void graphicsPopTargetSurface()
    {
        targetSurfaceStack.pop();

        if (targetSurfaceStack.size() > 0)
        {
            targetSurface = &targetSurfaceStack.top();
            targetSurfaceData = targetSurface->surface;
        }
        else
        {
            targetSurface = NULL;
            targetSurfaceData = NULL;
        }
    }

    void graphicsDrawPixelFast(int x, int y, uint32 color)
    {
        uint32 *pixels = (uint32 *)targetSurfaceData->pixels;
        pixels[x + y * targetSurface->width] = color;
    }

    void graphicsDrawPixelFast(int x, int y, float a, float r, float g, float b)
    {
        /*a = clampf(a, 0, 1.0f);
        r = clampf(r, 0, 1.0f);
        g = clampf(g, 0, 1.0f);
        b = clampf(b, 0, 1.0f);
*/
        Pixel pixel;
        Color finalColor;
        if (a < 1.0f)
        {
            graphicsDrawPixelFast(x, y, Color(a, r, g, b) * (GET_BACKGROUND_PIXEL(x, y)));
        }

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

    void graphicsDrawPixelRelativeFast(int x, int y, Color color)
    {
        graphicsDrawPixelFast(x + targetSurface->halfWidth, y + targetSurface->halfHeight, color);
    }

    void graphicsDrawPixelRelativeFast(int x, int y, uint32 color)
    {
        graphicsDrawPixelFast(x + targetSurface->halfWidth, y + targetSurface->halfHeight, color);
    }

    static bool graphicsValidateAndModifyRectDimensions(int *xStart, int *yStart, int *xEnd, int *yEnd)
    {
        if (*xStart >= targetSurface->width ||
            *yStart >= targetSurface->height ||
            *xEnd <= 0 ||
            *yEnd <= 0)
            return false;

        *xStart = *xStart < 0 ? 0 : *xStart;
        *yStart = *yStart < 0 ? 0 : *yStart;
        *xEnd = *xEnd > targetSurface->width ? targetSurface->width : *xEnd;
        *yEnd = *yEnd > targetSurface->height ? targetSurface->height : *yEnd;
        return true;
    }

    static bool graphicsValidateAndModifyRectDimensions(Recti *rect)
    {
        bool result = graphicsValidateAndModifyRectDimensions(&rect->x0, &rect->y0, &rect->x1, &rect->y1);
        return result;
    }

    void graphicsDrawRectFast(int xStart, int xEnd, int yStart, int yEnd, Color color)
    {
        for (int y = yStart; y < yEnd; ++y)
        {
            for (int x = xStart; x < xEnd; ++x)
            {
                uint32 finalColor = (color.blendU32(GET_BACKGROUND_PIXEL(x, y)));
                graphicsDrawPixelFast(x, y, finalColor);
            }
        }
    }

    void graphicsDrawRect(int xStart, int xEnd, int yStart, int yEnd, Color color)
    {
        if (!graphicsValidateAndModifyRectDimensions(&xStart, &yStart, &xEnd, &yEnd))
        {
            return;
        }
        graphicsDrawRectFast(xStart, xEnd, yStart, yEnd, color);
    }

    void graphicsDrawRect(int xStart, int xEnd, int yStart, int yEnd, uint32 color)
    {
        graphicsDrawRect(xStart, xEnd, yStart, yEnd, Color(color));
    }

    void graphicsDrawRect(Recti rect, uint32 color)
    {
        graphicsDrawRect(rect.x0, rect.x0 + rect.x1, rect.y0, rect.y0 + rect.y1, color);
    }

    void graphicsDrawRect(Recti rect, Color color)
    {
        graphicsDrawRect(rect.x0, rect.x1, rect.y0, rect.y1, color);
    }

    static Recti graphicsGetPixelRect(Rectf rect)
    {
        Recti result(getRelativePoint(rect.getPoint0(), targetSurface->rect), getRelativePoint(rect.getPoint1(), targetSurface->rect));
        return result;
    }

    void graphicsDrawRectAdditiveRelative(Rectf rect, Color color)
    {
        Recti pixelRect = graphicsGetPixelRect(rect);
        if (!graphicsValidateAndModifyRectDimensions(&pixelRect))
        {
            return;
        }
        for (int y = pixelRect.y0; y < pixelRect.y1; ++y)
        {
            for (int x = pixelRect.x0; x < pixelRect.x1; ++x)
            {
                uint32 finalColor = (color + (GET_BACKGROUND_PIXEL(x, y))).toU32();
                graphicsDrawPixelFast(x, y, finalColor);
            }
        }
    }

    void graphicsDrawRectRelative(float xStart, float yStart, float xEnd, float yEnd, Color color)
    {
        Recti rect = graphicsGetPixelRect({xStart, yStart, xEnd, yEnd});
        graphicsDrawRect(rect, color);
    }

    void graphicsDrawRectRelative(Rectf rect, Color color)
    {
        graphicsDrawRectRelative(rect.x0, rect.y0, rect.x1, rect.y1, color);
    }

    void graphicsDrawLineRelative(float xStart, float yStart, float xEnd, float yEnd, Color color)
    {

        Point2i start = getRelativePoint(xStart, yStart, targetSurface->rect);
        Point2i end = getRelativePoint(xEnd, yEnd, targetSurface->rect);

        if (start.y > end.y)
        {
            swapInt(&start.y, &end.y);
            swapInt(&start.x, &end.x);
        }

        int xStartPixel;
        int xEndPixel;
        if (start.x < end.x)
        {
            xStartPixel = start.x;
            xEndPixel = end.x;
        }
        else
        {
            xStartPixel = end.x;
            xEndPixel = start.x;
        }

        if (xEndPixel < 0)
            return;
        if (xStartPixel >= targetSurface->width)
            return;
        if (end.y < 0)
            return;
        if (start.y >= targetSurface->height)
            return;

        if (xEndPixel > targetSurface->width)
            xEndPixel = targetSurface->width;
        if (xStartPixel < 0)
            xStartPixel = 0;
        if (end.y > targetSurface->height)
            end.y = targetSurface->height;
        if (start.y < 0)
            start.y = 0;

        if (start.y == end.y)
        {
            for (int x = 0; x < xEndPixel - xStartPixel; ++x)
            {
                graphicsDrawPixelFast(xStartPixel + x, start.y, color);
            }
            return;
        }

        int xDir;
        if (start.x < end.x)
            xDir = 1;
        else
            xDir = -1;

        float xLength = (float)end.x - (float)start.x;
        float yLength = absFloat((float)end.y - (float)start.y);
        float xStep = xLength / yLength;
        for (int y = 0; y < absInt(end.y - start.y); ++y)
        {
            for (int x = 0; x < absInt(xStep) || x < 1; ++x)
            {
                if (xDir == -1)
                {
                    graphicsDrawPixelFast(start.x + (int)(((float)y + 1) * xStep) + x, start.y + y, color);
                }
                else
                {
                    graphicsDrawPixelFast(start.x + (int)(((float)y + 1) * xStep) - x, start.y + y, color);
                }
            }
        }
    }

    void graphicsDrawLineRelative(Vec2 start, Vec2 end, Color color)
    {
        graphicsDrawLineRelative(start.x, start.y, end.x, end.y, color);
    }

    void graphicsDrawTriangleRelative(Vec2 point0, Vec2 point1, Vec2 point2, Color color)
    {
        graphicsDrawLineRelative(point0, point1, color);
        graphicsDrawLineRelative(point1, point2, color);
        graphicsDrawLineRelative(point2, point0, color);
    }

    void graphicsDrawFillTriangleRelative(Vec2 point0, Vec2 point1, Vec2 point2, Color color)
    {
    }

    void graphicsBlitFast(const Surface *source,
                          int targetX0, int targetY0,
                          int targetX1, int targetY1,
                          int sourceX0, int sourceY0,
                          int sourceX1, int sourceY1,
                          float sourceXStep, float sourceYStep)
    {
        for (int y = 0; y < targetY1 - targetY0; ++y)
        {
            for (int x = 0; x < targetX1 - targetX0; ++x)
            {
                // uint32 sourcePixel = GET_PIXEL(sourceX0 + (int)((float)x * sourceXStep),
                //                                                   sourceY0 + (int)((float)y * sourceYStep), source);

                uint32 fgPixel = GET_PIXEL(sourceX0 + roundToInt((float)x * sourceXStep),
                                           sourceY0 + roundToInt((float)y * sourceYStep), source);
                uint32 bgPixel = GET_BACKGROUND_PIXEL(targetX0 + x, targetY0 + y);

                Color finalPixel = Color::blendU32(fgPixel, bgPixel);
                graphicsDrawPixelFast(targetX0 + x, targetY0 + y, finalPixel);
            }
        }
    }

    void graphicsBlitFast(const Surface *source, int x0, int y0, int x1, int y1)
    {
        if (x0 == 0 && y0 == 0 && x1 == source->width && y1 == source->height)
        {
            // The method will check if both surfaces are of equal size before copying
            if (graphicsCopyEqualSizeSurface(source))
                return;
        }
        int width = x1 - x0;
        int height = y1 - y0;
        float xStep = (float)source->width / (float)width;
        float yStep = (float)source->height / (float)height;
        graphicsBlitFast(source, x0, y0, x1, y1, 0, 0, source->width, source->height, xStep, yStep);
    }

    void graphicsBlitFast(const Surface *source, Recti rect)
    {
        graphicsBlitFast(source, rect.x0, rect.y0, rect.x1, rect.y1);
    }

    int debugTX0;
    int debugTY0;
    int debugTX1;
    int debugTY1;

    int debugSX0;
    int debugSY0;
    int debugSX1;
    int debugSY1;

    int debugTW;
    int debugSW;

    float debugSourceXStep;
    float debugSourceYStep;
    void graphicsBlit(const Surface *source,
                      int targetX0, int targetY0,
                      int targetX1, int targetY1,
                      int sourceX0, int sourceY0,
                      int sourceX1, int sourceY1)
    {
        if (targetX0 == targetX1 || targetY0 == targetY1 || sourceX0 == sourceX1 || sourceY0 == sourceY1)
            return;
        if (targetX0 > targetX1)
            swapInt(&targetX0, &targetX1);
        if (targetY0 > targetY1)
            swapInt(&targetY0, &targetY1);
        if (sourceX0 > sourceX1)
            swapInt(&sourceX0, &sourceX1);
        if (sourceY0 > sourceY1)
            swapInt(&sourceY0, &sourceY1);
        if (targetX0 >= targetSurface->width || targetY0 >= targetSurface->height || targetX1 <= 0 || targetY1 <= 0 || sourceX0 >= source->width || sourceY0 >= source->height || sourceX1 <= 0 || sourceY1 <= 0)
        {
            return;
        }

        int targetW = targetX1 - targetX0;
        int targetH = targetY1 - targetY0;

        int sourceW = sourceX1 - sourceX0;
        int sourceH = sourceY1 - sourceY0;

        float sourceXStep = ((float)sourceW / (float)targetW);
        float sourceYStep = ((float)sourceH / (float)targetH);
        /*
                debugTX0 = targetX0;
                debugTY0 = targetY0;
                debugTX1 = targetX1;
                debugTY1 = targetY1;

                debugSX0 = sourceX0;
                debugSY0 = sourceY0;
                debugSX1 = sourceX1;
                debugSY1 = sourceY1;

                debugTW = targetW;
                debugSW = sourceW;

                debugSourceXStep = sourceXStep;
                debugSourceYStep = sourceYStep;
        */

        if (targetX0 < 0)
        {
            sourceX0 -= (targetX0 * sourceXStep);
            targetX0 = 0;
        }
        if (targetY0 < 0)
        {
            sourceY0 -= (targetY0 * sourceYStep);
            targetY0 = 0;
        }
        if (targetX1 > targetSurface->width)
        {
            sourceX1 -= targetX1 - targetSurface->width;
            targetX1 = targetSurface->width;
        }
        if (targetY1 > targetSurface->height)
        {
            sourceY1 -= targetY1 - targetSurface->height;
            targetY1 = targetSurface->height;
        }

        if (sourceX0 < 0)
        {
            return;
            // sourceX0 = 0;
        }
        if (sourceX1 > source->width)
        {
            return;
            // sourceX1 = source->width;
        }
        if (sourceY0 < 0)
        {
            return;
            // sourceY0 = 0;
        }
        if (sourceY1 > source->height)
        {
            return;
            // sourceY1 = source->height;
        }

        graphicsBlitFast(source, targetX0, targetY0, targetX1, targetY1,
                         sourceX0, sourceY0, sourceX1, sourceY1,
                         sourceXStep, sourceYStep);
    }

    void graphicsBlit(const Surface *source, int x0, int y0, int x1, int y1)
    {
        graphicsBlit(source, x0, y0, x1, y1, 0, 0, source->width, source->height);
    }

    void graphicsBlit(const Surface *source, int x, int y)
    {
        graphicsBlit(source, x, y, source->width, source->height);
    }

    void graphicsBlit(const Surface *source, Recti targetRect)
    {
        graphicsBlit(source, targetRect.x0, targetRect.y0, targetRect.x1, targetRect.y1);
    }

    void graphicsBlit(const Surface *source, Recti targetRect, Recti sourceRect)
    {
        graphicsBlit(source, targetRect.x0, targetRect.y0, targetRect.x1, targetRect.y1, sourceRect.x0, sourceRect.y0, sourceRect.x1, sourceRect.y1);
    }

    void graphicsBlitRelative(const Surface *source, Rectf targetRect, Recti sourceRect)
    {
        Recti targetRectI(
            getRelativePoint(targetRect.x0, targetRect.y0, targetSurface->rect),
            getRelativePoint(targetRect.x1, targetRect.y1, targetSurface->rect));
        graphicsBlit(source, targetRectI, sourceRect);
    }

    void graphicsBlitRelative(const Surface *source, Rectf targetRect, Rectf sourceRect)
    {
        Recti sourceRectI(getTextureCoordPixelPos(sourceRect.x0, sourceRect.y0, source), getTextureCoordPixelPos(sourceRect.x1, sourceRect.y1, source));
        graphicsBlitRelative(source, targetRect, sourceRectI);
    }

    void graphicsBlitRelative(const Surface *source, Rectf targetRect)
    {
        graphicsBlitRelative(source, targetRect,
                             Rectf{0, 0, 1.0f, 1.0f});
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

    void graphicsMultiplyPixelValues(float r, float g, float b, Recti targetRect)
    {
        graphicsValidateAndModifyRectDimensions(&targetRect);
        uint8 *pixelElements = (uint8 *)targetSurfaceData->pixels;
        for (int y = 0; y < targetSurface->height; ++y)
        {
            for (int x = 0; x < targetSurface->width; ++x)
            {
                int pixelIndex = x + y * 4;
                uint32 temp = pixelElements[pixelIndex + rIndex] * r; 
                temp = temp > 255 ? 255 : temp;
                pixelElements[pixelIndex + rIndex] = temp;
                temp = pixelElements[pixelIndex + gIndex] * g; 
                temp = temp > 255 ? 255 : temp;
                pixelElements[pixelIndex + gIndex] = temp;
                temp = pixelElements[pixelIndex + bIndex] * b; 
                temp = temp > 255 ? 255 : temp;
                pixelElements[pixelIndex + bIndex] = temp;
            }
        }
    }

    void graphicsMultiplyPixelValues(float r, float g, float b, Rectf targetRect)
    {
        Recti targetPixelRect = graphicsGetPixelRect(targetRect);
        graphicsMultiplyPixelValues(r, g, b, targetPixelRect);
    }

    void graphicsMultiplyPixelValues(float r, float g, float b)
    {
        graphicsMultiplyPixelValues(r, g, b, Rectf(-1.0f, -1.0f, 1.0f, 1.0f));
    }

    void graphicsMultiplyPixelValues(float val)
    {
        graphicsMultiplyPixelValues(val, val, val);
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