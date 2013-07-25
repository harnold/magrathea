#include "app/Application.h"
#include "interface/Window.h"
#include "support/Exception.h"
#include "support/Utilities.h"

//----------------------------------------------------------------------------

Application* TheApp = nullptr;

//----------------------------------------------------------------------------

Application::Application()
    : m_mainWindow(nullptr)
{
    if (TheApp != nullptr)
    {
        throw CreationFailed(
            "Fehler in Application::Application(): "
            "Es darf nur ein Application-Objekt geben");
    }

    TheApp = this;
}

//----------------------------------------------------------------------------

Application::~Application()
{
    if (TheApp == this)
        TheApp = nullptr;
}

//----------------------------------------------------------------------------

bool Application::RemoveHandler(Handler* handler)
{
    if (handler == m_mainWindow)
    {
        m_mainWindow = nullptr;
        Quit();
    }

    return Looper::RemoveHandler(handler);
}

//----------------------------------------------------------------------------

::ResourceLoader* Application::ResourceLoader()
{
    throw InvalidOperation(
        "Fehler in Application::ResourceLoader(): "
        "Funktion wird von diesem Application-Objekt nicht unterstuetzt");
}

//----------------------------------------------------------------------------
