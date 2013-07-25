#ifndef support_Exception_h
#define support_Exception_h

//----------------------------------------------------------------------------

enum
{
    BAD_ALLOC           = 0,
    RANGE_ERROR         = 1,
    CREATION_FAILED     = 2,
    INVALID_OPERATION   = 3
};

//----------------------------------------------------------------------------
//
//  Bildet die Basis der Exception-Hierarchie.
//
class Exception
{
public:

    //
    //  Erstellt eine neue Exception mit der Fehlermeldung message und dem
    //  Fehlercode code.
    //
    Exception(const char* message, int code)
        : m_code(code), m_message(message) {}

    //
    //  Liefert den Fehlercode zurueck, der bei der Erstellung der Exception
    //  uebergeben wurde.
    //
    int Code() const
    { return m_code; }

    //
    //  Liefert die Fehlermeldung zurueck, die bei der Erstellung der
    //  Exception uebergeben wurde.
    //
    const char* Message() const
    { return m_message; }

private:

    int         m_code;
    const char* m_message;
};

//----------------------------------------------------------------------------
//
//  Eine Exception vom Typ BadAlloc gibt an, dass eine Speicherreservierung
//  fehlgeschlagen ist.
//
class BadAlloc: public Exception
{
public:

    //
    //  Erstellt eine neue Exception vom Typ BadAlloc.
    //
    BadAlloc(const char* message)
        : Exception(message, BAD_ALLOC) {}
};

//----------------------------------------------------------------------------
//
//  Eine Exception vom Typ CreationFailed gibt an, dass die Erstellung eines
//  Objekts fehlgeschlagen ist. Diese Exception wird verwendet, um Ausnahmen
//  anzuzeigen, die in einem Konstruktor auftreten.
//
class CreationFailed: public Exception
{
public:

    CreationFailed(const char* message)
        : Exception(message, CREATION_FAILED) {}
};

//----------------------------------------------------------------------------
//
//  Eine Exception vom Typ RangeError gibt an, das ein Index ausserhalb seines
//  zugelassenen Bereiches verwendet wurde.
//
class RangeError: public Exception
{
public:

    RangeError(const char* message)
        : Exception(message, RANGE_ERROR) {}
};

//----------------------------------------------------------------------------
//
//  Eine Exception vom Typ InvalidOperation gibt an, dass eine Operation von
//  einem Objekt nicht oder zum aktuellen Zeitpunkt nicht unterstuetzt wird.
//
class InvalidOperation: public Exception
{
public:

    InvalidOperation(const char* message)
        : Exception(message, INVALID_OPERATION) {}
};

//----------------------------------------------------------------------------
//
//  Erzeugt eine Exception vom Typ BadAlloc. Die Funktion kann mit
//  set_new_handler(ThrowBadAlloc) als new-Handler installiert werden.
//
void ThrowBadAlloc() throw(BadAlloc);

//----------------------------------------------------------------------------

#endif
