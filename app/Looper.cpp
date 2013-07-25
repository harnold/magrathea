#include "app/Looper.h"

//----------------------------------------------------------------------------

Looper::Looper(uint capacity)
    : m_defaultHandler(nullptr),
      m_currentMessage(nullptr),
      m_messageQueue(capacity)
{}

//----------------------------------------------------------------------------

Looper::~Looper()
{
    if (m_defaultHandler != nullptr)
        m_defaultHandler->SetLooper(nullptr);

    if (m_currentMessage != nullptr)
        delete m_currentMessage;

    m_handlers.DoForEach(UnlinkHandler);
}

//----------------------------------------------------------------------------

void Looper::AddHandler(Handler* handler)
{
    m_handlers.Append(handler);
    handler->SetLooper(this);
}

//----------------------------------------------------------------------------

bool Looper::RemoveHandler(Handler* handler)
{
    if (!m_handlers.RemoveRef(handler))
        return false;

    handler->SetLooper(nullptr);
    return true;
}

//----------------------------------------------------------------------------

void Looper::SetDefaultHandler(Handler* handler)
{
    if (handler->Looper() == this)
        m_defaultHandler = handler;
}

//----------------------------------------------------------------------------

bool Looper::PostMessage(Message* message, Handler* handler)
{
    if (m_messageQueue.IsFull())
        return false;

    Message* msg = new Message(*message);
    msg->SetHandler(handler);
    m_messageQueue.AddMessage(message);

    return true;
}

//----------------------------------------------------------------------------

bool Looper::PostMessage(uint what, Handler* handler)
{
    if (m_messageQueue.IsFull())
        return false;

    Message* msg = new Message(what, handler);
    m_messageQueue.AddMessage(msg);

    return true;
}

//----------------------------------------------------------------------------

void Looper::DispatchMessage(Message* message)
{
    Handler* handler = message->Handler();

    if (message->what = MSG_QUIT && handler == this)
    {
        if (QuitRequested())
            Quit();
    }
    else
    {
        if (handler != nullptr && handler->Looper() == this)
            handler->MessageReceived(message);
        else if (m_defaultHandler != nullptr)
            m_defaultHandler->MessageReceived(message);
        else
            MessageReceived(message);
    }
}

//----------------------------------------------------------------------------

bool Looper::QuitRequested()
{
    return true;
}

//----------------------------------------------------------------------------

void Looper::DispatchNextMessage()
{
    if (m_messageQueue.NextMessage())
    {
        m_currentMessage = m_messageQueue.GetNextMessage();
        DispatchMessage(m_currentMessage);
        delete m_currentMessage;
        m_currentMessage = nullptr;
    }
}

//----------------------------------------------------------------------------
