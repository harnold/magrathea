#ifndef interface_PlatformBitmap_h
#define interface_PlatformBitmap_h

#include "support/String.h"

class PlatformGraphics;

//---------------------------------------------------------------------------

class PlatformBitmap
{
public:

    virtual ~PlatformBitmap() {}

    int Width() const
    { return m_width; }

    int Height() const
    { return m_height; }

    int BitsPerPixel() const
    { return m_bpp; }

    int BytesPerLine() const
    { return BytesPerLine(m_width, m_bpp); }

    virtual void SetBits(const PlatformGraphics* g, void* data,
                         int startScan, int scanLines, int bpp) = 0;
    virtual void GetBits(const PlatformGraphics* g, void* data,
                         int startScan, int scanLines, int bpp) const = 0;
    virtual void Load(const PlatformGraphics* g, String filename) = 0;
    virtual void Save(const PlatformGraphics* g, String filename) = 0;

protected:

    PlatformBitmap(int width, int height, int bpp)
    { Init(width, height, bpp); }

    void Init(int width, int height, int bpp);

    static int BytesPerLine(int width, int bpp);

private:

    int m_width;
    int m_height;
    int m_bpp;
};

//---------------------------------------------------------------------------

inline int PlatformBitmap::BytesPerLine(int width, int bpp)
{
    unsigned bpl = ((unsigned) bpp >> 3) * (unsigned) width;
    return (int) ((bpl + 3) & ~3);
}

//---------------------------------------------------------------------------

#endif
