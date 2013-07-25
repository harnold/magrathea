#include "interface/WinGraphics.h"
#include "interface/Font.h"
#include "interface/WinBitmap.h"
#include "interface/WinImage.h"
#include "interface/WinWindow.h"
#include "support/Exception.h"
#include "support/Utilities.h"

//----------------------------------------------------------------------------

static int drawing_mode_table[] =
{
    R2_BLACK,                           // OP_BLACK
    R2_WHITE,                           // OP_WHITE
    R2_COPYPEN,                         // OP_COPY_SRC
    R2_NOP,                             // OP_COPY_DEST
    R2_NOTCOPYPEN,                      // OP_COPY_NOTSRC
    R2_NOT,                             // OP_COPY_NOTDEST
    R2_MASKNOTPEN,                      // OP_MASK_NOTSRC
    R2_MASKPEN,                         // OP_MASK_SRC
    R2_MASKPENNOT,                      // OP_MASK_NOTDEST
    R2_MERGENOTPEN,                     // OP_MERGE_NOTSRC
    R2_MERGEPEN,                        // OP_MERGE_SRC
    R2_MERGEPENNOT,                     // OP_MERGE_NOTDEST
    R2_XORPEN,                          // OP_XOR
    R2_NOTMASKPEN,                      // OP_NOT_MASK
    R2_NOTMERGEPEN,                     // OP_NOT_MERGE
    R2_NOTXORPEN                        // OP_NOT_XOR
};

//----------------------------------------------------------------------------

static int system_color_table[] =
{
    COLOR_3DDKSHADOW,                   // COL_DARK_SHADOW,
    COLOR_3DFACE,                       // COL_FACE_3D,
    COLOR_BTNFACE,                      // COL_BUTTON_FACE,
    COLOR_3DHIGHLIGHT,                  // COL_HIGHLIGHT_3D,
    COLOR_BTNHIGHLIGHT,                 // COL_BUTTON_HIGHLIGHT,
    COLOR_3DLIGHT,                      // COL_LIGHT_3D,
    COLOR_3DSHADOW,                     // COL_SHADOW_3D,
    COLOR_BTNSHADOW,                    // COL_BUTTON_SHADOW,
    COLOR_ACTIVEBORDER,                 // COL_ACTIVE_BORDER,
    COLOR_ACTIVECAPTION,                // COL_ACTIVE_CAPTION,
    COLOR_APPWORKSPACE,                 // COL_APP_WORKSPACE,
    COLOR_DESKTOP,                      // COL_DESKTOP,
    COLOR_BTNTEXT,                      // COL_BUTTON_TEXT,
    COLOR_CAPTIONTEXT,                  // COL_CAPTION_TEXT,
    COLOR_GRAYTEXT,                     // COL_GRAY_TEXT,
    COLOR_HIGHLIGHT,                    // COL_HIGHLIGHT,
    COLOR_HIGHLIGHTTEXT,                // COL_HIGHLIGHT_TEXT,
    COLOR_INACTIVEBORDER,               // COL_INACTIVE_BORDER,
    COLOR_INACTIVECAPTION,              // COL_INACTIVE_CAPTION,
    COLOR_INACTIVECAPTIONTEXT,          // COL_INACTIVE_CAPTION_TEXT,
    COLOR_INFOBK,                       // COL_INFO_BACKGROUND,
    COLOR_INFOTEXT,                     // COL_INFO_TEXT,
    COLOR_MENU,                         // COL_MENU,
    COLOR_MENUTEXT,                     // COL_MENU_TEXT,
    COLOR_SCROLLBAR,                    // COL_SCROLLBAR,
    COLOR_WINDOW,                       // COL_WINDOW_BACKGROUND,
    COLOR_WINDOWFRAME,                  // COL_WINDOW_FRAME,
    COLOR_WINDOWTEXT                    // COL_WINDOW_TEXT
};

//----------------------------------------------------------------------------

static uint font_family_table[] =
{
    FF_DONTCARE,                        // DEFAULT_FAMILY
    FF_DECORATIVE,                      // DECORATIVE
    FF_MODERN,                          // MODERN
    FF_ROMAN,                           // ROMAN
    FF_SCRIPT,                          // SCRIPT
    FF_SWISS                            // SWISS
};

//----------------------------------------------------------------------------

static Font system_font = Font("Tahoma", 8);

//----------------------------------------------------------------------------

static void SetRect(RECT* r, const Rect& rect)
{
    r->left = rect.Left();
    r->top = rect.Top();
    r->right = rect.Right();
    r->bottom = rect.Bottom();
}

//----------------------------------------------------------------------------

WinGraphics::WinGraphics()
    : m_dc(0),
      m_pen(0),
      m_oldPen(0),
      m_brush(0),
      m_oldBrush(0),
      m_font(0),
      m_oldFont(0)
{}

//----------------------------------------------------------------------------

void WinGraphics::SetDC(HDC dc)
{
    m_dc = dc;

    if (dc != 0)
    {
        m_pen = (HPEN) ::GetCurrentObject(m_dc, OBJ_PEN);
        m_brush = (HBRUSH) ::GetCurrentObject(m_dc, OBJ_BRUSH);
        m_font = (HFONT) ::GetCurrentObject(m_dc, OBJ_FONT);

        m_oldPen = m_pen;
        m_oldBrush = m_brush;
        m_oldFont = m_font;

        ::SetBkMode(m_dc, TRANSPARENT);
        ::SetTextColor(m_dc, RGB(0, 0, 0));
    }
}

//----------------------------------------------------------------------------

void WinGraphics::UnsetDC()
{
    if (m_dc != 0)
    {
        if (m_pen != m_oldPen)
        {
            ::SelectObject(m_dc, m_oldPen);
            ::DeleteObject(m_pen);
        }

        if (m_brush != m_oldBrush)
        {
            ::SelectObject(m_dc, m_oldBrush);
            ::DeleteObject(m_brush);
        }

        if (m_font != m_oldFont)
        {
            ::SelectObject(m_dc, m_oldFont);
            ::DeleteObject(m_font);
        }

        m_dc = 0;
    }
}

//----------------------------------------------------------------------------

void WinGraphics::Begin(PlatformWindow* window)
{
    throw InvalidOperation(
        "Fehler in WinGraphics::Begin(PlatformWindow*): "
        "Funktion wird von dieser WinGraphics-Klasse nicht unterstuetzt");
}

//----------------------------------------------------------------------------

void WinGraphics::End()
{
    throw InvalidOperation(
        "Fehler in WinGraphics::End(): "
        "Funktion wird von dieser WinGraphics-Klasse nicht unterstuetzt");
}

//----------------------------------------------------------------------------

Rect WinGraphics::UpdateRect() const
{
    throw InvalidOperation(
        "Fehler in WinGraphics::UpdateRect() const: "
        "Funktion wird von dieser WinGraphics-Klasse nicht unterstuetzt");
}

//----------------------------------------------------------------------------

void WinGraphics::SetDrawColor(Color c)
{
    m_pen = ::CreatePen(PS_SOLID, 0, (COLORREF) c.Rgb());
    HPEN pen = (HPEN) ::SelectObject(m_dc, m_pen);

    if (pen != 0 && pen != m_oldPen)
        ::DeleteObject(pen);

    ::SetTextColor(m_dc, (COLORREF) c.Rgb());
}

//----------------------------------------------------------------------------

void WinGraphics::SetFillColor(Color c)
{
    m_brush = ::CreateSolidBrush((COLORREF) c.Rgb());
    HBRUSH brush = (HBRUSH) ::SelectObject(m_dc, m_brush);

    if ((brush != 0) && (brush != m_oldBrush))
        ::DeleteObject(brush);
}

//----------------------------------------------------------------------------

void WinGraphics::SetFont(const Font& font)
{
    int height = -MulDiv(font.height, ::GetDeviceCaps(m_dc, LOGPIXELSY), 72);

    uint italic = TestBits(font.flags, Font::ITALIC) ? 1 : 0;
    uint underline = TestBits(font.flags, Font::UNDERLINE) ? 1 : 0;
    uint strikeOut = TestBits(font.flags, Font::STRIKE_OUT) ? 1 : 0;
    uint pitch = TestBits(font.flags, Font::FIXED_WIDTH) ?  FIXED_PITCH : DEFAULT_PITCH;
    uint family = font_family_table[font.family];

    m_font = ::CreateFont(height, font.width, font.escapement, font.orientation,
                         font.weight, italic, underline, strikeOut, ANSI_CHARSET,
                         OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, PROOF_QUALITY,
                         family | pitch, font.name.CStr());

    HFONT xfont = (HFONT) ::SelectObject(m_dc, m_font);

    if (xfont != 0 && xfont != m_oldFont)
        ::DeleteObject(xfont);
}

//----------------------------------------------------------------------------

void WinGraphics::SetCursor(int x, int y)
{
    ::MoveToEx(m_dc, x, y, 0);
}

//----------------------------------------------------------------------------

void WinGraphics::SetDrawingMode(drawing_mode mode)
{
    ::SetROP2(m_dc, drawing_mode_table[mode]);
}

//----------------------------------------------------------------------------

void WinGraphics::DrawLine(int x, int y)
{
    ::LineTo(m_dc, x, y);
}

//----------------------------------------------------------------------------

void WinGraphics::DrawLine(int x0, int y0, int x1, int y1)
{
    ::MoveToEx(m_dc, x0, y0, nullptr);
    ::LineTo(m_dc, x1, y1);
}

//----------------------------------------------------------------------------

void WinGraphics::DrawRect(int x, int y, int width, int height)
{
    int right = x + width;
    int bottom = y + height;

    ::MoveToEx(m_dc, x, y, 0);
    ::LineTo(m_dc, right, y);
    ::LineTo(m_dc, right, bottom);
    ::LineTo(m_dc, x, bottom);
    ::LineTo(m_dc, x, y);
}

//----------------------------------------------------------------------------

void WinGraphics::FillRect(int x, int y, int width, int height)
{
    ::Rectangle(m_dc, x, y, x + width, y + height);
}

//----------------------------------------------------------------------------

void WinGraphics::DrawEdge(const Rect& frame, int style, int edges, bool fill,
                           bool softed)
{
    RECT r;
    UINT middle = fill ? BF_MIDDLE : 0;
    UINT soft = softed ? BF_SOFT : 0;

    SetRect(&r, frame);
    ::DrawEdge(m_dc, &r, style, edges | middle | soft);
}

//----------------------------------------------------------------------------

void WinGraphics::DrawText(const Rect& frame, String text, uint flags)
{
    RECT r;
    UINT format = 0;

    SetRect(&r, frame);

    if (TestBits(flags, SINGLELINE_TEXT))
        format |= DT_SINGLELINE;
    else
        format |= DT_WORDBREAK;

    if (TestBits(flags, ALIGN_RIGHT))
        format |= DT_RIGHT;
    else if (TestBits(flags, ALIGN_H_CENTER))
        format |= DT_CENTER;
    else if (TestBits(flags, ALIGN_LEFT))
        format |= DT_LEFT;

    if (TestBits(flags, ALIGN_TOP))
        format |= DT_TOP;
    else if (TestBits(flags, ALIGN_BOTTOM))
        format |= DT_BOTTOM;
    else if (TestBits(flags, ALIGN_V_CENTER))
        format |= DT_VCENTER;

    ::DrawTextA(m_dc, text.CStr(), -1, &r, format);
}

//----------------------------------------------------------------------------

void WinGraphics::DrawText(int x, int y, String text, uint length)
{
    uint nchars = Min(length, text.Length());
    ::TextOut(m_dc, x, y, text.CStr(), nchars);
}

//----------------------------------------------------------------------------

void WinGraphics::GetTextBounds(String text, int* width, int* height)
{
    SIZE s;

    ::GetTextExtentPoint32(m_dc, text.CStr(), text.Length(), &s);
    *height = s.cy;
    *width = s.cx;
}

//----------------------------------------------------------------------------

void WinGraphics::SetPixel(int x, int y, Color c)
{
    ::SetPixel(m_dc, x, y, (COLORREF) c.Rgb());
}

//----------------------------------------------------------------------------

Color WinGraphics::GetPixel(int x, int y)
{
    return Color(::GetPixel(m_dc, x, y));
}

//----------------------------------------------------------------------------

void WinGraphics::DrawBitmap(HBITMAP hBitmap, int width, int height,
                                int x, int y)
{
    HDC hMemDC = ::CreateCompatibleDC(m_dc);
    HBITMAP oldBitmap = (HBITMAP) ::SelectObject(hMemDC, hBitmap);

    ::BitBlt(m_dc, x, y, width, height, hMemDC, 0, 0, SRCCOPY);

    ::SelectObject(hMemDC, oldBitmap);
    ::DeleteDC(hMemDC);
}

//----------------------------------------------------------------------------

void WinGraphics::DrawBitmap(HBITMAP hBitmap, const Rect& source,
                             const Rect& dest)
{
    HDC hMemDC = ::CreateCompatibleDC(m_dc);
    HBITMAP oldBitmap = (HBITMAP) ::SelectObject(hMemDC, hBitmap);

    if (source.width == dest.width && source.height == dest.height)
    {
        ::BitBlt(m_dc, dest.x, dest.y, dest.width, dest.height,
                 hMemDC, source.x, source.y, SRCCOPY);
    }
    else
    {
        ::StretchBlt(m_dc, dest.x, dest.y, dest.width, dest.height,
                     hMemDC, source.x, source.y, source.width, source.height,
                     SRCCOPY);
    }

    ::SelectObject(hMemDC, oldBitmap);
    ::DeleteDC(hMemDC);
}

//----------------------------------------------------------------------------

void WinGraphics::DrawBitmap(const PlatformBitmap* bitmap, int x, int y)
{
    DrawBitmap(static_cast<const WinBitmap*>(bitmap)->Handle(),
               bitmap->Width(), bitmap->Height(), x, y);
}

//----------------------------------------------------------------------------

void WinGraphics::DrawBitmap(const PlatformBitmap* bitmap, const Rect& source,
                             const Rect& dest)
{
    DrawBitmap(static_cast<const WinBitmap*>(bitmap)->Handle(), source, dest);
}

//----------------------------------------------------------------------------

void WinGraphics::DrawImage(const PlatformImage* image, int x, int y)
{
    DrawBitmap(static_cast<const WinImage*>(image)->Handle(),
               image->Width(), image->Height(), x, y);
}

//----------------------------------------------------------------------------

void WinGraphics::DrawImage(const PlatformImage* image, const Rect& source,
                            const Rect& dest)
{
    DrawBitmap(static_cast<const WinImage*>(image)->Handle(), source, dest);
}

//----------------------------------------------------------------------------

void WinGraphics::DrawFocusRect(const Rect& frame)
{
    RECT r;

    SetRect(&r, frame);
    ::DrawFocusRect(m_dc, &r);
}

//----------------------------------------------------------------------------

void WinGraphics::Sync()
{
    ::GdiFlush();
}

//----------------------------------------------------------------------------

Color WinGraphics::GetSystemColor(system_color color) const
{
    return Color(::GetSysColor(system_color_table[color]));
}

//----------------------------------------------------------------------------

Font WinGraphics::GetSystemFont() const
{
    return system_font;
}

//----------------------------------------------------------------------------
