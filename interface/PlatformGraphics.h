#ifndef interface_PlatformGraphics_h
#define interface_PlatformGraphics_h

#include "interface/Color.h"
#include "interface/Constants.h"
#include "interface/Font.h"
#include "interface/Rect.h"
#include "support/String.h"

#include <cstddef>

class PlatformBitmap;
class PlatformImage;
class PlatformWindow;

//----------------------------------------------------------------------------

class PlatformGraphics
{
public:

    virtual ~PlatformGraphics() {}

    virtual void Begin(PlatformWindow* window) = 0;
    virtual void End() = 0;

    virtual Rect UpdateRect() const = 0;
    virtual void SetDrawColor(Color c) = 0;
    virtual void SetFillColor(Color c) = 0;
    virtual void SetFont(const Font& font) = 0;
    virtual void SetCursor(int x, int y) = 0;
    virtual void SetDrawingMode(drawing_mode mode) = 0;
    virtual void DrawLine(int x, int y) = 0;
    virtual void DrawLine(int x0, int y0, int x1, int y1) = 0;
    virtual void DrawRect(int x, int y, int width, int height) = 0;
    virtual void FillRect(int x, int y, int width, int height) = 0;
    virtual void DrawEdge(const Rect& frame, int style, int edges, bool fill,
                          bool softed) = 0;
    virtual void DrawText(const Rect& frame, String text, uint flags) = 0;
    virtual void DrawText(int x, int y, String text, uint length) = 0;
    virtual void GetTextBounds(String text, int* width, int* height) = 0;
    virtual void SetPixel(int x, int y, Color c) = 0;
    virtual Color GetPixel(int x, int y) = 0;
    virtual void DrawBitmap(const PlatformBitmap* bitmap, int x, int y) = 0;
    virtual void DrawBitmap(const PlatformBitmap* bitmap, const Rect& source,
                            const Rect& dest) = 0;
    virtual void DrawImage(const PlatformImage* image, int x, int y) = 0;
    virtual void DrawImage(const PlatformImage* image, const Rect& source,
                           const Rect& dest) = 0;
    virtual void DrawFocusRect(const Rect& frame) = 0;
    virtual void Sync() = 0;

    virtual Color GetSystemColor(system_color color) const = 0;
    virtual Font GetSystemFont() const = 0;
};

//----------------------------------------------------------------------------

#endif
