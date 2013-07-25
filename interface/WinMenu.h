#ifndef interface_WinMenu_h
#define interface_WinMenu_h

#include "interface/PlatformMenu.h"
#include "platform/Win.h"

//----------------------------------------------------------------------------

class WinMenu: public PlatformMenu
{
public:

    enum { POPUP_MENU, MENU_BAR };

    WinMenu();
    WinMenu(uint type);

    virtual ~WinMenu();

    HMENU Handle() const
    { return m_hMenu; }

    bool LoadResource(uint id);

    void InsertItem(int index, String label, int command);
    void InsertSubmenu(int index, String label, const PlatformMenu* menu);
    void InsertSeparator(int index);
    void RemoveItem(int index);
    bool IsEnabled(int index) const;
    void SetEnabled(int index, bool flag);
    bool IsChecked(int index) const;
    void SetChecked(int index, bool flag);
    void SetDefaultItem(int index);
    void CheckRadioItem(int index, int first, int last);
    int Track(PlatformWindow* window, const Point& point, bool sendCommand);

private:

    HMENU m_hMenu;
};

//----------------------------------------------------------------------------

#endif
