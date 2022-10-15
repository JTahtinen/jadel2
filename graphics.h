#ifndef GRAPHICS_H
#define GRAPHICS_H
#include "defs.h"

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

    extern DLLIMPORT void initGraphics();
    extern DLLIMPORT bool pushTargetSurface(Surface *target);
    extern DLLIMPORT void drawPixelFast(int x, int y, uint32 color);
    extern DLLIMPORT void drawPixelFast(int x, int y, float a, float r, float g, float b);
    extern DLLIMPORT void drawRect(int xStart, int xEnd, int yStart, int yEnd, uint32 color);
    extern DLLIMPORT void drawRect(int xStart, int xEnd, int yStart, int yEnd, Color color);
    extern DLLIMPORT uint32 convertColorToU32(Color color);
    extern DLLIMPORT Color convertU32ToColor(unsigned int color);
    extern DLLIMPORT uint32 blendColors(Color foreground, Color background);
    extern DLLIMPORT uint32 blendColors(uint32 foreground, uint32 background);
    extern DLLIMPORT void blit(const Surface *source, int x, int y);
    extern DLLIMPORT void fill(unsigned int color);
    extern DLLIMPORT bool createSurface(int width, int height, Surface *target);
    extern DLLIMPORT uint32 createColor(uint8 a, uint8 r, uint8 g, uint8 b);
}
#endif