#include "app/ResourceLoader.h"

//----------------------------------------------------------------------------

String ResourceLoader::LoadString(uint id)
{
    throw InvalidOperation(
        "Fehler in ResourceLoader::LoadString(uint): "
        "Funktion wird von dieser ResourceLoader-Klasse nicht unterstuetzt");
}

//----------------------------------------------------------------------------

Bitmap* ResourceLoader::LoadBitmap(uint id)
{
    throw InvalidOperation(
        "Fehler in ResourceLoader::LoadBitmap(uint): "
        "Funktion wird von dieser ResourceLoader-Klasse nicht unterstuetzt");
}

//----------------------------------------------------------------------------

Menu* ResourceLoader::LoadMenu(uint id)
{
    throw InvalidOperation(
        "Fehler in ResourceLoader::LoadMenu(uint): "
        "Funktion wird von dieser ResourceLoader-Klasse nicht unterstuetzt");
}

//----------------------------------------------------------------------------
