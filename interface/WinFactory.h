#ifndef interface_WinFactory_h
#define interface_WinFactory_h

#include "interface/PlatformFactory.h"

//----------------------------------------------------------------------------

class WinFactory: public PlatformFactory
{
public:

    PlatformGraphics* CreatePlatformGraphics(
        PlatformWindow* window);

    PlatformGraphics* CreatePlatformGraphics(
        const PlatformGraphics* g, PlatformBitmap* bitmap);

    PlatformBitmap* CreatePlatformBitmap(
        const PlatformGraphics* g, int width, int height, int bpp);

    PlatformImage* CreatePlatformImage(
        int width, int height, int bpp);

    PlatformWindow* CreatePlatformView(
        PlatformWindow* parent, const Rect& frame, uint alignMode,
        Handler* handler);

    PlatformWindow* CreatePlatformWindow(
        const Rect& frame, String title, window_type type, uint flags,
        Handler* handler);

    PlatformMenu* CreatePlatformMenu();

    PlatformMenu* CreatePlatformMenuBar();
};

//----------------------------------------------------------------------------

#endif
