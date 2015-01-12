// TestingGrounds.cpp : Defines the entry point for the console application.
//
#include <cstdio>
#include "../ZETA_Alpha/Manager/SearchTreeManager.h"
#include "../ZETA_Alpha/Containers/SearchTree.h"
#include "../DOT_Alpha/Interface/EntityBase.h"

int main(int argc, char* argv[])
{
    printf("%d\n",sizeof(IEntityGroup));

    IEntityGroup *EntityGroup = new IEntityGroup();
    //How many attributes exist in our entire application? Warning: Only 256 can be stored per entity group
    EntityGroup->SetGlobalAttributeBufferSize(4);
    //Gives Entity group 4 attributes, providing the global address for each
    EntityGroup->AddAttribute(0);
    EntityGroup->AddAttribute(1);
    EntityGroup->AddAttribute(2);
    EntityGroup->AddAttribute(3);

    //Creates entity instance 0
    short MyIndex = EntityGroup->CreateNewInstance();

    //Change the values of our new instance's first and 3rd attributes
    EntityGroup->RuntimeBody[(MyIndex * EntityGroup->BlockSize)]
            = EntityGroup->RuntimeBody[(MyIndex * EntityGroup->BlockSize) + 2] = 30;
    AdvertisementBase SomeAd = AdvertisementBase();

    SomeAd.AddPositiveEffect(CostBase(0, 5)); //Increases global attributes 0 and 2 by 20
    SomeAd.AddPositiveEffect(CostBase(2, 5));

    SomeAd.AddNegativeEffect(CostBase(1, 70)); //Reduces global attributes 1 and 3 by 50
    SomeAd.AddNegativeEffect(CostBase(3, 70));

    //Computes the transfer constant for entity instance 0
    EntityGroup->Prepare(0);

    float True = EntityGroup->ReturnScore(0, SomeAd);
    printf(" %f \n",True);
    return 0;
}

