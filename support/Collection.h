#ifndef support_Collection_h
#define support_Collection_h

//----------------------------------------------------------------------------
//
//  Basisklasse fuer alle Collections (Container).
//
//  Collection bildet eine abstrakte Basisklasse fuer Collection-Klassen wie
//  Listen, Dictionaries usw. Collection verwaltet nur die "AutoDelete" -
//  Funktion, d.h. ob Elemente geloescht werden sollen, wenn sie aus der
//  Collection entfernt werden.
//
template <class T>
class Collection
{
public:

    //
    //  Setzt die AutoDelete-Option der Collection.
    //
    //  Wird AutoDelete erlaubt (enable ist true), so werden Objekte die aus
    //  der Collection entfernt werden automatisch mittels delete geloescht.
    //  Das ist sinnvoll, wenn die Collection die einzigen Referenzen auf
    //  diese Objekte enthaelt.
    //
    //  Wird AutoDelete nicht erlaubt (enable ist false), so werden die
    //  entfernten Objekte nicht geloescht. Dies ist die Standardeinstellung.
    //
    void SetAutoDelete(bool enable)
    { m_autoDelete = enable; }

    //
    //  Liefert den Status der AutoDelete-Option. Die Standardeinstellung ist
    //  false.
    //
    bool AutoDelete() const
    { return m_autoDelete; }

protected:

    //
    //  Erstellt eine Collection.
    //
    Collection()
        : m_autoDelete(false) {}

    //
    //  Zerstoert eine Collection.
    //
    virtual ~Collection() {}

    //
    //  Erstellt ein neues Datenelement. Die Standardimplementation liefert
    //  einfach nur den Zeiger data zurueck, d.h. es werden nur die Zeiger auf
    //  die Elemente kopiert (shallow copy).
    //
    //  Sollen Kopien der Elemente gespeichert werden (deep copy), so muss
    //  diese Funktion ueberschrieben werden.
    //
    virtual T* NewItem(T* data)
    { return data; }

    //
    //  Loescht ein Datenelement. Die Standardimplementation ruft fuer das
    //  Element data delete auf, wenn AutoDelete auf true gesetzt ist. Ist
    //  AutoDelete auf false, tut diese Funktion ueberhaupt nichts.
    //
    virtual void DeleteItem(T* data)
    { if (m_autoDelete) delete data; }

private:

    //
    //  Collections koennen nicht kopiert werden, weil ein ueberschriebenes
    //  NewItem() nicht von einem Konstruktor aus aufgerufen werden kann.
    //
    Collection(const Collection<T>&);

    //
    //  Abstrakte Collections koennen nicht zugewiesen werden.
    //
    Collection<T>& operator=(const Collection<T>&);

    bool m_autoDelete;
};

//----------------------------------------------------------------------------

#endif
