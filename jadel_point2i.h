#pragma once

namespace jadel
{
    struct Point2i
    {
        int x;
        int y;

        Point2i() = default;

        inline Point2i(int x, int y)
            : x(x), y(y)
        {
        }

        inline Point2i operator+(Point2i other) const
        {
            Point2i result = {x + other.x, y + other.y};
            return result;
        }

        inline Point2i operator-(Point2i other) const
        {
            Point2i result = {x - other.x, y - other.y};
            return result;
        }

        inline Point2i &operator+=(Point2i other)
        {
            *this = {x + other.x, y + other.y};
            return *this;
        }

        inline Point2i &operator-=(Point2i other)
        {
            *this = Point2i(x - other.x, y - other.y);
            return *this;
        }

        inline bool operator==(Point2i other) const
        {
            bool result = (this->x == other.x && this->y == other.y);
            return result;
        }

        inline bool operator!=(Point2i other) const
        {
            bool result = !(*this == other);
            return result;
        }
    };
}