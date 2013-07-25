#ifndef support_StringBody_h
#define support_StringBody_h

#include "support/Utilities.h"

//----------------------------------------------------------------------------
//
//  Interne Klasse, die einzelne Zeichenketten verwaltet. Die StringBody-
//  Klasse ermoeglicht es, dass mehrere Strings eine Zeichenkette gemeinsam
//  verwenden koennen.
//
struct StringBody
{
    static const uint DEFAULT_CAPACITY  = 20;
    static const uint MAX_CAPACITY      = 1u << 31;

    //
    //  Erstellt eine Zeichenkette durch Kopieren von str. Wird fuer str
    //  eine leere Zeichenkette ("") uebergeben, wird Speicher der Laenge
    //  StringBody::DEFAULT_CAPACITY reserviert.
    //
    explicit StringBody(const char* str = "");

    //
    //  Reserviert Speicher, um eine Zeichenkette der Laenge capacity
    //  aufnehmen zu koennen und initialisiert ihn mit str.
    //
    explicit StringBody(uint capacity, const char* str = "");

    //
    //  Uebernimmt die Zeichenkette str, ohne neuen Speicher zu reservieren.
    //  data muss ein mit new reserviertes Array mindestens der Groesse
    //  capacity sein.
    //
    StringBody(char* data, uint capacity);

    //
    //  Gibt den fuer die Zeichenkette reservierten Speicher wieder frei.
    //
    ~StringBody()
    { delete[] data; }

    //
    //  Erhoeht den Referenzzaehler um 1.
    //
    void AddRef()
    { ++refCount; }

    //
    //  Verringert den Referenzzaehler um 1 und loescht das Objekt, wenn der
    //  Zaehler danach kleiner oedr gleich 0 ist.
    //
    void ReleaseRef()
    {
        if (--refCount <= 0)
            delete this;
    }

    int     refCount;
    uint    capacity;
    uint    length;
    char*   data;

private:

    StringBody(const StringBody&);
    StringBody& operator=(const StringBody&);
};

//----------------------------------------------------------------------------

#endif
