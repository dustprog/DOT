#ifndef NODE_HEADER
#define NODE_HEADER
#include "GenericContainer.h"
#include "../Manager/ThreadManager.h"
template <class T>
struct TNode
{
private:
    TNode<T> *Parent;
    T Data;
public:
    const TNode<T> *GetParent() const { return Parent; }
    TContainer<TNode<T>*> Children;

    int ReturnByteSize()
    {
        int size = sizeof(Data)+sizeof(this->Parent)+1;
        for (TNode<T>* child : Children)
        {
            size += child->ReturnByteSize();
        }
        return size;
    }
    //Closes the node buffer. Allows for faster reading. Writing again after closing the buffer reopens it.
    void EndWrite()
    {
        for (TNode<T>* child : Children)
        {
            child->EndWrite();
        }
    }
    TNode(T data) : Parent(nullptr) {
        Data = data;
    }
    TNode() : Parent(nullptr) {
    }
    const T &getData() const { return Data; }
    void SetData(T NewData) { Data = NewData; }
};
#endif

