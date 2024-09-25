#include "jadel_graphics.h"
#include "jadel_graphics_internal.h"
#include "jadel_util.h"
#include "jadel_endian.h"
#include "jadel_math_geometry.h"
#include "jadel_message.h"
#include "jadel_assert.h"
#include <stdlib.h>
#include <stdio.h>
#include <xmmintrin.h>
#include <memory.h>

#define IF_FALSE_DESTROY_RETURN_FALSE(value) \
    {                                        \
        if (!value)                          \
        {                                    \
            destroy();                       \
            return false;                    \
        }                                    \
    }

#define GET_PIXEL_COORD_FROM_RELATIVE_COORD(x, y) \
    Point2i((int)((float)targetSurface->halfWidth + x * (float)targetSurface->halfWidth), (int)((float)targetSurface->halfHeight + y * (float)targetSurface->halfHeight))

namespace jadel
{
    static Surface nullSurface{0};

    void graphicsInit()
    {
        if (endianness == LITTLE_ENDIAN)
        {
            pixelAlphaIndex = 3;
            pixelRedIndex = 2;
            pixelGreenIndex = 1;
            pixelBlueIndex = 0;
        }
        else
        {
            pixelAlphaIndex = 0;
            pixelRedIndex = 1;
            pixelGreenIndex = 2;
            pixelBlueIndex = 3;
        }
    }

    int graphicsGetTextureCoordPixelX(float x, const Surface *surface)
    {
        int result = roundToInt(x * (float)surface->width);
        return result;
    }

    int graphicsGetTextureCoordPixelY(float y, const Surface *surface)
    {
        int result = roundToInt(y * (float)surface->height);
        return result;
    }

    Point2i graphicsGetTextureCoordPixelPos(float x, float y, const Surface *surface)
    {
        Point2i result(graphicsGetTextureCoordPixelX(x, surface), graphicsGetTextureCoordPixelY(y, surface));
        return result;
    }

    Vec2 graphicsGetTextureCoordPixelPosf(float x, float y, const Surface *surface)
    {
        Vec2 result(x * surface->width, y * surface->height);
        return result;
    }

    Graphics::Graphics()
        : defaultSurface(nullSurface)
    {
    }

    bool Graphics::init()
    {
        clearFlags();
        IF_FALSE_DESTROY_RETURN_FALSE(targetSurfaceStack.init(JADEL_GRAPHICS_TARGET_SURFACE_STACK_SIZE));
        IF_FALSE_DESTROY_RETURN_FALSE(graphicsCreateSurface(200, 200, &this->defaultSurface));
        pushTargetSurface(&defaultSurface);
        return true;
    }

    void Graphics::destroy()
    {
        graphicsFreeSurface(&this->defaultSurface);
        targetSurfaceStack.destroy();
    }

    bool Graphics::pushTargetSurface(Surface *target)
    {
        if (!target || targetSurfaceStack.size() == JADEL_GRAPHICS_TARGET_SURFACE_STACK_SIZE)
        {
            JADEL_DEBUGMSG("Graphics: Tried to push NULL target surface\n");
            return false;
        }
        TargetSurface t;
        t.surface = target;
        t.width = target->width;
        t.height = target->height;
        t.halfWidth = target->width / 2;
        t.halfHeight = target->height / 2;
        t.rect = {0, 0, target->width, target->height};
        targetSurfaceStack.push(t);
        targetSurface = &targetSurfaceStack.top();
        targetSurfaceData = target;
        this->targetSurface->clippingPlaneStack.init(30);
        pushClippingPlane(Rectf(-1.0f, -1.0f, 1.0f, 1.0f));
        return true;
    }

    void Graphics::popTargetSurface()
    {
        if (targetSurfaceStack.size() > 1)
        {
            targetSurface->clippingPlaneStack.destroy();
            targetSurfaceStack.pop();
            targetSurface = &targetSurfaceStack.top();
            targetSurfaceData = targetSurface->surface;
        }
    }

    void Graphics::drawLineRelative(float xStart, float yStart, float xEnd, float yEnd, Color color)
    {
        // Point2i pixelStart = getPointiRelativeToCenterInRecti(xStart, yStart, Recti(0, 0, targetSurface->width, targetSurface->height));
        // Point2i pixelEnd = getPointiRelativeToCenterInRecti(xEnd, yEnd, Recti(0, 0, targetSurface->width, targetSurface->height));
        Point2i pixelStart = GET_PIXEL_COORD_FROM_RELATIVE_COORD(xStart, yStart);
        Point2i pixelEnd = GET_PIXEL_COORD_FROM_RELATIVE_COORD(xEnd, yEnd);

        int dx = absInt(pixelEnd.x - pixelStart.x);
        int dy = absInt(pixelEnd.y - pixelStart.y);
        bool slope = dy > dx;

        if (slope)
        {
            JADEL_SWAP(pixelStart.x, pixelStart.y);
            JADEL_SWAP(pixelEnd.x, pixelEnd.y);
        }

        if (pixelStart.x > pixelEnd.x)
        {
            JADEL_SWAP(pixelStart.x, pixelEnd.x);
            JADEL_SWAP(pixelStart.y, pixelEnd.y);
        }

        dx = absInt(pixelEnd.x - pixelStart.x);
        dy = absInt(pixelEnd.y - pixelStart.y);
        int error = dx / 2;
        int y = pixelStart.y;
        int yStep = pixelStart.y < pixelEnd.y ? 1 : -1;

        for (int x = pixelStart.x; x < pixelEnd.x; ++x)
        {
            Point2i coord = slope ? Point2i(y, x) : Point2i(x, y);
            drawPixelFast(coord.x, coord.y, color);
            error -= dy;
            if (error < 0)
            {
                y += yStep;
                error += dx;
            }
        }
    }

    /*void Graphics::drawLineRelative(float xStart, float yStart, float xEnd, float yEnd, Color color)
    {
        Point2i start = getPointiRelativeToCenterInRecti(xStart, yStart, targetSurface->rect);
        Point2i end = getPointiRelativeToCenterInRecti(xEnd, yEnd, targetSurface->rect);

        if (start.y > end.y)
        {
            JADEL_SWAP(start.y, end.y);
            JADEL_SWAP(start.x, end.x);
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
                drawPixelFast(xStartPixel + x, start.y, color);
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
                    drawPixelFast(start.x + (int)(((float)y + 1) * xStep) + x, start.y + y, color);
                }
                else
                {
                    drawPixelFast(start.x + (int)(((float)y + 1) * xStep) - x, start.y + y, color);
                }
            }
        }
    }
*/
    void Graphics::drawLineRelative(Vec2 start, Vec2 end, Color color)
    {
        drawLineRelative(start.x, start.y, end.x, end.y, color);
    }

    void Graphics::drawTriangleRelative(Vec2 point0, Vec2 point1, Vec2 point2, Color color)
    {
        drawLineRelative(point0, point1, color);
        drawLineRelative(point1, point2, color);
        drawLineRelative(point2, point0, color);
    }

    /*void Graphics::drawFillTriangleRelative(Vec2 point0, Vec2 point1, Vec2 point2, Color color)
    {
    }*/

    void Graphics::fill(uint32 color)
    {
        for (int y = 0; y < targetSurface->height; ++y)
        {
            for (int x = 0; x < targetSurface->width; ++x)
            {
                JADEL_GRAPHICS_DRAW_PIXEL_FAST(x, y, color);
            }
        }
    }

    void Graphics::multiplyPixelValues(float r, float g, float b, Recti targetRect)
    {
        if (!graphicsValidateAndModifyRectDimensions(&targetRect, this))
        {
            return;
        }
        uint8 *pixelElements = (uint8 *)targetSurfaceData->pixels;
        for (int y = targetRect.y0; y < targetRect.y1; ++y)
        {
            for (int x = targetRect.x0; x < targetRect.x1; ++x)
            {
                int pixelIndex = (x + y * targetSurface->width) * 4;
                uint32 temp = pixelElements[pixelIndex + pixelRedIndex] * r;
                temp = temp > 255 ? 255 : temp;
                pixelElements[pixelIndex + pixelRedIndex] = temp;
                temp = pixelElements[pixelIndex + pixelGreenIndex] * g;
                temp = temp > 255 ? 255 : temp;
                pixelElements[pixelIndex + pixelGreenIndex] = temp;
                temp = pixelElements[pixelIndex + pixelBlueIndex] * b;
                temp = temp > 255 ? 255 : temp;
                pixelElements[pixelIndex + pixelBlueIndex] = temp;
            }
        }
    }

    void Graphics::multiplyPixelValues(float r, float g, float b, Rectf targetRect)
    {
        Recti targetPixelRect = getPixelRecti(targetRect);
        multiplyPixelValues(r, g, b, targetPixelRect);
    }

    void Graphics::multiplyPixelValues(float r, float g, float b)
    {
        multiplyPixelValues(r, g, b, Rectf(-1.0f, -1.0f, 1.0f, 1.0f));
    }

    void Graphics::multiplyPixelValues(float val)
    {
        multiplyPixelValues(val, val, val);
    }

    void Graphics::setClearColor(uint32 color)
    {
        clearColor = color;
    }

    void Graphics::clearTargetSurface()
    {
        fill(clearColor);
    }

    Vec2 Graphics::getPixelCoordFromRelativeCoord(Vec2 relativeCoord) const
    {
        Vec2 result = getPointfRelativeToCenterInRecti(relativeCoord, targetSurface->rect);
        return result;
    }

    Recti Graphics::getPixelRecti(Rectf relativeRect) const
    {
        Recti result(getPointiRelativeToCenterInRecti(relativeRect.getPointA(), targetSurface->rect),
                     getPointiRelativeToCenterInRecti(relativeRect.getPointB(), targetSurface->rect));
        return result;
    }

    Rectf Graphics::getPixelRectf(Rectf relativeRect) const
    {
        Rectf result(getPointfRelativeToCenterInRecti(relativeRect.getPointA(), targetSurface->rect),
                     getPointfRelativeToCenterInRecti(relativeRect.getPointB(), targetSurface->rect));
        return result;
    }


    uint32 graphicsCreateColor(uint8 a, uint8 r, uint8 g, uint8 b)
    {
        uint32 result = a << 24 | r << 16 | b << 8 | g;
        return result;
    }

    void Graphics::setFlagsEnabled(uint32 flags)
    {
        this->flags = flags;
    }

    void Graphics::setFlag(uint32 flag, bool enabled)
    {
        if (enabled)
        {
            this->flags |= flag;
        }
        else
        {
            this->flags &= ~flag;
        }
    }

    void Graphics::setFlagEnabled(uint32 flag)
    {
        this->flags |= flag;
    }

    void Graphics::clearFlags()
    {
        this->flags = 0;
    }

    bool Graphics::hasFlag(uint32 flag) const
    {
        bool result = flags & flag ? true : false;
        return result;
    }

    void Graphics::pushClippingPlane(Rectf clippingPlane)
    {
        Rectf finalClippingPlane;
        if (this->targetSurface->clippingPlaneStack.size() > 0)
        {
            Rectf topPlane = this->targetSurface->clippingPlaneStack.top();
            finalClippingPlane.x0 = JADEL_CLAMP_MIN(clippingPlane.x0, topPlane.x0);
            finalClippingPlane.y0 = JADEL_CLAMP_MIN(clippingPlane.y0, topPlane.y0);
            finalClippingPlane.x1 = JADEL_CLAMP_MAX(clippingPlane.x1, topPlane.x1);
            finalClippingPlane.y1 = JADEL_CLAMP_MAX(clippingPlane.y1, topPlane.y1);
        }
        else
        {
            finalClippingPlane = clippingPlane;
        }
        this->targetSurface->clippingPlaneStack.push(finalClippingPlane);
    }

    void Graphics::popClippingPlane()
    {
        if (this->targetSurface->clippingPlaneStack.size() == 1)
        {
            return;
        }
        this->targetSurface->clippingPlaneStack.pop();
    }
}