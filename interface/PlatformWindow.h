#ifndef interface_PlatformWindow_h
#define interface_PlatformWindow_h

#include "interface/Margins.h"
#include "interface/Rect.h"
#include "support/String.h"
#include "support/Utilities.h"

class PlatformMenu;

//----------------------------------------------------------------------------

class PlatformWindow
{
public:

    virtual ~PlatformWindow() {}

    virtual ulong WinId() const = 0;

    virtual bool IsVisible() const = 0;
    virtual void SetVisible(bool flag) = 0;

    virtual bool IsEnabled() const = 0;
    virtual void SetEnabled(bool flag) = 0;

    virtual bool IsActive() const = 0;
    virtual void Activate(bool flag) = 0;

    virtual bool IsMaximized() const = 0;
    virtual void Maximize() = 0;

    virtual bool IsMinimized() const = 0;
    virtual void Minimize() = 0;

    virtual bool HasFocus() const = 0;
    virtual void SetFocus(bool flag) = 0;

    virtual Rect Frame() const = 0;
    virtual void SetFrame(const Rect& rect) = 0;
    virtual Margins FrameMargins() const = 0;

    virtual String Text() const = 0;
    virtual void SetText(String text) = 0;

    virtual void Raise() = 0;
    virtual void Lower() = 0;

    virtual void Invalidate() = 0;
    virtual void Invalidate(const Rect& rect) = 0;
    virtual void UpdateIfNeeded() = 0;

    virtual void CaptureMouse() = 0;
    virtual void ReleaseMouse() = 0;

    virtual PlatformWindow* Parent() const = 0;
    virtual void SetParent(PlatformWindow* parent) = 0;

    virtual void ConvertToScreen(Point* point) const = 0;
    virtual void ConvertFromScreen(Point* point) const = 0;

    virtual void SetMenuBar(PlatformMenu* menu) = 0;
    virtual void RedrawMenuBar() = 0;

    virtual void Close() = 0;
};

//----------------------------------------------------------------------------

#endif
