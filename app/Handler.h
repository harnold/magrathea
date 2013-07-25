#ifndef app_Handler_h
#define app_Handler_h

#include "support/Utilities.h"

class Message;
class Looper;

//----------------------------------------------------------------------------
//
//  Basisklasse fuer alle Klassen, die auf Messages reagieren sollen.
//
//  Handler-Objekte regieren auf Messages, die ihnen von Looper-Objekten
//  gesendet werden. Dazu ruft der zugehoerige Looper die Funktion
//  MessageReceived() von Handler auf. Jedes Handler-Objekt besitzt genau ein
//  zugeordnetes Looper-Objekt.
//
//  Messages, die der Handler nicht bearbeitet, koennen an den naechsten
//  Handler weitergeleitet werden. Dazu muss einfach die geerbte Version von
//  MessageReceived() aufgerufen werden.
//
class Handler
{
public:

    //
    //  Zerstoert das Handler-Objekt. Falls nicht schon geschehen, wird der
    //  Handler aus der Handler-Liste des zugehoerigen Looper-Objektes
    //  entfernt.
    //
    virtual ~Handler();

    //
    //  Liefert einen Zeiger auf den naechsten Handler in der Handler-Liste
    //  oder 0, wenn es keinen naechsten Handler gibt.
    //
    Handler* NextHandler() const
    { return m_nextHandler; }

    //
    //  Setzt handler als naechsten Handler in der Handler-Liste. An diesen
    //  Handler werden Messages weitergeleitet, die der Handler, fuer den die
    //  Funktion aufgerufen wurde, nicht bearbeiten kann.
    //
    void SetNextHandler(Handler* handler);

    //
    //  Liefert das zugeordnete Looper-Objekt oder 0, wenn der Handler nicht
    //  mit einem Looper assoziiert ist. Der Handler muss mit einem Looper
    //  verbunden werden, bevor er Messages bearbeiten kann. Handler werden
    //  mit Looper-Objekten verbunden, indem deren AddHandler()-Funktion
    //  aufgerufen wird. Ein Handler kann zu einem Zeitpunkt nur mit einem
    //  Looper verbunden sein.
    //
    ::Looper* Looper() const
    { return m_looper; }

    //
    //  Setzt looper als mit diesem Handler assoziierten Looper. Diese
    //  Funktion wird normalerweise von Looper::AddHandler() aufgerufen.
    //
    void SetLooper(::Looper* looper)
    { m_looper = looper; }

    //
    //  Behandelt die Message message. Diese Funktion muss von abgeleiteten
    //  Klassen ueberschrieben werden, um den Handler mit Funktionalitaet
    //  auszustatten. Die Default-Implementation leitet message an den
    //  naechsten Handler in der Handler-Liste weiter.
    //
    virtual void MessageReceived(Message* message);

protected:

    //
    //  Erstellt einen neuen Handler.
    //
    Handler()
        : m_nextHandler(nullptr),
          m_looper(nullptr) {}

private:

    Handler*    m_nextHandler;
    ::Looper*   m_looper;
};

//----------------------------------------------------------------------------

#endif
