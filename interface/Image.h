#ifndef interface_Image_h
#define interface_Image_h

#include "interface/PlatformImage.h"
#include "support/Utilities.h"

//---------------------------------------------------------------------------
//
//  Die Image-Klasse dient zur Verwaltung und Darstellung von Bilddaten.
//
//  Im Unterschied zu Bitmaps speichern Image-Objekte ihre Bilddaten in einem
//  geraeteunabhaengigen Format. Auf die gespeicherten Daten kann mit der
//  Bits()-Funktion direkt zugegriffen werden. Im Gegenzug muss vor dem
//  Zeichnen eines Images in ein Graphics-Objekt aus dem Image erst eine
//  (geraeteabhaengige) Bitmap erzeugt werden, entweder explizit oder implizit
//  innerhalb der DrawImage()-Funktion.
//
class Image
{
public:

    //
    //  Erstellt ein Image-Objekt. Die Dimensionen des zu erstellenden Images
    //  werden in width und height uebergeben, die Farbtiefe in bpp (in
    //  Bits/Pixel).
    //
    Image(int width, int height, int bpp);

    //
    //  Zerstoert das Image-Objekt und gibt den dafuer reservierten
    //  Speicher wieder frei.
    //
    virtual ~Image();

    //
    //  Liefert die Breite des Images in Pixeln.
    //
    int Width() const
    { return m_platformImage->Width(); }

    //
    //  Liefert die Hoehe des Images in Pixeln.
    //
    int Height() const
    { return m_platformImage->Height(); }

    //
    //  Liefert die Farbtiefe des Images.
    //
    int BitsPerPixel() const
    { return m_platformImage->BitsPerPixel(); }

    //
    //  Liefert die Anzahl Bytes, die eine Zeile des Images benoetigt.
    //
    int BytesPerLine() const
    { return m_platformImage->BytesPerLine(); }

    //
    //  Bietet direkten Schreib- und Lesezugriff auf die Bilddaten des
    //  Images.
    //
    uchar* Bits()
    { return m_platformImage->Bits(); }

    //
    //  Bietet direkten Lesezugriff auf die Bilddaten des Images.
    //
    const uchar* Bits() const
    { return m_platformImage->Bits(); }

    PlatformImage* GetPlatformImage() const
    { return m_platformImage; }

private:

    PlatformImage* m_platformImage;
};

//---------------------------------------------------------------------------

#endif
