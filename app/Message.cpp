#include "app/Message.h"

//----------------------------------------------------------------------------

void Message::DataInfo::Init(const char* name, type_code type,
                             const void* data, ulong size)
{
    this->name = new char[strlen(name) + 1];
    this->type = type;
    this->data = new char[size];
    this->size = size;

    strcpy(this->name, name);
    memcpy(this->data, data, size);
}

//----------------------------------------------------------------------------

Message::Message(uint what, ::Handler* handler)
    : what(what),
      m_handler(handler),
      m_infoList(4)
{
    m_infoList.SetAutoDelete(true);
}

//----------------------------------------------------------------------------

Message::Message(const Message& message)
    : what(message.what),
      m_handler(message.m_handler),
      m_infoList(message.m_infoList.Capacity())
{
    m_infoList.SetAutoDelete(true);

    for (uint i = 0; i < message.m_infoList.Count(); ++i)
        m_infoList.Append(new DataInfo(*message.m_infoList[i]));
}

//----------------------------------------------------------------------------

Message::~Message()
{
    Clear();
}

//----------------------------------------------------------------------------

uint Message::Locate(const char* name, uint pos) const
{
    uint end = m_infoList.End();

    while (pos != end)
    {
        if (strcmp(name, m_infoList[pos]->name) == 0)
            break;

        ++pos;
    }

    return pos;
}

//----------------------------------------------------------------------------

bool Message::ReplaceData(const char* name, type_code type, void* data,
                          ulong size)
{
    DataInfo* info = m_infoList.At(Locate(name));

    if (!info)
        return false;

    delete[] info->data;

    info->type = type;
    info->size = size;
    info->data = new char[size];
    memcpy(info->data, data, size);

    return true;
}

//----------------------------------------------------------------------------

bool Message::FindData(const char* name, type_code type, void** data,
                       ulong* size) const
{
    DataInfo* info = m_infoList.At(Locate(name));

    if (!info)
        return false;

    if (type != ANY_TYPE && type != info->type)
        return false;

    *data = info->data;

    if (size != nullptr)
        *size = info->size;

    return true;
}

//----------------------------------------------------------------------------

void Message::Clear()
{
    what = MSG_UNKNOWN;
    m_handler = nullptr;
    m_infoList.Clear();
}

//----------------------------------------------------------------------------

bool Message::RemoveName(const char* name)
{
    uint pos = Locate(name);

    if (pos == m_infoList.End())
        return false;

    while (pos != m_infoList.End())
    {
        m_infoList.Remove(pos);
        pos = Locate(name, pos);
    }

    return true;
}

//----------------------------------------------------------------------------
