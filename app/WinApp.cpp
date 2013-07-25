#include "app/WinApp.h"
#include "interface/Window.h"
#include "interface/WinFactory.h"
#include "interface/WinWindow.h"
#include "support/Exception.h"
#include "support/Utilities.h"

//----------------------------------------------------------------------------

WinApp* TheWinApp = nullptr;

//----------------------------------------------------------------------------

const char* WinApp::DefaultWindowClassName = "DefaultWindowClass";

//----------------------------------------------------------------------------

WinApp::WinApp(HINSTANCE instance, LPSTR args, int cmdShow)
    : m_quit(false),
      m_hInstance(instance),
      m_cmdLine(args),
      m_cmdShow(cmdShow)
{
    if (TheWinApp != nullptr)
    {
        throw CreationFailed(
            "Fehler in WinApp::WinApp(HINSTANCE, LPSTR, int): "
            "Es darf nur ein WinApp-Objekt geben");
    }

    if (!RegisterWindowClass())
    {
        throw CreationFailed(
            "Fehler in WinApp::WinApp(HINSTANCE, LPSTR, int): "
            "Registrierung der Fensterklasse fehlgeschlagen");
    }

    ::InitCommonControls();

    TheWinApp = this;
    ThePlatformFactory = new WinFactory();
}

//----------------------------------------------------------------------------

WinApp::~WinApp()
{
    if (TheWinApp == this)
        TheWinApp = nullptr;

    delete ThePlatformFactory;
    ThePlatformFactory = nullptr;
}

//----------------------------------------------------------------------------

void WinApp::Exec(String name)
{
    HWND hWindow = static_cast<WinWindow*>(
        MainWindow()->GetPlatformWindow())->Handle();

    ::ShellExecute(hWindow, "open", name.CStr(), 0, 0, SW_SHOWNORMAL);
}

//----------------------------------------------------------------------------

void WinApp::Run()
{
    while (!m_quit)
    {
        MSG msg;

        while (::PeekMessage(&msg, (HWND) 0, 0, 0, PM_REMOVE))
        {
            ::TranslateMessage(&msg);
            ::DispatchMessageA(&msg);
        }

        DispatchNextMessage();
    }
}

//----------------------------------------------------------------------------

void WinApp::Quit()
{
    m_quit = true;
}

//----------------------------------------------------------------------------

::ResourceLoader* WinApp::ResourceLoader()
{
    return &m_resourceLoader;
}

//----------------------------------------------------------------------------

bool WinApp::RegisterWindowClass()
{
    WNDCLASS wc;

    wc.style = CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = WinApp::WindowProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = m_hInstance;
    wc.hIcon = 0;
    wc.hCursor = ::LoadCursor(0, IDC_ARROW);
    wc.hbrBackground = 0;
    wc.lpszMenuName = 0;
    wc.lpszClassName = WinApp::DefaultWindowClassName;

    return ::RegisterClass(&wc) != 0;
}

//----------------------------------------------------------------------------

void WinApp::CreateMouseMessage(UINT msg, WPARAM wParam, LPARAM lParam,
                                Message* message)
{
    uint fwKeys = wParam;
    uint keyState = NO_KEY_DOWN;

    switch (msg)
    {
    case WM_LBUTTONDOWN:
        message->what = MSG_MOUSE_DOWN;
        message->AddInt("button", LEFT_BUTTON);
        break;

    case WM_RBUTTONDOWN:
        message->what = MSG_MOUSE_DOWN;
        message->AddInt("button", RIGHT_BUTTON);
        break;

    case WM_LBUTTONUP:
        message->what = MSG_MOUSE_UP;
        message->AddInt("button", LEFT_BUTTON);
        break;

    case WM_RBUTTONUP:
        message->what = MSG_MOUSE_UP;
        message->AddInt("button", RIGHT_BUTTON);
        break;

    case WM_MOUSEMOVE:
        message->what = MSG_MOUSE_MOVED;
        break;

    case WM_LBUTTONDBLCLK:
        message->what = MSG_MOUSE_DOUBLECLICK;
        message->AddInt("button", LEFT_BUTTON);
        break;

    case WM_RBUTTONDBLCLK:
        message->what = MSG_MOUSE_DOUBLECLICK;
        message->AddInt("button", RIGHT_BUTTON);
        break;

    default:
        message->what = MSG_UNKNOWN;
    }

    if ((fwKeys & MK_CONTROL) != 0)
        keyState = (keyState | CONTROL_DOWN);

    if ((fwKeys & MK_LBUTTON) != 0)
        keyState = (keyState | LEFT_BUTTON_DOWN);

    if ((fwKeys & MK_RBUTTON) != 0)
        keyState = (keyState | RIGHT_BUTTON_DOWN);

    if ((fwKeys & MK_SHIFT) != 0)
        keyState = (keyState | SHIFT_DOWN);

    message->AddInt("keys", keyState);
    message->AddInt("xpos", LOWORD(lParam));
    message->AddInt("ypos", HIWORD(lParam));
}

//----------------------------------------------------------------------------

LRESULT CALLBACK WinApp::WindowProc(HWND hWindow, UINT msg,
                                    WPARAM wParam, LPARAM lParam)
{
    static Message message(MSG_UNKNOWN);

    message.Clear();

    switch (msg)
    {
    case WM_LBUTTONDOWN:
    case WM_RBUTTONDOWN:
    case WM_LBUTTONUP:
    case WM_RBUTTONUP:
    case WM_MOUSEMOVE:
    case WM_LBUTTONDBLCLK:
    case WM_RBUTTONDBLCLK:
        CreateMouseMessage(msg, wParam, lParam, &message);
        break;

    case WM_KEYDOWN:
        message.what = MSG_KEY_DOWN;
        message.AddInt("key", (int) wParam);
        break;

    case WM_KEYUP:
        message.what = MSG_KEY_UP;
        message.AddInt("key", (int) wParam);
        break;

    case WM_CHAR:
        message.what = MSG_KEY_PRESSED;
        message.AddInt("char", (int) wParam);
        break;

    case WM_TIMER:
        message.what = MSG_PULSE;
        message.AddInt("timer", wParam);
        break;

    case WM_PAINT:
        message.what = MSG_VIEW_DRAW;
        break;

    case WM_COMMAND:
        message.what = MSG_COMMAND;
        message.AddInt("cmd", LOWORD(wParam));
        break;

    case WM_SETFOCUS:
        message.what = MSG_VIEW_FOCUS_CHANGED;
        message.AddBool("focused", true);
        break;

    case WM_KILLFOCUS:
        message.what = MSG_VIEW_FOCUS_CHANGED;
        message.AddBool("focused", false);
        break;

    case WM_SIZE:
        message.what = MSG_VIEW_RESIZED;
        message.AddInt("width",  LOWORD(lParam));
        message.AddInt("height", HIWORD(lParam));
        break;

    case WM_MOVE:
        message.what = MSG_VIEW_MOVED;
        message.AddInt("xpos", (int) LOWORD(lParam));
        message.AddInt("ypos", (int) HIWORD(lParam));
        break;

    case WM_CLOSE:
        message.what = MSG_WINDOW_CLOSE;
        break;

    case WM_ACTIVATE:
        message.what = MSG_WINDOW_ACTIVATED;
        message.AddBool("active", LOWORD(wParam) != WA_INACTIVE);
        break;

    case WM_SHOWWINDOW:
        message.what = MSG_VIEW_VISIBILITY_CHANGED;
        message.AddBool("show", (BOOL) wParam == TRUE);
        break;

    case WM_ENABLE:
        message.what = MSG_VIEW_ENABLED;
        message.AddBool("enabled", (BOOL) wParam == TRUE);
        break;

    default:
        return ::DefWindowProc(hWindow, msg, wParam, lParam);
    }

    TheWinApp->PostMessage(&message, TheWinApp->GetMessageHandler(hWindow));

    return 0;
}

//----------------------------------------------------------------------------
