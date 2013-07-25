#include "interface/Font.h"

//----------------------------------------------------------------------------

Font::Font(String name, int height, int width, int weight, uint flags,
           int escapement, int orientation)
    : family(DEFAULT_FAMILY),
      name(name),
      height(height),
      width(width),
      weight(weight),
      flags(flags),
      escapement(escapement),
      orientation(orientation)
{}

//----------------------------------------------------------------------------

Font::Font(generic_family family, int height, int width, int weight, uint flags,
           int escapement, int orientation)
    : family(family),
      name(""),
      height(height),
      width(width),
      weight(weight),
      flags(flags),
      escapement(escapement),
      orientation(orientation)
{}

//----------------------------------------------------------------------------
