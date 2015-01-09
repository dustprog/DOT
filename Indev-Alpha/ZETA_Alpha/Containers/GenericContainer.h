#ifndef GENERICCONTAINER_H
#define GENERICCONTAINER_H
#include "NibbleContainer.h"
template<class T>
struct TContainerIterator;
template<class T>
struct TContainerIterator_Short;
#pragma pack(1)
template <class T>
struct TContainer
{
    UNibble Size : 8;
    T *Interface;
public:
    TContainer(UNibble Length)
    {
        this->Interface = new T[Length];
        this->Size = Length;
    }

    TContainer<T>() : Size(0),Interface(nullptr) { }

    T &operator[](UNibble idx) {
        return this->Interface[idx];
    }
    const T &operator[](UNibble idx) const {
        return this->Interface[idx];
    }
    UNibble size() const
    {
        return this->Size;
    }
    void push_back(const T &v) {
        resize(Size+1);
        Interface[Size-1] = v;
    }
    T &front() { return Interface[0]; }
    T &back() { return Interface[Size-1]; }
    void pop_back() { Size--; }
    void push_front(const T&ob) {
        resize(Size+1);
        if(std::is_pod<T>::value) {
            memmove(Interface+1,Interface,sizeof(T)*(Size-1));
        }
        Interface[0] = ob;
    }
    void resize(UNibble n)
    {
        TContainer temp(this->Size);
        for (int i = 0; i < this->Size; i++)
        {
            temp.Interface[i] = this->Interface[i];
        }
        this->Interface = new T[n];
        UNibble n_old = std::min(n, this->Size);
        this->Size = n;

        for (int i = 0; i < n_old; i++)
        {
            this->Interface[i] = temp.Interface[i];
        }
    }
    void insert(T val, UNibble Index)
    {
        TContainer<T> temp(Size);
        for (int i = 0; i < Size; i++)
        {
            temp.Interface[i] = this->Interface[i];
        }
        Size += 1; // make space for new element
        this->Interface = new T[Size];

        for (UNibble i = 0; i < Size; i++)
        {
            if (i == Index)
                Interface[i] = val;
            else if (i > Index)
                Interface[i + 1] = temp.Interface[i];
            else
                Interface[i] = temp.Interface[i];
        }

    }
    void Remove(UNibble Index)
    {
        TContainer<T> temp(Size - 1);
        int j = 0;
        for(int i = 0; i < Size; i++)
        {
            if(i != Index)
            {
                temp.Interface[j] = this->Interface[i];
                j++;
            }
        }
        this->Interface = temp.Interface;
        Size--;
    }

    TContainerIterator<T> begin();
    TContainerIterator<T> end();
    TContainerIterator<T> begin() const;
    TContainerIterator<T> end() const;
};
template <class T>
struct TContainer_Short
{
    unsigned short Size;
    T *Interface;
public:
    TContainer_Short(unsigned short Length)
    {
        this->Interface = new T[Length];
        this->Size = Length;
    }

    TContainer_Short<T>() : Size(0),Interface(nullptr) { }

    T &operator[](unsigned short  idx) {
        return this->Interface[idx];
    }
    const T &operator[](unsigned short idx) const {
        return this->Interface[idx];
    }
    short size() const
    {
        return this->Size;
    }
    void push_back(const T &v) {
        resize(Size+1);
        Interface[Size-1] = v;
    }
    T &front() { return Interface[0]; }
    T &back() { return Interface[Size-1]; }
    void pop_back() { Size--; }
    void push_front(const T&ob) {
        resize(Size+1);
        if(std::is_pod<T>::value) {
            memmove(Interface+1,Interface,sizeof(T)*(Size-1));
        }
        Interface[0] = ob;
    }
    void resize(unsigned short  n)
    {
        TContainer_Short temp(this->Size);
        for (int i = 0; i < this->Size; i++)
        {
            temp.Interface[i] = this->Interface[i];
        }
        this->Interface = new T[n];
        unsigned short n_old = std::min(n, this->Size);
        this->Size = n;

        for (int i = 0; i < n_old; i++)
        {
            this->Interface[i] = temp.Interface[i];
        }
    }
    void insert(T val, unsigned short Index)
    {
        TContainer_Short<T> temp(Size);
        for (int i = 0; i < Size; i++)
        {
            temp.Interface[i] = this->Interface[i];
        }
        Size += 1; // make space for new element
        this->Interface = new T[Size];

        for (unsigned short i = 0; i < Size; i++)
        {
            if (i == Index)
                Interface[i] = val;
            else if (i > Index)
                Interface[i + 1] = temp.Interface[i];
            else
                Interface[i] = temp.Interface[i];
        }

    }
    void Remove(unsigned short Index)
    {
        TContainer_Short<T> temp(Size - 1);
        int j = 0;
        for(unsigned short i = 0; i < Size; i++)
        {
            if(i != Index)
            {
                temp.Interface[j] = this->Interface[i];
                j++;
            }
        }
        this->Interface = temp.Interface;
        Size--;
    }

    TContainerIterator_Short<T> begin();
    TContainerIterator_Short<T> end();
    TContainerIterator_Short<T> begin() const;
    TContainerIterator_Short<T> end() const;
};
#pragma pack(1)
template<class T>
struct TContainerIterator {
    TContainer<T> *Owner;
    UNibble Index;
    TContainerIterator(TContainer<T> *owner=nullptr,UNibble idx=0) : Owner(owner),Index(idx) {
    }

    TContainerIterator &operator++() {
        ++Index;
        return *this;
    }
    TContainerIterator operator++(int) {
        TContainerIterator temp(*this);
        ++Index;
        return temp;
    }
    T &operator*() {
        return Owner->Interface[Index];
    }
    const T &operator*() const {
        return Owner->Interface[Index];
    }
    bool operator==(const TContainerIterator<T> &other) const {
        if(other.Owner==nullptr) {
            return (Owner==nullptr) || (Index>=Owner->Size);
        }
        else if(Owner==nullptr) {
            return (other.Owner==nullptr) || (other.Index>=other.Owner->Size);
        }
        return (Owner==other.Owner) && (Index==other.Index);
    }
    bool operator!=(const TContainerIterator<T> &other) const {
        return !(*this == other);
    }
};
template<class T>
TContainerIterator<T> TContainer<T>::begin() {
    return TContainerIterator<T>(this,0);
}
template<class T>
TContainerIterator<T> TContainer<T>::end() {
    return TContainerIterator<T>(nullptr,0);
}
template<class T>
TContainerIterator<T> TContainer<T>::begin() const {
    return TContainerIterator<T>(const_cast<TContainer<T> *>(this),0);
}
template<class T>
TContainerIterator<T> TContainer<T>::end() const {
    return TContainerIterator<T>(nullptr,0);
}

#pragma pack(1)
template<class T>
struct TContainerIterator_Short {
    TContainer_Short<T> *Owner;
    short Index;
    TContainerIterator_Short(TContainer_Short<T> *owner=nullptr,short idx=0) : Owner(owner),Index(idx) {
    }

    TContainerIterator_Short &operator++() {
        ++Index;
        return *this;
    }
    TContainerIterator_Short operator++(int) {
        TContainerIterator_Short temp(*this);
        ++Index;
        return temp;
    }
    T &operator*() {
        return Owner->Interface[Index];
    }
    const T &operator*() const {
        return Owner->Interface[Index];
    }
    bool operator==(const TContainerIterator_Short<T> &other) const {
        if(other.Owner==nullptr) {
            return (Owner==nullptr) || (Index>=Owner->Size);
        }
        else if(Owner==nullptr) {
            return (other.Owner==nullptr) || (other.Index>=other.Owner->Size);
        }
        return (Owner==other.Owner) && (Index==other.Index);
    }
    bool operator!=(const TContainerIterator_Short<T> &other) const {
        return !(*this == other);
    }
};
template<class T>
TContainerIterator_Short<T> TContainer_Short<T>::begin() {
    return TContainerIterator_Short<T>(this,0);
}
template<class T>
TContainerIterator_Short<T> TContainer_Short<T>::end() {
    return TContainerIterator_Short<T>(nullptr,0);
}
template<class T>
TContainerIterator_Short<T> TContainer_Short<T>::begin() const {
    return TContainerIterator_Short<T>(const_cast<TContainer_Short<T> *>(this),0);
}
template<class T>
TContainerIterator_Short<T> TContainer_Short<T>::end() const {
    return TContainerIterator_Short<T>(nullptr,0);
}
#endif
