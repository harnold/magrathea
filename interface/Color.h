#ifndef interface_Color_h
#define interface_Color_h

#include "support/Utilities.h"

//----------------------------------------------------------------------------
//
//  Repraesentiert einen Farbwert.
//
class Color
{
public:

    //
    //  Erzeugt ein neues Color-Objekt fuer schwarz (COLOR_BLACK).
    //
    Color()
        : m_rgb(0) {}

    //
    //  Erzeugt ein neues Color-Objekt mit dem Farbwert rgb. rgb muss
    //  folgendermassen codiert sein: 0x00bbggrr
    //
    explicit Color(uint rgb)
        : m_rgb(rgb) {}

    //
    //  Erzeugt einen neuen Farbwert aus den RGB-Anteilen.
    //
    Color(uchar r, uchar g, uchar b)
        : m_rgb(((uint) r) | ((uint) g << 8) | ((uint) b << 16)) {}

    //
    //  Liefert die einzelnen RGB-Anteile zurueck.
    //
    void Get(uchar* r, uchar* g, uchar* b)
    { *r = Red(); *g = Green(); *b = Blue(); }

    //
    //  Setzt den Farbwert entsprechend der uebergebenen RGB-Anteile.
    //
    void Set(uchar r, uchar g, uchar b)
    { m_rgb = ((uint) r) | ((uint) g << 8) | ((uint) b << 16); }

    //
    //  Liefert den Farbwert des Color-Objektes in der Form 0x00bbggrr.
    //
    uint Rgb() const
    { return m_rgb; }

    //
    //  Setzt den Farbwert des Color-Objektes. rgb muss in der Form
    //  0x00bbggrr codiert sein.
    //
    void SetRgb(uint rgb)
    { m_rgb = rgb; }

    //
    //  Liefert den Rotanteil des RGB-Wertes.
    //
    uchar Red() const
    { return (uchar) (m_rgb & 0x000000FF); }

    //
    //  Liefert den Gruenanteil des RGB-Wertes.
    //
    uchar Green() const
    { return (uchar) ((m_rgb & 0x0000FF00) >> 8); }

    //
    //  Liefert den Blauanteil des RGB-Wertes.
    //
    uchar Blue() const
    { return (uchar) ((m_rgb & 0x00FF0000) >> 16); }

    //
    //  Setzt den Rotanteil des RGB-Wertes.
    //
    void SetRed(uchar red)
    { m_rgb = (m_rgb & 0x00FFFF00) | red; }

    //
    //  Setzt den Gruenanteil des RGB-Wertes.
    //
    void SetGreen(uchar green)
    { m_rgb = (m_rgb & 0x00FF00FF) | ((uint) green << 8); }

    //
    //  Setzt den Blauanteil des RGB-Wertes.
    //
    void SetBlue(uchar blue)
    { m_rgb = (m_rgb & 0x0000FFFF) | ((uint) blue << 16); }

    //
    //  Testet zwei Farbwerte auf Gleichheit.
    //
    bool operator==(const Color& c) const
    { return m_rgb == c.m_rgb; }

private:

    uint m_rgb;
};

//----------------------------------------------------------------------------

#endif
