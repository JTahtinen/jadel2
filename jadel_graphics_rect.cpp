#include "jadel_graphics_internal.h"

namespace jadel
{

    static Rectf clipRectf(Rectf rect, Rectf clippingPlane)
    {
        Rectf result;
        result.x0 = rect.x0 < clippingPlane.x0 ? clippingPlane.x0 : rect.x0;
        result.x1 = rect.x1 > clippingPlane.x1 ? clippingPlane.x1 : rect.x1;
        result.y0 = rect.y0 < clippingPlane.y0 ? clippingPlane.y0 : rect.y0;
        result.y1 = rect.y1 > clippingPlane.y1 ? clippingPlane.y1 : rect.y1;
        return result;
    }

    static bool isRectfVisible(Rectf rect, Rectf clippingPlane)
    {
        bool result = (rect.x0 < clippingPlane.x1 && rect.x1 > clippingPlane.x0
                    && rect.y0 < clippingPlane.y1 && rect.y1 > clippingPlane.y0);
        return result;
    }

    void Graphics::drawRectFast(int xStart, int xEnd, int yStart, int yEnd, Color color)
    {
        for (int y = yStart; y < yEnd; ++y)
        {
            for (int x = xStart; x < xEnd; ++x)
            {
                uint32 finalColor = (color.blendU32(JADEL_GRAPHICS_GET_BACKGROUND_PIXEL(x, y)));
                drawPixelFast(x, y, finalColor);
            }
        }
    }

    void Graphics::drawRect(int xStart, int xEnd, int yStart, int yEnd, Color color)
    {
        if (!graphicsValidateAndModifyRectDimensions(&xStart, &yStart, &xEnd, &yEnd, this))
        {
            return;
        }
        drawRectFast(xStart, xEnd, yStart, yEnd, color);
    }

    void Graphics::drawRect(int xStart, int xEnd, int yStart, int yEnd, uint32 color)
    {
        drawRect(xStart, xEnd, yStart, yEnd, Color(color));
    }

    void Graphics::drawRect(Recti rect, uint32 color)
    {
        drawRect(rect.x0, rect.x0 + rect.x1, rect.y0, rect.y0 + rect.y1, color);
    }

    void Graphics::drawRect(Recti rect, Color color)
    {
        drawRect(rect.x0, rect.x1, rect.y0, rect.y1, color);
    }

    void Graphics::drawRectAdditiveRelative(Rectf rect, Color color)
    {
        Recti pixelRect = getPixelRecti(rect);
        if (!graphicsValidateAndModifyRectDimensions(&pixelRect, this))
        {
            return;
        }
        for (int y = pixelRect.y0; y < pixelRect.y1; ++y)
        {
            for (int x = pixelRect.x0; x < pixelRect.x1; ++x)
            {
                uint32 finalColor = (color + (JADEL_GRAPHICS_GET_BACKGROUND_PIXEL(x, y))).toU32();
                drawPixelFast(x, y, finalColor);
            }
        }
    }

    void Graphics::drawRectRelative(float xStart, float yStart, float xEnd, float yEnd, Color color)
    {   
        drawRectRelative(jadel::Rectf(xStart, yStart, xEnd, yEnd), color);
    }

    void Graphics::drawRectRelative(Rectf rect, Color color)
    {
        if (!isRectfVisible(rect, this->targetSurface->clippingPlaneStack.top()))
        {
            return;
        }
        jadel::Rectf clippedRelativeRect = clipRectf(rect, this->targetSurface->clippingPlaneStack.top());
        Recti recti = getPixelRecti({clippedRelativeRect.x0, clippedRelativeRect.y0, clippedRelativeRect.x1, clippedRelativeRect.y1});
        drawRect(recti, color);
    }

    bool graphicsValidateAndModifyRectDimensions(int *x0, int *y0, int *x1, int *y1, Graphics *graphics)
    {
        JADEL_SWAP_IF_COMPARISON_TRUE(*x0, *x1, >);
        JADEL_SWAP_IF_COMPARISON_TRUE(*y0, *y1, >);
        int width = graphics->targetSurface->width;
        int height = graphics->targetSurface->height;
        if (*x0 >= width ||
            *y0 >= height ||
            *x1 <= 0 ||
            *y1 <= 0)
        {
            return false;
        }
        *x0 = JADEL_CLAMP_MIN(*x0, 0);
        *y0 = JADEL_CLAMP_MIN(*y0, 0);
        *x1 = JADEL_CLAMP_MAX(*x1, width);
        *y1 = JADEL_CLAMP_MAX(*y1, height);

        return true;
    }

    bool graphicsRectfIsPartlyInbounds(float xStart, float yStart, float xEnd, float yEnd, Graphics *graphics)
    {
        float width = (float)graphics->targetSurface->width;
        float height = (float)graphics->targetSurface->height;
        bool result = (xStart < width ||
                       yStart < height ||
                       xEnd > 0 ||
                       yEnd < 0);
        return result;
    }

    bool graphicsRectfIsPartlyInbounds(Rectf rect, Graphics *graphics)
    {
        bool result = graphicsRectfIsPartlyInbounds(rect.x0, rect.y0, rect.x1, rect.y1, graphics);
        return result;
    }

    bool graphicsValidateAndModifyRectDimensions(Recti *rect, Graphics *graphics)
    {
        bool result = graphicsValidateAndModifyRectDimensions(&rect->x0, &rect->y0, &rect->x1, &rect->y1, graphics);
        return result;
    }

    Recti graphicsGetTargetPixelAreaFromRectf(Rectf targetRect, Graphics *graphics)
    {
        Recti result;
        result.x0 = JADEL_CLAMP_MIN(roundToInt(targetRect.x0), 0);
        result.y0 = JADEL_CLAMP_MIN(roundToInt(targetRect.y0), 0);
        result.x1 = JADEL_CLAMP_MAX(roundToInt(targetRect.x1), graphics->targetSurface->width);
        result.y1 = JADEL_CLAMP_MAX(roundToInt(targetRect.y1), graphics->targetSurface->height);
        return result;
    }

}