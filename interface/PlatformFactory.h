#ifndef interface_PlatformFactory_h
#define interface_PlatformFactory_h

#include "interface/Constants.h"
#include "interface/Rect.h"
#include "support/String.h"
#include "support/Utilities.h"

class PlatformBitmap;
class PlatformImage;
class PlatformGraphics;
class PlatformMenu;
class PlatformWindow;
class Handler;

//----------------------------------------------------------------------------

class PlatformFactory
{
public:

    virtual PlatformGraphics* CreatePlatformGraphics(
        PlatformWindow* window) = 0;

    virtual PlatformGraphics* CreatePlatformGraphics(
        const PlatformGraphics* g, PlatformBitmap* bitmap) = 0;

    virtual PlatformBitmap* CreatePlatformBitmap(
        const PlatformGraphics* g, int width, int height, int bpp) = 0;

    virtual PlatformImage* CreatePlatformImage(
        int width, int height, int bpp) = 0;

    virtual PlatformWindow* CreatePlatformView(
        PlatformWindow* parent, const Rect& frame, uint alignMode,
        Handler* handler) = 0;

    virtual PlatformWindow* CreatePlatformWindow(
        const Rect& frame, String title, window_type type, uint flags,
        Handler* handler) = 0;

    virtual PlatformMenu* CreatePlatformMenu() = 0;

    virtual PlatformMenu* CreatePlatformMenuBar() = 0;
};

//----------------------------------------------------------------------------

PlatformFactory* ThePlatformFactory;

//----------------------------------------------------------------------------

#endif
