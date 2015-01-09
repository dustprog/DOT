#ifndef SEARCHTREEMANAGER_H
#define SEARCHTREEMANAGER_H
#include "../Containers/Node.h"
#include "../Containers/TemplateTreeAddress.h"
struct SManager
{
    //Writes to first
    template<class T>
    static void Link(TNode<T> *First, TNode<T> *Second)
    {
        First->Children.resize(First->Children.size() + 1);
        First->Length = std::max(First->Length, Second->Length + 1);
        First->Children = Second;
        First->OnAddNewChain();
    }
    template<class T>
    static T Read(TNode<T> Tree, TAddress Address)
    {
        auto iter = Address.begin();
        auto fin = Address.end();

        TNode<T> Current = Tree;
        for ( ;iter!=fin; ++iter)
        {
            Current = *Current.Children[*iter];
        }
        return Current.ReturnData();
    }
};
#endif
