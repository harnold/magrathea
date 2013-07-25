#include "support/Exception.h"

//----------------------------------------------------------------------------

void ThrowBadAlloc() throw(BadAlloc)
{
    throw BadAlloc("Speicherreservierung fehlgeschlagen");
}

//----------------------------------------------------------------------------
