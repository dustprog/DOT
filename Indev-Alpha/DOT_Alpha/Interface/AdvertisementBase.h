#ifndef ADVERTISEMENTINTERFACE_HEADER
#define ADVERTISEMENTINTERFACE_HEADER
#include "InterfaceBase.h"
#include <3rd_party/libnabo-master/nabo/nabo.h>
//A container that can release more than more advertisement. Use this, rather than advertisementbase, to define ingame objects
struct AdvertisementBroadcaster
{
    TContainer<AdvertisementBase> Ads;
};

struct AdvertisementScore
{
	AdvertisementBase *Advert;
	float Score;
};
struct IndexScore
{
	UNibble Index : 8;
	float Score;
};
#endif
