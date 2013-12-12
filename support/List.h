#ifndef support_List_h
#define support_List_h

#include "support/Collection.h"
#include "support/Utilities.h"

template <class T>
class ListIterator;

template <class T>
class ListConstIterator;

//----------------------------------------------------------------------------
//
//  Template-Klasse, die eine doppelt verkettete Liste bereitstellt.
//
//  Listen speichern nur Zeiger auf Elemente (shallow copy). Soll dieses
//  Verhalten geaendert werden, d.h. sollen Kopien der Elemente gespeichert
//  werden (deep copy), muss die Funktion NewElement() dementsprechend
//  ueberschrieben werden.
//
//  Listenelemente, die aus der Liste entfernt werden, koennen automatisch
//  mit delete geloescht werden. Dazu muss mit SetAutoDelete() die AutoDelete-
//  Option auf true gesetzt werden. Die Standardvorgabe ist false, d.h. die
//  Elemente werden nicht geloescht.
//
template <class T>
class List: public Collection<T>
{
public:

    typedef ListIterator<T>         Iterator;
    typedef ListConstIterator<T>    ConstIterator;

    //
    //  Erstellt eine leere Liste.
    //
    List()
        : m_count(0), m_end(&m_end, &m_end) {}

    //
    //  Entfernt alle Elemente aus der Liste und zerstoert das Listenobjekt.
    //  Wenn AutoDelete auf true gesetzt ist, werden auch die Elemente
    //  geloescht.
    //
    virtual ~List()
    { Clear(); }

    //
    //  Weist der Liste eine Kopie der Liste list zu. Dabei werden nur die
    //  Zeiger auf die Elemente kopiert. Ist die Liste vorher nicht leer,
    //  werden alle Elemente entfernt und - falls AutoDelete auf true gesetzt
    //  ist - geloescht.
    //
    List& operator=(const List<T>& list);

    //
    //  Kopiert die Liste list an das Ende der Liste. Dabei werden nur die
    //  Zeiger auf die Elemente kopiert.
    //
    List& operator+=(const List<T>& list)
    { Append(list); return *this; }

    //
    //  Liefert die Anzahl der Elemente in der Liste.
    //
    uint Count() const
    { return m_count; }

    //
    //  Liefert das erste Element der Liste. Die Liste darf nicht leer sein.
    //
    T* First() const
    { return static_cast<Node*>(m_end.next)->data; }

    //
    //  Liefert einen Iterator, der auf das erste Element in der Liste zeigt.
    //
    Iterator Begin()
    { return Iterator(m_end.next); }

    ConstIterator Begin() const
    { return ConstIterator(m_end.next); }

    //
    //  Liefert das letzte Element der Liste. Die Liste darf nicht leer sein.
    //
    T* Last() const
    { return static_cast<Node*>(m_end.prev)->data; }

    //
    //  Liefert einen Iterator, der hinter das letzte Element in der Liste
    //  zeigt.
    //
    Iterator End()
    { return Iterator(&m_end); }

    ConstIterator End() const
    { return ConstIterator(&m_end); }

    //
    //  Liefert einen Iterator, der auf das erste Element zeigt, das gleich
    //  data ist. Wird kein Element gleich data gefunden, liefert die Funktion
    //  einen Iterator, der auf End() zeigt.
    //
    //  Fuer diese Funktion muss der Operator "==" fuer Elemente des Typs T
    //  definiert sein.
    //
    Iterator Find(const T& data)
    { return FindNext(data, Begin()); }

    ConstIterator Find(const T& data) const
    { return FindNext(data, Begin()); }

    //
    //  Sucht ausgehend von der Position pos das naechste Element, das gleich
    //  data ist und liefert einen Iterator, der auf dieses Element zeigt,
    //  bzw. auf End(), wenn kein solches Element gefunden wird.
    //
    //  Fuer diese Funktion muss der Operator "==" fuer Elemente des Typs T
    //  definiert sein.
    //
    Iterator FindNext(const T& data, const Iterator& pos);
    ConstIterator FindNext(const T& data, const ConstIterator& pos) const;

    //
    //  Liefert einen Iterator, der auf das erste Element zeigt, das eine
    //  Referenz auf data ist. Wird ein solches Element nicht gefunden, wird
    //  ein Iterator zurueckgeliefert, der auf End() zeigt.
    //
    Iterator FindRef(const T* data)
    { return FindNextRef(data, Begin()); }

    ConstIterator FindRef(const T* data) const
    { return FindNextRef(data, Begin()); }

    //
    //  Sucht ausgehend von der Position pos das naechste Element, das eine
    //  Referenz auf data ist und liefert einen Iterator, der auf dieses
    //  Element zeigt, bzw. auf End(), wenn kein solches Element gefunden
    //  wird.
    //
    Iterator FindNextRef(const T* data, const Iterator& pos);
    ConstIterator FindNextRef(const T* data, const ConstIterator& pos) const;

    //
    //  Liefert true, wenn die Liste mindestens ein Element gleich data
    //  enthaelt, sonst false.
    //
    bool Contains(const T& data) const
    { return Find(data) != End(); }

    //
    //  Liefert true, wenn die Liste mindestens eine Referenz auf data
    //  enthaelt.
    //
    bool ContainsRef(const T* data) const
    { return FindRef(data) != End(); }

    //
    //  Entfernt alle Elemente aus der Liste. Ist AutoDelete auf true gesetzt,
    //  werden die Elemente danach mit delete geloescht.
    //
    void Clear();

    //
    //  Fuegt data vor das Element an der Position pos in die Liste ein. Es
    //  wird nur der Zeiger auf data kopiert, solange NewItem() nicht
    //  ueberschrieben wird.
    //
    void Insert(T* data, const Iterator& pos);

    //
    //  Haengt data an das Ende der Liste an. Es wird nur der Zeiger auf data
    //  kopiert, solange NewItem() nicht ueberschrieben wird.
    //
    List<T>& Append(T* data)
    { Insert(data, End()); return *this; }

    //
    //  Kopiert die Liste list an das Ende der Liste. Dabei werden nur die
    //  Zeiger auf die Elemente kopiert.
    //
    List<T>& Append(const List<T>& list);

    //
    //  Entfernt das Element an der Position pos aus der Liste. Ist AutoDelete
    //  auf true gesetzt, wird das Element geloescht. Der Iterator pos zeigt
    //  danach auf das Element hinter dem geloeschten Element.
    //
    void Remove(Iterator& pos)
    { this->DeleteItem(Take(pos)); }

    //
    //  Entfernt das erste Element, das gleich data ist, aus der Liste. Ist
    //  AutoDelete auf true gesetzt, wird das Element geloescht. Die Funktion
    //  liefert true, wenn das Element entfernt werden konnte und false, wenn
    //  kein passendes Element in der Liste gefunden wurde.
    //
    //  Fuer diese Funktion muss der Operator "==" fuer Elemente des Typs T
    //  definiert sein.
    //
    bool Remove(const T& data);

    //
    //  Entfernt die erste Referenz auf data aus der Liste. Ist AutoDelete auf
    //  true gesetzt, wird das Element geloescht. Die Funktion liefert true,
    //  wenn das Element entfernt werden konnte und false, wenn kein passendes
    //  Element in der Liste gefunden wurde.
    //
    bool RemoveRef(T* data);

    //
    //  Entfernt das Element an der Position pos aus der Liste ohne es zu
    //  loeschen, unabhaengig von AutoDelete. Die Funktion liefert einen
    //  Zeiger auf das entfernte Objekt. Der Iterator pos zeigt danach auf das
    //  Element hinter dem entfernten Element.
    //
    T* Take(Iterator& pos);

    //
    //  Vertauscht die Elemente an den Positionen a und b. Die Iteratoren
    //  bleiben weiterhin gueltig (an den neuen Positionen).
    //
    void Swap(const Iterator& a, const Iterator& b)
    { Swap(a.m_node->data, b.m_node->data); }

    //
    //  Ruft fuer jedes Element der Liste die Funktion fn auf. Dies muss eine
    //  Funktion mit einem Parameter vom Typ T* ode ein entsprechendes
    //  Funktionsobjekt sein.
    //
    template <class Fn> void DoForEach(Fn fn) const;

private:

    //
    //  Collections koennen nicht kopiert werden, weil ein ueberschriebenes
    //  NewItem() nicht von einem Konstruktor aus aufgerufen werden kann.
    //
    List(const List<T>&);

    friend class ListIterator<T>;
    friend class ListConstIterator<T>;

    struct Link
    {
        Link() {}
        Link(Link* next, Link* prev)
            : next(next), prev(prev) {}

        Link* next;
        Link* prev;
    };

    struct Node: public Link
    {
        Node(T* data): data(data) {}
        Node(T* data, Link* next, Link* prev)
            : Link(next, prev), data(data) {}

        T* data;
    };

    inline void LinkNodes(Link* prev, Link* next)
    {
        prev->next = next;
        next->prev = prev;
    }

    inline void UnlinkNode(Link* node)
    { LinkNodes(node->prev, node->next); }

    uint    m_count;
    Link    m_end;
};

//----------------------------------------------------------------------------

template <class T>
bool operator==(const ListIterator<T>&, const ListConstIterator<T>&);

template <class T>
bool operator!=(const ListIterator<T>&, const ListConstIterator<T>&);

//----------------------------------------------------------------------------
//
//  Stellt einen Iterator fuer List-Collections bereit.
//
template <class T>
class ListIterator
{
    typedef typename List<T>::Link      Link;
    typedef typename List<T>::Node      Node;

public:

    //
    //  Erstellt einen Listen-Iterator, der auf kein Element zeigt. Vor der
    //  Verwendung muss sichergestellt werden, dass der Iterator auf ein
    //  gueltiges Element zeigt.
    //
    ListIterator()
        : m_node(nullptr) {}

    //
    //  Setzt den Iterator eine Position weiter. Der Iterator darf nicht auf
    //  das Listenende (hinter das letzte Element) zeigen.
    //
    ListIterator& operator++()
    { m_node = m_node->next; return *this; }

    //
    //  Setzt den Iterator eine Position zurueck. Der Iterator darf nicht auf
    //  den Listenanfang (auf das erste Element) zeigen.
    //
    ListIterator& operator--()
    { m_node = m_node->prev; return *this; }

    //
    //  Liefert die Adresse des Elements, auf das der Iterator aktuell zeigt.
    //  Der Iterator muss auf ein gueltiges Element zeigen.
    //
    T* Get() const
    { return static_cast<Node*>(m_node)->data; }

    //
    //  Ersetzt das Element, auf das der Iterator aktuell zeigt, durch data.
    //  Der Iterator muss auf ein gueltiges Element zeigen.
    //
    void Set(T* data);

    //
    //  Liefert die Adresse des Elements, auf das der Iterator aktuell zeigt.
    //  Der Iterator muss auf ein gueltiges Element zeigen.
    //
    T* operator->() const
    { return static_cast<Node*>(m_node)->data; }

    //
    //  Testet, ob der Iterator auf das gleiche Element zeigt wie it.
    //
    bool operator==(const ListIterator<T>& it) const
    { return m_node == it.m_node; }

    //
    //  Testet, ob der Iterator auf ein anderes Element zeigt als it.
    //
    bool operator!=(const ListIterator<T>& it) const
    { return m_node != it.m_node; }

private:

    friend class List<T>;
    friend class ListConstIterator<T>;

    friend bool ::operator==<>(const ListIterator<T>&,
                               const ListConstIterator<T>&);
    friend bool ::operator!=<>(const ListIterator<T>&,
                               const ListConstIterator<T>&);

    explicit ListIterator(Link* node)
        : m_node(node) {}

    Link* m_node;
};

//----------------------------------------------------------------------------
//
//  Stellt einen Const-Iterator fuer List-Collections bereit.
//
template <class T>
class ListConstIterator
{
    typedef const typename List<T>::Link    Link;
    typedef const typename List<T>::Node    Node;

public:

    //
    //  Erstellt einen Listen-Iterator, der auf kein Element zeigt. Vor der
    //  Verwendung muss sichergestellt werden, dass der Iterator auf ein
    //  gueltiges Element zeigt.
    //
    ListConstIterator()
        : m_node(nullptr) {}

    //
    //  Erstellt einen Const-Iterator als Kopie des Iterators it.
    //
    ListConstIterator(const ListIterator<T>& it)
        : m_node(it.m_node) {}

    //
    //  Weist dem Iterator die Position des Iterators it zu.
    //
    ListConstIterator& operator=(const ListIterator<T>& it)
    {
        if (this != &it)
            m_node = it.m_node;

        return *this;
    }

    //
    //  Setzt den Iterator eine Position weiter. Der Iterator darf nicht auf
    //  das Listenende (hinter das letzte Element) zeigen.
    //
    ListConstIterator& operator++()
    { m_node = m_node->next; return *this; }

    //
    //  Setzt den Iterator eine Position zurueck. Der Iterator darf nicht auf
    //  den Listenanfang (auf das erste Element) zeigen.
    //
    ListConstIterator& operator--()
    { m_node = m_node->prev; return *this; }

    //
    //  Liefert die Adresse des Elements, auf das der Iterator aktuell zeigt.
    //  Der Iterator muss auf ein gueltiges Element zeigen.
    //
    T* Get() const
    { return static_cast<Node*>(m_node)->data; }

    //
    //  Liefert die Adresse des Elements, auf das der Iterator aktuell zeigt.
    //  Der Iterator muss auf ein gueltiges Element zeigen.
    //
    T* operator->() const
    { return static_cast<Node*>(m_node)->data; }

    //
    //  Testet, ob der Iterator auf das gleiche Element zeigt wie it.
    //
    bool operator==(const ListConstIterator<T>& it) const
    { return m_node == it.m_node; }

    //
    //  Testet, ob der Iterator auf ein anderes Element zeigt als it.
    //
    bool operator!=(const ListConstIterator<T>& it) const
    { return m_node != it.m_node; }

private:

    friend class List<T>;

    friend bool ::operator==<>(const ListIterator<T>&,
                               const ListConstIterator<T>&);
    friend bool ::operator!=<>(const ListIterator<T>&,
                               const ListConstIterator<T>&);

    explicit ListConstIterator(Link* node)
        : m_node(node) {}

    Link* m_node;
};

//----------------------------------------------------------------------------

template <class T>
inline bool operator==(const ListIterator<T>& lhs,
                       const ListConstIterator<T>& rhs)
{ return lhs.m_node == rhs.m_node; }

template <class T>
inline bool operator!=(const ListIterator<T>& lhs,
                       const ListConstIterator<T>& rhs)
{ return lhs.m_node != rhs.m_node; }

//----------------------------------------------------------------------------

template <class T>
List<T>& List<T>::operator=(const List<T>& list)
{
    if (this != &list)
    {
        Clear();
        Append(list);
        this->SetAutoDelete(false);
    }

    return *this;
}

//----------------------------------------------------------------------------

template <class T>
typename List<T>::Iterator
List<T>::FindNext(const T& data, const Iterator& pos)
{
    ConstIterator end = End();
    Iterator p = pos;

    for (; p != end; ++p)
    {
        if (*p.Get() == data)
            break;
    }

    return p;
}

//----------------------------------------------------------------------------

template <class T>
typename List<T>::ConstIterator
List<T>::FindNext(const T& data, const ConstIterator& pos) const
{
    ConstIterator end = End();
    ConstIterator p = pos;

    for (; p != end; ++p)
    {
        if (*p.Get() == data)
            break;
    }

    return p;
}

//----------------------------------------------------------------------------

template <class T>
typename List<T>::Iterator
List<T>::FindNextRef(const T* data, const Iterator& pos)
{
    ConstIterator end = End();
    Iterator p = pos;

    for (; p != end; ++p)
    {
        if (p.Get() == data)
            break;
    }

    return p;
}

//----------------------------------------------------------------------------

template <class T>
typename List<T>::ConstIterator
List<T>::FindNextRef(const T* data, const ConstIterator& pos) const
{
    ConstIterator end = End();
    ConstIterator p = pos;

    for (; p != end; ++p)
    {
        if (p.Get() == data)
            break;
    }

    return p;
}

//----------------------------------------------------------------------------

template <class T>
void List<T>::Clear()
{
    while (m_end.next != &m_end)
    {
        Node* node = static_cast<Node*>(m_end.next);
        m_end.next = node->next;
        this->DeleteItem(node->data);
        delete node;
    }

    m_end.prev = &m_end;
    m_count = 0;
}

//----------------------------------------------------------------------------

template <class T>
void List<T>::Insert(T* data, const Iterator& pos)
{
    Node* new_node = new Node(this->NewItem(data));
    LinkNodes(pos.m_node->prev, new_node);
    LinkNodes(new_node, pos.m_node);
    ++m_count;
}

//----------------------------------------------------------------------------

template <class T>
List<T>& List<T>::Append(const List<T>& list)
{
    ConstIterator end = list.End();

    for (ConstIterator p = list.Begin(); p != end; ++p)
        Append(p.Get());

    return *this;
}

//----------------------------------------------------------------------------

template <class T>
bool List<T>::Remove(const T& data)
{
    Iterator p = Find(data);

    if (p == End())
        return false;

    Remove(p);
    return true;
}

//----------------------------------------------------------------------------

template <class T>
bool List<T>::RemoveRef(T* data)
{
    Iterator p = FindRef(data);

    if (p == End())
        return false;

    Remove(p);
    return true;
}

//----------------------------------------------------------------------------

template <class T>
T* List<T>::Take(Iterator& pos)
{
    Node* node = static_cast<Node*>(pos.m_node);
    T* data = node->data;

    ++pos;
    UnlinkNode(node);
    delete node;
    --m_count;

    return data;
}

//----------------------------------------------------------------------------

template <class T>
template <class Fn>
void List<T>::DoForEach(Fn fn) const
{
    ConstIterator p = Begin();
    ConstIterator end = End();

    while (p != end)
    {
        fn(p.Get());
        ++p;
    }
}

//----------------------------------------------------------------------------

template <class T>
void ListIterator<T>::Set(T* data)
{
    Node* node = static_cast<Node*>(m_node);

    this->DeleteItem(node->data);
    node->data = this->NewItem(data);
}

//----------------------------------------------------------------------------

#endif
