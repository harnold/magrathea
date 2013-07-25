#include "support/String.h"

//----------------------------------------------------------------------------

char _string_buffer[256];

//----------------------------------------------------------------------------

const String String::Empty = String(0, "");

//----------------------------------------------------------------------------

String String::Concat(const char* str1, uint len1,
                      const char* str2, uint len2)
{
    uint new_length = len1 + len2;
    String new_string(new_length);
    char* buf = new_string.m_body->data;

    strncpy(buf, str1, len1);
    strncpy(buf + len1, str2, len2);
    new_string.m_body->length = new_length;

    return new_string;
}

//----------------------------------------------------------------------------

String& String::Append(const char* str, uint len)
{
    uint new_length = m_body->length + len;

    if (new_length > m_body->capacity)
        AssignBody(new StringBody(new_length, m_body->data));
    else
        Single();

    strncpy(m_body->data + m_body->length, str, len);
    m_body->length = new_length;

    return *this;
}

//----------------------------------------------------------------------------

void String::Shrink()
{
    if (m_body->capacity == m_body->length)
        return;

    AssignBody(new StringBody(m_body->length, m_body->data));
}

//----------------------------------------------------------------------------

String& String::operator=(const String& str)
{
    if (this != &str)
    {
        AssignBody(str.m_body);
        m_body->AddRef();
    }

    return *this;
}

//----------------------------------------------------------------------------

String& String::operator=(const char* str)
{
    AssignBody(new StringBody(str));
    return *this;
}

//----------------------------------------------------------------------------

String String::Left(uint count) const
{
    String new_str(count);

    strncpy(new_str.m_body->data, m_body->data, count);
    new_str.m_body->data[count] = '\0';
    new_str.m_body->length = count;

    return new_str;
}

//----------------------------------------------------------------------------

String String::Right(uint count) const
{
    String new_str(count);

    strncpy(new_str.m_body->data, &m_body->data[m_body->length - count], count);
    new_str.m_body->data[count] = '\0';
    new_str.m_body->length = count;

    return new_str;
}

//----------------------------------------------------------------------------

String String::Mid(uint first, uint count) const
{
    String new_str(count);

    strncpy(new_str.m_body->data, &m_body->data[first], count);
    new_str.m_body->data[count] = 0;
    new_str.m_body->length = count;

    return new_str;
}

//----------------------------------------------------------------------------

void String::Reserve(uint size)
{
    if (size > m_body->capacity)
        AssignBody(new StringBody(size, m_body->data));
}

//----------------------------------------------------------------------------
