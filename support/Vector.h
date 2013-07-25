#ifndef support_Vector_h
#define support_Vector_h

#include "support/Collection.h"
#include "support/Utilities.h"

#include <climits>

//----------------------------------------------------------------------------
//
//  Template-Klasse, die ein Array mit automatischer Groessenanpassung
//  bereitstellt.
//
//  Der von einem Vector-Objekt reservierte Speicher waechst bei Bedarf um
//  einen festen Faktor. Normalerweise werden Vector-Objekte nicht kleiner.
//  Der reservierte Speicher kann aber mit der Funktion Shrink() auf die
//  minimal benoetigte Groesse reduziert werden.
//
//  Vektoren speichern nur Zeiger auf Elemente (shallow copy). Soll dieses
//  Verhalten geaendert werden, d.h. sollen Kopien der Elemente gespeichert
//  werden (deep copy), muss die Funktion NewElement() dementsprechend
//  ueberschrieben werden.
//
//  Elemente, die aus einem Vektor entfernt werden, koennen automatisch mit
//  delete geloescht werden. Dazu muss mit SetAutoDelete() die AutoDelete-
//  Option auf true gesetzt werden. Die Standardvorgabe ist false, d.h. die
//  Elemente werden nicht geloescht.
//
template <class T>
class Vector: public Collection<T> {

public:

    static const double GROWTH_FACTOR       = 1.5;
    static const uint   INITIAL_CAPACITY    = 8;
    static const uint   MIN_CAPACITY        = 4;
    static const uint   MAX_CAPACITY        = UINT_MAX / (1.5 * sizeof(T));

    //
    //  Erstellt einen neuen Vektor der Groesse count. Wenn mehr Speicher
    //  benoetigt wird, werden jeweils Bloecke der Laenge n*count reserviert.
    //
    Vector(uint capacity = INITIAL_CAPACITY)
        : m_capacity(capacity), m_count(0), m_data(new T*[capacity]) {}

    //
    //  Entfernt alle Elemente aus dem Vektor und zerstoert das Vector-Objekt.
    //  Ist AutoDelete auf true gesetzt, werden die Elemente geloescht.
    //
    virtual ~Vector();

    //
    //  Weist dem Vektor eine Kopie von vector zu. Es werden standardmaessig
    //  nur die Zeiger auf die Elemente kopiert. Ist der Vektor vorher nicht
    //  leer, werden alle Elemente entfernt und - falls AutoDelete auf true
    //  gesetzt ist - geloescht.
    //
    Vector<T>& operator=(const Vector<T>& vector);

    //
    //  Fuegt die Elemente von vector an das Ende des Vektors an. Dabei
    //  werden nur die Zeiger auf die Elemente kopiert.
    //
    Vector<T>& operator+=(const Vector<T>& vector)
    { return Append(vector); }

    //
    //  Liefert die Anzahl der Elemente des Vektors.
    //
    uint Count() const
    { return m_count; }

    //
    //  Liefert die Groesse des Vektors. Capacity() - Count() gibt die Anzahl
    //  der Elemente an, die noch in den Vektor eingefuegt werden koennen,
    //  ohne neuen Speicher zu reservieren.
    //
    uint Capacity() const
    { return m_capacity; }

    //
    //  Liefert die Position des ersten Elements.
    //
    uint Begin() const
    { return 0; }

    //
    //  Liefert die Position hinter dem letzten Element.
    //
    uint End() const
    { return m_count; }

    //
    //  Liefert einen Zeiger auf das Element an der Position pos. Im Gegensatz
    //  zur Funktion At() wird pos jedoch nicht auf Gueltigkeit geprueft.
    //
    T* operator[](uint pos) const
    { return m_data[pos]; }

    //
    //  Liefert einen Zeiger auf das Element an der Position pos. Ist pos
    //  ausserhalb des erlaubten Bereiches (d.h. pos > Count() - 1), wird
    //  Null zurueckgeliefert.
    //
    T* At(uint pos) const
    { return pos < m_count ? m_data[pos] : nullptr; }

    //
    //  Ersetzt das Element an der Position pos durch data.
    //
    void SetAt(uint pos, T* data);

    //
    //  Liefert den Index des ersten Elements das gleich data ist. Wird kein
    //  solches Element gefunden, liefert die Funktion die Position End().
    //
    //  Fuer diese Funktion muss der Operator "==" fuer Elemente des Typs T
    //  definiert sein.
    //
    uint Find(const T& data) const
    { return FindNext(data, 0); }

    //
    //  Liefert, ausgehend von der Position pos, den Index des ersten
    //  Elements das gleich data ist. Wird kein solches Element gefunden,
    //  liefert die Funktion die Position End().
    //
    //  Fuer diese Funktion muss der Operator "==" fuer Elemente des Typs T
    //  definiert sein.
    //
    uint FindNext(const T& data, uint pos) const;

    //
    //  Liefert den Index des ersten Elements, das eine Referenz auf data ist.
    //  Wird kein solches Element gefunden, liefert die Funktion die Position
    //  End().
    //
    uint FindRef(const T* data) const
    { return FindNextRef(data, 0); }

    //
    //  Liefert, ausgehend von der Position pos, den Index des ersten
    //  Elements, das eine Referenz auf data ist. Wird kein solches Element
    //  gefunden, liefert die Funktion die Position End().
    //
    uint FindNextRef(const T* data, uint pos) const;

    //
    //  Liefert true, wenn der Vektor ein Element enthaelt, das gleich data
    //  ist, sonst false.
    //
    bool Contains(const T& data) const
    { return Find(data) != End(); }

    //
    //  Liefert true, wenn der Vektor ein Element enthaelt, das eine Referenz
    //  auf data ist, sonst false.
    //
    bool ContainsRef(const T* data) const
    { return FindRef(data) != End(); }

    //
    //  Entfernt alle Elemente aus dem Vektor. Ist AutoDelete auf true
    //  gesetzt, werden die Elemente geloescht.
    //
    void Clear();

    //
    //  Fuegt die Elemente von vector an das Ende des Vektors an. Dabei
    //  werden nur die Zeiger auf die Elemente kopiert.
    //
    Vector<T>& Append(const Vector<T>& vector);

    //
    //  "Schrumpft" den Vektor auf die minimal benoetigte Groesse (in
    //  Blockeinheiten). Normalerweise koennen Vektoren nur wachsen. Wenn sehr
    //  grosse Vektoren nur noch wenige Elemente enthalten, kann mit dieser
    //  Funktion der benoetigte Speicherplatz reduziert werden.
    //
    void Shrink()
    { Resize(Count()); }

    //
    //  Stellt sicher, dass die Kapazitaet des Vektors mindestens size
    //  betraegt, so dass bis zu dieser Groesse Elemente eingefuegt werden
    //  koennten, ohne neuen Speicher anzufordern. Wenn in etwa bekannt ist,
    //  wieviele Elemente im Folgenden eingefuegt werden, sollte diese
    //  Funktion verwendet werden.
    //
    //  Die Funktion liefert true, wenn der Vektor vergroessert wurde, sonst
    //  false.
    //
    bool Reserve(uint size)
    { return size > m_capacity ? Resize(size) : false; }

    //
    //  Fuegt das Element data an der Position pos in den Vektor ein. Die
    //  nachfolgenden Elemente werden wenn noetig jeweils um eine Position
    //  nach hinten verschoben.
    //
    //  Es muss sichergestellt sein, dass data kein Nullzeiger ist und dass
    //  pos <= Count() ist.
    //
    void Insert(T* data, uint pos);

    //
    //  Haengt data an das Ende des Vektors an. Append() ist im allgemeinen
    //  effizienter als Insert(), da keine Elemente verschoben werden muessen.
    //  Diese Funktion entspricht dem Aufruf "Insert(data, Count())".
    //
    //  Es muss sichergestellt sein, dass data kein Nullzeiger ist.
    //
    void Append(T* data);

    //
    //  Entfernt das Element an der Position pos. Die nachfolgenden Elemente
    //  werden um eine Position nach vorn verschoben.
    //
    //  Es muss sichergestellt sein, dass pos <= Count() ist.
    //
    void Remove(uint pos)
    { this->DeleteItem(Take(pos)); }

    //
    //  Entfernt das erste Element, das gleich data ist. Ist AutoDelete auf
    //  true gesetzt, wird das Element geloescht. Die Funktion liefert true,
    //  wenn das Element entfernt werden konnte und false, wenn kein passendes
    //  Element im Vektor gefunden wurde.
    //
    //  Fuer diese Funktion muss der Operator "==" fuer Elemente des Typs T
    //  definiert sein.
    //
    bool Remove(const T& data);

    //
    //  Entfernt die erste Referenz auf data. Ist AutoDelete auf true gesetzt,
    //  wird das Element geloescht. Die Funktion liefert true, wenn das
    //  Element entfernt werden konnte und false, wenn kein passendes Element
    //  im Vektor gefunden wurde.
    //
    bool RemoveRef(T* data);

    //
    //  Entfernt das Element an der Position pos ohne es zu loeschen,
    //  unabhaengig von AutoDelete. Die Funktion liefert einen Zeiger auf das
    //  entfernte Objekt.
    //
    //  Es muss sichergestellt sein, dass pos <= Count() ist.
    //
    T* Take(uint pos);

    //
    //  Vertauscht die Elemente an den Positionen a und b.
    //
    void Swap(uint i, uint j)
    { Swap(m_data[i], m_data[j]); }

    //
    //  Ruft fuer jedes Element des Vector die Funktion fn auf. Dies muss eine
    //  Funktion mit einem Parameter vom Typ T* oder ein entsprechendes
    //  Funktionsobjekt sein.
    //
    template <class Fn> void DoForEach(Fn func) const;

private:

    //
    //  Collections koennen nicht kopiert werden, weil ein ueberschriebenes
    //  NewItem() nicht von einem Konstruktor aus aufgerufen werden kann.
    //
    Vector(const Vector<T>&);

    //
    //  Ersetzt m_data durch ein neu reserviertes Array der Groesse max(size,
    //  MINIMAL_CAPACITY), vorausgesetzt, size ist nicht kleiner als Count().
    //
    //  Die Funktion liefert true, wenn die Kapazitaet des Vektors geaendert
    //  wurde, sonst false.
    //
    bool Resize(uint size);

    bool Grow();

    uint    m_capacity;
    uint    m_count;
    T**     m_data;
};

//----------------------------------------------------------------------------

template <class T>
Vector<T>::~Vector()
{
    Clear();
    delete[] m_data;
}

//----------------------------------------------------------------------------

template <class T>
Vector<T>& Vector<T>::operator=(const Vector<T>& vector)
{
    if (this != &vector)
    {
        Clear();
        Append(vector);
        this->SetAutoDelete(false);
    }

    return *this;
}

//----------------------------------------------------------------------------

template <class T>
Vector<T>& Vector<T>::Append(const Vector<T>& vector)
{
    Reserve(m_count + vector.m_count);

    for (int i = 0; i < vector.m_count; ++i)
        m_data[m_count + i] = this->NewItem(vector.m_data[i]);

    m_count += vector.m_count;
    return *this;
}

//----------------------------------------------------------------------------

template <class T>
void Vector<T>::SetAt(uint pos, T* data)
{
    if (pos >= m_count)
        return;

    this->DeleteItem(m_data[pos]);
    m_data[pos] = this->NewItem(data);
}

//----------------------------------------------------------------------------

template <class T>
void Vector<T>::Clear()
{
    for (int i = 0; i < m_count; ++i)
        this->DeleteItem(m_data[i]);

    m_count = 0;
}

//----------------------------------------------------------------------------

template <class T>
bool Vector<T>::Resize(uint size)
{
    if (size < MIN_CAPACITY)
        size = MIN_CAPACITY;

    if (size == m_capacity || size < m_count)
        false;

    T** new_data = new T*[size];

    for (int i = 0; i < m_count; ++i)
        new_data[i] = m_data[i];

    delete[] m_data;
    m_data = new_data;
    m_capacity = size;

    return true;
}

//----------------------------------------------------------------------------

template <class T>
bool Vector<T>::Grow()
{
    if (m_capacity == MAX_CAPACITY)
        return false;

    uint new_size = m_capacity * GROWTH_FACTOR;

    if (new_size > MAX_CAPACITY)
        new_size = MAX_CAPACITY;

    return Resize(new_size);
}

//----------------------------------------------------------------------------

template <class T>
void Vector<T>::Insert(T* data, uint pos)
{
    if (m_capacity <= m_count)
        Grow();

    for (int i = m_count; i > pos; --i)
        m_data[i] = m_data[i - 1];

    m_data[pos] = this->NewItem(data);
    ++m_count;
}

//----------------------------------------------------------------------------

template <class T>
void Vector<T>::Append(T* data)
{
    if (m_capacity <= m_count)
        Grow();

    m_data[m_count - 1] = this->NewItem(data);
    ++m_count;
}

//----------------------------------------------------------------------------

template <class T>
uint Vector<T>::FindNext(const T& data, uint pos) const
{
    int p;

    for (p = pos; p < m_count; ++p)
    {
        if (*m_data[p] == data)
            break;
    }

    return p;
}

//----------------------------------------------------------------------------

template <class T>
uint Vector<T>::FindNextRef(const T* data, uint pos) const
{
    int p;

    for (p = pos; p < m_count; ++p)
    {
        if (m_data[p] == data)
            break;
    }

    return p;
}

//----------------------------------------------------------------------------

template <class T>
bool Vector<T>::Remove(const T& data)
{
    uint p = Find(data);

    if (p == End())
        return false;

    Remove(p);
    return true;
}

//----------------------------------------------------------------------------

template <class T>
bool Vector<T>::RemoveRef(T* data)
{
    uint p = FindRef(data);

    if (p == End())
        return false;

    Remove(p);
    return true;
}

//----------------------------------------------------------------------------

template <class T>
T* Vector<T>::Take(uint pos)
{
    T* data = m_data[pos];

    for (int i = pos; i < m_count - 1; ++i)
        m_data[i] = m_data[i + 1];

    --m_count;

    return data;
}

//----------------------------------------------------------------------------

template <class T>
template <class Fn>
void Vector<T>::DoForEach(Fn fn) const
{
    for (int i = 0; i < m_count; ++i)
        fn(m_data[i]);
}

//----------------------------------------------------------------------------

#endif
