#include "app/WinResourceLoader.h"
#include "app/WinApp.h"
#include "interface/Bitmap.h"
#include "interface/Menu.h"
#include "interface/WinMenu.h"
#include "interface/WinBitmap.h"
#include "platform/Win.h"

//----------------------------------------------------------------------------

static char string_buffer[4096];

//----------------------------------------------------------------------------

String WinResourceLoader::LoadString(uint id)
{
    if (!::LoadStringA(TheWinApp->Handle(), id, string_buffer,
                       sizeof(string_buffer)))
    {
        throw ResourceFailure(
            "Fehler in WinResourceLoader::LoadString(uint): "
            "Laden der Ressource fehlgeschlagen");
    }

    return String(string_buffer);
}

//----------------------------------------------------------------------------

Bitmap* WinResourceLoader::LoadBitmap(uint id)
{
    WinBitmap* winBmp = new WinBitmap();

    if (!winBmp->LoadResource(id))
    {
        throw ResourceFailure(
            "Fehler in WinResourceLoader::LoadBitmap(uint): "
            "Laden der Ressource fehlgeschlagen");
    }

    return new Bitmap(winBmp);
}

//----------------------------------------------------------------------------

Menu* WinResourceLoader::LoadMenu(uint id)
{
    WinMenu* winMenu = new WinMenu();

    if (!winMenu->LoadResource(id))
    {
        throw ResourceFailure(
            "Fehler in WinResourceLoader::LoadMenu(uint): "
            "Laden der Ressource fehlgeschlagen");
    }

    return new Menu(winMenu);
}

//----------------------------------------------------------------------------
