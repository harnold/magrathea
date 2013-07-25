#ifndef support_Signal_h
#define support_Signal_h

#include "support/Utilities.h"

template <class T>
class SignalConnection;

template <class T, class Recv>
class SignalConnectionImpl;

//----------------------------------------------------------------------------
//
//  Die Signal-Klasse dient zur Kommunikation zwischen Objekten.
//
//  Signale koennen mit typkompatiblen Elementfunktionen (Slots) "verbunden"
//  werden. Zwischen Signalen und Slots besteht eine n:n - Beziehung. Wird
//  ein Signal aktiviert, werden alle verbundenen Slots mit dem Parameter
//  des Signals aufgerufen.
//
template <class T>
class Signal
{
public:

    //
    //  Erstellt ein neues Signal-Objekt. Dabei wird eine (noch leere) Liste
    //  fuer die zu verbindenden Slots angelegt.
    //
    Signal()
        : m_connections(nullptr) {}

    //
    //  Zerstoert ein Signal-Objekt. Der fuer die Speicherung der Verbindungen
    //  reservierte Speicher wird freigegeben.
    //
    ~Signal();

    //
    //  Aktiviert das Signal. Alle verbundenen Slots werden nacheinander mit
    //  dem Parameter arg aufgerufen.
    //
    void operator()(T arg);

    //
    //  Verbindet das Signal mit dem Slot fn des Objekts receiver. Wird danach
    //  das Signal aktiviert, wird receiver::fn() aufgerufen.
    //
    template <class Recv>
    void Connect(Recv* receiver, void (Recv::*fn)(T))
    {
        m_connections = new SignalConnectionImpl<T, Recv>(
            receiver, fn, m_connections);
    }

    //
    //  Loest die Verbindung zwischen dem Signal und dem Slot fn des Objekts
    //  receiver. Wird danach das Signal aktiviert, wird receiver::fn() nicht
    //  mehr aufgerufen.
    //
    template <class Recv>
    void Disconnect(Recv* receiver, void (Recv::*fn)(T));

private:

    //
    //  Signale koennen nicht kopiert oder zugewiesen werden.
    //
    Signal(const Signal<T>&);
    Signal<T>& operator=(const Signal&);

    SignalConnection<T>* m_connections;
};

//----------------------------------------------------------------------------
//
//  Struktur zur Speicherung einer Verbindung zwischen dem Signal und
//  einem Slot.
//
template <class T>
struct SignalConnection
{
    SignalConnection(void* receiver, SignalConnection<T>* next)
        : receiver(receiver), next(next) {}

    virtual void EmitSignal(T) = 0;

    void*                   receiver;
    SignalConnection<T>*    next;
};

template <class T, class Recv>
struct SignalConnectionImpl: public SignalConnection<T>
{
    typedef void (Recv::*MemberFn)(T);

    SignalConnectionImpl(Recv* receiver, MemberFn fn, SignalConnection<T>* next)
        : SignalConnection<T>(receiver, next), fn(fn) {}

    void EmitSignal(T arg);

    MemberFn fn;
};

//----------------------------------------------------------------------------
//
//  Verbindet das Signal signal mit dem Slot fn des Objekts receiver. Wird
//  danach das Signal aktiviert, wird receiver::fn() aufgerufen.
//
template <class T, class Recv>
inline void Connect(Signal<T>& signal, Recv* receiver, void (Recv::*fn)(T))
{ signal.Connect(receiver, fn); }

//----------------------------------------------------------------------------
//
//  Loest die Verbindung zwischen dem Signal signal und dem Slot fn des
//  Objekts receiver. Wird danach das Signal aktiviert, wird receiver::fn()
//  nicht mehr aufgerufen.
//
template <class T, class Recv>
inline void Disconnect(Signal<T>& signal, Recv* receiver, void (Recv::*fn)(T))
{ signal.Disconnect(receiver, fn); }

//----------------------------------------------------------------------------

template <class T>
Signal<T>::~Signal()
{
    SignalConnection<T>** c_ptr = &m_connections;

    while (*c_ptr != nullptr)
    {
        SignalConnection<T>* c = *c_ptr;
        *c_ptr = c->next;
        delete c;
    }
}

//----------------------------------------------------------------------------

template <class T>
template <class Recv>
void Signal<T>::Disconnect(Recv* receiver, void (Recv::*fn)(T))
{
    SignalConnection<T>** c_ptr = &m_connections;

    while (*c_ptr != nullptr)
    {
        SignalConnection<T>* c = *c_ptr;

        if (c->receiver == receiver)
        {
            SignalConnectionImpl<T, Recv>* ci =
                static_cast<SignalConnectionImpl<T, Recv>*>(c);

            if (ci->fn == fn)
            {
                *c_ptr = ci->next;
                delete ci;
                return;
            }
        }

        c_ptr = &c->next;
    }
}

//----------------------------------------------------------------------------

template <class T>
void Signal<T>::operator()(T arg)
{
    SignalConnection<T>* c = m_connections;

    while (c != nullptr)
    {
        c->EmitSignal(arg);
        c = c->next;
    }
}

//----------------------------------------------------------------------------

template <class T, class Recv>
void SignalConnectionImpl<T, Recv>::EmitSignal(T arg)
{
    (static_cast<Recv*>(this->receiver)->*fn)(arg);
}

//----------------------------------------------------------------------------

#endif
