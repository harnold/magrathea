#ifndef interface_WinView_h
#define interface_WinView_h

#include "interface/PlatformWindow.h"
#include "interface/Point.h"
#include "platform/Win.h"
#include "support/String.h"

class Handler;

//----------------------------------------------------------------------------

class WinWindow: public PlatformWindow
{
public:

    WinWindow(WinWindow* parent, String text, int x, int y, int width, int height,
              DWORD style, DWORD extStyle, Handler* handler);

    ~WinWindow();

    HWND Handle() const
    { return m_hWindow; }

    ulong WinId() const
    { return reinterpret_cast<ulong>(m_hWindow); }

    bool IsVisible() const;
    void SetVisible(bool flag);

    bool IsEnabled() const;
    void SetEnabled(bool flag);

    bool IsActive() const;
    void Activate(bool flag);

    bool IsMaximized() const;
    void Maximize();

    bool IsMinimized() const;
    void Minimize();

    bool HasFocus() const;
    void SetFocus(bool flag);

    Rect Frame() const;
    void SetFrame(const Rect& rect);
    Margins FrameMargins() const;

    String Text() const;
    void SetText(String text);

    void Raise();
    void Lower();

    void Invalidate();
    void Invalidate(const Rect& rect);
    void UpdateIfNeeded();

    void CaptureMouse();
    void ReleaseMouse();

    PlatformWindow* Parent() const;
    void SetParent(PlatformWindow* parent);

    void ConvertToScreen(Point* point) const;
    void ConvertFromScreen(Point* point) const;

    void SetMenuBar(PlatformMenu* menu);
    void RedrawMenuBar();

    void Close();

private:

    WinWindow*  m_parent;
    HWND        m_hWindow;
};

//----------------------------------------------------------------------------

#endif
