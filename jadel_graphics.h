#pragma once

#include "jadel_defs.h"
#include "jadel_math.h"
#include "jadel_color.h"

namespace jadel
{
    extern int aIndex;
    extern int rIndex;
    extern int gIndex;
    extern int bIndex;

    union Pixel
    {
        uint32 content;
        unsigned char elems[4];
    };

    struct Surface
    {
        int width;
        int height;
        void *pixels;
    };

    extern DECLSPEC void graphicsInit();
    extern DECLSPEC int getRelativeX(float x, Recti rect);
    extern DECLSPEC int getRelativeY(float y, Recti rect);
    extern DECLSPEC Point2i getRelativePoint(float x, float y, Recti rect);
    extern DECLSPEC bool graphicsPushTargetSurface(Surface *target);
    extern DECLSPEC void graphicsPopTargetSurface();
    extern DECLSPEC void graphicsDrawPixelFast(int x, int y, uint32 color);
    extern DECLSPEC void graphicsDrawPixelFast(int x, int y, float a, float r, float g, float b);
    extern DECLSPEC void graphicsDrawPixelFast(int x, int y, Color color);
    extern DECLSPEC void graphicsDrawPixelRelativeFast(int x, int y, Color color);
    extern DECLSPEC void graphicsDrawPixelRelativeFast(int x, int y, uint32 color);
    extern DECLSPEC void graphicsDrawRectFast(int xStart, int xEnd, int yStart, int yEnd, Color color);
    extern DECLSPEC void graphicsDrawRect(int xStart, int xEnd, int yStart, int yEnd, uint32 color);
    extern DECLSPEC void graphicsDrawRect(int xStart, int xEnd, int yStart, int yEnd, Color color);
    extern DECLSPEC void graphicsDrawRect(Recti rect, uint32 color);
    extern DECLSPEC void graphicsDrawRect(Recti rect, Color color);
    extern DECLSPEC void graphicsDrawRectAdditiveRelative(Rectf rect, Color color);
    extern DECLSPEC void graphicsDrawRectRelative(float xStart, float yStart, float xEnd, float yEnd, Color color);
    extern DECLSPEC void graphicsDrawRectRelative(Rectf rect, Color color);
    extern DECLSPEC void graphicsDrawLineRelative(float xStart, float yStart, float xEnd, float yEnd, Color color);
    extern DECLSPEC void graphicsDrawLineRelative(Vec2 start, Vec2 end, Color color);
    extern DECLSPEC void graphicsDrawTriangleRelative(Vec2 point0, Vec2 point1, Vec2 point2, Color color);
    extern DECLSPEC void graphicsBlitFast(const Surface *source, int targetX0, int targetY0, int targetX1, int targetY1, int sourceX0, int sourceY0, int sourceX1, int sourceY1, float sourceXStep, float sourceYStep);
    extern DECLSPEC void graphicsBlitFast(const Surface *source, int x0, int y0, int x1, int y1);
    extern DECLSPEC void graphicsBlitFast(const Surface *source, Recti rect);
    extern DECLSPEC void graphicsBlit(const Surface *source, int targetX0, int targetY0, int targetX1, int targetY1, int sourceX0, int sourceY0, int sourceX1, int sourceY1);
    extern DECLSPEC void graphicsBlit(const Surface *source, int x, int y);
    extern DECLSPEC void graphicsBlit(const Surface *source, int x0, int y0, int x1, int y1);
    extern DECLSPEC void graphicsBlit(const Surface *source, Recti targetRect);
    extern DECLSPEC void graphicsBlit(const Surface *source, Recti targetRect, Recti sourceRect);
    // Function takes relative coordinates of the target surface, and absolute pixel dimensions of the source surface
    extern DECLSPEC void graphicsBlitRelative(const Surface* source, Rectf targetRect, Recti sourceRect);
    extern DECLSPEC void graphicsBlitRelative(const Surface *source, Rectf targetRect, Rectf sourceRect);
    extern DECLSPEC void graphicsBlitRelative(const Surface *source, Rectf targetRect);
    extern DECLSPEC bool graphicsCopyEqualSizeSurface(const Surface *source);
    extern DECLSPEC void graphicsFill(unsigned int color);
    extern DECLSPEC void graphicsMultiplyPixelValues(float r, float g, float b, Recti targetRect);
    extern DECLSPEC void graphicsMultiplyPixelValues(float r, float g, float b, Rectf targetRect);
    extern DECLSPEC void graphicsMultiplyPixelValues(float r, float g, float b);
    extern DECLSPEC void graphicsMultiplyPixelValues(float val);
    extern DECLSPEC bool graphicsCreateSurface(int width, int height, Surface *target);
    extern DECLSPEC uint32 graphicsCreateColor(uint8 a, uint8 r, uint8 g, uint8 b);
    extern DECLSPEC void graphicsSetClearColor(uint32 color);
    extern DECLSPEC void graphicsClearTargetSurface();
}