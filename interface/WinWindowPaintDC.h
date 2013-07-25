#ifndef interface_WinWindowPaintDC_h
#define interface_WinWindowPaintDC_h

#include "interface/WinGraphics.h"

class WinWindow;

//----------------------------------------------------------------------------

class WinWindowPaintDC: public WinGraphics
{
public:

    WinWindowPaintDC()
        : m_isPainting(false),
          m_hWindow(0) {}

    ~WinWindowPaintDC() {}

    HWND WindowHandle() const
    { return m_hWindow; }

    const PAINTSTRUCT* PaintStruct() const
    { return &m_paintstruct; }

    void Begin(PlatformWindow* window);
    void End();

    Rect UpdateRect() const;

private:

    bool        m_isPainting;
    HWND        m_hWindow;
    PAINTSTRUCT m_paintstruct;
};

//----------------------------------------------------------------------------

#endif
