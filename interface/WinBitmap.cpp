#include "interface/WinBitmap.h"
#include "app/WinApp.h"
#include "interface/WinGraphics.h"
#include "support/Exception.h"

#include <fstream>

//---------------------------------------------------------------------------

WinBitmap::WinBitmap()
    : PlatformBitmap(0, 0, 0),
      m_hBitmap(0)
{}

//---------------------------------------------------------------------------

WinBitmap::WinBitmap(const WinGraphics* g, int width, int height, int bpp)
    : PlatformBitmap(width, height, bpp)
{
    m_hBitmap = ::CreateCompatibleBitmap(g->Handle(), width, height);

    if (!m_hBitmap)
    {
        throw CreationFailed(
            "Fehler in WinBitmap::WinBitmap(const WinGraphics*, int, int, int): "
            "Bitmap konnte nicht erstellt werden");
    }
}

//---------------------------------------------------------------------------

WinBitmap::~WinBitmap()
{
    if (m_hBitmap != 0)
        ::DeleteObject(m_hBitmap);
}

//---------------------------------------------------------------------------

void WinBitmap::SetBits(const PlatformGraphics* g, void* data,
                        int startScan, int scanLines, int bpp)
{
    HDC dc = static_cast<const WinGraphics*>(g)->Handle();

    BITMAPINFO bitmapInfo;
    BITMAPINFOHEADER* infoHeader = &bitmapInfo.bmiHeader;
    RGBQUAD rgbNull = { 0, 0, 0, 0 };

    if (scanLines == 0)
        scanLines = Height() - startScan;

    if (bpp == 0)
        bpp = BitsPerPixel();

    infoHeader->biSize = sizeof(BITMAPINFOHEADER);
    infoHeader->biWidth = Width();
    infoHeader->biHeight = -Height();
    infoHeader->biPlanes = 1;
    infoHeader->biBitCount = bpp;
    infoHeader->biCompression = BI_RGB;
    infoHeader->biSizeImage = Width() * Height() * (bpp >> 3);
    infoHeader->biXPelsPerMeter = 65 * ::GetDeviceCaps(dc, LOGPIXELSX);
    infoHeader->biYPelsPerMeter = 65 * ::GetDeviceCaps(dc, LOGPIXELSY);
    infoHeader->biClrUsed = 0;
    infoHeader->biClrImportant = 0;

    bitmapInfo.bmiColors[0] = rgbNull;

    ::SetDIBits(dc, m_hBitmap, startScan, scanLines, data, &bitmapInfo,
                DIB_RGB_COLORS);
}

//---------------------------------------------------------------------------

void WinBitmap::GetBits(const PlatformGraphics* g, void* data,
                        int startScan, int scanLines, int bpp) const
{
    HDC dc = static_cast<const WinGraphics*>(g)->Handle();

    BITMAPINFO bitmapInfo;
    BITMAPINFOHEADER* infoHeader = &bitmapInfo.bmiHeader;
    RGBQUAD rgbNull = { 0, 0, 0, 0 };

    if (scanLines == 0)
        scanLines = Height() - startScan;

    if (bpp == 0)
        bpp = BitsPerPixel();

    infoHeader->biSize = sizeof(BITMAPINFOHEADER);
    infoHeader->biWidth = Width();
    infoHeader->biHeight = -Height();
    infoHeader->biPlanes = 1;
    infoHeader->biBitCount = bpp;
    infoHeader->biCompression = BI_RGB;
    infoHeader->biSizeImage = Width() * Height() * (bpp >> 3);
    infoHeader->biXPelsPerMeter = 65 * ::GetDeviceCaps(dc, LOGPIXELSX);
    infoHeader->biYPelsPerMeter = 65 * ::GetDeviceCaps(dc, LOGPIXELSY);
    infoHeader->biClrUsed = 0;
    infoHeader->biClrImportant = 0;

    bitmapInfo.bmiColors[0] = rgbNull;

    ::GetDIBits(dc, m_hBitmap, startScan, scanLines, data, &bitmapInfo,
                DIB_RGB_COLORS);
}

//---------------------------------------------------------------------------

void WinBitmap::Load(const PlatformGraphics* g, String filename)
{
    if (m_hBitmap != 0)
        ::DeleteObject(m_hBitmap);

    HDC dc = static_cast<const WinGraphics*>(g)->Handle();

    BITMAPFILEHEADER fileHeader;
    BITMAPINFO bitmapInfo;
    RGBQUAD rgbNull = { 0, 0, 0, 0 };

    std::ifstream file(filename.CStr(), std::ios::in | std::ios::binary);

    file.read((char *) &fileHeader, sizeof(BITMAPFILEHEADER));
    file.read((char *) &bitmapInfo, sizeof(BITMAPINFOHEADER));

    int width = bitmapInfo.bmiHeader.biWidth;
    int height = bitmapInfo.bmiHeader.biHeight;
    int bpp = bitmapInfo.bmiHeader.biBitCount;

    if (height < 0)
        height = -height;

    Init(width, height, bpp);

    m_hBitmap = ::CreateCompatibleBitmap(dc, width, height);

    ulong size = height * BytesPerLine();
    char* bits = new char[size];

    file.seekg(fileHeader.bfOffBits, std::ios::beg);
    file.read(bits, size);

    bitmapInfo.bmiColors[0] = rgbNull;
    ::SetDIBits(dc, m_hBitmap, 0, height, bits, &bitmapInfo, DIB_RGB_COLORS);

    delete[] bits;
}

//---------------------------------------------------------------------------

void WinBitmap::Save(const PlatformGraphics* g, String filename)
{
    HDC dc = static_cast<const WinGraphics*>(g)->Handle();

    BITMAPINFO bitmapInfo;
    BITMAPINFOHEADER* infoHeader = &bitmapInfo.bmiHeader;
    BITMAPFILEHEADER fileHeader;
    RGBQUAD rgbNull = { 0, 0, 0, 0 };

    std::ofstream file(filename.CStr(), std::ios::out | std::ios::binary);

    ulong size = Height() * BytesPerLine();
    char* bits = new char[size];

    fileHeader.bfType = 0x4d42;
    fileHeader.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
    fileHeader.bfSize = (DWORD) fileHeader.bfOffBits + size;

    infoHeader->biSize = sizeof(BITMAPINFOHEADER);
    infoHeader->biWidth = Width();
    infoHeader->biHeight = Height();
    infoHeader->biPlanes = 1;
    infoHeader->biBitCount = BitsPerPixel();
    infoHeader->biCompression = BI_RGB;
    infoHeader->biSizeImage = size;
    infoHeader->biXPelsPerMeter = 65 * ::GetDeviceCaps(dc, LOGPIXELSX);
    infoHeader->biYPelsPerMeter = 65 * ::GetDeviceCaps(dc, LOGPIXELSY);
    infoHeader->biClrUsed = 0;
    infoHeader->biClrImportant = 0;

    bitmapInfo.bmiColors[0] = rgbNull;

    ::GetDIBits(dc, m_hBitmap, 0, Height(), bits, &bitmapInfo, DIB_RGB_COLORS);

    file.write((char *) &fileHeader, sizeof(BITMAPFILEHEADER));
    file.write((char *) infoHeader, sizeof(BITMAPINFOHEADER));
    file.write(bits, size);

    delete[] bits;
}

//---------------------------------------------------------------------------

bool WinBitmap::LoadResource(uint id)
{
    if (m_hBitmap != 0)
        ::DeleteObject(m_hBitmap);

    m_hBitmap = ::LoadBitmapA(TheWinApp->Handle(), MAKEINTRESOURCE(id));

    if (!m_hBitmap)
        return false;

    BITMAP bmp;
    ::GetObject(m_hBitmap, sizeof(bmp), &bmp);
    Init(bmp.bmWidth, bmp.bmHeight, bmp.bmBitsPixel);

    return true;
}

//---------------------------------------------------------------------------
