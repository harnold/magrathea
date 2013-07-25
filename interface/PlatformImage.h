#ifndef interface_PlatformImage_h
#define interface_PlatformImage_h

#include "support/String.h"
#include "support/Utilities.h"

//---------------------------------------------------------------------------

class PlatformImage
{
public:

    virtual ~PlatformImage() {}

    int Width() const
    { return m_width; }

    int Height() const
    { return m_height; }

    int BitsPerPixel() const
    { return m_bpp; }

    int BytesPerLine() const
    { return BytesPerLine(m_width, m_bpp); }

    virtual uchar* Bits() = 0;
    virtual const uchar* Bits() const = 0;

protected:

    PlatformImage(int width, int height, int bpp)
    { Init(width, height, bpp); }

    void Init(int width, int height, int bpp);

    static int BytesPerLine(int width, int bpp);

private:

    int m_width;
    int m_height;
    int m_bpp;
};

//---------------------------------------------------------------------------

inline int PlatformImage::BytesPerLine(int width, int bpp)
{
    unsigned bpl = ((unsigned) bpp >> 3) * (unsigned) width;
    return (int) ((bpl + 3) & ~3);
}

//---------------------------------------------------------------------------

#endif
