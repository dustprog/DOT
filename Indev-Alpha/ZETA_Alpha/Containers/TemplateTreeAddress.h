#ifndef TEMPLATETREEADDRESS
#define TEMPLATETREEADDRESS
#include "generic_typedefs.h"
#include "NibbleContainer.h"

#pragma pack(1)
struct TAddress : public NContainer
{
public:
    TAddress(UNibble Size = ADDR_SIZE * 2) : NContainer(Size) { }
};

#endif // TEMPLATETREEADDRESS

