#include "jadel_graphics_internal.h"
#include "jadel_util.h"
#include "jadel_endian.h"
#include "jadel_message.h"
#include "jadel_input.h"

namespace jadel
{
    struct WeightedTexelSquare
    {
        int leftIndex;
        int rightIndex;
        int bottomIndex;
        int topIndex;
        float weights[4];
    };

    Point2i *texelIndexBuffer;

    uint32 getAveragePixel(uint32 *pixels, uint32 numPixels)
    {
        uint32 averageAlpha = 0;
        uint32 averageRed = 0;
        uint32 averageGreen = 0;
        uint32 averageBlue = 0;
        for (uint32 i = 0; i < numPixels; ++i)
        {
            uint32 pixel = pixels[i];
            averageAlpha += (pixel & ALPHA_MASK) >> ALPHA_SHIFT;
            averageRed += (pixel & RED_MASK) >> RED_SHIFT;
            averageGreen += (pixel & GREEN_MASK) >> GREEN_SHIFT;
            averageBlue += (pixel & BLUE_MASK) >> BLUE_SHIFT;
        }
        averageAlpha /= numPixels;
        averageRed /= numPixels;
        averageGreen /= numPixels;
        averageBlue /= numPixels;

        uint32 result = (((averageAlpha << ALPHA_SHIFT)) |
                         ((averageRed << RED_SHIFT)) |
                         ((averageGreen << GREEN_SHIFT)) |
                         ((averageBlue << BLUE_SHIFT)));
        return result;
    }

    uint32 getWeightedAveragePixel(WeightedTexelSquare texelSquare, const Surface *source)
    {
        uint32 texels[4];
        texels[0] = JADEL_GRAPHICS_GET_PIXEL(texelSquare.leftIndex, texelSquare.bottomIndex, source);
        texels[1] = JADEL_GRAPHICS_GET_PIXEL(texelSquare.rightIndex, texelSquare.bottomIndex, source);
        texels[2] = JADEL_GRAPHICS_GET_PIXEL(texelSquare.leftIndex, texelSquare.topIndex, source);
        texels[3] = JADEL_GRAPHICS_GET_PIXEL(texelSquare.rightIndex, texelSquare.topIndex, source);
        float averageAlpha = 0;
        float averageRed = 0;
        float averageGreen = 0;
        float averageBlue = 0;
        for (uint32 i = 0; i < 4; ++i)
        {
            uint32 pixel = texels[i];
            float weight = texelSquare.weights[i];
            averageAlpha += (float)((pixel & ALPHA_MASK) >> ALPHA_SHIFT) * weight;
            averageRed += (float)((pixel & RED_MASK) >> RED_SHIFT) * weight;
            averageGreen += (float)((pixel & GREEN_MASK) >> GREEN_SHIFT) * weight;
            averageBlue += (float)((pixel & BLUE_MASK) >> BLUE_SHIFT) * weight;
        }

        uint32 result = (((uint32)averageAlpha << ALPHA_SHIFT) |
                         ((uint32)averageRed << RED_SHIFT) |
                         ((uint32)averageGreen << GREEN_SHIFT) |
                         ((uint32)averageBlue << BLUE_SHIFT));
        return result;
    }

    static Recti clampRecti(Recti rect, int minX, int minY, int maxX, int maxY)
    {
        Recti result(JADEL_CLAMP(rect.x0, minX, maxX), JADEL_CLAMP(rect.y0, minY, maxY),
                     JADEL_CLAMP(rect.x1, minX, maxX), JADEL_CLAMP(rect.y1, minY, maxY));
        return result;
    }

    // x and y must be higher than or equal to 0
    inline WeightedTexelSquare getClosestTexelSquare(float x, float y, const Surface *source)
    {
        int integerX = (int)x;
        int integerY = (int)y;
        float relativeX = x - (float)integerX;
        float relativeY = y - (float)integerY;

        int relativeLeft = -(roundToInt(1.0f + relativeX) - 1);
        int relativeRight = relativeLeft + 1;
        int relativeBottom = -(roundToInt(1.0f + relativeY) - 1);
        int relativeTop = relativeBottom + 1;

        WeightedTexelSquare result;

        float leftDist = -(0.5f - ((-relativeX) - 0.5f));
        float bottomDist = -(0.5f - ((-relativeY) - 0.5f));

        float leftDistSquared = square(leftDist);
        float bottomDistSquared = square(bottomDist);
        float rightDistSquared = square(1.0f - leftDist);
        float topDistSquared = square(1.0f - bottomDist);

        /*
                Vec2 bottomLeftVec(((-relativeX) - 0.5f) + 1.0f, ((-relativeY) - 0.5f) + 1.0f);
                Vec2 topRightVec = jadel::Vec2(1.0f, 1.0f) - bottomLeftVec;
                Vec2 topLeftVec = jadel::Vec2(bottomLeftVec.x, topRightVec.y);
                Vec2 bottomRightVec = jadel::Vec2(topRightVec.x, bottomLeftVec.y);
        */
        // float bottomLeftDistance = bottomLeftVec.length();
        // float topRightDistance = topRightVec.length();
        // float topLeftDistance = topLeftVec.length();
        // float bottomRightDistance = bottomRightVec.length();

        /*      float bottomLeftDistance = bottomLeftVec.x * bottomLeftVec.x + bottomLeftVec.y * bottomLeftVec.y;
              float topRightDistance = topRightVec.x * topRightVec.x + topRightVec.y * topRightVec.y;
              float topLeftDistance = topLeftVec.x * topLeftVec.x + topLeftVec.y * topLeftVec.y;
              float bottomRightDistance = bottomRightVec.x * bottomRightVec.x + bottomRightVec.y * bottomRightVec.y;
      */

        float bottomLeftDistance = (leftDistSquared + bottomDistSquared);
        float bottomRightDistance = (rightDistSquared + bottomDistSquared);
        float topLeftDistance = (leftDistSquared + topDistSquared);
        float topRightDistance = (rightDistSquared + topDistSquared);

        float oneOverTotalDistance = 1.0f / (bottomLeftDistance + topRightDistance + topLeftDistance + bottomRightDistance);

        result.weights[0] = (1.0f - (bottomLeftDistance * oneOverTotalDistance)) / 3.0f;
        result.weights[1] = (1.0f - (bottomRightDistance * oneOverTotalDistance)) / 3.0f;
        result.weights[2] = (1.0f - (topLeftDistance * oneOverTotalDistance)) / 3.0f;
        result.weights[3] = (1.0f - (topRightDistance * oneOverTotalDistance)) / 3.0f;
        int closestX = roundToInt(x);
        int closestY = roundToInt(y);
        // int closestX = x;
        // int closestY = y;
        result.leftIndex = closestX + relativeLeft;
        result.rightIndex = closestX + relativeRight;
        result.bottomIndex = closestY + relativeBottom;
        result.topIndex = closestY + relativeTop;

        return result;
    }

    inline WeightedTexelSquare getEdgeTexelSquare(float x, float y, const Surface *source)
    {
        WeightedTexelSquare result = getClosestTexelSquare(x, y, source);

        //result.leftIndex = result.leftIndex < 0 ? result.leftIndex + ((-(result.leftIndex / source->width) + 1) * source->width) : result.leftIndex;
        //result.rightIndex = result.rightIndex > source->width - 1 ? result.rightIndex - ((-(result.rightIndex / source->width) + 1) * source->width) : result.rightIndex;
        //result.bottomIndex = result.bottomIndex < 0 ? result.bottomIndex + ((-(result.bottomIndex / source->height) + 1) * source->height) : result.bottomIndex;
        //result.topIndex = result.topIndex > source->height - 1 ? result.topIndex - ((-(result.topIndex / source->height) + 1) * source->height) : result.topIndex;
        
        if (result.leftIndex < 0 || result.leftIndex > source->width - 1) result.leftIndex -= (result.leftIndex / absInt(result.leftIndex)) * source->width;
        if (result.rightIndex < 0 || result.rightIndex > source->width - 1) result.rightIndex -= (result.rightIndex / absInt(result.rightIndex)) * source->width;
        if (result.bottomIndex < 0 || result.bottomIndex > source->height - 1) result.bottomIndex -= (result.bottomIndex / absInt(result.bottomIndex)) * source->height;
        if (result.topIndex < 0 || result.topIndex > source->height - 1) result.topIndex -= (result.topIndex / absInt(result.topIndex)) * source->height;
        return result;
    }

    // void Graphics::blitFilteredFast(const Surface *source, int targetX0, int targetY0, int targetX1, int targetY1, int sourceX0, int sourceY0, int sourceX1, int sourceY1, float sourceXStep, float sourceYStep)
    // {
    //     int sourceWidth = sourceX1 - sourceX0;
    //     int sourceHeight = sourceY1 - sourceY0;

    //     int numXSourceRects = (int)((float)sourceWidth / sourceXStep);
    //     int numYSourceRects = (int)((float)sourceHeight / sourceYStep);
    //     int sourceYStart = 0;
    //     for (int y = 0; y < numYSourceRects; ++y)
    //     {
    //         uint32 filterPixels[9];
    //         // int sourceYStart = jadel::roundToInt((float)y * sourceYStep);
    //         // int sourceYEnd = jadel::roundToInt((float)(y + 1) * sourceYStep);

    //         int sourceYEnd = roundToInt((float)(y + 1) * sourceYStep);

    //         int numYPixels = sourceYEnd - sourceYStart;
    //         int yPixelsToAvg = numYPixels > 3 ? 3 : numYPixels;
    //         int bottomPixel = yPixelsToAvg < 3 ? 0 : (numYPixels % 2 == 0 ? numYPixels / 2 - 2 : numYPixels / 2 - 1);
    //         // int bottomPixel = jadel::roundToInt((float)numYPixels * 0.5f) - 1;

    //         int sourceXStart = 0;
    //         for (int x = 0; x < numXSourceRects; ++x)
    //         {
    //             int sourceXEnd = roundToInt((float)(x + 1) * sourceXStep);

    //             int numXPixels = sourceXEnd - sourceXStart;
    //             int xPixelsToAvg = numXPixels > 3 ? 3 : numXPixels;
    //             int leftPixel = xPixelsToAvg < 3 ? 0 : (numXPixels % 2 == 0 ? numXPixels / 2 - 2 : numXPixels / 2 - 1);
    //             // int leftPixel = jadel::roundToInt((float)numXPixels * 0.5f) - 1;

    //             int numFilterPixels = 0;
    //             for (int filY = 0; filY < yPixelsToAvg; ++filY)
    //             {
    //                 for (int filX = 0; filX < xPixelsToAvg; ++filX)
    //                 {
    //                     filterPixels[numFilterPixels++] = JADEL_GRAPHICS_GET_PIXEL(sourceX0 + sourceXStart + leftPixel + filX, sourceY0 + sourceYStart + bottomPixel + filY, source);
    //                 }
    //             }
    //             uint32 backgroundPixel = JADEL_GRAPHICS_GET_BACKGROUND_PIXEL(targetX0 + x, targetY0 + y);
    //             uint32 foregroundPixel;
    //             if (numFilterPixels == 0)
    //             {
    //                 foregroundPixel = JADEL_GRAPHICS_GET_PIXEL(sourceX0 + sourceXStart, sourceY0 + sourceYStart, source);
    //             }
    //             else
    //             {
    //                 foregroundPixel = getAveragePixel(filterPixels, numFilterPixels);
    //             }
    //             uint32 finalPixel = Color::blendU32(foregroundPixel, backgroundPixel);
    //             JADEL_GRAPHICS_DRAW_PIXEL_FAST(targetX0 + x, targetY0 + y, finalPixel);
    //             // graphics.drawRectFast(xOffset + sourceXStart, xOffset + sourceXStart + sourceXEnd, yOffset + sourceYStart, yOffset + sourceYStart + sourceYEnd, color);
    //             sourceXStart = sourceXEnd;
    //         }
    //         sourceYStart = sourceYEnd;
    //     }
    // }

    /*
        void Graphics::blitFilteredFast(const Surface *source, int targetX0, int targetY0, int targetX1, int targetY1, int sourceX0, int sourceY0, int sourceX1, int sourceY1, float sourceXStep, float sourceYStep)
        {
            uint32 filterPixels[9];

            float currentYStep = (float)sourceY0;
            int sourceCurrentStartY = roundToInt(currentYStep);
            for (int y = 0; y < targetY1 - targetY0; ++y)
            {
                int sourceCurrentEndY = roundToInt(currentYStep + sourceYStep);
                int numSourcePixelsY = sourceCurrentEndY - sourceCurrentStartY;
                int numYPixelsToFilter = numSourcePixelsY > 3 ? 3 : numSourcePixelsY;
                //int bottomSourceY = sourceCurrentStartY + roundToInt((float)numSourcePixelsY * 0.5f) - 1;
                int bottomSourceY = sourceCurrentStartY + numYPixelsToFilter < 3 ? 0 : (numSourcePixelsY % 2 == 0 ? numSourcePixelsY / 2 - 2: numSourcePixelsY / 2 - 1);
                //int topSourceY = bottomSourceY + numYPixelsToFilter;
                // float midY = sourceStartY + (float)y * yStep - yStep * 0.5f;
                // int midIndexY = roundToInt(midY);

                float currentXStep = (float)sourceX0;
                int sourceCurrentStartX = roundToInt(currentXStep);
                for (int x = 0; x < targetX1 - targetX0; ++x)
                {
                    int sourceCurrentEndX = roundToInt(currentXStep + sourceXStep);
                    int numSourcePixelsX = sourceCurrentEndX - sourceCurrentStartX;
                    int numXPixelsToFilter = numSourcePixelsX > 3 ? 3 : numSourcePixelsX;
                    //int leftSourceX = sourceCurrentStartX + roundToInt((float)numSourcePixelsX * 0.5f) - 1;
                    int leftSourceX = sourceCurrentStartX + numXPixelsToFilter < 3 ? 0 : (numSourcePixelsX % 2 == 0 ? numSourcePixelsX / 2 - 2: numSourcePixelsX / 2 - 1);
                    uint32 i = 0;
                    for (int yy = 0; yy < numYPixelsToFilter; ++yy)
                    {
                        uint32 yIndex = JADEL_CLAMP(bottomSourceY + yy, sourceY0, sourceY1);
                        // uint32 yIndex = bottomSourceY + yy;
                        for (int xx = 0; xx < numXPixelsToFilter; ++xx)
                        {
                            uint32 xIndex = JADEL_CLAMP(leftSourceX + xx, sourceX0, sourceX1);
                            //  uint32 xIndex = leftSourceX + xx;
                            filterPixels[i++] = JADEL_GRAPHICS_GET_PIXEL(xIndex, yIndex, source);
                        }
                    }
                    uint32 foregroundPixel;
                    if (i < 10)
                    {
                        foregroundPixel = getAveragePixel(filterPixels, i);
                    }
                    else
                    {
                        jadel::message("ERROR: Num pixels %d\n", i);
                        return;
                    }
                    // float midX = sourceStartX + (float)x * xStep - xStep * 0.5f;
                    // int midIndexX = roundToInt(midX);
           //          Recti filterRect(midIndexX,
           //                           midIndexY,
           //                           midIndexX + 1,
           //                           midIndexY + 1);
           //      filterRect = clampRecti(filterRect, 0, 0,
           //                              source->width - 1, source->height - 1);
           //      filterPixels[0] = JADEL_GRAPHICS_GET_PIXEL(filterRect.x0, filterRect.y0, source);
           //      filterPixels[1] = JADEL_GRAPHICS_GET_PIXEL(filterRect.x1, filterRect.y0, source);
           //      filterPixels[2] = JADEL_GRAPHICS_GET_PIXEL(filterRect.x0, filterRect.y1, source);
           //      filterPixels[3] = JADEL_GRAPHICS_GET_PIXEL(filterRect.x1, filterRect.y1, source);

                    // uint32 foregroundPixel = JADEL_GRAPHICS_GET_PIXEL(roundToInt(sourceStartX + (float)x * xStep), roundToInt(sourceStartY + (float)y * yStep), source);
                    // uint32 foregroundPixel = filterPixels[0];
                    //              uint32 foregroundPixel = getAveragePixel(filterPixels, 4);
                    uint32 backgroundPixel = JADEL_GRAPHICS_GET_BACKGROUND_PIXEL(targetX0 + x, targetY0 + y);
                    uint32 finalPixel = Color::blendU32(foregroundPixel, backgroundPixel);
                    JADEL_GRAPHICS_DRAW_PIXEL_FAST(targetX0 + x, targetY0 + y, finalPixel);
                    // midX += xStep;
                    currentXStep += sourceXStep;
                    sourceCurrentStartX = sourceCurrentEndX;
                }
                currentYStep += sourceYStep;
                sourceCurrentStartY = sourceCurrentEndY;

                // midY += yStep;
            }
        }

    */

    class BilinearFilter
    {
        const Surface *_sourceTexture;
        Vec2 _sourceTextureStart;
        Vec2 _sourceStep;
        Vec2 _sourceStepInitialOffset;
        Recti _targetPixelRect;
        Graphics *_graphics;

    public:
        BilinearFilter(const Surface *source, Vec2 sourceTexStart, Vec2 sourceStep, Vec2 sourceStepInitialOffset, Recti targetRect, Graphics *graphics)
            : _sourceTexture(source),
              _sourceTextureStart(sourceTexStart),
              _sourceStepInitialOffset(sourceStepInitialOffset),
              _sourceStep(sourceStep),
              _targetPixelRect(targetRect),
              _graphics(graphics)
        {
        }

        void blitFilteredEdgeFast(int targetOffsetStartX, int targetOffsetStartY, int targetOffsetEndX, int targetOffsetEndY)
        {
            float currentEdgeYStep = (_sourceTextureStart.y - _sourceStepInitialOffset.y) + (targetOffsetStartY * _sourceStep.y) + (_sourceStep.y * 0.5);
            for (int y = _targetPixelRect.y0 +  targetOffsetStartY; y < _targetPixelRect.y0 + targetOffsetEndY; ++y)
            {
                float currentEdgeXStep = (_sourceTextureStart.x - _sourceStepInitialOffset.x) + (targetOffsetStartX * _sourceStep.x) + (_sourceStep.x * 0.5);
                for (int x = _targetPixelRect.x0 + targetOffsetStartX ; x < _targetPixelRect.x0 + targetOffsetEndX; ++x)
                {
                    WeightedTexelSquare filterTexelSquare = getEdgeTexelSquare(currentEdgeXStep, currentEdgeYStep, _sourceTexture);
                    uint32 foregroundPixel = getWeightedAveragePixel(filterTexelSquare, _sourceTexture);
                    uint32 backgroundPixel = JADEL_GRAPHICS_GET_PIXEL(x, y, _graphics->targetSurfaceData);
                    // uint32 finalPixel = Color::blendU32(foregroundPixel, backgroundPixel);
                    uint32 finalPixel = colorBlend(foregroundPixel, backgroundPixel);
                    JADEL_GRAPHICS_DRAW_PIXEL_FAST(x, y, finalPixel, _graphics->targetSurfaceData);
                    currentEdgeXStep += _sourceStep.x;
                }
                currentEdgeYStep += _sourceStep.y;
            }
        }
    };

    void Graphics::blitFilteredFast(const Surface *source, int targetX0, int targetY0, int targetX1, int targetY1, float sourceX0, float sourceY0, float sourceStepOffsetX, float sourceStepOffsetY, float sourceStepX, float sourceStepY)
    {

        BilinearFilter filter(source, Vec2(sourceX0, sourceY0), Vec2(sourceStepX, sourceStepY), Vec2(sourceStepOffsetX, sourceStepOffsetY), Recti(targetX0, targetY0, targetX1, targetY1), this);
        float targetWidth = (float)targetX1 - (float)targetX0;
        float targetHeight = (float)targetY1 - (float)targetY0;
    
        // TODO: Make sure these are pixel perfect
        Vec2 targetPixelsPerSourceTexel(1.0f / sourceStepX, 1.0f / sourceStepY);
        // float nonEdgeSourceStepXStart = targetPixelsPerSourceTexel.x;
        // float nonEdgeSourceStepYStart = targetPixelsPerSourceTexel.y;
        // float nonEdgeSourceStepXEnd = (float)targetWidth - targetPixelsPerSourceTexel.x;
        // float nonEdgeSourceStepYEnd = (float)targetHeight - targetPixelsPerSourceTexel.y;

        bool leftSourceEdgePassed = sourceX0 + sourceStepOffsetX >= targetPixelsPerSourceTexel.x * 0.5f;
        bool bottomSourceEdgePassed = sourceY0 + sourceStepOffsetY >= targetPixelsPerSourceTexel.y * 0.5f;
        int targetStartIndexOffsetX = leftSourceEdgePassed ? 0 : roundToInt(targetPixelsPerSourceTexel.x * 0.5f);
        int targetStartIndexOffsetY = bottomSourceEdgePassed ? 0 : roundToInt(targetPixelsPerSourceTexel.y * 0.5f);

        int targetEndIndexOffsetX = (int)targetWidth - roundToInt(targetPixelsPerSourceTexel.x) - 1;
        int targetEndIndexOffsetY = (int)targetHeight - roundToInt(targetPixelsPerSourceTexel.y) - 1;

        if (!leftSourceEdgePassed)
        {
            filter.blitFilteredEdgeFast(0, 0, targetStartIndexOffsetX, (int)targetHeight);
            // blitFilteredEdgeFast(source, sourceStepX, sourceStepY, targetX0, targetY0, targetStartIndexOffsetX, targetY1, sourceX0, sourceY0, sourceStepOffsetX, sourceStepOffsetY, this);
        }
        if (!bottomSourceEdgePassed)
        {
            filter.blitFilteredEdgeFast(targetStartIndexOffsetX, 0, targetEndIndexOffsetX, targetStartIndexOffsetY);
        filter.blitFilteredEdgeFast(targetEndIndexOffsetX, 0, (int)targetWidth, (int)targetHeight);
            // blitFilteredEdgeFast(source, sourceStepX, sourceStepY, targetX0, targetY0, targetEndIndexX, targetY0, (int)targetWidth, (int)targetHeight, sourceStepOffsetX, sourceStepOffsetY, this);
            // blitFilteredEdgeFast(source, sourceStepX, sourceStepY, targetX0, targetY0, targetStartIndexX, 0, targetEndIndexX, targetStartIndexY, sourceStepOffsetX, sourceStepOffsetY, this);
        }
        filter.blitFilteredEdgeFast(targetStartIndexOffsetX, targetEndIndexOffsetY, targetEndIndexOffsetX, (int)targetHeight);
        // blitFilteredEdgeFast(source, sourceStepX, sourceStepY, targetX0, targetY0, targetStartIndexX, targetEndIndexY, targetEndIndexX, (int)targetHeight, sourceStepOffsetX, sourceStepOffsetY, this);

        float currentYStep = sourceY0 - sourceStepOffsetY + targetStartIndexOffsetY * sourceStepY + 0.5f * sourceStepY;
        for (int y = targetY0 + targetStartIndexOffsetY; y < targetY0 + targetEndIndexOffsetY; ++y)
        {
            float currentXStep = sourceX0 - sourceStepOffsetX + targetStartIndexOffsetX * sourceStepX + 0.5f * sourceStepX;
            for (int x = targetX0 + targetStartIndexOffsetX; x < targetX0 + targetEndIndexOffsetX; ++x)
            {
                WeightedTexelSquare filterTexelSquare = getClosestTexelSquare(currentXStep, currentYStep, source);
                uint32 foregroundPixel = getWeightedAveragePixel(filterTexelSquare, source);
                uint32 backgroundPixel = JADEL_GRAPHICS_GET_BACKGROUND_PIXEL(x, y);
                uint32 finalPixel = Color::blendU32(foregroundPixel, backgroundPixel);
                JADEL_GRAPHICS_DRAW_PIXEL_FAST(x, y, finalPixel);
                currentXStep += sourceStepX;
            }
            currentYStep += sourceStepY;
        }
    }

    void Graphics::blitNearestFast(const Surface *source, int targetX0, int targetY0, int targetX1, int targetY1, float sourceX0, float sourceY0, float sourceStepOffsetX, float sourceStepOffsetY, float sourceStepX, float sourceStepY)
    {
        float sourceCurrentStepY = sourceY0 - sourceStepOffsetY + 0.5f * sourceStepY;
        for (int y = targetY0; y < targetY1; ++y)
        {
            float sourceCurrentStepX = sourceX0 - sourceStepOffsetX + 0.5f * sourceStepX;
            for (int x = targetX0; x < targetX1; ++x)
            {
                uint32 fgPixel = JADEL_GRAPHICS_GET_PIXEL((int)(sourceCurrentStepX),
                                                          (int)(sourceCurrentStepY), source);
                uint32 bgPixel = JADEL_GRAPHICS_GET_BACKGROUND_PIXEL(x, y);

                //uint32 finalPixel = Color::blendU32(fgPixel, bgPixel);
                uint32 finalPixel = colorBlend(fgPixel, bgPixel);
                JADEL_GRAPHICS_DRAW_PIXEL_FAST(x, y, finalPixel);
                sourceCurrentStepX += sourceStepX;
            }
            sourceCurrentStepY += sourceStepY;
        }
    }

    void Graphics::blit(const Surface *source, Rectf targetRect, Rectf sourceTextureCoords)
    {
        Rectf workingTargetRect = targetRect;
        workingTargetRect.orderLowToHigh();
        if (!graphicsRectfIsPartlyInbounds(workingTargetRect, this))
        {
            return;
        }

        Recti pixelTargetRect = workingTargetRect.roundToRecti();

        Rectf targetRemainder = workingTargetRect - pixelTargetRect;

        Vec2 sourceStep = sourceTextureCoords / workingTargetRect;

        float sourceStepOffsetX = targetRemainder.x0 * sourceStep.x;
        float sourceStepOffsetY = targetRemainder.y0 * sourceStep.y;
        Recti finalTargetRect = graphicsGetTargetPixelAreaFromRectf(workingTargetRect, this);
        float finalSourceX0 = pixelTargetRect.x0 < 0 ? sourceTextureCoords.x0 - ((float)pixelTargetRect.x0 * sourceStep.x) : sourceTextureCoords.x0;
        float finalSourceY0 = pixelTargetRect.y0 < 0 ? sourceTextureCoords.y0 - ((float)pixelTargetRect.y0 * sourceStep.y) : sourceTextureCoords.y0;

        if (this->hasFlag(JADEL_GRAPHICS_BLIT_FILTER_BILINEAR))
        {
            blitFilteredFast(source, finalTargetRect.x0, finalTargetRect.y0, finalTargetRect.x1, finalTargetRect.y1, finalSourceX0, finalSourceY0, sourceStepOffsetX, sourceStepOffsetY, sourceStep.x, sourceStep.y);
        }
        else
        {
            blitNearestFast(source, finalTargetRect.x0, finalTargetRect.y0, finalTargetRect.x1, finalTargetRect.y1, finalSourceX0, finalSourceY0, sourceStepOffsetX, sourceStepOffsetY, sourceStep.x, sourceStep.y);
        }
    }

    void Graphics::blit(const Surface *source, Rectf targetRectAbsolute)
    {
        Rectf sourceTextureCoords(graphicsGetTextureCoordPixelPosf(0, 0, source), graphicsGetTextureCoordPixelPosf(1.0f, 1.0f, source));
        blit(source, targetRectAbsolute, sourceTextureCoords);
    }

    void Graphics::blit(const Surface *source, float xAbsolute, float yAbsolute)
    {
        Rectf targetRectAbsolute(xAbsolute, yAbsolute, xAbsolute + (float)source->width, yAbsolute + (float)source->height);
        blit(source, targetRectAbsolute);
    }

    void Graphics::blit(const Surface *source, float x0Absolute, float y0Absolute, float x1Absolute, float y1Absolute)
    {
        Rectf targetRectAbsolute(x0Absolute, y0Absolute, x1Absolute, y1Absolute);
        blit(source, targetRectAbsolute);
    }

    void Graphics::blitRelative(const Surface *source, Rectf targetRectRelative, Rectf sourceTextureCoordsRelative)
    {
        Rectf targetRectAbsolute = getPixelRectf(targetRectRelative);
        Rectf sourceRectTexCoords(graphicsGetTextureCoordPixelPosf(sourceTextureCoordsRelative.x0, sourceTextureCoordsRelative.y0, source),
                                  graphicsGetTextureCoordPixelPosf(sourceTextureCoordsRelative.x1, sourceTextureCoordsRelative.y1, source));
        blit(source, targetRectAbsolute, sourceRectTexCoords);
    }

    void Graphics::blitRelative(const Surface *source, Rectf targetRectRelative)
    {
        Rectf sourceTextureCoordsRelative(0, 0, 1.0f, 1.0f);
        blitRelative(source, targetRectRelative, sourceTextureCoordsRelative);
    }

    void Graphics::blit(const Surface *source)
    {
        if (!this->hasFlag(JADEL_GRAPHICS_BLIT_FILTER_BILINEAR) && blitEqualSizeSurface(source))
        {
            return;
        }
        blitRelative(source, Rectf(-1.0f, -1.0f, 1.0f, 1.0f));
    }

    bool Graphics::blitEqualSizeSurface(const Surface *source)
    {
        if (source->width != targetSurface->width || source->height != targetSurface->height)
            return false;
        bool result = memcpy(targetSurfaceData->pixels, source->pixels, source->width * source->height * 4);
        return result != NULL;
    }
}