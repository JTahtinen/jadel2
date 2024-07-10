#pragma once

namespace jadel
{
    struct Rectf;

    struct Point2i;

    struct DECLSPEC Recti
    {
        int x0;
        int y0;
        int x1;
        int y1;

        Recti(int x0, int y0, int x1, int y1);
        
        Recti(Point2i a, Point2i b);

        Recti();

        void orderLowToHigh();

        int getWidth() const;

        int getHeight() const;
        
        Point2i getDimensions() const;

        Point2i getPointA() const;
        
        Point2i getPointB() const;
        
        operator Rectf() const;
    };
}