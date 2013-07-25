#include "interface/MenuBar.h"
#include "interface/PlatformFactory.h"

//---------------------------------------------------------------------------

MenuBar::MenuBar()
    : Menu(1)
{
    Menu::Init(ThePlatformFactory->CreatePlatformMenuBar());
}

//---------------------------------------------------------------------------
