#ifndef interface_Rect_h
#define interface_Rect_h

#include "interface/Point.h"
#include "interface/Size.h"
#include "support/Utilities.h"

//----------------------------------------------------------------------------

struct Rect
{
    int x, y, width, height;

    Rect() {}

    Rect(int x, int y, int width, int height)
        : x(x), y(y), width(width), height(height) {}

    Rect(const Point& p, const Size& s)
        : x(p.x), y(p.y), width(s.width), height(s.height) {}

    Rect(const Point& topLeft, const Point& bottomRight)
        : x(topLeft.x),
          y(topLeft.y),
          width(bottomRight.x - topLeft.x),
          height(bottomRight.y - topLeft.y) {}

    bool operator==(const Rect& r) const
    { return x == r.x && y == r.y
          && width == r.width && height == r.height; }

    Point TopLeft() const
    { return Point(x, y); }

    Point BottomRight() const
    { return Point(x + width, y + height); }

    int Left() const
    { return x; }

    int Right() const
    { return x + width; }

    int Top() const
    { return y; }

    int Bottom() const
    { return y + height; }

    bool IsValid() const
    { return width >= 0 && height >= 0; }

    bool IsEmpty() const
    { return width <= 0 || height <= 0; }

    Rect operator&(const Rect& r) const;
    Rect operator|(const Rect& r) const;

    Rect& operator&=(const Rect& r);
    Rect& operator|=(const Rect& r);

    bool Contains(const Point& p) const
    { return p.x >= x && p.x <= x + width
          && p.y >= y && p.y <= y + height; }

    bool Contains(const Rect& r) const
    { return r.x >= x && r.x + r.width <= x + width
          && r.y >= y && r.y + height <= y + height; }

    bool Intersects(const Rect& r) const
    { return Contains(r.TopLeft()) || Contains(r.BottomRight()); }

    void ResizeBy(int dx, int dy)
    { width += dx; height += dy; }

    void ResizeTo(int width, int height)
    { this->width = width; this->height = height; }

    void ResizeTo(const Size& size)
    { width = size.width; height = size.height; }

    void InflateBy(int dx, int dy)
    { x -= dx; width += 2 * dx; y -= dy; height += 2 * dy; }

    void OffsetBy(int dx, int dy)
    { x += dx; y += dy; }

    void OffsetTo(int x, int y)
    { this->x = x; this->y = y; }

    void OffsetTo(const Point& point)
    { x = point.x; y = point.y; }

    void Set(int x, int y, int width, int height)
    {
        this->x = x;
        this->y = y;
        this->width = width;
        this->height = height;
    }

    void Get(int* x, int* y, int* width, int* height) const
    {
        *x = this->x;
        *y = this->y;
        *width = this->width;
        *height = this->height;
    }
};

//----------------------------------------------------------------------------

#endif
