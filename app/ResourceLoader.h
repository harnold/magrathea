#ifndef app_ResourceLoader_h
#define app_ResourceLoader_h

#include "support/Exception.h"
#include "support/String.h"
#include "support/Utilities.h"

class Bitmap;
class Menu;

//---------------------------------------------------------------------------

class ResourceFailure: public Exception
{
public:

    enum { CODE = 10 };

    ResourceFailure(const char* message)
        : Exception(message, CODE) {}
};

//---------------------------------------------------------------------------

class ResourceLoader
{
public:

    virtual String LoadString(uint id);
    virtual Bitmap* LoadBitmap(uint id);
    virtual Menu* LoadMenu(uint id);
};

//---------------------------------------------------------------------------

#endif
