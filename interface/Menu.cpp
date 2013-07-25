#include "interface/Menu.h"
#include "interface/PlatformFactory.h"

//---------------------------------------------------------------------------

Menu::Menu()
    : m_platformMenu(ThePlatformFactory->CreatePlatformMenu())
{}

//---------------------------------------------------------------------------

Menu::~Menu()
{
    delete m_platformMenu;
}

//---------------------------------------------------------------------------
