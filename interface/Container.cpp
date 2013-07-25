#include "interface/Container.h"
#include "interface/Window.h"
#include "support/Utilities.h"

//----------------------------------------------------------------------------

Container::Container(Container* parent, const Rect& frame, uint alignMode)
    : View(parent, frame, alignMode),
      m_oldSize(frame.width, frame.height),
      m_window(nullptr)
{
    m_childList.SetAutoDelete(true);
}

//----------------------------------------------------------------------------

Container::Container()
{
    m_childList.SetAutoDelete(true);
}

//----------------------------------------------------------------------------

void Container::Init(PlatformWindow* platformWindow, Graphics* graphics,
                     Container* parent, const Rect& frame, uint alignMode)
{
    View::Init(platformWindow, graphics, parent, frame, alignMode);
    m_oldSize.Set(frame.width, frame.height);
    m_window = nullptr;
}

//----------------------------------------------------------------------------

Container::~Container() {}

//----------------------------------------------------------------------------

void Container::AddChild(View* view)
{
    if (view->Parent() != nullptr || view == Parent())
        return;

    view->SetParent(this);
    m_childList.Append(view);
}

//----------------------------------------------------------------------------

bool Container::RemoveChild(View* view)
{
    uint pos = m_childList.FindRef(view);

    if (pos == m_childList.End())
        return false;

    view->SetParent(nullptr);
    m_childList.Remove(pos);
    return true;
}

//----------------------------------------------------------------------------

void Container::FrameResized(const Size& size)
{
    int dx = size.width - m_oldSize.width;
    int dy = size.height - m_oldSize.height;

    uint count = CountChildren();

    for (int i = 0; i < count; ++i)
    {
        View* view = m_childList[i];

        if (view->Parent() == nullptr)
            continue;

        uint alignMode = view->AlignMode();
        Rect frame = Frame();

        if (TestBits(alignMode, ALIGN_LEFT_RIGHT))
            frame.ResizeBy(dx, 0);
        else if (TestBits(alignMode, ALIGN_RIGHT))
            frame.OffsetBy(dx, 0);

        if (TestBits(alignMode, ALIGN_TOP_BOTTOM))
            frame.ResizeBy(0, dy);
        else if (TestBits(alignMode, ALIGN_BOTTOM))
            frame.OffsetBy(0, dy);

        view->SetFrame(frame);
    }

    m_oldSize = size;
}

//----------------------------------------------------------------------------

void Container::AttachedToWindow()
{
    View::AttachedToWindow();

    ::Window* window = Window();
    uint count = CountChildren();

    for (int i = 0; i < count; ++i)
    {
        View* view = m_childList[i];
        view->SetWindow(window);
        view->AttachedToWindow();
    }

    AllAttached();
}

//----------------------------------------------------------------------------

void Container::DetachedFromWindow()
{
    View::DetachedFromWindow();

    uint count = CountChildren();

    for (int i = 0; i < count; ++i)
        m_childList[i]->DetachedFromWindow();

    AllDetached();

    for (int i = 0; i < count; ++i)
        m_childList[i]->SetWindow(nullptr);
}

//----------------------------------------------------------------------------

void Container::AllAttached() {}
void Container::AllDetached() {}

//----------------------------------------------------------------------------
