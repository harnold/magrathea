#include "interface/Rect.h"

//----------------------------------------------------------------------------

Rect& Rect::operator&=(const Rect& r)
{
    x = Max(x, r.x);
    y = Max(y, r.y);
    width = Min(Right(), r.Right()) - x;
    height = Min(Bottom(), r.Bottom()) - y;
}

//----------------------------------------------------------------------------

Rect& Rect::operator|=(const Rect& r)
{
    x = Min(x, r.x);
    y = Min(y, r.y);
    width = Max(Right(), r.Right()) - x;
    height = Max(Bottom(), r.Bottom()) - y;
}

//----------------------------------------------------------------------------

Rect Rect::operator&(const Rect& r) const
{
    return Rect(Point(Max(x, r.x), Max(y, r.y)),
                Point(Min(x + width, r.x + r.width),
                      Min(y + height, r.y + r.height)));
}

//----------------------------------------------------------------------------

Rect Rect::operator|(const Rect& r) const
{
    return Rect(Point(Min(x, r.x), Min(y, r.y)),
                Point(Max(x + width, r.x + r.width),
                      Max(y + height, r.y + r.height)));
}

//----------------------------------------------------------------------------
