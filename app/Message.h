#ifndef app_Message_h
#define app_Message_h

#include "interface/Point.h"
#include "interface/Rect.h"
#include "support/String.h"
#include "support/Utilities.h"
#include "support/Vector.h"

#include <cstring>

class Handler;

//----------------------------------------------------------------------------
//
//  Botschafts-Typen
//
enum
{
    //  Mouse-Botschaften
    MSG_MOUSE_DOWN              = 0x0010,
    MSG_MOUSE_UP                = 0x0011,
    MSG_MOUSE_MOVED             = 0x0012,
    MSG_MOUSE_DOUBLECLICK       = 0x0013,
    MSG_MOUSE_WHEEL             = 0x0014,

    //  Tastatur-Botschaften
    MSG_KEY_DOWN                = 0x0020,
    MSG_KEY_UP                  = 0x0021,
    MSG_KEY_PRESSED             = 0x0022,

    // Timer-Botschaften
    MSG_PULSE                   = 0x0030,

    // View-Botschaften
    MSG_VIEW_DRAW               = 0x0040,
    MSG_VIEW_FOCUS_CHANGED      = 0x0041,
    MSG_VIEW_RESIZED            = 0x0042,
    MSG_VIEW_MOVED              = 0x0043,
    MSG_VIEW_VISIBILITY_CHANGED = 0x0044,
    MSG_VIEW_ENABLED            = 0x0045,

    // Window-Botschaften
    MSG_WINDOW_ACTIVATED        = 0x0050,
    MSG_WINDOW_CLOSE            = 0x0051,

    // System-Botschaften
    MSG_SYSTEM                  = 0x0090,
    MSG_COMMAND                 = 0x0091,

    // Applikations-/Thread-Botschaften
    MSG_QUIT                    = 0x0060,

    // benutzerdefinierte Botschaften
    MSG_USER                    = 0x1000,

    // allgemeine Botschaft
    MSG_UNKNOWN                 = 0xFFFF
};

//----------------------------------------------------------------------------
//
//  Mausbutton-Konstanten, die bei MSG_MOUSE_DOWN und MSG_MOUSE_UP benutzt
//  werden, um den ausloesenden Mausbutton zu identifizieren.
//
enum
{
    LEFT_BUTTON                 = 0x0001,
    RIGHT_BUTTON                = 0x0002,
    MIDDLE_BUTTON               = 0x0004
};

//----------------------------------------------------------------------------
//
//  Mausbutton- und Tastaturkonstanten, die bei Mausbotschaften benutzt
//  werden, um den Status der Maus- bzw. Steuertasten zu identifizieren.
//
enum
{
    NO_KEY_DOWN                 = 0x0000,
    LEFT_BUTTON_DOWN            = 0x0001,
    RIGHT_BUTTON_DOWN           = 0x0002,
    MIDDLE_BUTTON_DOWN          = 0x0004,
    SHIFT_DOWN                  = 0x0008,
    CONTROL_DOWN                = 0x0010,
    ALT_DOWN                    = 0x0020
};

//----------------------------------------------------------------------------
//
//  Botschafts-Klasse zur flexiblen Kommunikation zwischen Objekten.
//
//  Die Message-Klasse ermoeglicht es, beliebige Botschaften zwischen Objekten
//  zu versenden, ohne die Typhierarchie unnoetig aufzublaehen. Einem Message-
//  Objekt koennen dynamisch Daten von beliebigem Typ hinzugefuegt werden. Die
//  Daten werden dazu ueber eine Typkonstante (z.B. LONG_TYPE, POINTER_TYPE
//  etc.) und einen Zeichenketten-Bezeichner identifiziert.
//
//  Jedes Message-Objekt erhaelt darueber hinaus eine Message-Konstante, die
//  die Art der Botschaft identifiziert. Zugriff auf den Message-Typ liefert
//  die public-Variable what.
//
//  Es koennen jederzeit eigene Message-Typen definiert werden. Um einen
//  Konflikt mit den vordefinierten Konstanten wie MSG_MOUSE_DOWN usw.
//  auszuschliessen, sollten eigene Konstanten in der Form MSG_USER + x
//  definiert werden.
//
//  Message-Objekte speichern stets Kopien der eingetragenen Daten. Diese
//  werden geloescht, wenn die Daten entfernt werden, oder das Message-Objekt
//  geloescht wird.
//
class Message
{
public:

    //
    //  Erstellt ein leeres Message-Objekt und setzt das what-Feld auf den
    //  uebergebenen Wert.  Ueber den Parameter handler kann das Zielobjekt
    //  der Botschaft angegeben werden.
    //
    Message(uint what, ::Handler* handler = nullptr);

    //
    //  Erstellt eine Kopie des Message-Objektes message. Die in message
    //  enthaltenen Daten werden kopiert.
    //
    Message(const Message& message);

    //
    //  Loescht die in dem Message-Objekt enthaltenen Daten und das Objekt
    //  selbst.
    //
    virtual ~Message();

    //
    //  Liefert true, wenn das Message-Objekt eine Systembotschaft enthaelt.
    //
    bool IsSystem() const
    { return what >= MSG_USER; }

    //
    //  Liefert den Ziel-Handler des Message-Objektes oder 0, wenn kein
    //  Handler angegeben wurde.
    //
    ::Handler* Handler() const
    { return m_handler; }

    //
    //  Setzt den Ziel-Handler der Message auf handler.
    //
    void SetHandler(::Handler* handler)
    { m_handler = handler; }

    //
    //  Fuegt dem Message-Objekt dynamisch Daten hinzu. Die Daten werden unter
    //  dem Bezeichner name und dem Typcode type eingetragen. data muss einen
    //  Zeiger auf die zu kopierenden Daten und size die Anzahl der zu
    //  kopierenden Bytes enthalten.
    //
    void AddData(const char* name, type_code type, void* data, ulong size)
    { m_infoList.Append(new DataInfo(name, type, data, size)); }

    //
    //  Fuegt dem Message-Objekt einen bool-Wert hinzu.
    //
    void AddBool(const char* name, bool b)
    { m_infoList.Append(new DataInfo(name, BOOL_TYPE, &b, sizeof(b))); }

    //
    //  Fuegt dem Message-Objekt einen char-Wert hinzu.
    //
    void AddChar(const char* name, char c)
    { m_infoList.Append(new DataInfo(name, CHAR_TYPE, &c, sizeof(c))); }

    //
    //  Fuegt dem Message-Objekt einen short-Wert hinzu.
    //
    void AddShort(const char* name, short x)
    { m_infoList.Append(new DataInfo(name, SHORT_TYPE, &x, sizeof(x))); }

    //
    //  Fuegt dem Message-Objekt einen int-Wert hinzu.
    //
    void AddInt(const char* name, int x)
    { m_infoList.Append(new DataInfo(name, INT_TYPE, &x, sizeof(x))); }

    //
    //  Fuegt dem Message-Objekt einen long-Wert hinzu.
    //
    void AddLong(const char* name, long x)
    { m_infoList.Append(new DataInfo(name, LONG_TYPE, &x, sizeof(x))); }

    //
    //  Fuegt dem Message-Objekt einen float-Wert hinzu.
    //
    void AddFloat(const char* name, float x)
    { m_infoList.Append(new DataInfo(name, FLOAT_TYPE, &x, sizeof(x))); }

    //
    //  Fuegt dem Message-Objekt einen double-Wert hinzu.
    //
    void AddDouble(const char* name, double x)
    { m_infoList.Append(new DataInfo(name, DOUBLE_TYPE, &x, sizeof(x))); }

    //
    //  Fuegt dem Message-Objekt einen Zeiger hinzu. Dabei wird nur der Zeiger
    //  selbst kopiert, nicht die Daten, auf die er verweist.
    //
    void AddPointer(const char* name, void* p)
    { m_infoList.Append(new DataInfo(name, POINTER_TYPE, &p, sizeof(p))); }

    //
    //  Fuegt dem Message-Objekt eine Zeichenkette hinzu. Dabei wird die
    //  Zeichenkette selbst, inklusive des abschliessenden Null-Zeichens,
    //  kopiert.
    //
    void AddString(const char* name, const char* s)
    { m_infoList.Append(new DataInfo(name, STRING_TYPE, s,
                                    sizeof(char) * (strlen(s) + 1))); }

    //
    //  Fuegt dem Message-Objekt einen Point-Wert hinzu.
    //
    void AddPoint(const char* name, const Point& p)
    { m_infoList.Append(new DataInfo(name, POINT_TYPE, &p, sizeof(p))); }

    //
    //  Fuegt dem Message-Objekt einen Size-Wert hinzu.
    //
    void AddSize(const char* name, const Size& s)
    { m_infoList.Append(new DataInfo(name, SIZE_TYPE, &s, sizeof(s))); }

    //
    //  Fuegt dem Message-Objekt einen Rect-Wert hinzu.
    //
    void AddRect(const char* name, const Rect& r)
    { m_infoList.Append(new DataInfo(name, RECT_TYPE, &r, sizeof(r))); }

    //
    //  Ersetzt die unter dem Bezeichner name eingetragenen Daten durch die
    //  durch type, data und size spezifizierten. Die alten Daten werden
    //  geloescht.
    //
    bool ReplaceData(const char* name, type_code type, void* data,
                     ulong size);

    //
    //  Sucht nach dem ersten Eintrag unter dem Bezeichner name. Enthaelt type
    //  nicht ANY_TYPE, wird nur nach Daten mit diesem Typ gesucht. data muss
    //  die Adresse eines Zeigers enthalten, der nach dem Aufruf von FindData()
    //  auf die gefundenen Daten verweisen soll. Die Groesse der gefundenen
    //  Daten in Byte wird in size zurueckgeliefert.
    //
    //  Die Funktion liefert true, wenn ein entsprechender Eintrag gefunden
    //  wurde, sonst false. Nur im true-Fall sind data und size gueltig.
    //
    bool FindData(const char* name, type_code type, void** data,
                  ulong* size = nullptr) const;

    //
    //  Sucht nach dem ersten bool-Eintrag unter dem Bezeichner name.  Die
    //  Funktion liefert true, wenn ein solcher Eintrag gefunden wurde. In
    //  diesem Fall zeigt bp auf diesen Wert. Enthaelt das Message-Objekt
    //  keinen solchen Eintrag, liefert sie false.
    //
    bool FindBool(const char* name, bool* bp) const;

    //
    //  Sucht nach dem ersten char-Eintrag unter dem Bezeichner name.  Die
    //  Funktion liefert true, wenn ein solcher Eintrag gefunden wurde. In
    //  diesem Fall zeigt cp auf diesen Wert. Enthaelt das Message-Objekt
    //  keinen solchen Eintrag, liefert sie false.
    //
    bool FindChar(const char* name, char* cp) const;

    //
    //  Sucht nach dem ersten short-Eintrag unter dem Bezeichner name.  Die
    //  Funktion liefert true, wenn ein solcher Eintrag gefunden wurde. In
    //  diesem Fall zeigt xp auf diesen Wert. Enthaelt das Message-Objekt
    //  keinen solchen Eintrag, liefert sie false.
    //
    bool FindShort(const char* name, short* xp) const;

    //
    //  Sucht nach dem ersten int-Eintrag unter dem Bezeichner name.  Die
    //  Funktion liefert true, wenn ein solcher Eintrag gefunden wurde. In
    //  diesem Fall zeigt xp auf diesen Wert. Enthaelt das Message-Objekt
    //  keinen solchen Eintrag, liefert sie false.
    //
    bool FindInt(const char* name, int* xp) const;

    //
    //  Sucht nach dem ersten long-Eintrag unter dem Bezeichner name.  Die
    //  Funktion liefert true, wenn ein solcher Eintrag gefunden wurde. In
    //  diesem Fall zeigt xp auf diesen Wert. Enthaelt das Message-Objekt
    //  keinen solchen Eintrag, liefert sie false.
    //
    bool FindLong(const char* name, long* xp) const;

    //
    //  Sucht nach dem ersten float-Eintrag unter dem Bezeichner name.  Die
    //  Funktion liefert true, wenn ein solcher Eintrag gefunden wurde. In
    //  diesem Fall zeigt xp auf diesen Wert. Enthaelt das Message-Objekt
    //  keinen solchen Eintrag, liefert sie false.
    //
    bool FindFloat(const char* name, float* xp) const;

    //
    //  Sucht nach dem ersten double-Eintrag unter dem Bezeichner name.  Die
    //  Funktion liefert true, wenn ein solcher Eintrag gefunden wurde.  In
    //  diesem Fall zeigt xp diesen Wert. Enthaelt das Message-Objekt keinen
    //  solchen Eintrag, liefert sie false.
    //
    bool FindDouble(const char* name, double* xp) const;

    //
    //  Sucht nach dem ersten Zeiger-Eintrag unter dem Bezeichner name.  Die
    //  Funktion liefert true, wenn ein solcher Eintrag gefunden wurde. In
    //  diesem Fall zeigt pp auf diesen Wert. Enthaelt das Message-Objekt
    //  keinen solchen Eintrag, liefert sie false.
    //
    bool FindPointer(const char* name, void** pp) const;

    //
    //  Sucht nach dem ersten Zeichenketten-Eintrag unter dem Bezeichner name.
    //  Die Funktion liefert true, wenn ein solcher Eintrag gefunden wurde. In
    //  diesem Fall zeigt sp auf die Zeichenkette. Enthaelt das Message-
    //  Objekt keinen solchen Eintrag, liefert sie false.
    //
    bool FindString(const char* name, const char** sp) const;

    //
    //  Sucht nach dem ersten Point-Eintrag unter dem Bezeichner name.  Die
    //  Funktion liefert true, wenn ein solcher Eintrag gefunden wurde. In
    //  diesem Fall zeigt pp auf dieses Point-Objekt. Enthaelt das Message-
    //  Objekt keinen solchen Eintrag, liefert sie false.
    //
    bool FindPoint(const char* name, Point* pp) const;

    //
    //  Sucht nach dem ersten Size-Eintrag unter dem Bezeichner name.  Die
    //  Funktion liefert true, wenn ein solcher Eintrag gefunden wurde. In
    //  diesem Fall zeigt sp auf dieses Size-Objekt. Enthaelt das Message-
    //  Objekt keinen solchen Eintrag, liefert sie false.
    //
    bool FindSize(const char* name, Size* sp) const;

    //
    //  Sucht nach dem ersten Rect-Eintrag unter dem Bezeichner name.  Die
    //  Funktion liefert true, wenn ein solcher Eintrag gefunden wurde. In
    //  diesem Fall zeigt rp auf dieses Rect-Objekt. Enthaelt das Message-
    //  Objekt keinen solchen Eintrag, liefert sie false.
    //
    bool FindRect(const char* name, Rect* rp) const;

    //
    //  Entfernt alle unter dem Bezeichner name eingetragenen Daten aus dem
    //  Message-Objekt und gibt den dafuer reservierten Speicher wieder frei.
    //
    bool RemoveName(const char* name);

    //
    //  Entfernt alle in dem Message-Objekt eingetragenen Daten und gibt den
    //  dafuer reservierten Speicher wieder frei.
    //
    void Clear();

    //
    //  Liefert die Anzahl der eingetragenen Datenelemente.
    //
    uint Count() const
    { return m_infoList.Count(); }

    //
    //  Bietet Zugriff auf den Message-Typ.
    //
    uint what;

private:

    struct DataInfo
    {
        DataInfo(const char* name, type_code type,
                 const void* data, ulong size)
        { Init(name, type, data, size); }

        DataInfo(const DataInfo& info)
        { Init(info.name, info.type, info.data, info.size); }

        ~DataInfo()
        {
            delete[] name;
            delete[] data;
        }

        DataInfo& operator=(const DataInfo& info)
        {
            delete[] name;
            delete[] data;

            Init(info.name, info.type, info.data, info.size);
            return *this;
        }

        bool operator==(const char* name) const
        { return strcmp(this->name, name); }

        void Init(const char* name, type_code type,
                  const void* data, ulong size);

        char*       name;
        type_code   type;
        char*       data;
        ulong       size;
    };

    //
    //  Liefert den Index des ersten Listenelements mit dem Bezeichner name,
    //  beginnend bei pos. Ist name nicht enthalten, wird m_infoList.End()
    //  zurueckgeliefert.
    //
    uint Locate(const char* name, uint pos = 0) const;

    ::Handler*          m_handler;
    Vector<DataInfo>    m_infoList;
};

//----------------------------------------------------------------------------

inline bool Message::FindBool(const char* name, bool* bp) const
{
    bool found;
    bool* data;

    if (found = FindData(name, BOOL_TYPE, (void**) &data))
        *bp = *data;

    return found;
}

//----------------------------------------------------------------------------

inline bool Message::FindChar(const char* name, char* cp) const
{
    bool found;
    char* data;

    if (found = FindData(name, CHAR_TYPE, (void**) &data))
        *cp = *data;

    return found;
}

//----------------------------------------------------------------------------

inline bool Message::FindShort(const char* name, short* xp) const
{
    bool found;
    short* data;

    if (found = FindData(name, SHORT_TYPE, (void**) &data))
        *xp = *data;

    return found;
}

//----------------------------------------------------------------------------

inline bool Message::FindInt(const char* name, int* xp) const
{
    bool found;
    int* data;

    if (found = FindData(name, INT_TYPE, (void**) &data))
        *xp = *data;

    return found;
}

//----------------------------------------------------------------------------

inline bool Message::FindLong(const char* name, long* xp) const
{
    bool found;
    long* data;

    if (found = FindData(name, LONG_TYPE, (void**) &data))
        *xp = *data;

    return found;
}

//----------------------------------------------------------------------------

inline bool Message::FindFloat(const char* name, float* xp) const
{
    bool found;
    float* data;

    if (found = FindData(name, FLOAT_TYPE, (void**) &data))
        *xp = *data;

    return found;
}

//----------------------------------------------------------------------------

inline bool Message::FindDouble(const char* name, double* xp) const
{
    bool found;
    double* data;

    if (found = FindData(name, DOUBLE_TYPE, (void**) &data))
        *xp = *data;

    return found;
}

//----------------------------------------------------------------------------

inline bool Message::FindPointer(const char* name, void** pp) const
{
    bool found;
    void** data;

    if (found = FindData(name, POINTER_TYPE, (void**) &data))
        *pp = *data;

    return found;
}

//----------------------------------------------------------------------------

inline bool Message::FindString(const char* name, const char** sp) const
{
    bool found;
    char** data;

    if (found = FindData(name, STRING_TYPE, (void**) &data))
        *sp = *data;

    return found;
}

//----------------------------------------------------------------------------

inline bool Message::FindPoint(const char* name, Point* pp) const
{
    bool found;
    Point* data;

    if (found = FindData(name, POINT_TYPE, (void**) &data))
        *pp = *data;

    return found;
}

//----------------------------------------------------------------------------

inline bool Message::FindSize(const char* name, Size* sp) const
{
    bool found;
    Size* data;

    if (found = FindData(name, SIZE_TYPE, (void**) &data))
        *sp = *data;

    return found;
}

//----------------------------------------------------------------------------

inline bool Message::FindRect(const char* name, Rect* rp) const
{
    bool found;
    Rect* data;

    if (found = FindData(name, RECT_TYPE, (void**) &data))
        *rp = *data;

    return found;
}

//----------------------------------------------------------------------------

#endif
