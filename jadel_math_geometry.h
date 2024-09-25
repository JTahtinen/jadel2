#pragma once
#include "jadel_math.h"
#include "jadel_rectf.h"
#include "jadel_recti.h"
#include "jadel_point2i.h"
#include "jadel_vec2.h"
#include "jadel_vec3.h"
#include "jadel_mat3.h"
#include "jadel_mat4.h"

namespace jadel
{
    inline Rectf operator*(Mat3 left, Rectf right)
    {
        Rectf result(left * right.getPointA(), left * right.getPointB());
        return result;
    }

    inline Rectf operator*(Mat4 left, Rectf right)
    {
        Rectf result(left * right.getPointA(), left * right.getPointB());
        return result;
    }

    inline Rectf operator*(Rectf left, Mat3 right)
    {
        Rectf result(left.getPointA() * right, left.getPointB() * right);
        return result;
    }

    inline Rectf operator*(Rectf left, Mat4 right)
    {
        Rectf result(left.getPointA() * right, left.getPointB() * right);
        return result;
    }

    inline float getAbsolutePointfBetween(float relativePoint, float min, float max)
    {
        float halfWidth = (max - min) * 0.5f;
        float result = relativePoint * halfWidth + halfWidth;
        return result;
    }

    inline Vec2 getPointfRelativeToCenterInRecti(Vec2 point, Recti rect)
    {
        Vec2 result(getAbsolutePointfBetween(point.x, (float)rect.x0, (float)rect.x1),
                    getAbsolutePointfBetween(point.y, (float)rect.y0, (float)rect.y1));
        return result;
    }

    inline int getAbsolutePointiBetween(float relativePoint, int min, int max)
    {
        int result = roundToInt(getAbsolutePointfBetween(relativePoint, (float)min, (float)max));
        return result;
    }

    //**** getPointiRelativeToCenterInRecti ****//
    inline Point2i getPointiRelativeToCenterInRecti(float x, float y, int xStart, int yStart, int xEnd, int yEnd)
    {
        Point2i result(getAbsolutePointiBetween(x, xStart, xEnd), getAbsolutePointiBetween(y, yStart, yEnd));
        return result;
    }

    inline Point2i getPointiRelativeToCenterInRecti(float xStart, float yStart, Recti rect)
    {
        Point2i result(getPointiRelativeToCenterInRecti(xStart, yStart, rect.x0, rect.y0, rect.x1, rect.y1));
        return result;
    }

    inline Point2i getPointiRelativeToCenterInRecti(Vec2 point, Recti rect)
    {
        Point2i result(getPointiRelativeToCenterInRecti(point.x, point.y, rect));
        return result;
    }

    //********************************************//

    //**** pointfWithinRectf ****//
    inline bool pointfWithinRectf(float x, float y, float xStart, float yStart, float xEnd, float yEnd)
    {
        bool result = (JADEL_IS_VAL_BETWEEN(x, xStart, xEnd) &&
                       JADEL_IS_VAL_BETWEEN(y, yStart, yEnd));
        return result;
    }

    inline bool pointfWithinRectf(float x, float y, Rectf rect)
    {
        bool result = pointfWithinRectf(x, y, rect.x0, rect.y0, rect.x1, rect.y1);
        return result;
    }

    inline bool pointfWithinRectf(jadel::Vec2 point, Rectf rect)
    {
        bool result = pointfWithinRectf(point.x, point.y, rect);
        return result;
    }
}