#ifndef support_ManagedPtr_h
#define support_ManagedPtr_h

#include "support/Utilities.h"

//----------------------------------------------------------------------------
//
//  "Smart-Pointer"-Klasse mit Referenzzaehler
//
//  ManagedPtr-Objekte dienen als "smart pointer". Fuer jedes dynamisch
//  angelegte Objekt, das einem ManagedPtr-Objekt zugewiesen wird, wird ein
//  Referenzzaehler erstellt. Wird die letzte Referenz auf ein Objekt
//  geloescht, wird das Objekt automatisch aus dem Speicher entfernt.
//  Ansonsten koennen ManagedPtr-Objekte wie normale Zeiger verwendet werden.
//
//  Objekte, die von ManagedPtr verwaltet werden sollen, muessen mit 'new'
//  angelegte Einzelobjekte sein, keine Arrays!
//
template <class T> class ManagedPtr
{
public:

    //
    //  Erzeugt ein neues ManagedPtr um das Objekt ptr zu verwalten. ptr muss
    //  ein dynamisch mit new angelegtes Einzelobjekt sein.
    //
    explicit ManagedPtr(T* ptr = nullptr)
        : m_ptr(ptr),
          m_refCount(ptr != nullptr ? new int(1) : nullptr) {}

    //
    //  Erzeugt eine Kopie eines ManagedPtr-Objekts.
    //
    template <class U>
    ManagedPtr(ManagedPtr<U>& m)
        : m_ptr(m.m_ptr),
          m_refCount(m.m_refCount)
    {
        if (m.m_ptr != nullptr)
            m.AddRef();
    }

    //
    //  Entfernt ein ManagedPtr und verringert den Referenzzaehler des
    //  verwalteten Objekts um 1. Erreicht der Zaehler 0, werden das
    //  verwaltete Objekt und der Zaehler aus dem Speicher entfernt.
    //
    ~ManagedPtr()
    { Release(); }

    //
    //  Liefert das verwaltete Objekt zurueck.
    //
    T& operator*() const
    { return *m_ptr; }

    //
    //  Liefert die Adresse des verwalteten Objekts zurueck. Damit kann auf
    //  Strukturelemente zugegriffen werden, falls T ein strukturierter
    //  Datentyp ist.
    //
    T* operator->() const
    { return m_ptr; }

    //
    //  Weist dem ManagedPtr ein neues Objekt zu. Das alte Objekt wird
    //  freigegeben und entfernt, wenn keine weiteren Referenzen darauf
    //  existieren.
    //
    template <class U> ManagedPtr& operator=(ManagedPtr<U>& m);

    //
    //  Weist dem ManagedPtr ein neues Objekt zu. Das alte Objekt wird
    //  freigegeben und entfernt, wenn keine weiteren Referenzen darauf
    //  existieren.
    //
    ManagedPtr& operator=(T* ptr);

private:

    friend T* GetPtr(const ManagedPtr<T>&);
    friend bool operator==(const ManagedPtr<T>&, const ManagedPtr<U>&)
    friend bool operator==(const ManagedPtr<T>&, const U*)
    friend bool operator==(const T*, const ManagedPtr<U>&)

    //
    //  Erhoeht den Referenzzaehler manuell um 1.
    //
    int AddRef()
    { return ++(*m_refCount); }

    //
    //  Verringert den Referenzzaehler manuell um 1.
    //
    int ReleaseRef()
    { return --(*m_refCount); }

    void Release()
    {
        if (m_ptr != nullptr && ReleaseRef() <= 0)
        {
            delete m_ptr;
            delete m_refCount;
        }
    }

    T*      m_ptr;
    int*    m_refCount;
};

//----------------------------------------------------------------------------
//
//  Liefert die Adresse des von m verwalteten Objektes zurueck.
//
template <class T>
T* GetPtr(const ManagedPtr<T>& m)
{ return m.m_ptr; }

//----------------------------------------------------------------------------
//
//  Liefert true, wenn lhs und rhs auf das gleiche Objekt verweisen.
//
template <class T, class U>
inline bool operator==(const ManagedPtr<T>& m1, const ManagedPtr<U>& m2)
{ return m1.m_ptr == m2.m_ptr; }

template <class T, class U>
inline bool operator==(const ManagedPtr<T>& m, const U* ptr)
{ return m.m_ptr == ptr; }

template <class T, class U>
inline bool operator==(const T* ptr, const ManagedPtr<U>& m)
{ return ptr == m.m_ptr; }

//----------------------------------------------------------------------------

template <class T>
template <class U>
ManagedPtr<T>& ManagedPtr<T>::operator=(ManagedPtr<U>& m)
{
    if (m.m_ptr != m_ptr)
    {
        Release();

        m_ptr = m.m_ptr;
        m_refCount = m.m_refCount;

        if (m.m_ptr != nullptr)
            m.AddRef();
    }

    return *this;
}

//----------------------------------------------------------------------------

template <class T>
ManagedPtr<T>& ManagedPtr<T>::operator=(T* ptr)
{
    if (ptr != m_ptr)
    {
        Release();

        m_ptr = ptr;
        m_refCount = new int(1);
    }

    return *this;
}

//----------------------------------------------------------------------------

#endif
