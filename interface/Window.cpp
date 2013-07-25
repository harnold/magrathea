#include "interface/Window.h"
#include "app/Application.h"
#include "interface/Control.h"
#include "interface/Graphics.h"
#include "interface/MenuBar.h"
#include "interface/PlatformFactory.h"
#include "support/Utilities.h"

//----------------------------------------------------------------------------

Window::Window(const Rect& frame, String title, window_look look,
               window_feel feel, uint flags)
    : m_menuBar(nullptr),
      m_defaultControl(nullptr),
      m_focusedControl(nullptr)
{
    PlatformWindow* platformWindow =
        ThePlatformFactory->CreatePlatformWindow(
            frame, title, static_cast<window_type>(look | feel), flags, this);

    Container::Init(platformWindow, new Graphics(), nullptr,
                    platformWindow->Frame(), ALIGN_NONE);
}

//----------------------------------------------------------------------------

Window::Window(const Rect& frame, String title, window_type type,
               uint flags)
    : m_menuBar(nullptr),
      m_defaultControl(nullptr),
      m_focusedControl(nullptr)
{
    PlatformWindow* platformWindow =
        ThePlatformFactory->CreatePlatformWindow(
            frame, title, type, flags, this);

    Container::Init(platformWindow, new Graphics(), nullptr,
                    platformWindow->Frame(), ALIGN_NONE);
}

//----------------------------------------------------------------------------

Window::~Window() {}

//----------------------------------------------------------------------------

void Window::Activate(bool flag)
{
    GetPlatformWindow()->Activate(flag);
}

//----------------------------------------------------------------------------

void Window::SetMenuBar(::MenuBar* menu)
{
    m_menuBar = menu;

    if (menu != nullptr)
        GetPlatformWindow()->SetMenuBar(menu->GetPlatformMenu());
}

//----------------------------------------------------------------------------

void Window::SetDefaultControl(Control* control)
{
    if (m_defaultControl != nullptr)
        m_defaultControl->SetDefault(false);

    if (control != nullptr && control->Window() == this)
    {
        m_defaultControl = control;
        control->SetDefault(true);
    }
    else
    {
        m_defaultControl = nullptr;
    }
}

//----------------------------------------------------------------------------

void Window::SetFocusedControl(Control* control)
{
    if (m_focusedControl != nullptr)
        m_focusedControl->SetFocus(false);

    if (control != nullptr && control->Window() == this)
    {
        m_defaultControl = control;
        control->SetFocus(false);
    }
    else
    {
        m_focusedControl = nullptr;
    }
}

//----------------------------------------------------------------------------

Margins Window::GetFrameMargins() const
{
    return GetPlatformWindow()->FrameMargins();
}

//----------------------------------------------------------------------------

void Window::AddChild(View* view)
{
    Container::AddChild(view);

    if (view->Parent() == this)
    {
        view->SetWindow(this);
        view->AttachedToWindow();
    }
}

//----------------------------------------------------------------------------

bool Window::RemoveChild(View* view)
{
    view->DetachedFromWindow();

    if (Container::RemoveChild(view))
    {
        view->SetWindow(nullptr);
        return true;
    }
    else return false;
}

//----------------------------------------------------------------------------

void Window::MessageReceived(Message* message)
{
    bool flag;

    switch (message->what)
    {
    case MSG_WINDOW_ACTIVATED:
        message->FindBool("active", &flag);
        WindowActivated(flag);
        break;

    case MSG_WINDOW_CLOSE:
        if (CloseRequested())
            Close();
        break;

    default:
        Container::MessageReceived(message);
    }
}

//----------------------------------------------------------------------------

void Window::Close()
{
    GetPlatformWindow()->Close();

    if (TheApp->MainWindow() == this && TheApp->QuitRequested())
        TheApp->Quit();
}

//----------------------------------------------------------------------------

bool Window::CloseRequested()
{
    return true;
}

//----------------------------------------------------------------------------

void Window::WindowActivated(bool flag)
{
    if (flag && m_focusedControl != nullptr)
        m_focusedControl->SetFocus(true);
}

//----------------------------------------------------------------------------
