#pragma once
#include "jadel_rectf.h"
#include "jadel_recti.h"
#include "jadel_math.h"
#include "jadel_vec2.h"

namespace jadel
{
    Rectf::Rectf(float x0, float y0, float x1, float y1)
        : x0(x0), y0(y0), x1(x1), y1(y1)
    {
        this->orderLowToHigh();
    }

    Rectf::Rectf(Vec2 p0, Vec2 p1)
        : Rectf(p0.x, p0.y, p1.x, p1.y)
    {
    }

    Rectf::Rectf()
    {
    }

    void Rectf::orderLowToHigh()
    {
        JADEL_SWAP_IF_COMPARISON_TRUE(this->x0, this->x1, >);
        JADEL_SWAP_IF_COMPARISON_TRUE(this->y0, this->y1, >);
    }

    float Rectf::getWidth() const
    {
        float result = this->x1 - this->x0;
        return result;
    }

    float Rectf::getHeight() const
    {
        float result = this->y1 - this->y0;
        return result;
    }

    Vec2 Rectf::getDimensions() const
    {
        Vec2 result = getPointB() - getPointA();
        return result;
    }

    Vec2 Rectf::getPointA() const
    {
        Vec2 result(x0, y0);
        return result;
    }

    Vec2 Rectf::getPointB() const
    {
        Vec2 result(x1, y1);
        return result;
    }

    Recti Rectf::roundToRecti() const
    {
        Recti result(roundToInt(this->x0), roundToInt(this->y0), roundToInt(this->x1), roundToInt(this->y1));
        return result;
    }

    Rectf Rectf::operator+(Vec2 offset) const
    {
        Rectf result(this->getPointA() + offset, this->getPointB() + offset);
        return result;
    }

    Rectf &Rectf::operator+=(Vec2 offset)
    {
        this->x0 += offset.x;
        this->y0 += offset.y;
        this->x1 += offset.x;
        this->y1 += offset.y;
        return *this;
    }

    Rectf Rectf::operator-(Vec2 offset) const
    {
        Rectf result(this->getPointA() - offset, this->getPointB() - offset);
        return result;
    }

    Rectf Rectf::operator-(Rectf other) const
    {
        Rectf result(this->x0 - other.x0, this->y0 - other.y0, 
                    this->x1 - other.x1, this->y1 - other.y1);
        return result;
    }

    Rectf Rectf::operator-(Recti other) const
    {
        Rectf result = *this - (Rectf)other;
        return result;
    }

    Rectf Rectf::operator*(float scalar) const
    {
        Rectf result(getPointA() * scalar, getPointB() * scalar);
        return result;
    }

    Rectf &Rectf::operator*=(float scalar)
    {
        this->x0 *= scalar;
        this->y0 *= scalar;
        this->x1 *= scalar;
        this->y1 *= scalar;
        return *this;
    }

    Rectf Rectf::operator/(float val) const
    {
        if (val == 0)
        {
            return *this;
        }
        Rectf result(this->x0 / val, this->y0 / val, this->x1 / val, this->y1 / val);
        return result;
    }

    Rectf &Rectf::operator/=(float val)
    {
        if (val == 0)
        {
            return *this;
        }
        this->x0 /= val;
        this->y0 /= val;
        this->x1 /= val;
        this->y1 /= val;
        return *this;
    }

    Vec2 Rectf::operator/(Rectf other) const
    {
        Vec2 result(this->getWidth() / other.getWidth(),
                    this->getHeight() / other.getHeight());
        return result;
    }

    Rectf::operator Recti() const
    {
        Recti result((int)this->x0, (int)this->y0, (int)this->x1, (int)this->y1);
        return result;
    }
}