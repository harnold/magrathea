#include "app/MessageQueue.h"
#include "app/Message.h"

//----------------------------------------------------------------------------

MessageQueue::MessageQueue(uint capacity)
    : m_capacity(Max(capacity, MINIMAL_CAPACITY)),
      m_first(0),
      m_count(0),
      m_queue(new Message*[capacity])
{
    for (int i = 0; i < m_capacity; ++i)
        m_queue[i] = nullptr;
}

//----------------------------------------------------------------------------

MessageQueue::~MessageQueue()
{
    for (int i = 0; i < m_capacity; ++i)
        delete m_queue[i];

    delete[] m_queue;
}

//----------------------------------------------------------------------------

void MessageQueue::AddMessage(Message* message)
{
    if (m_count < m_capacity)
        m_queue[(m_first + m_count) % m_capacity] = message;

    ++m_count;
}

//----------------------------------------------------------------------------

Message* MessageQueue::GetNextMessage()
{
    if (m_count == 0)
        return nullptr;

    Message* msg = m_queue[m_first];
    m_queue[m_first] = nullptr;
    m_first = (m_first + 1) % m_capacity;
    --m_count;

    return msg;
}

//----------------------------------------------------------------------------
