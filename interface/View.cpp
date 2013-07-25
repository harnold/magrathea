#include "interface/View.h"
#include "app/Application.h"
#include "app/Message.h"
#include "interface/Container.h"
#include "interface/Graphics.h"
#include "interface/PlatformFactory.h"

//----------------------------------------------------------------------------

View::View(Container* parent, const Rect& frame, uint alignMode)
    : m_graphics(new Graphics()),
      m_parent(parent),
      m_alignMode(alignMode),
      m_window(nullptr)
{
    if (parent != nullptr)
    {
        m_platformWindow =
            ThePlatformFactory->CreatePlatformView(
                parent->GetPlatformWindow(), frame, alignMode, this);

        parent->AddChild(this);
    }
    else
    {
        m_platformWindow =
            ThePlatformFactory->CreatePlatformView(
                nullptr, frame, alignMode, this);
    }

    m_frame = m_platformWindow->Frame();

    TheApp->AddHandler(this);
}

//----------------------------------------------------------------------------

void View::Init(PlatformWindow* platformWindow, Graphics* graphics,
                Container* parent, const Rect& frame, uint alignMode)
{
    m_platformWindow = platformWindow;
    m_graphics = graphics;
    m_parent = parent;
    m_frame = frame;
    m_alignMode = alignMode;
    m_window = nullptr;

    TheApp->AddHandler(this);
}

//----------------------------------------------------------------------------

View::~View()
{
    TheApp->RemoveHandler(this);

    RemoveSelf();
    delete m_platformWindow;
    delete m_graphics;
}

//----------------------------------------------------------------------------

void View::MoveBy(int dx, int dy)
{
    Rect r = Frame();
    r.OffsetBy(dx, dy);
    SetFrame(r);
}

//----------------------------------------------------------------------------

void View::MoveTo(int x, int y)
{
    Rect r = Frame();
    r.OffsetTo(x, y);
    SetFrame(r);
}

//----------------------------------------------------------------------------

void View::ResizeBy(int dx, int dy)
{
    Rect r = Frame();
    r.ResizeBy(dx, dy);
    SetFrame(r);
}

//----------------------------------------------------------------------------

void View::ResizeTo(int w, int h)
{
    Rect r = Frame();
    r.ResizeTo(w, h);
    SetFrame(r);
}

//----------------------------------------------------------------------------

void View::SetParent(Container* parent)
{
    m_parent = parent;

    if (parent != nullptr)
        m_platformWindow->SetParent(parent->GetPlatformWindow());
    else
        m_platformWindow->SetParent(nullptr);
}

//----------------------------------------------------------------------------

void View::SetWindow(::Window* window)
{
    m_window = window;
}

//----------------------------------------------------------------------------

bool View::RemoveSelf()
{
    if (m_parent == nullptr)
        return false;

    m_parent->RemoveChild(this);
    m_parent = nullptr;
    m_platformWindow->SetParent(nullptr);
    return true;
}

//----------------------------------------------------------------------------

void View::ConvertToScreen(Rect* rect) const
{
    Point p1 = rect->TopLeft();
    Point p2 = rect->BottomRight();

    m_platformWindow->ConvertToScreen(&p1);
    m_platformWindow->ConvertToScreen(&p2);

    rect->Set(p1.x, p1.y, p2.x, p2.y);
}

//----------------------------------------------------------------------------

void View::ConvertFromScreen(Rect* rect) const
{
    Point p1 = rect->TopLeft();
    Point p2 = rect->BottomRight();

    m_platformWindow->ConvertFromScreen(&p1);
    m_platformWindow->ConvertFromScreen(&p2);

    rect->Set(p1.x, p1.y, p2.x, p2.y);
}

//----------------------------------------------------------------------------

void View::ConvertToParent(Point* point) const
{
    if (m_parent != nullptr)
    {
        ConvertToScreen(point);
        m_parent->ConvertFromScreen(point);
    }
}

//----------------------------------------------------------------------------

void View::ConvertToParent(Rect* rect) const
{
    if (m_parent != nullptr)
    {
        ConvertToScreen(rect);
        m_parent->ConvertFromScreen(rect);
    }
}

//----------------------------------------------------------------------------

void View::ConvertFromParent(Point* point) const
{
    if (m_parent != nullptr)
    {
        m_parent->ConvertToScreen(point);
        ConvertFromScreen(point);
    }
}

//----------------------------------------------------------------------------

void View::ConvertFromParent(Rect* rect) const
{
    if (m_parent != nullptr)
    {
        m_parent->ConvertToScreen(rect);
        ConvertFromScreen(rect);
    }
}

//----------------------------------------------------------------------------

Margins View::GetFrameMargins() const
{
    return Margins(0, 0, 0, 0);
}

//----------------------------------------------------------------------------

void View::MessageReceived(Message* message)
{
    bool flag;
    int key, id;
    Size size;
    Point point;

    switch (message->what)
    {
    case MSG_VIEW_DRAW:
        m_graphics->Begin(this);
        Draw(*m_graphics);
        m_graphics->End();
        break;

    case MSG_KEY_DOWN:
        message->FindInt("key", &key);
        KeyDown(key);
        break;

    case MSG_KEY_UP:
        message->FindInt("key", &key);
        KeyUp(key);
        break;

    case MSG_KEY_PRESSED:
        message->FindInt("char", &key);
        KeyPressed(key);
        break;

    case MSG_MOUSE_DOWN:
        message->FindInt("xpos", &point.x);
        message->FindInt("ypos", &point.y);
        message->FindInt("keys", &key);
        message->FindInt("button", &id);
        MouseDown(point, id, key);
        break;

    case MSG_MOUSE_UP:
        message->FindInt("xpos", &point.x);
        message->FindInt("ypos", &point.y);
        message->FindInt("keys", &key);
        message->FindInt("button", &id);
        MouseUp(point, id, key);
        break;

    case MSG_MOUSE_MOVED:
        message->FindInt("xpos", &point.x);
        message->FindInt("ypos", &point.y);
        message->FindInt("keys", &key);
        MouseMoved(point, key);
        break;

    case MSG_MOUSE_DOUBLECLICK:
        message->FindInt("xpos", &point.x);
        message->FindInt("ypos", &point.y);
        message->FindInt("keys", &key);
        message->FindInt("button", &id);
        MouseDoubleClick(point, id, key);
        break;

    case MSG_PULSE:
        message->FindInt("timer", &id);
        Pulse(id);
        break;

    case MSG_VIEW_FOCUS_CHANGED:
        message->FindBool("focused", &flag);
        FocusChanged(flag);
        break;

    case MSG_VIEW_RESIZED:
        message->FindInt("width", &size.width);
        message->FindInt("height", &size.height);
        m_frame.ResizeTo(size);
        FrameResized(size);
        break;

    case MSG_VIEW_MOVED:
        message->FindInt("xpos", &point.x);
        message->FindInt("ypos", &point.y);
        m_frame.OffsetTo(point);
        FrameMoved(point);
        break;

    case MSG_VIEW_VISIBILITY_CHANGED:
        message->FindBool("show", &flag);
        VisibilityChanged(flag);
        break;

    case MSG_VIEW_ENABLED:
        message->FindBool("enabled", &flag);
        Enabled(flag);
        break;

    case MSG_COMMAND:
        message->FindInt("cmd", &id);
        ProcessCommand(id);
        break;

    default:
        Handler::MessageReceived(message);
    }
}

//----------------------------------------------------------------------------

void View::Draw(Graphics& g)
{
    static Color faceColor = g.GetSystemColor(COL_BUTTON_FACE);

    Rect rect(0, 0, Width(), Height());

    g.SetDrawColor(faceColor);
    g.SetFillColor(faceColor);
    g.FillRect(rect);
}

//----------------------------------------------------------------------------

void View::KeyPressed(int charCode) {}
void View::KeyDown(int keyCode) {}
void View::KeyUp(int keyCode) {}
void View::MouseDown(const Point& point, int button, int keyState) {}
void View::MouseUp(const Point& point, int button, int keyState) {}
void View::MouseMoved(const Point& point, int keyState) {}
void View::MouseDoubleClick(const Point& point, int button, int keyState) {}
void View::Pulse(int timerId) {}
void View::FocusChanged(bool focused) {}
void View::FrameResized(const Size& size) {}
void View::FrameMoved(const Point& topleft) {}
void View::VisibilityChanged(bool visible) {}
void View::Enabled(bool enabled) {}
void View::ProcessCommand(int commandId) {}
void View::AttachedToWindow() {}
void View::DetachedFromWindow() {}

//----------------------------------------------------------------------------
