#include "interface/WinWindow.h"
#include "interface/WinMenu.h"
#include "app/WinApp.h"
#include "support/Exception.h"
#include "support/Utilities.h"

//----------------------------------------------------------------------------

WinWindow::WinWindow(WinWindow* parent, String text, int x, int y,
                     int width, int height, DWORD style, DWORD extStyle,
                     Handler* handler)
    : m_parent(parent)
{
    HWND hParent = parent ? parent->Handle() : 0;

    m_hWindow = CreateWindowEx(
        extStyle, WinApp::DefaultWindowClassName, text.CStr(), style,
        x, y, width, height, hParent, 0, TheWinApp->Handle(), nullptr);

    if (!m_hWindow)
    {
        throw CreationFailed(
            "Fehler in WinWindow::WinWindow(...): "
            "Fenster konnte nicht erstellt werden");
    }

    TheWinApp->RegisterMessageHandler(m_hWindow, handler);
}

//----------------------------------------------------------------------------

WinWindow::~WinWindow()
{
    TheWinApp->RemoveMessageHandler(m_hWindow);
    ::DestroyWindow(m_hWindow);
}

//----------------------------------------------------------------------------

bool WinWindow::IsVisible() const
{
    return ::IsWindowVisible(m_hWindow) == TRUE;
}

//----------------------------------------------------------------------------

void WinWindow::SetVisible(bool flag)
{
    int showCmd = flag ? SW_SHOW : SW_HIDE;
    ::ShowWindow(m_hWindow, showCmd);
}

//----------------------------------------------------------------------------

bool WinWindow::IsEnabled() const
{
    return ::IsWindowEnabled(m_hWindow) == TRUE;
}

//----------------------------------------------------------------------------

void WinWindow::SetEnabled(bool flag)
{
    BOOL enable = flag ? TRUE : FALSE;
    ::EnableWindow(m_hWindow, enable);
}

//----------------------------------------------------------------------------

bool WinWindow::IsActive() const
{
    return ::GetActiveWindow() == m_hWindow;
}

//----------------------------------------------------------------------------

void WinWindow::Activate(bool flag)
{
    HWND hwnd = flag ? m_hWindow : ::GetWindow(m_hWindow, GW_HWNDNEXT);
    ::SetActiveWindow(hwnd);
}

//----------------------------------------------------------------------------

bool WinWindow::IsMaximized() const
{
    return ::IsZoomed(m_hWindow) == TRUE;
}

//----------------------------------------------------------------------------

void WinWindow::Maximize()
{
    ::ShowWindow(m_hWindow, SW_MAXIMIZE);
}

//----------------------------------------------------------------------------

bool WinWindow::IsMinimized() const
{
    return ::IsIconic(m_hWindow) == TRUE;
}

//----------------------------------------------------------------------------

void WinWindow::Minimize()
{
    ::ShowWindow(m_hWindow, SW_MINIMIZE);
}

//----------------------------------------------------------------------------

bool WinWindow::HasFocus() const
{
    return ::GetFocus() == m_hWindow;
}

//----------------------------------------------------------------------------

void WinWindow::SetFocus(bool flag)
{
    HWND hwnd = flag ? m_hWindow : ::GetWindow(m_hWindow, GW_HWNDNEXT);
    ::SetFocus(hwnd);
}

//----------------------------------------------------------------------------

Rect WinWindow::Frame() const
{
    RECT r;

    ::GetWindowRect(m_hWindow, &r);

    Point p1(r.left, r.top);
    Point p2(r.right, r.bottom);

    if (m_parent != nullptr)
    {
        m_parent->ConvertFromScreen(&p1);
        m_parent->ConvertFromScreen(&p2);
    }

    return Rect(p1, p2);
}

//----------------------------------------------------------------------------

void WinWindow::SetFrame(const Rect& frame)
{
    ::MoveWindow(m_hWindow, frame.x, frame.y, frame.width, frame.height, TRUE);
}

//----------------------------------------------------------------------------

Margins WinWindow::FrameMargins() const
{
    POINT p = { 0, 0 };
    RECT wr, cr;

    ::ClientToScreen(m_hWindow, &p);
    ::GetWindowRect(m_hWindow, &wr);
    ::GetClientRect(m_hWindow, &cr);

    int left = p.x - wr.left;
    int top = p.x - wr.top;
    int right = left + cr.right;
    int bottom = top + cr.bottom;

    return Margins(left, top, right, bottom);
}

//----------------------------------------------------------------------------

String WinWindow::Text() const
{
    size_t capacity = ::GetWindowTextLength(m_hWindow);
    char* text = new char[capacity];

    ::GetWindowText(m_hWindow, text, capacity);
    return String::FromBuffer(text, capacity);
}

//----------------------------------------------------------------------------

void WinWindow::SetText(String text)
{
    ::SetWindowText(m_hWindow, text.CStr());
}

//----------------------------------------------------------------------------

void WinWindow::Raise()
{
    ::SetWindowPos(m_hWindow, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
}

//----------------------------------------------------------------------------

void WinWindow::Lower()
{
    ::SetWindowPos(m_hWindow, HWND_BOTTOM, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
}

//----------------------------------------------------------------------------

void WinWindow::Invalidate()
{
    ::InvalidateRect(m_hWindow, nullptr, FALSE);
}

//----------------------------------------------------------------------------

void WinWindow::Invalidate(const Rect& rect)
{
    RECT r;

    r.left = rect.x;
    r.top = rect.y;
    r.right = rect.Right();
    r.bottom = rect.Bottom();

    ::InvalidateRect(m_hWindow, &r, FALSE);
}

//----------------------------------------------------------------------------

void WinWindow::UpdateIfNeeded()
{
    ::UpdateWindow(m_hWindow);
}

//----------------------------------------------------------------------------

void WinWindow::CaptureMouse()
{
    ::SetCapture(m_hWindow);
}

//----------------------------------------------------------------------------

void WinWindow::ReleaseMouse()
{
    ::ReleaseCapture();
}

//----------------------------------------------------------------------------

PlatformWindow* WinWindow::Parent() const
{
    return m_parent;
}

//----------------------------------------------------------------------------

void WinWindow::SetParent(PlatformWindow* parent)
{
    m_parent = static_cast<WinWindow*>(parent);

    HWND hwnd = parent ? m_parent->Handle() : 0;
    ::SetParent(m_hWindow, hwnd);
}

//----------------------------------------------------------------------------

void WinWindow::ConvertToScreen(Point* point) const
{

    POINT p = { point->x, point->y };

    ::ClientToScreen(m_hWindow, &p);
    point->Set(p.x, p.y);
}

//----------------------------------------------------------------------------

void WinWindow::ConvertFromScreen(Point* point) const
{
    POINT p = { point->x, point->y };

    ::ScreenToClient(m_hWindow, &p);
    point->Set(p.x, p.y);
}

//----------------------------------------------------------------------------

void WinWindow::SetMenuBar(PlatformMenu* menu)
{
    HMENU hMenu = menu ? static_cast<WinMenu*>(menu)->Handle() : 0;

    ::SetMenu(m_hWindow, hMenu);
    ::DrawMenuBar(m_hWindow);
}

//----------------------------------------------------------------------------

void WinWindow::RedrawMenuBar()
{
    ::DrawMenuBar(m_hWindow);
}

//----------------------------------------------------------------------------

void WinWindow::Close()
{
    ::CloseWindow(m_hWindow);
}

//----------------------------------------------------------------------------
