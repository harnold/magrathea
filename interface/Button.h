#ifndef interface_Button_h
#define interface_Button_h

#include "interface/Control.h"
#include "support/Signal.h"

//---------------------------------------------------------------------------
//
//  Die Button-Klasse stellt ein Pushbutton-Control bereit.
//
class Button: public Control
{
    friend class Window;

public:

    Button(int id, Container* parent, const Rect& frame, String label,
           uint alignMode = ALIGN_NONE);

    Signal<int> Clicked;

    void SetValue(int value);
    void SetDefault(bool flag = true);
    void Draw(Graphics& g);
    void KeyDown(int keyCode);
    void KeyUp(int keyCode);
    void Enabled(bool enabled);
    void MouseDown(const Point& point, int button, int keyState);
    void MouseUp(const Point& point, int button, int keyState);
    void MouseMoved(const Point& point, int keyState);
    void FocusChanged(bool focused = true);

private:

    bool m_clicked;
};

//---------------------------------------------------------------------------

#endif
