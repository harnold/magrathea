#ifndef interface_View_h
#define interface_View_h

#include "app/Handler.h"
#include "interface/Constants.h"
#include "interface/PlatformWindow.h"
#include "interface/Margins.h"
#include "interface/Rect.h"
#include "support/String.h"

class Container;
class Graphics;
class Window;

//----------------------------------------------------------------------------
//
//  Basisklasse fuer visuelle Objekte.
//
//  View bildet eine Basisklasse fuer alle Objekte, die eine visuelle
//  Repraesentation besitzen.
//
//  Die eigentliche Darstellung auf dem Bildschirm wird mittels eines Graphics-
//  Objektes realisiert, welches explizit erstellt, oder - bei Reaktion auf
//  MSG_VIEW_DRAW Botschaften - vom System bereitgestellt wird.
//
class View: public Handler
{
    friend class Container;
    friend class Window;

public:

    //
    //  Erstellt ein View im Container parent. frame enthaelt Position und
    //  Ausdehnung des Views im Koordinatensystem von parent.  alignMode gibt
    //  an, wie sich das Objekt verhaelt, wenn sich die Groesse seines
    //  uebergeordneten Containers veraendert.
    //
    View(Container* parent, const Rect& frame, uint alignMode = ALIGN_NONE);

    ~View();

    //
    //  Liefert Position und Ausdehnung des Views im Koordinatensystem
    //  seines Parent-Views zurueck.
    //
    Rect Frame() const
    { return m_frame; }

    //
    //  Liefert die Raender des Views zurueck.
    //
    Margins FrameMargins() const
    { return GetFrameMargins(); }

    //
    //  Liefert die Ausdehnung des Views in seinem eigenen Koordinatensystem
    //  zurueck.
    //
    Rect Bounds() const
    { return Rect(0, 0, m_frame.width, m_frame.height); }

    //
    //  Liefert die Breite des Views zurueck.
    //
    int Width() const
    { return m_frame.width; }

    //
    //  Liefert die Hoehe des Views zurueck.
    //
    int Height() const
    { return m_frame.height; }

    //
    //  Aendert Position und Ausdehnung des Views auf die Werte in frame.
    //
    void SetFrame(const Rect& frame)
    { m_platformWindow->SetFrame(frame); m_frame = frame; }

    //
    //  Liefert den Ausrichtungsmodus des Views zurueck.
    //
    uint AlignMode() const
    { return m_alignMode; }

    //
    //  Liefert true, wenn das View zur Zeit angezeigt wird.
    //
    bool IsVisible() const
    { return m_platformWindow->IsVisible(); }

    //
    //  Zeigt das View an oder verbirgt das View. Das View wird nur sichtbar,
    //  wenn alle seine uebergeordneten View-Objekte sichtbar sind.
    //
    void SetVisible(bool flag = true)
    { m_platformWindow->SetVisible(flag); }

    //
    //  Liefert true, wenn das View zur Zeit aktiviert ist.
    //
    bool IsEnabled() const
    { return m_platformWindow->IsEnabled(); }

    //
    //  Aktiviert oder deaktiviert das View. Aktive Views reagieren auf
    //  Interaktionen des Benutzers, deaktivierte Views dagegen nicht.
    //
    void SetEnabled(bool flag = true)
    { m_platformWindow->SetEnabled(flag); }

    //
    //  Liefert true, wenn das View den Eingabe-Fokus besitzt.
    //
    bool HasFocus() const
    { return m_platformWindow->HasFocus(); }

    //
    //  Setzt den Eingabefokus auf das View.
    //
    void SetFocus(bool flag = true)
    { m_platformWindow->SetFocus(flag); }

    //
    //  Kennzeichnet den gesamten Bereich des Views und ungueltig. Beim
    //  naechsten Aufruf von Draw() wird das gesamte View neu gezeichnet.
    //
    void Invalidate()
    { m_platformWindow->Invalidate(); }

    //
    //  Kennzeichnet den durch rect angegebenen Bereich des Views als
    //  ungueltig. Der Bereich wird zu dem Bereich hinzugefuegt, der beim
    //  naechsten Aufruf von Draw() neu gezeichnet werden muss.
    //
    void Invalidate(const Rect& rect)
    { m_platformWindow->Invalidate(rect); }

    //
    //  Veranlasst ein sofortiges Neuzeichnen des Vies, falls seit dem letzten
    //  Zeichnen Bereiche des Views mit Invalidate() als ungueltig
    //  gekennzeichnet wurden.
    //
    void UpdateIfNeeded()
    { m_platformWindow->UpdateIfNeeded(); }

    //
    //  Verschiebt das View im Koordinatensystem seines Parent-Objektes um
    //  dx Punkte in x-Richtung und um dy Punkte in y-Richtung.
    //
    void MoveBy(int dx, int dy);

    //
    //  Verschiebt das View so, dass die linke obere Ecke des Views im Punkt
    //  (x, y) liegt.
    //
    void MoveTo(int x, int y);

    //
    //  Verschiebt das View so, dass die linke obere Ecke des Views im Punkt
    //  point liegt.
    //
    void MoveTo(const Point& point)
    { MoveTo(point.x, point.y); }

    //
    //  Aendert die Groesse des Views um dx Punkte in x-Richtung und um dy
    //  Punkte in y-Richtung.
    //
    void ResizeBy(int dx, int dy);

    //
    //  Aendert die Groesse des Views auf w * h Punkte.
    //
    void ResizeTo(int w, int h);

    //
    //  Bindet die Maus an das View, d.h. alle Mausaktionen werden an das
    //  View-Objekt weitergeleitet, unabhaengig davon, ob sich der Cursor
    //  innerhalb des Views befindet.
    //
    void CaptureMouse()
    { m_platformWindow->CaptureMouse(); }

    //
    //  Gibt die Maus nach einem Aufruf von CaptureMouse() wieder frei.
    //
    void ReleaseMouse()
    { m_platformWindow->ReleaseMouse(); }

    //
    //  Liefert einen Zeiger auf das Parent-Objekt des Views, oder 0, wenn
    //  das View-Objekt kein uebergeordnetes Objekt besitzt.
    //
    Container* Parent() const
    { return m_parent; }

    //
    //  Liefert das Fenster-Objekt, dem dieser Container (direkt oder
    //  indirekt) hinzugefuegt wurde, oder 0.
    //
    ::Window* Window() const
    { return m_window; }

    //
    //  Entfernt das View-Objekt aus der Child-Liste seines Parent-Objektes.
    //  Die Funktion liefert true, wenn das Objekt entfernt wurde, und false,
    //  wenn das View-Objekt kein Parent-Objekt besitzt.
    //
    bool RemoveSelf();

    //
    //  Die folgenen Funktionen konvertieren Koordinaten und Rechtecke
    //  zwischen verschiedenen Koordinatensystemen.
    //
    void ConvertToScreen(Point* point) const
    { m_platformWindow->ConvertToScreen(point); }

    void ConvertFromScreen(Point* point) const
    { m_platformWindow->ConvertFromScreen(point); }

    void ConvertToScreen(Rect* rect) const;
    void ConvertFromScreen(Rect* rect) const;
    void ConvertToParent(Point* point) const;
    void ConvertToParent(Rect* rect) const;
    void ConvertFromParent(Point* point) const;
    void ConvertFromParent(Rect* rect) const;

    //
    //  Wird aufgerufen, um die Botschaft message zu verarbeiten. Abgeleitete
    //  Klassen koennen diese Funktion ueberschreiben, um auf zusaetzliche
    //  Botschaften zu reagieren. Sie sollten aber stets die geerbte Version
    //  von MessageReceived() aufrufen, um das Standardverhalten von Views
    //  zu garantieren.
    //
    void MessageReceived(Message* message);

    //
    //  Draw() wird aufgerufen, wenn der Inhalt des Views neu gezeichnet
    //  werden muss. Abgeleitete Klassen muessen Draw() implementieren, um
    //  sich selbst auf dem Bildschirm darzustellen.
    //
    virtual void Draw(Graphics& g);

    //
    //  Wird aufgerufen, wenn ein View den Focus besitzt und der Benutzer
    //  eine Taste gedrueckt hat. charCode enthaelt den (ASCII-) Zeichencode
    //  der gedrueckten Taste.
    //
    virtual void KeyPressed(int charCode);

    //
    //  Wird aufgerufen, wenn ein View den Focus besitzt und der Benutzer
    //  eine Taste drueckt. keyCode enthaelt den Tastencode (KEY_xxxx).
    //
    virtual void KeyDown(int keyCode);

    //
    //  Wird aufgerufen, wenn ein View den Focus besitzt und der Benutzer
    //  eine Taste loslaesst. keyCode enthaelt den Tastencode (KEY_xxxx).
    //
    virtual void KeyUp(int keyCode);

    //
    //  Wird aufgerufen, wenn der Benutzer im View die Maustaste drueckt.
    //
    virtual void MouseDown(const Point& point, int button, int keyState);

    //
    //  Wird aufgerufen, wenn der Benutzer im View die Maustaste loslaesst.
    //
    virtual void MouseUp(const Point& point, int button, int keyState);

    //
    //  Wird aufgerufen, wenn der Benutzer im View die Maustaste bewegt.
    //
    virtual void MouseMoved(const Point& point, int keyState);

    //
    //  Wird bei einem Doubleclick im View aufgerufen.
    //
    virtual void MouseDoubleClick(const Point& point, int button, int keyState);

    //
    //  Wird regelmaessig aufgerufen, wenn fuer das View ein Timer erstellt
    //  wurde. timerId enthaelt die Bezeichnung des Timer-Objektes.
    //
    virtual void Pulse(int timerId);

    //
    //  Wird aufgerufen, wenn der Status des Focus geaender wurde.
    //
    virtual void FocusChanged(bool focused);

    //
    //  Wird aufgerufen, wenn die Groesse der Views intern oder extern
    //  geaendert wurde.
    //
    virtual void FrameResized(const Size& size);

    //
    //  Wird aufgerufen, wenn die Position des Views intern oder extern
    //  geaendert wurde.
    //
    virtual void FrameMoved(const Point& topleft);

    //
    //  Wird aufgerufen, wenn das View mit SetVisible() neu angezeigt oder
    //  verborgen wird.
    //
    virtual void VisibilityChanged(bool visible);

    //
    //  Wird aufgerufen, wenn das View aktiviert oder deaktiviert wird.
    //
    virtual void Enabled(bool enabled);

    //
    //  Wird bei einer Kommando-Botschaft aufgerufen.
    //
    virtual void ProcessCommand(int commandId);

    //
    //  Wird aufgerufen, nachdem das View (direkt oder indirekt) einem Fenster
    //  hinzugefuegt wurde.
    //
    virtual void AttachedToWindow();

    //
    //  Wird aufgerufen, bevor das View (direkt oder indirekt) von einem
    //  Fenster geloest wird.
    //
    virtual void DetachedFromWindow();

    PlatformWindow* GetPlatformWindow() const
    { return m_platformWindow; }

protected:

    View() {}

    void Init(PlatformWindow* platformWindow, Graphics* graphics,
              Container* parent, const Rect& frame, uint alignMode);

    virtual void SetParent(Container* parent);
    virtual void SetWindow(::Window* window);

    virtual Margins GetFrameMargins() const;

private:

    PlatformWindow* m_platformWindow;
    Graphics*       m_graphics;
    Container*      m_parent;
    Rect            m_frame;
    uint            m_alignMode;
    ::Window*       m_window;
};

//----------------------------------------------------------------------------

#endif
