#include "interface/WinImage.h"
#include "support/Exception.h"

//---------------------------------------------------------------------------

WinImage::WinImage(int width, int height, int bpp)
    : PlatformImage(width, height, bpp)
{
    BITMAPINFO bmpInfo;
    BITMAPINFOHEADER* bmpHeader = &bmpInfo.bmiHeader;
    RGBQUAD rgbNull = { 0, 0, 0, 0 };

    bmpHeader->biSize = sizeof(BITMAPINFOHEADER);
    bmpHeader->biWidth = width;
    bmpHeader->biHeight = -height;
    bmpHeader->biPlanes = 1;
    bmpHeader->biBitCount = bpp;
    bmpHeader->biCompression = BI_RGB;
    bmpHeader->biSizeImage = 0;
    bmpHeader->biXPelsPerMeter = 0;
    bmpHeader->biYPelsPerMeter = 0;
    bmpHeader->biClrUsed = 0;
    bmpHeader->biClrImportant = 0;

    bmpInfo.bmiColors[0] = rgbNull;

    m_hBitmap = ::CreateDIBSection((HDC) 0, &bmpInfo, DIB_RGB_COLORS,
                                   (void**) &m_bits, (HANDLE) 0, 0);

    if (!m_hBitmap)
    {
        throw CreationFailed(
            "Fehler in WinImage::WinImage(int, int, int): "
            "Bitmap konnte nicht erstellt werden");
    }
}

//---------------------------------------------------------------------------

WinImage::~WinImage()
{
    if (m_hBitmap != 0)
        ::DeleteObject(m_hBitmap);
}

//---------------------------------------------------------------------------
