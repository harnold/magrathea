#include "interface/WinMenu.h"
#include "app/WinApp.h"
#include "interface/WinWindow.h"
#include "support/Exception.h"

//---------------------------------------------------------------------------

WinMenu::WinMenu()
    : m_hMenu(0) {}

//---------------------------------------------------------------------------

WinMenu::WinMenu(uint type)
{
    if (type == POPUP_MENU)
        m_hMenu = ::CreatePopupMenu();
    else
        m_hMenu = ::CreateMenu();

    if (!m_hMenu)
    {
        throw CreationFailed(
            "Fehler in WinMenu::WinMenu(uint): "
            "Menue konnte nicht erstellt werden");
    }
}

//---------------------------------------------------------------------------

WinMenu::~WinMenu()
{
    if (m_hMenu != 0)
        ::DestroyMenu(m_hMenu);
}

//---------------------------------------------------------------------------

void WinMenu::InsertItem(int index, String label, int command)
{
    MENUITEMINFO info;

    info.cbSize = sizeof(MENUITEMINFO);
    info.fMask = MIIM_DATA | MIIM_ID | MIIM_STATE | MIIM_TYPE;
    info.fType = MFT_STRING;
    info.fState = MFS_ENABLED | MFS_UNCHECKED;
    info.wID = command;
    info.hSubMenu = 0;
    info.hbmpChecked = 0;
    info.hbmpUnchecked = 0;
    info.dwItemData = 0;
    info.dwTypeData = (LPTSTR) label.CStr();
    info.cch = label.Length();

    ::InsertMenuItem(m_hMenu, index, TRUE, &info) != 0;
}

//---------------------------------------------------------------------------

void WinMenu::InsertSubmenu(int index, String label, const PlatformMenu* menu)
{
    MENUITEMINFO info;

    info.cbSize = sizeof(MENUITEMINFO);
    info.fMask = MIIM_DATA | MIIM_ID | MIIM_STATE | MIIM_TYPE | MIIM_SUBMENU;
    info.fType = MFT_STRING;
    info.fState = MFS_ENABLED | MFS_UNCHECKED;
    info.wID = 0;
    info.hSubMenu = static_cast<const WinMenu*>(menu)->m_hMenu;
    info.hbmpChecked = 0;
    info.hbmpUnchecked = 0;
    info.dwItemData = 0;
    info.dwTypeData = (LPTSTR) label.CStr();
    info.cch = label.Length();

    ::InsertMenuItem(m_hMenu, index, TRUE, &info);
}

//---------------------------------------------------------------------------

void WinMenu::InsertSeparator(int index)
{
    MENUITEMINFO info;

    info.cbSize = sizeof(MENUITEMINFO);
    info.fMask = MIIM_DATA | MIIM_ID | MIIM_STATE | MIIM_TYPE;
    info.fType = MFT_SEPARATOR;
    info.fState = MFS_ENABLED | MFS_UNCHECKED;
    info.wID = 0;
    info.hSubMenu = 0;
    info.hbmpChecked = 0;
    info.hbmpUnchecked = 0;
    info.dwItemData = 0;
    info.dwTypeData = 0;
    info.cch = 0;

    ::InsertMenuItem(m_hMenu, index, TRUE, &info) != 0;
}

//---------------------------------------------------------------------------

void WinMenu::RemoveItem(int index)
{
    ::RemoveMenu(m_hMenu, index, MF_BYPOSITION);
}

//---------------------------------------------------------------------------

bool WinMenu::IsEnabled(int index) const
{
    uint state = ::GetMenuState(m_hMenu, index, MF_BYPOSITION);
    return TestBits(state, MF_GRAYED) || TestBits(state, MF_DISABLED);
}

//---------------------------------------------------------------------------

void WinMenu::SetEnabled(int index, bool flag)
{
    uint state = flag ? MF_ENABLED : MF_GRAYED;
    ::EnableMenuItem(m_hMenu, index, MF_BYPOSITION | state);
}

//---------------------------------------------------------------------------

bool WinMenu::IsChecked(int index) const
{
    uint state = ::GetMenuState(m_hMenu, index, MF_BYPOSITION);
    return TestBits(state, MF_CHECKED);
}

//---------------------------------------------------------------------------

void WinMenu::SetChecked(int index, bool flag)
{
    uint state = flag ? MF_CHECKED : MF_UNCHECKED;
    ::CheckMenuItem(m_hMenu, index, MF_BYPOSITION | state);
}

//---------------------------------------------------------------------------

void WinMenu::CheckRadioItem(int index, int first, int last)
{
    ::CheckMenuRadioItem(m_hMenu, first, last, index, MF_BYPOSITION);
}

//---------------------------------------------------------------------------

void WinMenu::SetDefaultItem(int index)
{
    MENUITEMINFO info;

    info.cbSize = sizeof(MENUITEMINFO);
    info.fMask  = MIIM_STATE;

    ::GetMenuItemInfo(m_hMenu, index, TRUE, &info);
    info.fState |= MFS_DEFAULT;
    ::SetMenuItemInfo(m_hMenu, index, TRUE, &info);
}

//---------------------------------------------------------------------------

int WinMenu::Track(PlatformWindow* window, const Point& point,
                   bool sendCommand)
{
    uint flags = sendCommand ? 0 : (TPM_NONOTIFY | TPM_RETURNCMD);

    Point screenPos = point;
    window->ConvertToScreen(&screenPos);

    return ::TrackPopupMenu(
        m_hMenu,
        TPM_LEFTALIGN | TPM_TOPALIGN | TPM_LEFTBUTTON | TPM_RIGHTBUTTON | flags,
        point.x, point.y, 0, static_cast<WinWindow*>(window)->Handle(), 0);
}

//---------------------------------------------------------------------------

bool WinMenu::LoadResource(uint id)
{
    if (m_hMenu != 0)
        ::DestroyMenu(m_hMenu);

    m_hMenu = ::LoadMenuA(TheWinApp->Handle(), MAKEINTRESOURCE(id));

    return m_hMenu != 0;
}

//---------------------------------------------------------------------------
