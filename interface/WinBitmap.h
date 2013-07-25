#ifndef interface_WinBitmap_h
#define interface_WinBitmap_h

#include "interface/PlatformBitmap.h"
#include "platform/Win.h"

class WinGraphics;

//---------------------------------------------------------------------------

class WinBitmap: public PlatformBitmap
{
public:

    WinBitmap();
    WinBitmap(const WinGraphics* g, int width, int height, int bpp);

    ~WinBitmap();

    HBITMAP Handle() const
    { return m_hBitmap; }

    bool LoadResource(uint id);

    void SetBits(const PlatformGraphics* g, void* data,
                 int startScan, int scanLines, int bpp);
    void GetBits(const PlatformGraphics* g, void* data,
                 int startScan, int scanLines, int bpp) const;
    void Load(const PlatformGraphics* g, String filename);
    void Save(const PlatformGraphics* g, String filename);

private:

    HBITMAP m_hBitmap;
};

//---------------------------------------------------------------------------

#endif
