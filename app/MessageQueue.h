#ifndef app_MessageQueue_h
#define app_MessageQueue_h

#include "support/Utilities.h"

class Message;

//----------------------------------------------------------------------------

class MessageQueue
{
public:

    static const uint MINIMAL_CAPACITY = 1;

    MessageQueue(uint capacity);

    ~MessageQueue();

    uint Capacity() const
    { return m_capacity; }

    uint Count() const
    { return m_count; }

    bool IsEmpty() const
    { return m_count == 0; }

    bool IsFull() const
    { return m_count >= m_capacity; }

    void AddMessage(Message* message);

    Message* NextMessage() const
    { return m_queue[m_first]; }

    Message* GetNextMessage();

private:

    uint        m_capacity;
    uint        m_first;
    uint        m_count;
    Message**   m_queue;
};

//----------------------------------------------------------------------------

#endif
