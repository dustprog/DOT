#ifndef SEARCHTREE_H
#define SEARCHTREE_H

#include "Node.h"
#include "TemplateTreeAddress.h"

template <class T>
struct TSearchTree
{
private:
    UNibble CurrentPosition;
    TAddress Address;
    TNode<T> Interface;
    NContainerIterator AddressIter;
public:
    T *ReturnPointer()
    {
        return Interface;
    }
    //Returns the vertical size of the chain
    UNibble size()
    {
        return Interface.ReturnLength();
    }
    TAddress ReturnCurrentWriteHead()
    {
        return Address;
    }
    //Pushes the address foward one steps
    void IncrementWriteHead()
    {
        ++AddressIter;
        CurrentPosition++;
    }
    //Sets the proceeding address in the chain to "Next"
    void IncrementWriteHead(UNibble Next)
    {
        AddressIter.write(Next);
        ++AddressIter;
        CurrentPosition++;
    }
    void ReduceWriteHead()
    {
        --AddressIter;
        CurrentPosition--;
    }
    void SetFixedAddressSize(UNibble Size)
    {
        Address.Resize(Size);
    }
    TSearchTree() : AddressIter(&Address,0)
    {
        Interface = TNode<T>();
    }
    void SetWriteHead(TAddress point)
    {
        Address = point;
        AddressIter.Owner = &Address;
    }
};
#endif
