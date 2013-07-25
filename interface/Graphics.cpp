#include "interface/Graphics.h"
#include "interface/PlatformFactory.h"

//----------------------------------------------------------------------------

Graphics::Graphics(View* view)
    : m_platformGraphics(
        ThePlatformFactory->CreatePlatformGraphics(view->GetPlatformWindow())),
      m_drawColor(COLOR_BLACK),
      m_fillColor(COLOR_WHITE)
{}

//----------------------------------------------------------------------------

Graphics::Graphics(const Graphics& g, Bitmap* bitmap)
    : m_platformGraphics(
        ThePlatformFactory->CreatePlatformGraphics(
            g.GetPlatformGraphics(), bitmap->GetPlatformBitmap())),
      m_drawColor(COLOR_BLACK),
      m_fillColor(COLOR_WHITE)
{}

//----------------------------------------------------------------------------

Graphics::~Graphics()
{
    delete m_platformGraphics;
}

//----------------------------------------------------------------------------
