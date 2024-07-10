#pragma once

#include "jadel_point2i.h"
#include "jadel_math.h"
#include "jadel_recti.h"
#include "jadel_rectf.h"

namespace jadel
{
    Recti::Recti(int x0, int y0, int x1, int y1)
        : x0(x0), y0(y0), x1(x1), y1(y1)
    {
    }

    Recti::Recti(Point2i a, Point2i b)
        : Recti(a.x, a.y, b.x, b.y)
    {
    }

    Recti::Recti()
        : Recti(0, 0, 0, 0)
    {
    }

    void Recti::orderLowToHigh()
    {
        JADEL_SWAP_IF_COMPARISON_TRUE(this->x0, this->x1, >);
        JADEL_SWAP_IF_COMPARISON_TRUE(this->y0, this->y1, >);
    }

    int Recti::getWidth() const
    {
        int result = this->x1 - this->x0;
        return result;
    }

    int Recti::getHeight() const
    {
        int result = this->y1 - this->y0;
        return result;
    }

    Point2i Recti::getDimensions() const
    {
        Point2i result = this->getPointB() - this->getPointA();
        return result;
    }

    Point2i Recti::getPointA() const
    {
        Point2i result(x0, y0);
        return result;
    }

    Point2i Recti::getPointB() const
    {
        Point2i result(x1, y1);
        return result;
    }

    Recti::operator Rectf() const
    {
        Rectf result((float)this->x0, (float)this->y0, (float)this->x1, (float)this->y1);
        return result;
    }
};
