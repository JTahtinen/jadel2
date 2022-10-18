#pragma once

#include "jadel_defs.h"
#include "jadel_math.h"

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

    struct Color
    {
        float a;
        float r;
        float g;
        float b;
    };

    struct Surface
    {
        int width;
        int height;
        void *pixels;
    };

    extern DECLSPEC void graphicsInit();
    extern DECLSPEC bool graphicsPushTargetSurface(Surface *target);
    extern DECLSPEC bool graphicsPopTargetSurface();
    extern DECLSPEC void graphicsDrawPixelFast(int x, int y, uint32 color);
    extern DECLSPEC void graphicsDrawPixelFast(int x, int y, float a, float r, float g, float b);
    extern DECLSPEC void graphicsDrawRect(int xStart, int xEnd, int yStart, int yEnd, uint32 color);
    extern DECLSPEC void graphicsDrawRect(int xStart, int xEnd, int yStart, int yEnd, Color color);
    extern DECLSPEC uint32 graphicsConvertColorToU32(Color color);
    extern DECLSPEC Color graphicsConvertU32ToColor(unsigned int color);
    extern DECLSPEC uint32 graphicsBlendColors(Color foreground, Color background);
    extern DECLSPEC uint32 graphicsBlendColors(uint32 foreground, uint32 background);
    extern DECLSPEC void graphicsBlit(const Surface *source, int x, int y);
    extern DECLSPEC void graphicsBlit(const Surface *source, int x, int y, int w, int h);
    extern DECLSPEC void graphicsBlit(const Surface *source, Recti dimensions);
    extern DECLSPEC void graphicsFill(unsigned int color);
    extern DECLSPEC bool graphicsCreateSurface(int width, int height, Surface *target);
    extern DECLSPEC uint32 graphicsCreateColor(uint8 a, uint8 r, uint8 g, uint8 b);
    extern DECLSPEC void graphicsSetClearColor(uint32 color);
    extern DECLSPEC void graphicsClearTargetSurface();
}