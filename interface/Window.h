#ifndef interface_Window_h
#define interface_Window_h

#include "interface/Constants.h"
#include "interface/Container.h"

class Control;
class MenuBar;

//----------------------------------------------------------------------------

class Window: public Container
{
public:

    Window(const Rect& frame, String title, window_look look, window_feel feel,
           uint flags);

    Window(const Rect& frame, String title, window_type type, uint flags);

    ~Window();

    bool IsActive() const
    { return GetPlatformWindow()->IsActive(); }

    void Activate(bool flag = true);

    bool IsMaximized() const
    { return GetPlatformWindow()->IsMaximized(); }

    void Maximize()
    { GetPlatformWindow()->Maximize(); }

    bool IsMinimized() const
    { return GetPlatformWindow()->IsMinimized(); }

    void Minimize()
    { GetPlatformWindow()->Minimize(); }

    ::MenuBar* MenuBar() const
    { return m_menuBar; }

    void SetMenuBar(::MenuBar* menu);

    void RedrawMenuBar()
    { GetPlatformWindow()->RedrawMenuBar(); }

    Control* DefaultControl() const
    { return m_defaultControl; }

    void SetDefaultControl(Control* control);

    Control* FocusedControl() const
    { return m_focusedControl; }

    void SetFocusedControl(Control* control);

    void AddChild(View* view);
    bool RemoveChild(View* view);
    void MessageReceived(Message* message);

    virtual void Close();

    virtual bool CloseRequested();
    virtual void WindowActivated(bool flag);

protected:

    Margins GetFrameMargins() const;

private:

    ::MenuBar*  m_menuBar;
    Control*    m_defaultControl;
    Control*    m_focusedControl;
};

//----------------------------------------------------------------------------

#endif
