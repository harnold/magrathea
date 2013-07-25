#include "interface/Control.h"
#include "interface/Constants.h"

//----------------------------------------------------------------------------

Control::Control(int id, Container* parent, const Rect& frame, String label,
                 uint alignMode)
    : View(parent, frame, alignMode),
      m_id(id),
      m_value(CONTROL_OFF),
      m_label(label)
{}

//----------------------------------------------------------------------------

void Control::SetValue(int value)
{
    if (m_value != value)
    {
        m_value = value;
        ValueChanged(m_id);
    }
}

//----------------------------------------------------------------------------

void Control::SetLabel(String label)
{
    m_label = label;
}

//----------------------------------------------------------------------------

void Control::SetDefault(bool flag)
{
    m_isDefault = flag;
}

//----------------------------------------------------------------------------
