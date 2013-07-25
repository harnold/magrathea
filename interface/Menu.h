#ifndef interface_Menu_h
#define interface_Menu_h

#include "interface/PlatformMenu.h"
#include "interface/Point.h"
#include "interface/View.h"
#include "support/String.h"

//---------------------------------------------------------------------------

class Menu
{
public:

    Menu();
    Menu(PlatformMenu* platformMenu)
        : m_platformMenu(platformMenu) {}

    virtual ~Menu();

    void InsertItem(int index, String label, int command)
    { m_platformMenu->InsertItem(index, label, command); }

    void InsertSubmenu(int index, String label, const Menu* menu)
    { m_platformMenu->InsertSubmenu(index, label, menu->m_platformMenu); }

    void InsertSeparator(int index)
    { m_platformMenu->InsertSeparator(index); }

    void RemoveItem(int index)
    { m_platformMenu->RemoveItem(index); }

    bool IsEnabled(int index) const
    { return m_platformMenu->IsEnabled(index); }

    void SetEnabled(int index, bool flag = true)
    { m_platformMenu->SetEnabled(index, flag); }

    bool IsChecked(int index) const
    { return m_platformMenu->IsChecked(index); }

    void SetChecked(int index, bool flag = true)
    { m_platformMenu->SetChecked(index, flag); }

    void CheckRadioItem(int index, int first, int last)
    { m_platformMenu->CheckRadioItem(index, first, last); }

    void SetDefaultItem(int index)
    { m_platformMenu->SetDefaultItem(index); }

    int Track(View* view, const Point& point, bool sendCommand)
    { return m_platformMenu->Track(view->GetPlatformWindow(), point,
                                   sendCommand); }

    PlatformMenu* GetPlatformMenu() const
    { return m_platformMenu; }

protected:

    explicit Menu(int dummy) {}

    void Init(PlatformMenu* platformMenu)
    { m_platformMenu = platformMenu; }

private:

    PlatformMenu* m_platformMenu;
};

//---------------------------------------------------------------------------

#endif
