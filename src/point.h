#pragma once

#include <stdint.h>

struct Point
{
    Point(int16_t x, int16_t y)
        : x(x)
        , y(y)
    {
    }

    Point operator+(const Point& v) const
    {
        return Point(x + v.x, y + v.y);
    }

    int16_t x;
    int16_t y;
};
