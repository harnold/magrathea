#include "interface/WinWindowDC.h"
#include "interface/WinWindow.h"
#include "support/Exception.h"

//----------------------------------------------------------------------------

WinWindowDC::WinWindowDC(WinWindow* window)
{
    m_hWindow = window->Handle();
    HDC dc = ::GetDC(m_hWindow);

    if (!dc)
    {
        throw CreationFailed(
            "Fehler in WinWindowDC::WinWindowDC(WinWindow*): "
            "Grafik-Kontext konnte nicht erstellt werden");
    }

    WinGraphics::SetDC(dc);
}

//----------------------------------------------------------------------------

WinWindowDC::~WinWindowDC()
{
    WinGraphics::UnsetDC();
    ::ReleaseDC(m_hWindow, Handle());
}

//----------------------------------------------------------------------------
