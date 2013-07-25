#ifndef interface_WinGraphics_h
#define interface_WinGraphics_h

#include "interface/PlatformGraphics.h"
#include "platform/Win.h"

class WinWindow;

//----------------------------------------------------------------------------

class WinGraphics: public PlatformGraphics
{
public:

    ~WinGraphics() {}

    HDC Handle() const
    { return m_dc; }

    HPEN PenHandle() const
    { return m_pen; }

    HBRUSH BrushHandle() const
    { return m_brush; }

    HFONT FontHandle() const
    { return m_font; }

    void Begin(PlatformWindow* window);
    void End();

    Rect UpdateRect() const;
    void SetDrawColor(Color c);
    void SetFillColor(Color c);
    void SetFont(const Font& font);
    void SetCursor(int x, int y);
    void SetDrawingMode(drawing_mode mode);
    void DrawLine(int x, int y);
    void DrawLine(int x0, int y0, int x1, int y1);
    void DrawRect(int x, int y, int width, int height);
    void FillRect(int x, int y, int width, int height);
    void DrawEdge(const Rect& frame, int style, int edges, bool fill, bool softed);
    void DrawText(const Rect& frame, String text, uint flags);
    void DrawText(int x, int y, String text, uint length);
    void GetTextBounds(String text, int* width, int* height);
    void SetPixel(int x, int y, Color c);
    Color GetPixel(int x, int y);
    void DrawBitmap(const PlatformBitmap* bitmap, int x, int y);
    void DrawBitmap(const PlatformBitmap* bitmap,
                    const Rect& source, const Rect& dest);
    void DrawImage(const PlatformImage* image, int x, int y);
    void DrawImage(const PlatformImage* image, const Rect& source,
                   const Rect& dest);
    void DrawFocusRect(const Rect& frame);
    void Sync();

    Color GetSystemColor(system_color color) const;
    Font GetSystemFont() const;

protected:

    WinGraphics();

    //
    //  SetDC() muss aufgerufen werden, bevor das Graphics-Objekt zum Zeichnen
    //  verwendet werden kann.
    //
    void SetDC(HDC dc);

    //
    //  Nach einem Aufruf von SetDC() muss UnsetDC() aufgerufen werden,
    //  nachdem die Zeichenoperationen beendet sind, auf jeden Fall vor dem
    //  Aufruf des Destruktors von Graphics.
    //
    void UnsetDC();

private:

    void DrawBitmap(HBITMAP hBitmap, int width, int height, int x, int y);
    void DrawBitmap(HBITMAP hBitmap, const Rect& source, const Rect& dest);

    HDC     m_dc;
    HPEN    m_pen;
    HPEN    m_oldPen;
    HBRUSH  m_brush;
    HBRUSH  m_oldBrush;
    HFONT   m_font;
    HFONT   m_oldFont;
};

//----------------------------------------------------------------------------

#endif
