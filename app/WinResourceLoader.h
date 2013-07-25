#ifndef app_WinResourceLoader_h
#define app_WinResourceLoader_h

#include "app/ResourceLoader.h"

//----------------------------------------------------------------------------

class WinResourceLoader: public ResourceLoader
{
public:

    String LoadString(uint id);
    Bitmap* LoadBitmap(uint id);
    Menu* LoadMenu(uint id);
};

//----------------------------------------------------------------------------

#endif
