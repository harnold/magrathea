#ifndef interface_Control_h
#define interface_Control_h

#include "interface/View.h"
#include "support/Signal.h"

//----------------------------------------------------------------------------
//
//  Die Control-Klasse ist eine Basisklasse fuer alle Views, mit denen der
//  Benutzer interagieren kann. Typischerweise uebersetzen sie physische
//  Ereignisse (z.B. Mausbutton wurde gedrueckt) in logische (z.B. Control
//  wurde aktiviert). Die Weitergabe solcher logischen Ereignisse erfolgt
//  meist in Form von Signalen. Zur Identifikation besitzen Controls eine
//  zusaetzliche Control-Id.
//
class Control: public View
{
public:

    //
    //  Erstellt ein Control im Container parent. id ist die Control-Id.
    //  frame enthaelt Position und Dimensionen des Controls im
    //  Koordinatensystem von parent. label enthaelt einen Textbezeichner.
    //  alignMode gibt an, wie sich das Control bei Groessenaenderungen seines
    //  Parent-Objektes verhaelt.
    //
    Control(int id, Container* parent, const Rect& frame, String label,
            uint alignMode = ALIGN_NONE);

    //
    //  Liefert den Control-Identifier.
    //
    int Id() const
    { return m_id; }

    //
    //  Setzt den Control-Identifier auf id.
    //
    void SetId(int id)
    { m_id = id; }

    //
    //  Liefert den aktuellen Wert des Controls.
    //
    int Value() const
    { return m_value; }

    //
    //  Setzt den Wert des Controls auf value.
    //
    virtual void SetValue(int value);

    //
    //  Liefert den Textbezeichner des Controls zurueck.
    //
    String Label() const
    { return m_label; }

    //
    //  Setzt den Textbezeichner des Controls.
    //
    virtual void SetLabel(String label);

    //
    //  Liefert true, wenn das Control das Default-Control seines Fensters
    //  ist.
    //
    bool IsDefault() const
    { return m_isDefault; }

    //
    //  Setzt oder loescht den Default-Status fuer dieses Control.
    //
    virtual void SetDefault(bool flag = true);

    Signal<int> ValueChanged;

private:

    int     m_id;
    int     m_value;
    String  m_label;
    bool    m_isDefault;
};

//----------------------------------------------------------------------------

#endif
