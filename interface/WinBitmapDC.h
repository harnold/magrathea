#ifndef interface_WinBitmapDC_h
#define interface_WinBitmapDC_h

#include "interface/WinGraphics.h"

class WinBitmap;

//----------------------------------------------------------------------------

class WinBitmapDC: public WinGraphics
{
public:

    WinBitmapDC(const WinGraphics* g, WinBitmap* bitmap);

    ~WinBitmapDC();

    HBITMAP BitmapHandle() const
    { return m_bitmap; }

private:

    HBITMAP m_bitmap;
    HBITMAP m_oldBitmap;
};

//----------------------------------------------------------------------------

#endif
