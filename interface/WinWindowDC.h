#ifndef interface_WinWindowDC_h
#define interface_WinWindowDC_h

#include "interface/WinGraphics.h"

class WinWindow;

//----------------------------------------------------------------------------

class WinWindowDC: public WinGraphics
{
public:

    WinWindowDC(WinWindow* window);

    ~WinWindowDC();

    HWND WindowHandle() const
    { return m_hWindow; }

private:

    HWND m_hWindow;
};

//----------------------------------------------------------------------------

#endif
