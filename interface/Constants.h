#ifndef interface_Constants_h
#define interface_Constants_h

#include "interface/Color.h"

//----------------------------------------------------------------------------
//
//  Standard-Farbwerte
//
#define COLOR_BLACK     (Color(0, 0, 0))
#define COLOR_RED       (Color(255, 0, 0))
#define COLOR_GREEN     (Color(0, 255, 0))
#define COLOR_BLUE      (Color(0, 0, 255))
#define COLOR_YELLOW    (Color(255, 255, 0))
#define COLOR_CYAN      (Color(0, 255, 255))
#define COLOR_MAGENTA   (Color(255, 0, 255))
#define COLOR_WHITE     (Color(255, 255, 255))

//----------------------------------------------------------------------------
//
//  Konstanten fuer die Ausrichtung von Views
//
enum
{
    ALIGN_LEFT          = 0x0001,   // links ausrichten
    ALIGN_RIGHT         = 0x0002,   // rechts ausrichten
    ALIGN_LEFT_RIGHT    = 0x0003,   // links und rechts ausrichten
    ALIGN_H_CENTER      = 0x0004,   // horizontal zentrieren
    ALIGN_TOP           = 0x0008,   // oben ausrichten
    ALIGN_BOTTOM        = 0x0010,   // unten ausrichten
    ALIGN_TOP_BOTTOM    = 0x0018,   // oben und unten ausrichten
    ALIGN_V_CENTER      = 0x0020,   // vertikal zentrieren
    ALIGN_NONE          = 0x0009,   // links oben ausrichten
    ALIGN_ALL           = 0x001B    // an allen Seiten ausrichten
};

//----------------------------------------------------------------------------
//
//  moegliche Zeichenmodi eines Graphics-Objektes.
//
enum drawing_mode
{
    OP_BLACK,           // =  0
    OP_WHITE,           // =  1
    OP_COPY_SRC,        // =  SRC
    OP_COPY_DEST,       // =  DEST
    OP_COPY_NOTSRC,     // = !SRC
    OP_COPY_NOTDEST,    // = !DEST
    OP_MASK_NOTSRC,     // =  DEST & !SRC
    OP_MASK_SRC,        // =  DEST &  SRC
    OP_MASK_NOTDEST,    // = !DEST &  SRC
    OP_MERGE_NOTSRC,    // =  DEST | !SRC
    OP_MERGE_SRC,       // =  DEST |  SRC
    OP_MERGE_NOTDEST,   // = !DEST |  SRC
    OP_XOR,             // =  DEST ^  SRC
    OP_NOT_MASK,        // = !(DEST & SRC)
    OP_NOT_MERGE,       // = !(DEST | SRC)
    OP_NOT_XOR          // = !(DEST ^ SRC)
};

//----------------------------------------------------------------------------
//
//  Konstanten zur Ermittlung der Systemfarben mit GetSystemColor()
//
enum system_color
{
    COL_DARK_SHADOW,
    COL_FACE_3D,
    COL_BUTTON_FACE,
    COL_HIGHLIGHT_3D,
    COL_BUTTON_HIGHLIGHT,
    COL_LIGHT_3D,
    COL_SHADOW_3D,
    COL_BUTTON_SHADOW,
    COL_ACTIVE_BORDER,
    COL_ACTIVE_CAPTION,
    COL_APP_WORKSPACE,
    COL_DESKTOP,
    COL_BUTTON_TEXT,
    COL_CAPTION_TEXT,
    COL_GRAY_TEXT,
    COL_HIGHLIGHT,
    COL_HIGHLIGHT_TEXT,
    COL_INACTIVE_BORDER,
    COL_INACTIVE_CAPTION,
    COL_INACTIVE_CAPTION_TEXT,
    COL_INFO_BACKGROUND,
    COL_INFO_TEXT,
    COL_MENU,
    COL_MENU_TEXT,
    COL_SCROLLBAR,
    COL_WINDOW_BACKGROUND,
    COL_WINDOW_FRAME,
    COL_WINDOW_TEXT
};

//----------------------------------------------------------------------------
//
//  Konstanten fuer den Kantenstil bei DrawEdge()
//
enum
{
    RAISED_OUTER_EDGES  = 1,
    SUNKEN_OUTER_EDGES  = 2,
    RAISED_INNER_EDGES  = 4,
    SUNKEN_INNER_EDGES  = 8,
    RAISED_EDGES        = RAISED_OUTER_EDGES | RAISED_INNER_EDGES,
    SUNKEN_EDGES        = SUNKEN_OUTER_EDGES | SUNKEN_INNER_EDGES
};

//----------------------------------------------------------------------------
//
//  Konstanten, die angeben, welche Kanten gezeichnet werden sollen.
//  z.B. bei DrawEdge()
//
enum
{
    LEFT_EDGE           = 1,
    TOP_EDGE            = 2,
    RIGHT_EDGE          = 4,
    BOTTOM_EDGE         = 8,
    ALL_EDGES           = LEFT_EDGE | TOP_EDGE | RIGHT_EDGE | BOTTOM_EDGE
};

//----------------------------------------------------------------------------
//
//  Format der Textausgabe
//
enum
{
    SINGLELINE_TEXT     = 0x0100,
    MULTILINE_TEXT      = 0x0200
};

//----------------------------------------------------------------------------
//
//  Standardwerte fuer Controls, werden mit SetValue() und Value()
//  verwendet.
//
enum
{
    CONTROL_ON      = 0x01,
    CONTROL_OFF     = 0x02
};

//----------------------------------------------------------------------------

enum window_look
{
    TITLED_WINDOW_LOOK          = 0x0100,
    DOCUMENT_WINDOW_LOOK        = 0x0200,
    FLOATING_WINDOW_LOOK        = 0x0300,
    MODAL_WINDOW_LOOK           = 0x0400,
    BORDERED_WINDOW_LOOK        = 0x0500,
    NO_BORDER_WINDOW_LOOK       = 0x0600
};

//----------------------------------------------------------------------------

enum window_feel
{
    NORMAL_WINDOW_FEEL          = 0x0001,
    MODAL_WINDOW_FEEL           = 0x0002,
    FLOATING_WINDOW_FEEL        = 0x0003,
    CHILD_WINDOW_FEEL           = 0x0004
};

//----------------------------------------------------------------------------

enum window_type
{
    TITLED_WINDOW       = TITLED_WINDOW_LOOK    | NORMAL_WINDOW_FEEL,
    DOCUMENT_WINDOW     = DOCUMENT_WINDOW_LOOK  | NORMAL_WINDOW_FEEL,
    MODAL_WINDOW        = MODAL_WINDOW_LOOK     | MODAL_WINDOW_FEEL,
    FLOATING_WINDOW     = FLOATING_WINDOW_LOOK  | FLOATING_WINDOW_FEEL,
    BORDERED_WINDOW     = BORDERED_WINDOW_LOOK  | NORMAL_WINDOW_FEEL
};

//----------------------------------------------------------------------------

enum
{
    WINDOW_NOT_MINIMIZABLE      = 0x0002,
    WINDOW_NOT_MAXIMIZABLE      = 0x0004,
    WINDOW_NOT_RESIZABLE        = 0x0008,
    WINDOW_NO_WINDOW_MENU       = 0x0010
};

//----------------------------------------------------------------------------

#endif
