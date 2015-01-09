#ifndef TEMPLATETREEADDRESS
#define TEMPLATETREEADDRESS
#include "generic_typedefs.h"
#include "NibbleContainer.h"

#pragma pack(1)
struct TAddress : public NContainer
{
public:
    TAddress() : NContainer((UNibble)ADDR_SIZE * 2) { }
//    template<class J>
//    J Retrieve(TContainerType<J> Input)
//    {
//        return SManager::Read((TNode<J>)Input, this).ReturnData();
//    }
};

#endif // TEMPLATETREEADDRESS

