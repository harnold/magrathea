#ifndef app_Looper_h
#define app_Looper_h

#include "app/Handler.h"
#include "app/Message.h"
#include "app/MessageQueue.h"
#include "support/List.h"
#include "support/Utilities.h"

//----------------------------------------------------------------------------
//
//  Basisklasse, die eine Botschaftsschleife bereitstellt.
//
//  Looper-Objekte bearbeiten Botschaften, ihnen von anderen Objekten oder
//  vom Betriebssystem gesendet werden. Die Botschaften werden an Handler-
//  Objekte weitergeleitet, die sich mit der Funktion AddHandler() registriert
//  haben.
//
class Looper: public Handler
{
public:

    static const int DEFAULT_CAPACITY = 50;

    //
    //  Erstellt ein neues Looper-Objekt.
    //
    Looper(uint capacity = DEFAULT_CAPACITY);

    //
    //  Zerstoert das Looper-Objekt und loest die Verbindung zu allen
    //  registrierten Handler-Objekten.
    //
    virtual ~Looper();

    //
    //  Liefert einen Zeiger auf das Message-Objekt das gerade verarbeitet
    //  wird.  Wird die Funktion von ausserhalb der Botschaftsschleife
    //  aufgerufen, wird 0 zurueckgeliefert.
    //
    Message* CurrentMessage() const
    { return m_currentMessage; }

    //
    //  Liefert die Botschaft-Warteschlange des Looper-Objekts zurueck.
    //
    const ::MessageQueue* MessageQueue() const
    { return &m_messageQueue; }

    ::MessageQueue* MessageQueue()
    { return &m_messageQueue; }

    //
    //  Liefert den Default-Handler zurueck. Dieser Handler wird immer dann
    //  aufgerufen, wenn das Zielobjekt einer Botschaft undefiniert (0) oder
    //  nicht in der Handler-Liste des Loopers eingetragen ist.
    //
    Handler* DefaultHandler() const
    { return m_defaultHandler; }

    //
    //  Registriert handler als Default-Handler. Dieser Handler wird immer
    //  dann aufgerufen, wenn das Zielobjekt einer Botschaft undefiniert (0)
    //  oder nicht in der Handler-Liste des Loopers eingetragen ist.
    //
    void SetDefaultHandler(Handler* handler);

    //
    //  Sendet die Botschaft message an den Handler handler.  Das
    //  Message-Objekt wird von PostMessage() kopiert und der
    //  Botschafts-Warteschlange des Loopers hinzugefuegt.
    //
    bool PostMessage(Message* message, Handler* handler = nullptr);

    //
    //  Erzeugt eine Botschaft vom Typ what und sendet sie an den Handler
    //  handler.  Dazu wird ein neues Message-Objekt erzeugt und der
    //  Botschafts-Warteschlange des Loopers hinzugefuegt.
    //
    bool PostMessage(uint what, Handler* handler = nullptr);

    //
    //  Fuegt der Handler-Liste das Handler-Objekt handler hinzu.
    //
    virtual void AddHandler(Handler* handler);

    //
    //  Entfernt das Handler-Objekt handler aus der Handler-Liste. Die
    //  Funktion liefert true, wenn der Handler entfernt wurde und false, wenn
    //  der Handler nicht in der Liste enthalten war.
    //
    virtual bool RemoveHandler(Handler* handler);

    //
    //  Verteilt die Botschaft message an das entsprechende Handler-Objekt.
    //  Der Handler wird wie folgt ermittelt:
    //
    //  1. Ist message->Handler() != 0 und ist dieses Looper-Objekt der
    //  diesem Handler zugeordnete Looper, wird die MessageReceived()-
    //  Funktion dieses Handlers aufgerufen.
    //
    //  2. Ist DefaultHandler() != 0, wird DefaultHandler()->MessageReceived()
    //  aufgerufen.
    //
    //  3. Ansonsten wird die MessageReceived()-Funktion des Looper-Objekts
    //  aufgerufen.
    //
    virtual void DispatchMessage(Message* message);

    //
    //  Wird aufgerufen, wenn der Looper eine Botschaft vom Typ MSG_QUIT
    //  erhaelt.  Liefert diese Funktion true zurueck, wird Quit() aufgerufen.
    //
    virtual bool QuitRequested();

    //
    //  Started die Botschaftsschleife.
    //
    virtual void Run() = 0;

    //
    //  Beendet die Botschaftsschleife.
    //
    virtual void Quit() = 0;

protected:

    static void UnlinkHandler(Handler* handler)
    { handler->SetLooper(nullptr); }

    //
    //  Verteilt die naechste Botschaft, falls die Botschafts-Warteschlange
    //  nicht leer ist.
    //
    virtual void DispatchNextMessage();

private:

    Handler*        m_defaultHandler;
    List<Handler>   m_handlers;
    Message*        m_currentMessage;
    ::MessageQueue  m_messageQueue;
};

//----------------------------------------------------------------------------

#endif
