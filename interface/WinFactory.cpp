#include "interface/WinFactory.h"
#include "interface/Constants.h"
#include "interface/WinBitmap.h"
#include "interface/WinBitmapDC.h"
#include "interface/WinGraphics.h"
#include "interface/WinImage.h"
#include "interface/WinMenu.h"
#include "interface/WinWindow.h"
#include "interface/WinWindowDC.h"
#include "interface/WinWindowPaintDC.h"

//----------------------------------------------------------------------------

PlatformGraphics* WinFactory::CreatePlatformGraphics(PlatformWindow* window)
{
    if (window != nullptr)
        return new WinWindowDC(static_cast<WinWindow*>(window));
    else
        return new WinWindowPaintDC();
}

//----------------------------------------------------------------------------

PlatformGraphics* WinFactory::CreatePlatformGraphics(
    const PlatformGraphics* g, PlatformBitmap* bitmap)
{
    return new WinBitmapDC(static_cast<const WinGraphics*>(g),
                           static_cast<WinBitmap*>(bitmap));
}

//----------------------------------------------------------------------------

PlatformBitmap* WinFactory::CreatePlatformBitmap(
    const PlatformGraphics* g, int width, int height, int bpp)
{
    if (g != nullptr)
        return new WinBitmap(static_cast<const WinGraphics*>(g),
                             width, height, bpp);
    else
        return new WinBitmap();
}

//----------------------------------------------------------------------------

PlatformImage* WinFactory::CreatePlatformImage(
    int width, int height, int bpp)
{
    return new WinImage(width, height, bpp);
}

//----------------------------------------------------------------------------

PlatformWindow* WinFactory::CreatePlatformView(
    PlatformWindow* parent, const Rect& frame, uint alignMode,
    Handler* handler)
{
    DWORD style = WS_CHILD | WS_CLIPCHILDREN | WS_VISIBLE;
    DWORD extStyle = 0;

    return new WinWindow(static_cast<WinWindow*>(parent), String::Empty,
                         frame.x, frame.y, frame.width, frame.height,
                         style, extStyle, handler);
}

//----------------------------------------------------------------------------

PlatformWindow* WinFactory::CreatePlatformWindow(
    const Rect& frame, String title, window_type type, uint flags,
    Handler* handler)
{
    int x, y, w, h;

    frame.Get(&x, &y, &w, &h);

    if (frame.x + frame.width < 0 || frame.y + frame.height < 0)
    {
        x = CW_USEDEFAULT;
        y = CW_USEDEFAULT;
    }

    if (frame.width < 0)
        w = CW_USEDEFAULT;

    if (frame.height < 0)
        h = CW_USEDEFAULT;

    uint style = 0;
    uint extStyle = 0;

    uint look = type & 0xFF00;
    uint feel = type & 0x00FF;

    switch (look)
    {
    case TITLED_WINDOW_LOOK:

        style |= WS_OVERLAPPED | WS_CAPTION | WS_CLIPCHILDREN;
        extStyle |= WS_EX_OVERLAPPEDWINDOW;

        if (!TestBits(flags, WINDOW_NOT_RESIZABLE))
            style |= WS_SIZEBOX;

        if (!TestBits(flags, WINDOW_NOT_MINIMIZABLE))
            style |= WS_MINIMIZEBOX;

        if (!TestBits(flags, WINDOW_NOT_MAXIMIZABLE))
            style |= WS_MAXIMIZEBOX;

        if (!TestBits(flags, WINDOW_NO_WINDOW_MENU))
            style |= WS_SYSMENU;

        break;

    case DOCUMENT_WINDOW_LOOK:

        style |= WS_OVERLAPPED | WS_CAPTION | WS_VSCROLL | WS_HSCROLL | WS_CLIPCHILDREN;
        extStyle |= WS_EX_OVERLAPPEDWINDOW;

        if (!TestBits(flags, WINDOW_NOT_RESIZABLE))
            style |= WS_SIZEBOX;

        if (!TestBits(flags, WINDOW_NOT_MINIMIZABLE))
            style |= WS_MINIMIZEBOX;

        if (!TestBits(flags, WINDOW_NOT_MAXIMIZABLE))
            style |= WS_MAXIMIZEBOX;

        if (!TestBits(flags, WINDOW_NO_WINDOW_MENU))
            style |= WS_SYSMENU;

        break;

    case FLOATING_WINDOW_LOOK:

        style |= WS_POPUP | WS_CAPTION | WS_CLIPCHILDREN;
        extStyle |= WS_EX_TOOLWINDOW;

        if (!TestBits(flags, WINDOW_NOT_RESIZABLE))
            style |= WS_SIZEBOX;

        break;

    case MODAL_WINDOW_LOOK:

        style |= WS_DLGFRAME | WS_CLIPCHILDREN;
        extStyle |= WS_EX_DLGMODALFRAME;

        break;

    case BORDERED_WINDOW_LOOK:

        style |= WS_POPUP | WS_BORDER | WS_CLIPCHILDREN;
        extStyle |= 0;

        break;

    case NO_BORDER_WINDOW_LOOK:

        style |= WS_CLIPCHILDREN;
        extStyle |= 0;

    default:

        style |= WS_POPUP | WS_BORDER | WS_CAPTION | WS_CLIPCHILDREN;
        extStyle |= 0;
    }

    if (feel == MODAL_WINDOW_FEEL)
        extStyle |= WS_EX_CONTROLPARENT;

    else if (feel == FLOATING_WINDOW_FEEL)
        extStyle |= WS_EX_TOPMOST;

    else if (feel == CHILD_WINDOW_FEEL)
        style |= WS_CHILD | WS_CLIPSIBLINGS;

    return new WinWindow(nullptr, title.CStr(), x, y, w, h, style, extStyle,
                         handler);
}

//----------------------------------------------------------------------------

PlatformMenu* WinFactory::CreatePlatformMenu()
{
    return new WinMenu(WinMenu::POPUP_MENU);
}

//----------------------------------------------------------------------------

PlatformMenu* WinFactory::CreatePlatformMenuBar()
{
    return new WinMenu(WinMenu::MENU_BAR);
}

//----------------------------------------------------------------------------
