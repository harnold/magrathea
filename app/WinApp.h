#ifndef app_WinApp_h
#define app_WinApp_h

#include "app/Application.h"
#include "app/WinResourceLoader.h"
#include "platform/Win.h"
#include "support/Dict.h"

//----------------------------------------------------------------------------
//
//  Anwendungsklasse fuer Windows-Applikationen.
//
//  Jedes Windows-Programm muss als erste Aktion genau ein Objekt der Klasse
//  WinApp erstellen. Dadurch wird die globale Variable TheWinApp
//  initialisiert, die von vielen Klassen benoetigt wird.
//
class WinApp: public Application
{
public:

    static const char* DefaultWindowClassName;

    //
    //  Erstellt ein WinApp-Objekt. Die Parameter instance, cmdLine und
    //  cmdShow entsprechen den Parametern der WinMain()-Funktion.
    //
    WinApp(HINSTANCE instance, LPSTR cmdLine, int cmdShow);

    //
    //  Zerstoert das Objekt. Das sollte als letzte Aktion des Programms
    //  erfolgen, nachdem alle anderen Objekte zerstoert sind.
    //
    ~WinApp();

    //
    //  Liefert das Windows-Handle der Applikation zurueck.
    //
    HINSTANCE Handle() const
    { return m_hInstance; }

    //
    //  Registriert den zu einem Fenster-Handle gehoerenden Message-Handler.
    //  Windows-Botschaften an hWindow werden danach in Message-Objekte
    //  uebersetzt und an handler gesendet.
    //
    void RegisterMessageHandler(HWND hWindow, Handler* handler)
    { m_messageHandlers.Replace(hWindow, handler); }

    //
    //  Liefert den fuer hWindow registrierten Handler zurueck, oder 0, wenn
    //  kein solcher Handler registriert wurde.
    //
    Handler* GetMessageHandler(HWND hWindow)
    { m_messageHandlers[hWindow]; }

    //
    //  Entfernt die Registrierung des zu hWindow gehoerenden Handlers.
    //
    void RemoveMessageHandler(HWND hWindow)
    { m_messageHandlers.Remove(hWindow); }

    //
    //  Startet einen neuen Prozess. name kann der Pfad einer ausfuehrbaren
    //  Datei sein, oder der eines Dokumentes. In diesem Fall wird die mit
    //  dem Dokument assoziierte Anwendung gestartet.
    //
    void Exec(String name);

    //
    //  Startet die Botschaftsschleife. In dieser Schleife werden
    //  Botschaften geholt und mit DispatchMessage() an ein Handler-Objekt
    //  uebertragen.
    //
    void Run();

    //
    //  Beendet die Botschaftsschleife des Objekts und danach das Programm.
    //
    void Quit();

    //
    //  Liefert einen ResourceLoader fuer Windows-Resourcen zurueck.
    //
    ::ResourceLoader* ResourceLoader();

private:

    //
    //  Dient als Standardhandler fuer alle Windows-Botschaften. Die Funktion
    //  wandelt diese in systemunabhaengige Message-Objekte um und versendet
    //  sie an die Objekte, auf die sich die Botschaften beziehen.
    //
    static LRESULT CALLBACK WindowProc(HWND hWindow, UINT winMsg,
                                       WPARAM wParam, LPARAM lParam);

    static void CreateMouseMessage(UINT winMsg, WPARAM wParam, LPARAM lParam,
                                   Message* message);

    bool RegisterWindowClass();

    bool                m_quit;
    HINSTANCE           m_hInstance;
    LPSTR               m_cmdLine;
    int                 m_cmdShow;
    Dict<HWND, Handler> m_messageHandlers;
    WinResourceLoader   m_resourceLoader;
};

//----------------------------------------------------------------------------

extern WinApp* TheWinApp;

//----------------------------------------------------------------------------

#endif
