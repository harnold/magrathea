#ifndef interface_PlatformMenu_h
#define interface_PlatformMenu_h

#include "interface/Point.h"
#include "support/String.h"

class PlatformWindow;

//----------------------------------------------------------------------------

class PlatformMenu
{
public:

    virtual void InsertItem(int index, String label, int command) = 0;
    virtual void InsertSubmenu(int index, String label,
                               const PlatformMenu* menu) = 0;
    virtual void InsertSeparator(int index) = 0;
    virtual void RemoveItem(int index) = 0;
    virtual bool IsEnabled(int index) const = 0;
    virtual void SetEnabled(int index, bool flag) = 0;
    virtual bool IsChecked(int index) const = 0;
    virtual void SetChecked(int index, bool flag) = 0;
    virtual void CheckRadioItem(int index, int first, int last) = 0;
    virtual void SetDefaultItem(int index) = 0;
    virtual int Track(PlatformWindow* window, const Point& point,
                      bool sendCommand) = 0;
};

//----------------------------------------------------------------------------

#endif
