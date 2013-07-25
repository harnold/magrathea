#ifndef interface_Point_h
#define interface_Point_h

#include "support/Utilities.h"

struct Rect;

//----------------------------------------------------------------------------

struct Point
{
    int x, y;

    Point() {}

    Point(int x, int y)
        : x(x), y(y) {}

    bool operator==(const Point& p) const
    { return x == p.x && y == p.y; }

    Point operator+(const Point& p) const
    { return Point(x + p.x, y + p.y); }

    Point operator-(const Point& p) const
    { return Point(x - p.x, y - p.y); }

    Point& operator+=(const Point& p)
    { x += p.x; y += p.y; return *this; }

    Point& operator-=(const Point& p)
    { x -= p.x; y -= p.y; return *this; }

    void Set(int x, int y)
    { this->x = x; this->y = y; }

    void ConstrainTo(const Rect& r);
};

//----------------------------------------------------------------------------

#endif
