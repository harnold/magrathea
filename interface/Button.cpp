#include "interface/Button.h"
#include "app/Keycodes.h"
#include "interface/Graphics.h"

//---------------------------------------------------------------------------

Button::Button(int id, Container* parent, const Rect& frame, String label,
               uint alignMode)
    : Control(id, parent, frame, label, alignMode),
      m_clicked(false) {}

//---------------------------------------------------------------------------

void Button::SetValue(int value)
{
    Control::SetValue(value);
    Invalidate();
}

//---------------------------------------------------------------------------

void Button::SetDefault(bool flag)
{
    Control::SetDefault(flag);
    Invalidate();
}

//---------------------------------------------------------------------------

void Button::Draw(Graphics& g)
{
    static Color highColor  = g.GetSystemColor(COL_HIGHLIGHT_3D);
    static Color grayColor  = g.GetSystemColor(COL_GRAY_TEXT);
    static Color textColor  = g.GetSystemColor(COL_BUTTON_TEXT);
    static Color faceColor  = g.GetSystemColor(COL_BUTTON_FACE);
    static Color lightColor = g.GetSystemColor(COL_BUTTON_HIGHLIGHT);
    static Color shadColor  = g.GetSystemColor(COL_BUTTON_SHADOW);

    g.SetFont(g.GetSystemFont());

    Rect r = Bounds();

    // Default-Rechteck zeichnen
    if (IsDefault())
        g.SetDrawColor(COLOR_BLACK);
    else
        g.SetDrawColor(faceColor);

    g.DrawRect(r);
    r.InflateBy(-1, -1);

    // Button nicht gedrueckt
    if (Value() == CONTROL_OFF)
    {
        // Schaltflaeche zeichnen
        g.SetDrawColor(COLOR_BLACK);
        g.DrawRect(r);
        r.InflateBy(-1, -1);

        g.SetFillColor(faceColor);
        g.SetDrawColor(lightColor);
        g.FillRect(r);

        g.SetDrawColor(shadColor);
        g.SetCursor(r.Left(), r.Bottom() - 1);
        g.DrawLine(r.Right() - 1 , r.Bottom() - 1);
        g.DrawLine(r.Right() - 1, r.Top());
        g.SetCursor(r.Left() + 1, r.Bottom() - 2);
        g.DrawLine(r.Right() - 2 , r.Bottom() - 2);
        g.DrawLine(r.Right() - 2, r.Top() + 1);

        // Text zeichnen
        if (IsEnabled())
        {
            // Button aktiv -> Text normal
            g.SetDrawColor(textColor);
            g.DrawText(r, Label(), SINGLELINE_TEXT | ALIGN_H_CENTER | ALIGN_V_CENTER);
        }
        else
        {
            // Button inaktiv -> "grayed" Text zeichnen
            g.SetDrawColor(highColor);
            r.OffsetBy(1, 1);
            g.DrawText(r, Label(), SINGLELINE_TEXT | ALIGN_H_CENTER | ALIGN_V_CENTER);

            g.SetDrawColor(grayColor);
            r.OffsetBy(-1, -1);
            g.DrawText(r, Label(), SINGLELINE_TEXT | ALIGN_H_CENTER | ALIGN_V_CENTER);
        }
    }

    // Button gedrueckt
    else
    {
        // Schaltflaeche zeichnen
        g.SetDrawColor(COLOR_BLACK);
        g.DrawRect(r);
        r.InflateBy(-1, -1);

        g.SetFillColor(faceColor);
        g.SetDrawColor(shadColor);
        g.FillRect(r);

        // Text zeichnen
        r.OffsetBy(1, 1);
        g.SetDrawColor(textColor);
        g.DrawText(r, Label(), SINGLELINE_TEXT | ALIGN_H_CENTER | ALIGN_V_CENTER);
        r.OffsetBy(-1, -1);
    }

    if (HasFocus())
    {
        r.InflateBy(-4, -4);
        g.SetDrawColor(textColor);
        g.DrawFocusRect(r);
    }
}

//---------------------------------------------------------------------------

void Button::KeyDown(int keyCode)
{
    if (keyCode == KEY_SPACE || keyCode == KEY_RETURN)
    {
        SetValue(CONTROL_ON);
        m_clicked = true;
        Invalidate();
    }
}

//---------------------------------------------------------------------------

void Button::KeyUp(int keyCode)
{
    if (keyCode == KEY_SPACE || keyCode == KEY_RETURN)
    {
        SetValue(CONTROL_OFF);
        Invalidate();
        Clicked(Id());
        m_clicked = false;
    }
}

//---------------------------------------------------------------------------

void Button::Enabled(bool enabled)
{
    Invalidate();
}

//---------------------------------------------------------------------------

void Button::MouseDown(const Point& point, int button, int keyState)
{
    SetValue(CONTROL_ON);
    m_clicked = true;
    Invalidate();
    CaptureMouse();
}

//---------------------------------------------------------------------------

void Button::MouseUp(const Point& point, int button, int keyState)
{
    bool signal = false;

    if (Value() == CONTROL_ON)
    {
        SetValue(CONTROL_OFF);
        Invalidate();
        signal = true;
    }

    m_clicked = false;
    ReleaseMouse();

    if (signal)
        Clicked(Id());
}

//---------------------------------------------------------------------------

void Button::MouseMoved(const Point& point, int keyState)
{
    Rect r = Bounds();

    if (r.Contains(point))
    {
        if (m_clicked && Value() != CONTROL_ON)
        {
            SetValue(CONTROL_ON);
            Invalidate();
        }
    }
    else if (Value() != CONTROL_OFF)
    {
        SetValue(CONTROL_OFF);
        Invalidate();
    }
}

//---------------------------------------------------------------------------

void Button::FocusChanged(bool focused)
{
    Invalidate();
}

//---------------------------------------------------------------------------
