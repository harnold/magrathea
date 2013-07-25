#ifndef interface_Graphics_h
#define interface_Graphics_h

#include "interface/Bitmap.h"
#include "interface/Color.h"
#include "interface/Image.h"
#include "interface/PlatformGraphics.h"
#include "interface/View.h"
#include "support/String.h"
#include "support/Utilities.h"

//----------------------------------------------------------------------------
//
//  Die Graphics-Klasse stellt ein abstraktes Grafikobjekt zum Zeichnen auf
//  Views oder Bitmaps zur Verfuegung.
//
//  Graphics-Objekte muessen fuer jedes View- oder Bitmap-Objekt seperat
//  erstellt werden, sie sind nicht zwischen verschiedenen Objekten
//  uebertragbar. Graphics-Objekte koennen auf folgende Arten erstellt werden:
//
//  Wird das zuzuordnende View-Objekt im Konstruktor des Graphics-Objektes
//  angegeben, so wird ein Graphics-Objekt zum synchronen Zeichnen erstellt,
//  das sofort benutzt werden kann, unabhaengig davon, ob ein Neuzeichnen des
//  Views angefordert wird. Solche Graphics-Objekte sollten nach der
//  Ausfuehrung der Zeichenoperationen moeglichst schnell wieder freigegeben
//  werden, um Ressourcen zu sparen.
//
//  Wird im Konstruktor des Graphics-Objektes kein zuzuordnendes View-Objekt
//  angegeben, wird ein Graphics-Objekt zum asynchronen Zeichnen eines Views
//  erstellt. Vor der Ausfuehrung der Zeichenoperationen muss erst Begin()
//  aufgerufen werden, um dem Graphics-Objekt ein View-Objekt zuzuordnen.
//  Nach der Beendigung der Zeichenoperationen muessen diese mit End()
//  abgeschlossen werden.
//
//  Ein View sollte den groessten Teil seiner Zeichenoperationen asynchron,
//  d.h. innerhalb von Draw() ausfuehren. Die OnPaint()-Methode wird
//  automatisch in einen Begin()/End() - Block eingeschlossen.
//
//  Schliesslich kann ein View-Objekt zum direkten Zeichnen in Bitmaps
//  erstellt werden. Dazu muss dem Konstruktor des Graphics-Objekts das
//  Bitmap-Objekt und ein damit kompatibles Graphics-Objekt uebergeben werden.
//
class Graphics
{
public:

    //
    //  Erstellt ein Graphics-Objekt. Wenn view nicht 0 ist, kann das
    //  Graphics-Objekt nach der Erstellung sofort verwendet werden.
    //  Andernfalls muss vor seiner Verwendung erst mit Begin() ein
    //  View-Objekt zugewiesen werden.
    //
    Graphics(View* view = nullptr);

    //
    //  Erstellt ein Graphics-Objekt zum direkten Zeichnen in eine Bitmap.
    //  Das Graphics-Objekt g muss das kompatibel zu dem Graphics-Objekt sein,
    //  dass zum Erstellen von bitmap verwendet wurde.
    //
    Graphics(const Graphics& g, Bitmap* bitmap);

    //
    //  Zerstoert das Graphics-Objekt und loest die Verbindung zum
    //  zugehoerigen View-Objekt.
    //
    virtual ~Graphics();

    //
    //  Ordnet dem Graphics-Objekt ein View-Objekt zu. Diese Funktion muss
    //  aufgerufen werden, wenn innerhalb der Draw()-Methode von View
    //  gezeichnet werden soll.
    //
    //  Begin() darf ausschliesslich innerhalb von View::Draw() aufgerufen
    //  werden. Jeder mit Begin() eingeleitete Zeichenvorgang muss mit einem
    //  Aufruf von End() abgeschlossen werden.
    //
    void Begin(View* view)
    { m_platformGraphics->Begin(view->GetPlatformWindow()); }

    //
    //  Beendet einen mit Begin() eingeleiteten Zeichenvorgang. Das Graphics-
    //  Objekt ist danach ungueltig. Zu jedem Aufruf von Begin() muss genau
    //  ein Aufruf von End() existieren.
    //
    void End()
    { m_platformGraphics->End(); }

    //
    //  Liefert das kleinste Rechteck innerhalb des Views zurueck, dass
    //  neugezeichnet werden muss. Die Funktion liefert nur dann ein gueltiges
    //  Ergebnis, wenn sie zwischen Begin() und End() aufgerufen wird.
    //
    Rect UpdateRect() const
    { return m_platformGraphics->UpdateRect(); }

    //
    //  Setzt die Zeichenfarbe des Graphics-Objektes.
    //
    void SetDrawColor(Color c)
    {
        m_drawColor = c;
        m_platformGraphics->SetDrawColor(c);
    }

    //
    //  Liefert die aktuelle Zeichenfarbe des Graphics-Objektes.
    //
    Color DrawColor() const
    { return m_drawColor; }

    //
    //  Setzt die Fuellfarbe des Graphics-Objektes.
    //
    void SetFillColor(Color c)
    {
        m_fillColor = c;
        m_platformGraphics->SetFillColor(c);
    }

    //
    //  Liefert die aktuelle Fuellfarbe des Graphics-Objekts.
    //
    Color FillColor() const
    { return m_fillColor; }

    //
    //  Setzt den Font des Graphics-Objektes entsprechend den in font
    //  enthaltenen Daten. Das Graphics-Objekt und font werden durch diese
    //  Funktion nicht miteinander verbunden, vielmehr werden nur die Daten
    //  aus font kopiert.
    //
    void SetFont(const Font& font)
    { m_platformGraphics->SetFont(font); }

    //
    //  Setzt den Grafikcursor auf die Position (x, y).
    //
    void SetCursor(int x, int y)
    { m_platformGraphics->SetCursor(x, y); }

    //
    //  Setzt den Zeichenmodus des Graphics-Objektes. mode muss eine OP_xxx -
    //  Konstante sein. Der Zeichenmodus gibt an, wie bei Zeichenoperationen
    //  Quell- und Zielfarbe kombiniert werden.
    //
    void SetDrawingMode(drawing_mode mode)
    { m_platformGraphics->SetDrawingMode(mode); }

    //
    //  Zeichnet eine Linie von der aktuellen Position des Grafikcursors zum
    //  Punkt (x, y). Der Endpunkt (x, y) wird nicht mitgezeichnet.
    //
    void DrawLine(int x, int y)
    { m_platformGraphics->DrawLine(x, y); }

    //
    //  Zeichnet eine Linie von der aktuellen Position des Grafikcursors zum
    //  Punkt p. Der Endpunkt p wird nicht mitgezeichnet.
    //
    void DrawLine(const Point& p)
    { m_platformGraphics->DrawLine(p.x, p.y); }

    //
    //  Zeichnet eine Linie von (x0, y0) nach (x1, y1). Der Endpunkt (x1, y1)
    //  wird nicht mitgezeichnet.
    //
    void DrawLine(int x0, int y0, int x1, int y1)
    { m_platformGraphics->DrawLine(x0, y0, x1, y1); }

    //
    //  Zeichnet eine Linie von p0 nach p1. Der Endpunkt p1 wird nicht
    //  mitgezeichnet.
    //
    void DrawLine(const Point& p0, const Point& p1)
    { m_platformGraphics->DrawLine(p0.x, p0.y, p1.x, p1.y); }

    //
    //  Zeichnet einen rechteckigen Rahmen.
    //
    void DrawRect(int x, int y, int width, int height)
    { m_platformGraphics->DrawRect(x, y, width, height); }

    void DrawRect(const Rect& r)
    { m_platformGraphics->DrawRect(r.x, r.y, r.width, r.height); }

    //
    //  Zeichnet ein Rechteck und fuellt es mit der aktuellen Fuellfarbe.
    //
    void FillRect(int x, int y, int width, int height)
    { m_platformGraphics->FillRect(x, y, width, height); }

    void FillRect(const Rect& r)
    { m_platformGraphics->FillRect(r.x, r.y, r.width, r.height); }

    //
    //  Zeichnet schattierte Kanten eines Rechtecks. edges gibt an, welche
    //  Seiten gezeichnet werden sollen. edges muss eine Kombination von
    //  DRAW_xxx_EDGE - Konstanten sein. style gibt den Zeichenstil an. style
    //  muss eine Kombination von DRAW_xxx_INNER und DRAW_xxx_OUTER sein.
    //  fill gibt an, ob das Rechteck entsprechend den Systemfarben gefuellt
    //  werden soll.
    //
    void DrawEdge(const Rect& frame, int style, int edges = ALL_EDGES,
                  bool fill = true, bool softed = true)
    { m_platformGraphics->DrawEdge(frame, style, edges, fill, softed); }

    //
    //  Zeichnet den String text mit dem aktuell ausgewaehlten Font.
    //  (x, y) bezeichnet den Startpunkt der Textausgabe. Wenn der Parameter
    //  length angegeben wird, gibt er die max. Anzahl Zeichen an, die
    //  ausgegeben werden sollen.
    //
    void DrawText(int x, int y, String text)
    { m_platformGraphics->DrawText(x, y, text, text.Length()); }

    void DrawText(int x, int y, String text, uint length)
    { m_platformGraphics->DrawText(x, y, text, length); }

    void DrawText(const Rect& frame, String text,
                  uint flags = SINGLELINE_TEXT | ALIGN_LEFT | ALIGN_TOP)
    { m_platformGraphics->DrawText(frame, text, flags); }

    //
    //  Liefert Breite und Hoehe des String text, bezogen auf den aktuell
    //  ausgewaehlten Font.
    //
    void GetTextBounds(String text, int* width, int* height)
    { return m_platformGraphics->GetTextBounds(text, width, height); }

    //
    //  Setzt ein Pixel an (x, y) in der aktuellen Farbe bzw. in der
    //  Farbe c.
    //
    void SetPixel(int x, int y)
    { m_platformGraphics->SetPixel(x, y, m_drawColor); }

    void SetPixel(int x, int y, Color c)
    { m_platformGraphics->SetPixel(x, y, c); }

    //
    //  Liefert die Farbe des Pixels an (x, y).
    //
    Color GetPixel(int x, int y)
    { return m_platformGraphics->GetPixel(x, y); }

    //
    //  Zeichnet ein Bitmap-Objekt mit der linken oberen Ecke an (x, y).
    //
    void DrawBitmap(const Bitmap* bitmap, int x, int y)
    {
        m_platformGraphics->DrawBitmap(
            bitmap->GetPlatformBitmap(), x, y);
    }

    //
    //  Zeichent ein Bitmap-Objekt in das Zielrechteck dest. Stimmen die
    //  Groessen von dest und image nicht ueberein, wird die Bitmap
    //  verkleinert oder vergroessert gezeichnet.
    //
    void DrawBitmap(const Bitmap* bitmap, const Rect& dest)
    {
        m_platformGraphics->DrawBitmap(
            bitmap->GetPlatformBitmap(),
            Rect(0, 0, bitmap->Width(), bitmap->Height()), dest);
    }

    //
    //  Zeichnet den Ausschnitt source aus der Bitmap bitmap in das
    //  Zielrechteck dest. Stimmen die Groessen von dest und source nicht
    //  ueberein, wird der Ausschnitt verkleinert oder vergroessert
    //  gezeichnet.
    //
    void DrawBitmap(const Bitmap* bitmap, const Rect& source, const Rect& dest)
    {
        m_platformGraphics->DrawBitmap(
            bitmap->GetPlatformBitmap(), source, dest);
    }

    //
    //  Zeichnet ein Image-Objekt mit der linken oberen Ecke an (x, y).
    //
    void DrawImage(const Image* image, int x, int y)
    {
        m_platformGraphics->DrawImage(
            image->GetPlatformImage(), x, y);
    }

    //
    //  Zeichent ein Image-Objekt in das Zielrechteck dest. Stimmen die
    //  Groessen von dest und image nicht ueberein, wird das Image
    //  verkleinert oder vergroessert gezeichnet.
    //
    void DrawImage(const Image* image, const Rect& dest)
    {
        m_platformGraphics->DrawImage(
            image->GetPlatformImage(),
            Rect(0, 0, image->Width(), image->Height()), dest);
    }

    //
    //  Zeichnet den Ausschnitt source aus dem Image image in das
    //  Zielrechteck dest. Stimmen die Groessen von dest und source nicht
    //  ueberein, wird der Ausschnitt verkleinert oder vergroessert
    //  gezeichnet.
    //
    void DrawImage(const Image* image, const Rect& source, const Rect& dest)
    {
        m_platformGraphics->DrawImage(
            image->GetPlatformImage(), source, dest);
    }

    //
    //  Zeichnet ein Focus-Rechteck.
    //
    void DrawFocusRect(const Rect& frame)
    { m_platformGraphics->DrawFocusRect(frame); }

    //
    //  Stellt sicher, dass alle Zeichenoperationen des aktuellen Threads
    //  abgeschlossen sind.
    //
    void Sync()
    { m_platformGraphics->Sync(); }

    //
    //  Liefert die Systemfarbe color zurueck.
    //
    Color GetSystemColor(system_color color) const
    { return m_platformGraphics->GetSystemColor(color); }

    //
    //  Liefert die Standard-Schrift fuer Bedienelemente.
    //
    Font GetSystemFont() const
    { return m_platformGraphics->GetSystemFont(); }

    PlatformGraphics* GetPlatformGraphics() const
    { return m_platformGraphics; }

private:

    PlatformGraphics*   m_platformGraphics;
    Color               m_drawColor;
    Color               m_fillColor;
};

//----------------------------------------------------------------------------

#endif
