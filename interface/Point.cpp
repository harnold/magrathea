#include "interface/Point.h"
#include "interface/Rect.h"

//----------------------------------------------------------------------------

void Point::ConstrainTo(const Rect& r)
{
    if (x > r.Right())
        x = r.Right();
    else if (x < r.x)
        x = r.x;

    if (y > r.Bottom())
        y = r.Bottom();
    else if (y < r.y)
        y = r.y;
}

//----------------------------------------------------------------------------
