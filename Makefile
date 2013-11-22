CFLAGS := -I .

CXX := i686-w64-mingw32-c++
DEP := $(CXX) -MM

DIRS := app interface support

OBJS := app/Application.o \
	app/Handler.o \
	app/Looper.o \
	app/Message.o \
	app/MessageQueue.o \
	app/ResourceLoader.o \
	app/WinApp.o \
	app/WinResourceLoader.o \
	interface/Bitmap.o \
	interface/Button.o \
	interface/Container.o \
	interface/Control.o \
	interface/Font.o \
	interface/Graphics.o \
	interface/Image.o \
	interface/Menu.o \
	interface/MenuBar.o \
	interface/Point.o \
	interface/PlatformBitmap.o \
	interface/PlatformImage.o \
	interface/Rect.o \
	interface/View.o \
	interface/WinBitmap.o \
	interface/WinBitmapDC.o \
	interface/WinFactory.o \
	interface/WinGraphics.o \
	interface/WinImage.o \
	interface/WinMenu.o \
	interface/WinWindow.o \
	interface/WinWindowDC.o \
	interface/WinWindowPaintDC.o \
	interface/Window.o \
	support/Exception.o \
	support/StringBody.o \
	support/String.o

.PHONY: all
all: $(OBJS)

-include $(OBJS:.o=.d)

%.o: %.cpp
	$(CXX) -c $(CFLAGS) -o $@ $<
	@$(DEP) $(CFLAGS) $< > $*.d
	@mv -f $*.d $*.d.tmp
	@sed -e 's|.*:|$*.o:|' < $*.d.tmp > $*.d
	@rm -f $*.d.tmp

.PHONY: clean
clean:
	for dir in $(DIRS); do \
	    rm -f $$dir/*.o $$dir/*.d $$dir/*.d.tmp; \
	done
