#include "support/StringBody.h"
#include "support/Utilities.h"

#include <cstring>

//----------------------------------------------------------------------------

StringBody::StringBody(const char* str)
    : refCount(1),
      length(strnlen(str, MAX_CAPACITY))
{
    capacity = length > 0 ? length : DEFAULT_CAPACITY;
    data = new char[capacity + 1];
    strncpy(data, str, capacity);
    data[capacity] = '\0';
}

//----------------------------------------------------------------------------

StringBody::StringBody(uint capacity, const char* str)
    : refCount(1),
      capacity(capacity),
      length(strnlen(str, capacity))
{
    data = new char[capacity + 1];
    strncpy(data, str, capacity);
    data[capacity] = '\0';
}

//----------------------------------------------------------------------------

StringBody::StringBody(char* data, uint capacity)
    : refCount(1),
      capacity(capacity),
      data(data)
{
    length = strnlen(data, capacity);
    data[length] = '\0';
}

//----------------------------------------------------------------------------
