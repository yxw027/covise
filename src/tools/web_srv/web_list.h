/* This file is part of COVISE.

   You can use it under the terms of the GNU Lesser General Public License
   version 2.1 or later, see lgpl-2.1.txt.

 * License: LGPL 2+ */

#ifndef LIST_H
#define LIST_H

#define CONT 1 // CONTAINER TYPE LIST - with data dealloc

template <class T>
class Liste;
template <class T>
class ListeElement;

template <class T>
class ListeElement // element of the list
{
    friend class Liste<T>;

private:
    T *data; // pointer to data
    ListeElement<T> *next; // pointer to next list element
    ListeElement() // initialize with 0
    {
        data = NULL;
        next = NULL;
    };
    ListeElement(T *d) // initialize with data
    {
        data = d;
        next = NULL;
    };
    ~ListeElement() // destructor
        {};
    void del_data()
    {
        if (data)
            delete data;
    };
};

template <class T>
class Liste // list class
{
private:
    int m_type; // 0/1 - without/with data dealloc
    int m_nelem; // number of elements of the list
    ListeElement<T> *list; // root of the list
    ListeElement<T> *last; // last element in list
    ListeElement<T> *iter; // iterator for next() requests
    void deleteAll(ListeElement<T> *current);

public:
    Liste()
    {
        m_type = 1;
        m_nelem = 0;
        list = NULL;
        last = NULL;
        iter = NULL;
    };
    Liste(int typ)
    {
        m_type = typ;
        m_nelem = 0;
        list = NULL;
        last = NULL;
        iter = NULL;
    };
    ~Liste();
    void add(T *d); // add new element
    void remove(T *d); // remove element
    T *next(); // get next element (inrements iter)
    void reset() // reset iter to begin of the list
    {
        iter = list;
    };
    T *get_last() // get last element in list
    {
        return last->data;
    };
    int isEmpty()
    {
        return (list == NULL);
    }
    int get_nbList() // get the number of elements
    {
        return m_nelem;
    }
    // in the list
    void empty_list();
};

template <class T>
void Liste<T>::deleteAll(ListeElement<T> *current)
{
    if (NULL != current->next)
    {
        deleteAll(current->next);
    }
    if (m_type)
        current->del_data();
    delete current;
}

template <class T>
Liste<T>::~Liste()
{
    if (NULL != list)
    {
        deleteAll(list);
    }
}

template <class T>
void Liste<T>::empty_list()
{
    if (list == NULL)
        return;

    deleteAll(list);
    m_nelem = 0;
    list = NULL;
    last = NULL;
    iter = NULL;
}

template <class T>
void Liste<T>::add(T *d)
{
    if (list == NULL)
    {
        list = new ListeElement<T>(d);
        m_nelem++;
        return;
    }
    if (list->data == NULL)
    {
        list->data = d;
        return;
    }

    ListeElement<T> *ptr = new ListeElement<T>(d);

    ListeElement<T> *tmpptr = list;
    m_nelem++;
    while (tmpptr->next != NULL)
        tmpptr = tmpptr->next;

    tmpptr->next = ptr;
    last = ptr;

    return;
}

template <class T>
void Liste<T>::remove(T *d)
{
    if (list == NULL)
        return;
    if (list->data == d)
    {
        ListeElement<T> *tmp = list;
        list = list->next;
        if (m_type)
            tmp->del_data();
        delete tmp;
        m_nelem--;
        return;
    }

    ListeElement<T> *ptr = list;

    while (ptr->next != NULL)
    {
        if (ptr->next->data == d)
        {
            if (ptr->next == last)
                last = ptr;
            if (ptr->next == iter)
                iter = ptr;
            ListeElement<T> *tmp = ptr->next;
            ptr->next = tmp->next;
            if (m_type)
                tmp->del_data();
            delete tmp;
            m_nelem--;
            return;
        }
        ptr = ptr->next;
    }
    return;
}

template <class T>
T *Liste<T>::next()
{
    if (iter == NULL)
        return NULL;

    ListeElement<T> *tmpptr;
    tmpptr = iter;
    iter = iter->next;
    return tmpptr->data;
}
#endif
