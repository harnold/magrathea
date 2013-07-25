#include "interface/Bitmap.h"
#include "interface/Graphics.h"
#include "interface/PlatformFactory.h"
#include "support/Utilities.h"

//---------------------------------------------------------------------------

Bitmap::Bitmap()
    : m_platformBitmap(
        ThePlatformFactory->CreatePlatformBitmap(nullptr, 0, 0, 0)) {}

//---------------------------------------------------------------------------

Bitmap::Bitmap(const Graphics& g, int width, int height, int bpp)
    : m_platformBitmap(
        ThePlatformFactory->CreatePlatformBitmap(
            g.GetPlatformGraphics(), width, height, bpp)) {}

//---------------------------------------------------------------------------

Bitmap::Bitmap(const Graphics& g, const Bitmap& bitmap)
{
    if (this == &bitmap)
        return;

    m_platformBitmap = ThePlatformFactory->CreatePlatformBitmap(
        g.GetPlatformGraphics(), bitmap.Width(), bitmap.Height(),
        bitmap.BitsPerPixel());

    uchar* bits = new uchar[Height() * BytesPerLine()];
    bitmap.GetBits(g, bits, 0, Height(), BitsPerPixel());
    SetBits(g, bits, 0, Height(), BitsPerPixel());
    delete[] bits;
}

//---------------------------------------------------------------------------

Bitmap::Bitmap(const Graphics& g, String filename)
    : m_platformBitmap(
        ThePlatformFactory->CreatePlatformBitmap(nullptr, 0, 0, 0))
{
    Load(g, filename);
}

//---------------------------------------------------------------------------

Bitmap::~Bitmap()
{
    delete m_platformBitmap;
}

//---------------------------------------------------------------------------

void Bitmap::SetBits(const Graphics& g, void* data, int startScan,
                     int scanLines, int bpp)
{
    m_platformBitmap->SetBits(
        g.GetPlatformGraphics(), data, startScan, scanLines, bpp);
}

//---------------------------------------------------------------------------

void Bitmap::GetBits(const Graphics& g, void* data, int startScan,
                     int scanLines, int bpp) const
{
    m_platformBitmap->GetBits(
        g.GetPlatformGraphics(), data, startScan, scanLines, bpp);
}

//---------------------------------------------------------------------------

void Bitmap::Load(const Graphics& g, String filename)
{
    m_platformBitmap->Load(g.GetPlatformGraphics(), filename);
}

//---------------------------------------------------------------------------

void Bitmap::Save(const Graphics& g, String filename)
{
    m_platformBitmap->Save(g.GetPlatformGraphics(), filename);
}

//---------------------------------------------------------------------------
