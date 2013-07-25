#include "interface/WinBitmapDC.h"
#include "interface/WinBitmap.h"
#include "support/Exception.h"
#include "support/Utilities.h"

//----------------------------------------------------------------------------

WinBitmapDC::WinBitmapDC(const WinGraphics* g, WinBitmap* bitmap)
{
    HDC dc = ::CreateCompatibleDC(g->Handle());

    if (!dc)
    {
        throw CreationFailed(
            "Fehler in WinBitmapDC::WinBitmapDC(WinGraphics&, WinBitmap*): "
            "Grafik-Kontext konnte nicht erstellt werden");
    }

    WinGraphics::SetDC(dc);
    m_oldBitmap = (HBITMAP) ::GetCurrentObject(dc, OBJ_BITMAP);
    m_bitmap = bitmap->Handle();
    ::SelectObject(dc, m_bitmap);
}

//----------------------------------------------------------------------------

WinBitmapDC::~WinBitmapDC()
{
    ::SelectObject(Handle(), m_oldBitmap);
    WinGraphics::UnsetDC();
    ::DeleteDC(Handle());
}

//----------------------------------------------------------------------------
