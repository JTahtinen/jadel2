#pragma once

#include "jadel_vec2.h"

namespace jadel
{
    struct Recti;
    
    struct DECLSPEC Rectf
    {
        float x0;
        float y0;
        float x1;
        float y1;

        Rectf(float x0, float y0, float x1, float y1);
        
        Rectf(Vec2 p0, Vec2 p1);
        
        Rectf();

        void orderLowToHigh();
        
        float getWidth() const;
        
        float getHeight() const;

        Vec2 getDimensions() const;

        Vec2 getPointA() const;

        Vec2 getPointB() const;

        Recti roundToRecti() const;

        Rectf operator+(Vec2 offset) const;
        
        Rectf &operator+=(Vec2 offset);
        
        Rectf operator-(Vec2 offset) const;
        
        Rectf operator-(Rectf other) const;

        Rectf operator-(Recti other) const;
        
        Rectf operator*(float scalar) const;
        
        Rectf &operator*=(float scalar);
        
        Rectf operator/(float val) const;
        
        Rectf &operator/=(float val);

        Vec2 operator/(Rectf other) const;

        operator Recti() const;
    };
}