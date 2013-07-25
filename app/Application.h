#ifndef app_Application_h
#define app_Application_h

#include "app/Looper.h"

class ResourceLoader;
class Window;

//----------------------------------------------------------------------------
//
//  Allgemeine Anwendungsklasse
//
class Application: public Looper
{
public:

    //
    //  Zerstoert das Objekt.
    //
    ~Application();

    //
    //  Registriert window als Hauptfenster der Anwendung. Wird window
    //  geschlossen, wird die Anwendung beendet.
    //
    void SetMainWindow(Window* window)
    { m_mainWindow = window; }

    //
    //  Liefert einen Zeiger auf das Hauptfenster der Anwendung.
    //
    Window* MainWindow() const
    { return m_mainWindow; }

    //
    //  Entfernt das Handler-Objekt handler aus der Handler-Liste. Die
    //  Funktion liefert true, wenn der Handler entfernt wurde und false,
    //  wenn der Handler nicht in der Liste enthalten war.
    //
    //  Ist handler das Hauptfenster-Objekt, wird die Botschaftsschleife
    //  und damit die Anwendung beendet.
    //
    bool RemoveHandler(Handler* handler);

    //
    //  Liefert ein plattformabhaengiges ResourceLoader-Objekt zurueck.
    //
    virtual ::ResourceLoader* ResourceLoader();

protected:

    //
    //  Erstellt ein Application-Objekt.
    //
    Application();

private:

    Window* m_mainWindow;
};

//----------------------------------------------------------------------------

extern Application* TheApp;

//----------------------------------------------------------------------------

#endif
