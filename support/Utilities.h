#ifndef support_Utilities_h
#define support_Utilities_h

#include <cstddef>

//----------------------------------------------------------------------------

#define nullptr (0)

//----------------------------------------------------------------------------

typedef unsigned char   uchar;
typedef unsigned short  ushort;
typedef unsigned int    uint;
typedef unsigned long   ulong;

//----------------------------------------------------------------------------

enum type_code
{
    CHAR_TYPE           = 0x0000,
    SHORT_TYPE          = 0x0001,
    INT_TYPE            = 0x0002,
    LONG_TYPE           = 0x0003,
    BOOL_TYPE           = 0x0004,
    FLOAT_TYPE          = 0x0005,
    DOUBLE_TYPE         = 0x0006,
    POINT_TYPE          = 0x0007,
    SIZE_TYPE           = 0x0008,
    RECT_TYPE           = 0x0009,
    POINTER_TYPE        = 0x000A,
    STRING_TYPE         = 0x000B,
    ANY_TYPE            = 0x00FF
};

//----------------------------------------------------------------------------

template <class T>
inline bool operator!=(const T& x, const T& y)
{
    return !(x == y);
}

//----------------------------------------------------------------------------

template <class T>
inline bool operator>(const T& x, const T& y)
{
    return y < x;
}

//----------------------------------------------------------------------------

template <class T>
inline bool operator<=(const T& x, const T& y)
{
    return !(y < x);
}

//----------------------------------------------------------------------------

template <class T>
inline bool operator>=(const T& x, const T& y)
{
    return !(x < y);
}

//----------------------------------------------------------------------------

template <class T>
inline const T& Min(const T& a, const T& b)
{
    return a < b ? a : b;
}

//----------------------------------------------------------------------------

template <class T>
inline const T& Max(const T& a, const T& b)
{
    return a < b ? b : a;
}

//----------------------------------------------------------------------------

template <class T>
inline void Swap(T& a, T& b)
{
    T tmp = a;
    a = b;
    b = tmp;
}

//----------------------------------------------------------------------------

inline bool TestBits(uint value, uint bits)
{
    return (value & bits) == bits;
}

//----------------------------------------------------------------------------

#endif
