#ifndef interface_WinImage_h
#define interface_WinImage_h

#include "interface/PlatformImage.h"
#include "platform/Win.h"

//---------------------------------------------------------------------------

class WinImage: public PlatformImage
{
public:

    WinImage(int width, int height, int bpp);

    ~WinImage();

    uchar* Bits()
    { return m_bits; }

    const uchar* Bits() const
    { return m_bits; }

    HBITMAP Handle() const
    { return m_hBitmap; }

private:

    HBITMAP m_hBitmap;
    uchar*  m_bits;
};

//---------------------------------------------------------------------------

#endif
