#include "interface/Image.h"
#include "interface/PlatformFactory.h"

//---------------------------------------------------------------------------

Image::Image(int width, int height, int bpp)
    : m_platformImage(
        ThePlatformFactory->CreatePlatformImage(width, height, bpp))
{}

//---------------------------------------------------------------------------

Image::~Image()
{
    delete m_platformImage;
}

//---------------------------------------------------------------------------
