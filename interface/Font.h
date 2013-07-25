#ifndef interface_Font_h
#define interface_Font_h

#include "support/String.h"

//----------------------------------------------------------------------------
//
//  Die Font-Struktur dient zur Verwaltung von Schriften (Fonts).
//
//  Font-Objekte sind passiv, sie beinhalten nur Strukturen zur Speicherung
//  der Fonteigenschaften.
//
struct Font
{
    //
    //  Standard-Fontfamilien
    //
    enum generic_family
    {
        DEFAULT_FAMILY,
        DECORATIVE,
        MODERN,
        ROMAN,
        SCRIPT,
        SWISS
    };

    //
    //  Standardwerte fuer die Zeichendicke (font weight). Die Werte muessen
    //  im Bereich 0..1000 liegen.
    //
    enum weight
    {
        DEFAULT_WEIGHT  = 0,
        LIGHT           = 100,
        NORMAL          = 400,
        BOLD            = 700,
        BLACK           = 900
    };

    //
    //  Mittlere Zeichenbreite.
    //
    enum width
    {
        DEFAULT_WIDTH   = 0
    };

    //
    //  Weitere Font-Eigenschaften
    //
    enum flags
    {
        ITALIC          = 1,
        UNDERLINE       = 2,
        STRIKE_OUT      = 4,
        FIXED_WIDTH     = 8
    };

    //
    //  Erstellt ein Font-Objekt mit den uebergebenen Eigenschaften.
    //
    Font(String name,
         int height,
         int width = DEFAULT_WIDTH,
         int weight = DEFAULT_WEIGHT,
         uint flags = 0,
         int escapement = 0,
         int orientation = 0);

    //
    //  Erstellt ein Font-Objekt mit den uebergebenen Eigenschaften.
    //
    Font(generic_family family,
         int height,
         int width = DEFAULT_WIDTH,
         int weight = DEFAULT_WEIGHT,
         uint flags = 0,
         int escapement = 0,
         int orientation = 0);

    generic_family  family;
    String          name;
    int             height;
    int             width;
    int             weight;
    uint            flags;
    int             escapement;
    int             orientation;
};

//----------------------------------------------------------------------------

#endif
