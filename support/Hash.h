#ifndef support_Hash_h
#define support_Hash_h

#include "support/Utilities.h"

//----------------------------------------------------------------------------
//
//  Berechnet den FNA-1a-Hashwert des Speicherbereiches [p, p + size).
//
inline uint HashBuffer(const char* p, uint size)
{
    uint h = 2166136261u;

    for (int i = 0; i < size; ++i)
        h = (h ^ *p++) * 16777619u;

    return h;
}

//----------------------------------------------------------------------------
//
//  Berechnet den FNA-1a-Hashwert der Null-terminierten Zeichenkette p.
//
inline uint HashString(const char* p)
{
    uint h = 2166136261u;

    while (*p != '\0')
        h = (h ^ *p++) * 16777619u;

    return h;
}

//----------------------------------------------------------------------------
//
//  Template fuer Hash-Funktionsobjekte.
//
template <class T>
struct DefaultHash
{
    uint operator()(const T& key) const
    { return HashBuffer(reinterpret_cast<const char*>(&key), sizeof(T)); }
};

//----------------------------------------------------------------------------
//
//  Partielle Hash-Spezialisierung fuer Zeigertypen.
//
template <class T>
struct DefaultHash<T*>
{
    uint operator()(T* key) const
    { return reinterpret_cast<uint>(key); }
};

//----------------------------------------------------------------------------
//
//  Hash-Spezialisierung fuer Null-terminierte Zeichenketten.
//
template <>
struct DefaultHash<const char*>
{
    uint operator()(const char* key) const
    { return HashString(key); }
};

//----------------------------------------------------------------------------
//
//  Hash-Spezialiserung fuer uint.
//
template <>
struct DefaultHash<uint>
{
    uint operator()(uint key) const
    { return key; }
};

//----------------------------------------------------------------------------

#endif
