#ifndef interface_Bitmap_h
#define interface_Bitmap_h

#include "interface/PlatformBitmap.h"
#include "support/String.h"

class Graphics;

//---------------------------------------------------------------------------
//
//  Die Bitmap-Klasse dient zur Verwaltung und Darstellung von Bilddaten.
//
//  Bitmap-Objekte koennen eine beliebige Farbtiefe besitzen. Intern
//  werden sie aber immer so angelegt, dass sie optimal zum Pixelformat
//  des Bildschirms passen, um bei der Darstellung eine moeglichst gute
//  Performance zu erzielen.
//
class Bitmap
{
public:

    //
    //  Erstellt ein leeres Bitmap-Objekt.
    //
    Bitmap();

    //
    //  Erstellt ein neues Bitmap-Objekt. Die Dimensionen der zu
    //  erstellenden Bitmap werden in width und height uebergeben, die
    //  Farbtiefe in bpp (in Bits/Pixel). g muss ein gueltiges Graphics-
    //  Objekt sein und kann nach dem Aufruf wieder geloescht werden.
    //
    Bitmap(const Graphics& g, int width, int height, int bpp);

    //
    //  Erstellt eine Kopie eines Bitmap-Objektes. Dabei werden die Bitmap-
    //  Daten kopiert (deep copy). g muss ein gueltiges Graphics-Objekt
    //  sein und  kann nach dem Aufruf wieder geloescht werden.
    //
    Bitmap(const Graphics& g, const Bitmap& bitmap);

    //
    //  Erstellt ein Bitmap-Objekt aus einer bmp-Datei. Es koennen nur
    //  Bitmaps im RGB-Format (ohne Palette) erstellt werden. g muss ein
    //  gueltiges Graphics-Objekt sein und kann nach dem Aufruf wieder
    //  geloescht werden.
    //
    Bitmap(const Graphics& g, String filename);

    //
    //  Erstellt ein Bitmap-Objekt aus einem PlatformBitmap-Objekt.
    //
    Bitmap(PlatformBitmap* platformBitmap)
        : m_platformBitmap(platformBitmap) {}

    //
    //  Zerstoert das Bitmap-Objekt und gibt den dafuer reservierten
    //  Speicher wieder frei.
    //
    virtual ~Bitmap();

    //
    //  Liefert die Breite der Bitmap in Pixeln.
    //
    int Width() const
    { return m_platformBitmap->Width(); }

    //
    //  Liefert die Hoehe der Bitmap in Pixeln.
    //
    int Height() const
    { return m_platformBitmap->Height(); }

    //
    //  Liefert die Farbtiefe der Bitmap.
    //
    int BitsPerPixel() const
    { return m_platformBitmap->BitsPerPixel(); }

    //
    //  Liefert die Anzahl Bytes, die eine Zeile der Bitmap benoetigt.
    //
    int BytesPerLine() const
    { return m_platformBitmap->BytesPerLine(); }

    //
    //  Setzt die Grafikdaten der Bitmap. Ein Zeiger auf die Daten wird in
    //  data uebergeben. Die Bitmapdaten werden beginnend mit der Zeile
    //  startScan in die Bitmap kopiert. Insgesamt werden scanLines Zeilen
    //  uebernommen. Die Farbtiefe der Daten wird in bpp uebergeben.
    //
    void SetBits(const Graphics& g, void* data, int startScan = 0,
                 int scanLines = 0, int bpp = 0);

    //
    //  Liefert die Grafikdaten der Bitmap. data muss dazu auf einen
    //  ausreichend grossen Speicherbereich zeigen. Es werden beginnend mit
    //  der Zeile startScan insgesamt scanLines Zeilen kopiert. bpp gibt die
    //  Farbtiefe der Daten an.
    //
    void GetBits(const Graphics& g, void* data, int startScan = 0,
                 int scanLines = 0, int bpp = 0) const;

    //
    //  Laedt eine Bitmap aus einer bmp-Datei. Der Dateiname wird in filename
    //  uebergeben. g muss ein gueltiges Graphics-Objekt sein und kann nach
    //  dem Aufruf wieder geloescht werden.
    //
    void Load(const Graphics& g, String filename);

    //
    //  Speichert das Bitmap-Objekt in einer bmp-Datei. Der Dateiname wird
    //  in filename uebergeben. g muss ein gueltiges Graphics-Objekt sein
    //  und kann nach dem Aufruf wieder geloescht werden.
    //
    void Save(const Graphics& g, String filename);

    PlatformBitmap* GetPlatformBitmap() const
    { return m_platformBitmap; }

private:

    PlatformBitmap* m_platformBitmap;
};

//---------------------------------------------------------------------------

#endif
