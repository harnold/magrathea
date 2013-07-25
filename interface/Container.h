#ifndef interface_Container_h
#define interface_Container_h

#include "interface/View.h"
#include "support/Vector.h"

class Window;

//----------------------------------------------------------------------------
//
//  Basisklasse fuer visuelle Container-Objekte.
//
//  Container stellt eine Basisklasse fuer visuelle Objekte (Views) bereit,
//  die andere View-Objekte enthalten und verwalten koennen. Einem Container
//  koennen Views mit der Funktion AddChild() hinzugefuegt werden, wodurch er
//  zu einem Parent-Objekten dieser Views wird.
//
class Container: public View
{
public:

    //
    //  Erstellt einen leeren Container. frame enthaelt Position und
    //  Ausdehnung des Containers im Koordinatensystem des uebergeordneten
    //  Containers parent. name enthaelt den Namen des Containers.  alignMode
    //  gibt an, wie sich das Objekt verhaelt, wenn sich die Groesse seines
    //  uebergeordneten Containers veraendert.
    //
    Container(Container* parent, const Rect& frame, uint alignMode = ALIGN_NONE);

    //
    //  Zerstoert den Container und alle untergeordneten Views.
    //
    ~Container();

    //
    //  Liefert das Fenster-Objekt, dem dieser Container (direkt oder
    //  indirekt) hinzugefuegt wurde, oder 0.
    //
    ::Window* Window() const
    { return m_window; }

    //
    //  Liefert die Anzahl der registrierten Child-Views.
    //
    uint CountChildren() const
    { return m_childList.Count(); }

    //
    //  Liefert einen Zeiger auf das Child-View an der Position index in der
    //  Child-Liste. Liegt index ausserhalb des gueltigen Bereiches von 0 bis
    //  CountChildren() - 1, liefert die Funktion Null.
    //
    View* ChildAt(uint index) const
    { return m_childList.At(index); }

    //
    //  Fuegt das View-Objekt view als untergeordnetes Objekt in die Child-
    //  Liste ein. Nach dem Hinzufuegen wird view mit einem Aufruf von
    //  AttachedToParent() benachrichtigt.  Diese Funktion setzt den Container
    //  nicht als Parent-Objekt von view.
    //
    //  Ein View-Objekt kann nur einem einzigen Container-Objekt untergeordnet
    //  sein, wenn view bereits ein Parent-Objekt besitzt, wird es nicht in
    //  die Child-Liste eingefuegt.
    //
    virtual void AddChild(View* view);

    //
    //  Entfernt view aus der Child-Liste. view behaelt dabei alle seine
    //  untergeordneten Views. Nach dem Entfernen wird view mit einem Aufruf
    //  von DetachedFromParent() benachrichtigt. Diese Funktion aendert nicht
    //  das Parent-Objekt von view. Die Funktion liefert true, wenn view
    //  entfernt wurde, und false, wenn view nicht in der Child-Liste
    //  enthalten ist.
    //
    virtual bool RemoveChild(View* view);

    //
    //  Wird aufgerufen, wenn die Groesse des Containers veraendert wird.
    //  Daraufhin fuehrt der Container eine Reorganisation der in ihm
    //  enthaltenen View-Komponenten durch. Abgeleitete Klassen, die diese
    //  Funktion ueberschreiben, muessen die geerbte Version aufrufen, um
    //  diese Funktionalitaet zu erhalten.
    //
    void FrameResized(const Size& size);

    void AttachedToWindow();
    void DetachedFromWindow();

    virtual void AllAttached();
    virtual void AllDetached();

protected:

    Container();

    void Init(PlatformWindow* platformWindow, Graphics* graphics,
              Container* parent, const Rect& frame, uint alignMode);

private:

    Vector<View>    m_childList;
    Size            m_oldSize;
    ::Window*       m_window;
};

//----------------------------------------------------------------------------

#endif
