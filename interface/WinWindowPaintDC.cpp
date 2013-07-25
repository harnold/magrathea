#include "interface/WinWindowPaintDC.h"
#include "interface/WinWindow.h"
#include "support/Exception.h"

//----------------------------------------------------------------------------

void WinWindowPaintDC::Begin(PlatformWindow* window)
{
    if (m_isPainting)
    {
        throw InvalidOperation(
            "Fehler in WinWindowPaintDC::Begin(PlatformWindow*): "
            "Letzter Aufruf von Begin() nicht mit End() abgeschlossen");
    }

    HWND hWindow = static_cast<WinWindow*>(window)->Handle();
    HDC dc = ::BeginPaint(hWindow, &m_paintstruct);

    if (!dc)
    {
        throw CreationFailed(
            "Fehler in WinWindowPaintDC::Begin(PlatformWindow*): "
            "Grafik-Kontext konnte nicht erstellt werden");
    }

    WinGraphics::SetDC(dc);
    m_isPainting = true;
    m_hWindow = hWindow;
}

//----------------------------------------------------------------------------

void WinWindowPaintDC::End()
{
    if (m_isPainting)
    {
        WinGraphics::UnsetDC();
        ::EndPaint(m_hWindow, &m_paintstruct);
        m_isPainting = false;
        m_hWindow = 0;
    }
}

//----------------------------------------------------------------------------

Rect WinWindowPaintDC::UpdateRect() const
{
    if (m_isPainting)
    {
        return Rect(Point(m_paintstruct.rcPaint.left,
                          m_paintstruct.rcPaint.top),
                    Point(m_paintstruct.rcPaint.right,
                          m_paintstruct.rcPaint.bottom));
    }
    else return Rect();
}

//----------------------------------------------------------------------------
