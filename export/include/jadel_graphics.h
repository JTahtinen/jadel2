#pragma once
#include "jadel_defs.h"
#include "jadel_surface.h"
#include "jadel_math_geometry.h"
#include "jadel_color.h"
#include "jadel_stack.h"

namespace jadel
{
    enum
    {
        JADEL_GRAPHICS_DEFAULT = 0,
        JADEL_GRAPHICS_BLIT_FILTER_NONE = 1 << 0,
        JADEL_GRAPHICS_BLIT_FILTER_BILINEAR = 1 << 1,
    };

    union Pixel
    {
        uint32 content;
        unsigned char elems[4];
    };


    struct TargetSurface
    {
        Surface *surface;
        int width;
        int height;
        int halfWidth;
        int halfHeight;
        Recti rect;
    };

    struct DECLSPEC Graphics
    {
        Surface defaultSurface;
        Surface *targetSurfaceData;
        TargetSurface *targetSurface;
        uint32 flags;
        Stack<TargetSurface> targetSurfaceStack;
        uint32 clearColor;

        Graphics();

        bool init();
        
        void destroy();

        bool pushTargetSurface(Surface *target);
        
        void popTargetSurface();
        
        void drawPixelFast(int x, int y, uint32 color);
        
        void drawPixelFast(int x, int y, float a, float r, float g, float b);
        
        void drawPixelFast(int x, int y, Color color);
        
        void drawPixelRelativeFast(int x, int y, Color color);
        
        void drawPixelRelativeFast(int x, int y, uint32 color);
        
        void drawRectFast(int xStart, int xEnd, int yStart, int yEnd, Color color);
        
        void drawRect(int xStart, int xEnd, int yStart, int yEnd, uint32 color);
        
        void drawRect(int xStart, int xEnd, int yStart, int yEnd, Color color);
        
        void drawRect(Recti rect, uint32 color);
        
        void drawRect(Recti rect, Color color);
        
        void drawRectAdditiveRelative(Rectf rect, Color color);
        
        void drawRectRelative(float xStart, float yStart, float xEnd, float yEnd, Color color);
        
        void drawRectRelative(Rectf rect, Color color);
        
        void drawLineRelative(float xStart, float yStart, float xEnd, float yEnd, Color color);
        
        void drawLineRelative(Vec2 start, Vec2 end, Color color);
        
        void drawTriangleRelative(Vec2 point0, Vec2 point1, Vec2 point2, Color color);
        
        void blitFilteredFast(const Surface *source, int targetX0, int targetY0, int targetX1, int targetY1, float sourceX0, float sourceY0, float sourceStepOffsetX, float sourceStepOffsetY, float sourceStepX, float sourceStepY);
        
        void blitNearestFast(const Surface *source, int targetX0, int targetY0, int targetX1, int targetY1, float sourceX0, float sourceY0, float sourceStepOffsetX, float sourceStepOffsetY, float sourceStepX, float sourceStepY);
        
        void blit(const Surface *source, Rectf targetRectAbsolute, Rectf sourceTextureCoordsRelative);
        
        void blit(const Surface *source, Rectf targetRectAbsolute);
                
        void blit(const Surface *source, float xAbsolute, float yAbsolute);
        
        void blit(const Surface *source, float x0Absolute, float y0Absolute, float x1Absolute, float y1Absolute);
        
        void blit(const Surface *source);

        void blitRelative(const Surface *source, Rectf targetRectRelative, Rectf sourceRect);
        
        void blitRelative(const Surface *source, Rectf targetRectRelative);
        
        bool blitEqualSizeSurface(const Surface *source);

        void fill(uint32 color);
        
        void multiplyPixelValues(float r, float g, float b, Recti targetRect);

        void multiplyPixelValues(float r, float g, float b, Rectf targetRect);

        void multiplyPixelValues(float r, float g, float b);

        void multiplyPixelValues(float val);
        
        void setClearColor(uint32 color);

        void clearTargetSurface();
        
        Recti getPixelRecti(Rectf relativeRect) const;

        Rectf getPixelRectf(Rectf relativeRect) const;

        void setFlagsEnabled(uint32 flags);

        void setFlag(uint32 flag, bool enabled);

        void setFlagEnabled(uint32 flag);

        void clearFlags();

        bool hasFlag(uint32 flag) const;
    };

    extern DECLSPEC uint32 graphicsCreateColor(uint8 a, uint8 r, uint8 g, uint8 b);
}