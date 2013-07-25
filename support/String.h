#ifndef support_String_h
#define support_String_h

#include "support/Hash.h"
#include "support/StringBody.h"
#include "support/Utilities.h"

#include <cstdlib>
#include <cstring>

//----------------------------------------------------------------------------
//
//  Die String-Klasse erlaubt es, im Gegensatz zum Datentyp char*, mit Strings
//  wie mit fundamentalen Datentypen zu arbeiten. Das bedeutet zum Beispiel,
//  dass automatisch immer ausreichend Speicherplatz reserviert wird. Fuer
//  Zuweisungen und Vergleiche koennen die ueblichen Operatoren verwendet
//  werden.
//
//  Die Strings werden so verwaltet, dass der reservierte Speicherbereich
//  zunaechst nur waechst, aber nicht verringert wird. Das kann Vorteile
//  bringen, wenn Strings oft modifiziert und zugewiesen werden. Um den nicht
//  benoetigten Speicher freizugeben, kann aber jederzeit die Funktion
//  Shrink() aufgerufen werden. Die Groesse des reservierten Speichers kann
//  mit der Funktion Resize() geaendert werden.
//
//  Die String-Klasse verwendet eine 'Copy-on-Write'-Strategie. Das bedeutet,
//  dass bei Zuweisungen von Strings erst dann tatsaechlich eine Kopie der
//  Zeichenkette angelegt wird, wenn einer der Strings veraendert wird. Um
//  explizit eine einzelne Kopie eines Strings zu erzeugen, kann die Funktion
//  Single() aufgerufen werden.
//
class String
{
public:

    static const String Empty;

    //
    //  Erzeugt einen String und weist ihm den Speicherbereich data zu. data
    //  muss ein mit new reserviertes Array mindestens der Groesse capacity
    //  sein.
    //
    static String FromBuffer(char* data, uint capacity)
    { return String(new StringBody(data, capacity)); }

    //
    //  Erzeugt einen String und weist ihm die Zeichenkette str zu.
    //
    String(const char* str = "")
        : m_body(new StringBody(str)) {}

    //
    //  Erzeugt einen String, der Platz fuer capacity Zeichen hat, und
    //  initialisiert ihn mit str. Mit dieser Funktion koennen spaetere
    //  Speicherzuweisungen vermieden werden, wenn im voraus eine maximale
    //  Laenge bekannt ist.
    //
    explicit String(uint capacity, const char* str = "")
        : m_body(new StringBody(capacity, str)) {}

    //
    //  Erzeugt eine Kopie des Strings str.
    //
    String(const String& str)
        : m_body(str.m_body)
    { m_body->AddRef(); }

    //
    //  Entfernt den String und gibt den reservierten Speicher frei.
    //
    ~String()
    { m_body->ReleaseRef(); }

    //
    //  Liefert den String als Zeichenkette zurueck.
    //
    const char* CStr() const
    { return m_body->data; }

    //
    //  Der Index-Operator liefert Schreibzugriff auf einzelne Zeichen des
    //  Strings. Falls die Zeichenkette von mehreren Strings referenziert
    //  wird, so wird zunaechst eine eigene Kopie erzeugt.
    //
    char& operator[](uint index)
    { Single(); return m_body->data[index]; }

    //
    //  Liefert Lesezugriff auf einzelne Zeichen des Strings.
    //
    const char& operator[](uint index) const
    { return m_body->data[index]; }

    //
    //  Haengt an den String die Zeichen des Strings str an.
    //
    String& operator+=(const String& str)
    { return Append(str.m_body->data, str.m_body->length); }

    //
    //  Haengt an den String die Zeichen des Strings str an.
    //
    String& operator+=(const char* str)
    { return Append(str, strlen(str)); }

    //
    //  Weist dem String die Zeichenkette des Strings str zu.
    //
    String& operator=(const String& str);

    //
    //  Weist dem String die Zeichenkette str zu.
    //
    String& operator=(const char* str);

    //
    //  Erzeugt einen neuen String mit den ersten count Zeichen des Strings,
    //  fuer den díe Funktion aufgerufen wurde.
    //
    String Left(uint count) const;

    //
    //  Erzeugt einen neuen String mit den letzten count Zeichen des Strings,
    //  fuer den die Funktion aufgerufen wurde.
    //
    String Right(uint count) const;

    //
    //  Erzeugt einen String mit einer Kopie der ersten count Zeichen ab der
    //  Position first (von 0 an gerechnet).
    //
    String Mid(uint first, uint count) const;

    //
    //  Wenn mehr Speicher als notwendig reserviert ist, wird der Speicher
    //  freigegeben und genau soviel Speicher reserviert, wie zur Speicherung
    //  der Zeichenkette notwendig ist.
    //
    void Shrink();

    //
    //  Setzt die Groesse des reservierten Speichers neu. Ist size kleiner als
    //  die Laenge der Zeichenkette, wird diese abgeschnitten.
    //
    void Reserve(uint size);

    //
    //  Liefert die Laenge des Strings in Zeichen.
    //
    uint Length() const
    { return m_body->length; }

    //
    //  Liefern den im String dargestellten Wert zurueck.
    //
    int ToInt() const
    { return atoi(CStr()); }

    long ToLong() const
    { return atol(CStr()); }

    float ToFloat() const
    { return (float) atof(CStr()); }

    double ToDouble() const
    { return atof(CStr()); }

private:

    //
    //  Erstellt einen String durch direkte Uebernahmen von body. Diese
    //  Funktion erhoeht nicht den Referenzzaehler von body.
    //
    String(StringBody* body)
        : m_body(body) {}

    //
    //  Erstellt eine eigene Kopie der Zeichenkette des Strings. Dadurch wird
    //  garantiert, dass kein anderer String die Zeichenkette referenziert.
    //
    void Single()
    {
        if (m_body->refCount > 1)
        {
            m_body->ReleaseRef();
            m_body = new StringBody(m_body->length, m_body->data);
        }
    }

    //
    //  Gibt die Referenz auf den aktuell verwalteten StringBody frei und
    //  uebernimmt body. Diese Funktion erhoeht nicht den Referenzzaehler von
    //  body.
    //
    void AssignBody(StringBody* body)
    {
        m_body->ReleaseRef();
        m_body = body;
    }

    StringBody* m_body;

    static String Concat(const char* str1, uint len1,
                         const char* str2, uint len2);

    String& Append(const char* str, uint len);

    friend String operator+(const String& str1, const String& str2);
    friend String operator+(const String& str1, const char* str2);
    friend String operator+(const char* str1, const String& str2);
};

//----------------------------------------------------------------------------

extern char _string_buffer[256];

//----------------------------------------------------------------------------
//
//  Verknuepft zwei Strings zu einem neuen String.
//
inline String operator+(const String& str1, const String& str2)
{ return String::Concat(str1.CStr(), str1.Length(), str2.CStr(), str2.Length()); }

inline String operator+(const String& str1, const char* str2)
{ return String::Concat(str1.CStr(), str1.Length(), str2, strlen(str2)); }

inline String operator+(const char* str1, const String& str2)
{ return String::Concat(str1, strlen(str1), str2.CStr(), str2.Length()); }

//----------------------------------------------------------------------------
//
//  Testet zwei Strings auf Gleichheit.
//
inline bool operator==(const String& str1, const String& str2)
{ return strcmp(str1.CStr(), str2.CStr()) == 0; }

inline bool operator==(const char* str1, const String& str2)
{ return strcmp(str1, str2.CStr()) == 0; }

inline bool operator==(const String& str1, const char* str2)
{ return strcmp(str1.CStr(), str2) == 0; }

//----------------------------------------------------------------------------
//
//  Liefert true, wenn str1 (alphanumerisch) kleiner ist als str2,
//  sonst false.
//
inline bool operator<(const String& str1, const String& str2)
{ return strcmp(str1.CStr(), str2.CStr()) < 0; }

inline bool operator<(const char* str1, const String& str2)
{ return strcmp(str1, str2.CStr()) < 0; }

inline bool operator<(const String& str1, const char* str2)
{ return strcmp(str1.CStr(), str2) < 0; }

//----------------------------------------------------------------------------
//
//  Erzeugt einen String aus einem numerischen Wert.
//
inline String ToString(int value)
{
    _itoa(value, _string_buffer, 10);
    return String(_string_buffer);
}

inline String ToString(long value)
{
    _ltoa(value, _string_buffer, 10);
    return String(_string_buffer);
}

inline String ToString(float value, int digits = 4)
{
    _gcvt(value, digits, _string_buffer);
    return String(_string_buffer);
}

inline String ToString(double value, int digits = 4)
{
    _gcvt(value, digits, _string_buffer);
    return String(_string_buffer);
}

//----------------------------------------------------------------------------
//
//  Hash-Spezialierung fuer String-Objekte.
//
template <>
struct DefaultHash<String>
{
    uint operator()(const String& key) const
    { return HashString(key.CStr()); }
};

//----------------------------------------------------------------------------

#endif
