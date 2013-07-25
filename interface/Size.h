#ifndef interface_Size_h
#define interface_Size_h

#include "support/Utilities.h"

//----------------------------------------------------------------------------

struct Size
{
    int width;
    int height;

    Size() {}

    Size(int width, int height)
        : width(width), height(height) {}

    bool operator==(const Size& s) const
    { return width == s.width && height == s.height; }

    bool IsValid() const
    { return width >= 0 && height >= 0; }

    bool IsEmpty() const
    { return width <= 0 || height <= 0; }

    Size operator+(const Size& s) const
    { return Size(width + s.width, height + s.height); }

    Size operator-(const Size& s) const
    { return Size(width - s.width, height - s.height); }

    Size& operator+=(const Size& s)
    { width += s.width; height += s.height; return *this; }

    Size& operator-=(const Size& s)
    { width -= s.width; height -= s.height; return *this; }

    void Set(int width, int height)
    { this->width = width; this->height = height; }
};

//----------------------------------------------------------------------------

#endif
