#ifndef support_Dict_h
#define support_Dict_h

#include "support/Collection.h"
#include "support/Hash.h"
#include "support/Utilities.h"

//----------------------------------------------------------------------------
//
//  Template-Klasse, die eine Hashtabelle bereitstellt.
//
//  Die Dict-Klasse stellt eine Datenstruktur bereit, die es gestattet,
//  Elemente anhand von Schluesseln zu identifizieren. Dies wird ueber
//  eine Hashtabelle realisiert.
//
//  Wichtig fuer eine gute Performance der Struktur ist die Groesse und ihre
//  Auslastung. Idealerweise ist als Groesse eine Primzahl zu waehlen, die
//  groesser als die zu erwartende maximale Anzahl der Elemente ist. Wird die
//  Auslastung der Tabelle zu hoch, d.h. ist Count() >> Size(), sollte die
//  Groesse der Tabelle mit der Funktion Resize() angepasst werden.
//
//  Dicts speichern nur Zeiger auf Elemente (shallow copy). Soll dieses
//  Verhalten geaendert werden, d.h. sollen Kopien der Elemente gespeichert
//  werden (deep copy), muss die Funktion NewElement() dementsprechend
//  ueberschrieben werden.
//
//  Elemente, die aus der Struktur entfernt werden, koennen automatisch
//  mit delete geloescht werden. Dazu muss mit SetAutoDelete() die AutoDelete-
//  Option auf true gesetzt werden. Die Standardvorgabe ist false, d.h. die
//  Elemente werden nicht geloescht.
//
template <class Key, class T, class Hash = DefaultHash<Key> >
class Dict: public Collection<T>
{
public:

    //
    //  Erstelle ein leeres Dictionary der Groesse size. Als Groesse sollte
    //  eine grosse Primzahl gewaehlt werden, um optimale Performance zu
    //  erzielen.
    //
    Dict(int size = 17);

    //
    //  Entfernt alle Elemente aus dem Dictionary und zerstoert das Objekt.
    //  Wenn AutoDelete auf true gesetzt ist, werden auch die Elemente
    //  geloescht.
    //
    virtual ~Dict();

    //
    //  Liefert die Groesse der Hashtabelle.
    //
    //  Dict-Objekte koennen beliebig viele Elemente enthalten. Um jedoch eine
    //  gute Performance zu erzielen, sollte die Anzahl der Elemente die
    //  Groesse der Hashtabelle nicht uebersteigen. Die Groesse der Tabelle
    //  kann mit Resize() angepasst werden.
    //
    uint Size() const
    { return m_size; }

    //
    //  Liefert die Anzahl der in der Hashtabelle gespeicherten Elemente.
    //
    uint Count() const
    { return m_count; }

    //
    //  Weist dem Objekt eine Kopie des Dictionaries dict zu. Dabei werden nur
    //  die Zeiger auf die Elemente kopiert. Ist das Dictionary vorher nicht
    //  leer, werden alle Elemente entfernt und - falls AutoDelete auf true
    //  gesetzt ist - geloescht. Danach wird AutoDelete auf false gesetzt.
    //
    Dict<Key, T, Hash>& operator=(const Dict<Key, T, Hash>& dict);

    //
    //  Liefert einen Zeiger auf das erste Element mit dem Schluessel key
    //  oder 0, wenn kein solches Element existiert. Dieser Operator
    //  entspricht der Funktion Find().
    //
    T* operator[](const Key& key) const
    { return Find(key); }

    //
    //  Entfernt alle Elemente aus dem Dictionary. Ist AutoDelete auf true
    //  gesetzt, werden die Elemente danach mit delete geloescht.
    //
    void Clear();

    //
    //  Fuegt das Element data unter dem Schluessel key in das Dictionary ein.
    //  Dabei wird standardmaessig nur der Zeiger auf das Element kopiert.
    //  Soll dieses Verhalten geaendert werden, muss die Funktion NewItem()
    //  ueberschrieben werden.
    //
    void Insert(const Key& key, T* data);

    //
    //  Ersetzt das zuletzt unter dem Schluessel key eingefuegte Element
    //  durch data. Wenn kein Element unter diesem Schluessel existiert, wird
    //  data neu eingefuegt.
    //
    void Replace(const Key& key, T* data);

    //
    //  Entfernt das zuletzt unter dem Schluessel key eingefuegte Element. Ist
    //  AutoDelete auf true gesetzt, wird das Element geloescht. Die Funktion
    //  liefert true, wenn das Element entfernt wurde, und false, wenn kein
    //  Element unter diesem Schluessel existiert.
    //
    bool Remove(const Key& key);

    //
    //  Entfernt das zuletzt unter dem Schluessel key eingefuegte Element aus
    //  dem Dictionary ohne es zu loeschen, auch wenn AutoDelete auf true
    //  gesetzt ist.  Die Funktion liefert einen Zeiger auf das entfernte
    //  Element, oder 0, wenn kein solches Element gefunden wird.
    //
    T* Take(const Key& key);

    //
    //  Liefert einen Zeiger auf das erste Element mit dem Schluessel key,
    //  oder 0, wenn kein solches Element existiert.
    //
    T* Find(const Key& key) const;

    //
    //  Aendert die Groesse der Hashtabelle auf den Wert von size. Diese
    //  Operation ist sinnvoll, wenn das Dictionary zu viele Elemente enthaelt
    //  und dadurch die Performance sinkt.
    //
    //  Da die Resize()-Operation relativ aufwendig ist, sollte sie nach
    //  Moeglichkeit dadurch vermieden werden, dass von vornherein ein
    //  Dictionary mit ausreichender Groesse angelegt wird.
    //
    //  Der Wert von size sollte eine grosse Primzahl sein, um eine gute
    //  Performance der Struktur zu erzielen.
    //
    void Resize(uint size);

    //
    //  Ruft fuer jedes Element des Dictionaries die Funktion fn auf. Dies
    //  muss eine Funktion mit zwei Parametern der Typen Key und T* oder ein
    //  entsprechendes Funktionsobjekt sein.
    //
    template <class Fn> void DoForEach(Fn func) const;

private:

    //
    //  Collections koennen nicht kopiert werden, weil ein ueberschriebenes
    //  NewItem() nicht von einem Konstruktor aus aufgerufen werden kann.
    //
    Dict(const Dict<Key, T, Hash>&);

    struct Node
    {
        Node(const Key& key, T* data, Node* next)
            : key(key), data(data), next(next) {}

        Key     key;
        T*      data;
        Node*   next;
    };

    //
    //  Berechnet den Hashwert des Schluessels key.
    //
    uint Lookup(const Key& key) const
    { return m_hasher(key) % m_size; }

    //
    //  Berechnet den Hashwert des Schluessels key fuer eine Tabelle der
    //  Groesse size.
    //
    uint Lookup(const Key& key, uint size) const
    { return m_hasher(key) % size; }

    //
    //  Erstellt das Dictionary als Kopie von dict. Diese Funktion loescht
    //  nicht das Array m_nodes!
    //
    void _Copy(const Dict<Key, T, Hash>& dict);

    uint    m_size;
    uint    m_count;
    Node**  m_nodes;
    Hash    m_hasher;
};

//----------------------------------------------------------------------------

template <class Key, class T, class Hash>
Dict<Key, T, Hash>::Dict(int size)
    : m_size(size), m_count(0), m_nodes(new Node*[size])
{
    for (int i = 0; i < size; ++i)
        m_nodes[i] = nullptr;
}

//----------------------------------------------------------------------------

template <class Key, class T, class Hash>
Dict<Key, T, Hash>::~Dict()
{
    Clear();
    delete[] m_nodes;
}

//----------------------------------------------------------------------------

template <class Key, class T, class Hash>
Dict<Key, T, Hash>& Dict<Key, T, Hash>::operator=(const Dict<Key, T, Hash>& dict)
{
    if (this != &dict)
    {
        Clear();
        delete[] m_nodes;

        _Copy(dict);
        this->SetAutoDelete(false);
    }

    return *this;
}

//----------------------------------------------------------------------------

template <class Key, class T, class Hash>
void Dict<Key, T, Hash>::Clear()
{
    for (int i = 0; i < m_size; ++i)
    {
        while (m_nodes[i] != nullptr)
        {
            Node* node = m_nodes[i];
            m_nodes[i] = node->next;
            this->DeleteItem(node->data);
            delete node;
        }
    }

    m_count = 0;
}

//----------------------------------------------------------------------------

template <class Key, class T, class Hash>
void Dict<Key, T, Hash>::Insert(const Key& key, T* data)
{
    uint index = Lookup(key);
    m_nodes[index] = new Node(key, this->NewItem(data), m_nodes[index]);
    ++m_count;
}

//----------------------------------------------------------------------------

template <class Key, class T, class Hash>
void Dict<Key, T, Hash>::Replace(const Key& key, T* data)
{
    Remove(key);
    Insert(key, data);
}

//----------------------------------------------------------------------------

template <class Key, class T, class Hash>
bool Dict<Key, T, Hash>::Remove(const Key& key)
{
    T* data = Take(key);

    if (!data)
        return false;

    this->DeleteItem(data);
    return true;
}

//----------------------------------------------------------------------------

template <class Key, class T, class Hash>
T* Dict<Key, T, Hash>::Take(const Key& key)
{
    Node** node_ptr = &m_nodes[Lookup(key)];

    while (*node_ptr != nullptr)
    {
        Node* node = *node_ptr;

        if (node->key == key)
        {
            T* data = node->data;
            *node_ptr = node->next;

            delete node;
            --m_count;
            return data;
        }

        node_ptr = &node->next;
    }

    return nullptr;
}

//----------------------------------------------------------------------------

template <class Key, class T, class Hash>
T* Dict<Key, T, Hash>::Find(const Key& key) const
{
    Node* node = m_nodes[Lookup(key)];

    while (node != nullptr)
    {
        if (node->key == key)
            return node->data;

        node = node->next;
    }

    return nullptr;
}

//----------------------------------------------------------------------------

template <class Key, class T, class Hash>
void Dict<Key, T, Hash>::Resize(uint size)
{
    Node** new_table = new Node*[size];

    for (int i = 0; i < size; ++i)
        new_table[i] = nullptr;

    for (int i = 0; i < m_size; ++i)
    {
        Node** node_ptr = &m_nodes[i];

        while (*node_ptr != nullptr)
        {
            Node* node = *node_ptr;
            *node_ptr = node->next;

            int new_index = Lookup(node->key, size);
            node->next = new_table[new_index];
            new_table[new_index] = node;
        }
    }

    delete[] m_nodes;
    m_nodes = new_table;
    m_size = size;
}

//----------------------------------------------------------------------------

template <class Key, class T, class Hash>
void Dict<Key, T, Hash>::_Copy(const Dict<Key, T, Hash>& dict)
{
    m_size = dict.m_size;
    m_count = dict.m_count;
    m_nodes = new Node*[dict.m_size];

    for (int i = 0; i < m_size; ++i)
    {
        m_nodes[i] = nullptr;

        Node** new_node_ptr = &m_nodes[i];
        Node* src_node = dict.m_nodes[i];

        while (src_node != nullptr)
        {
            Node* new_node = new Node(
                src_node->key, this->NewItem(src_node->data), nullptr);

            *new_node_ptr = new_node;
            new_node_ptr = &new_node->next;
            src_node = src_node->next;
        }
    }
}

//----------------------------------------------------------------------------

template <class Key, class T, class Hash>
template <class Fn>
void Dict<Key, T, Hash>::DoForEach(Fn fn) const
{
    for (int i = 0; i < m_size; ++i)
    {
        Node* node = m_nodes[i];

        while (node != nullptr)
        {
            fn(node->key, node->data);
            node = node->next;
        }
    }
}

//----------------------------------------------------------------------------

#endif
