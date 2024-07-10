#pragma once
#include "jadel_util.h"
#include "jadel_defs.h"
#include "jadel_math_geometry.h"
#include "jadel_graphics.h"

#define JADEL_GRAPHICS_TARGET_SURFACE_STACK_SIZE (20)

#define JADEL_GRAPHICS_GET_PIXEL(x, y, surface) (((uint32 *)surface->pixels)[(x) + (y) * surface->width])

#define JADEL_GRAPHICS_GET_BACKGROUND_PIXEL(x, y) JADEL_GRAPHICS_GET_PIXEL(x, y, targetSurfaceData)

#define JADEL_GET_DRAW_PIXEL_MACRO(_1, _2, _3, _4, NAME, ...) NAME

#define JADEL_GRAPHICS_DRAW_PIXEL_FAST(...) JADEL_EXPAND_MACRO(JADEL_GET_DRAW_PIXEL_MACRO(__VA_ARGS__, JADEL_GRAPHICS_DRAW_PIXEL_FAST4, JADEL_GRAPHICS_DRAW_PIXEL_FAST3)(__VA_ARGS__))

#define JADEL_GRAPHICS_DRAW_PIXEL_FAST4(x, y, colorU32, target) \
    (((uint32 *)(target)->pixels)[(x) + (y) * (target)->width] = colorU32)

#define JADEL_GRAPHICS_DRAW_PIXEL_FAST3(x, y, colorU32) JADEL_GRAPHICS_DRAW_PIXEL_FAST4(x, y, colorU32, targetSurfaceData)

namespace jadel
{
    void graphicsInit();

    bool graphicsValidateAndModifyRectDimensions(Recti *rect, Graphics *graphics);

    bool graphicsValidateAndModifyRectDimensions(int *x0, int *y0, int *x1, int *y1, Graphics *graphics);

    bool graphicsRectfIsPartlyInbounds(float xStart, float yStart, float xEnd, float yEnd, Graphics *graphics);

    bool graphicsRectfIsPartlyInbounds(Rectf rect, Graphics *graphics);

    int graphicsGetTextureCoordPixelX(float x, const Surface *surface);

    int graphicsGetTextureCoordPixelY(float y, const Surface *surface);

    Point2i graphicsGetTextureCoordPixelPos(float x, float y, const Surface *surface);

    Vec2 graphicsGetTextureCoordPixelPosf(float x, float y, const Surface *surface);

    Recti graphicsGetTargetPixelAreaFromRectf(Rectf targetRect, Graphics *graphics);

    extern int pixelAlphaIndex;
    
    extern int pixelRedIndex;
    
    extern int pixelGreenIndex;
    
    extern int pixelBlueIndex;

    extern Point2i *texelIndexBuffer;
}
