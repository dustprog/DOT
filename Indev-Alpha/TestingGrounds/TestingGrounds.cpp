// TestingGrounds.cpp : Defines the entry point for the console application.
//
#include <cstdio>
#include "../ZETA_Alpha/Manager/SearchTreeManager.h"
#include "../ZETA_Alpha/Containers/SearchTree.h"
#include "../DOT_Alpha/Managers/World.h"
#include <iostream>
#include <iomanip>
#include <chrono>
#include <ctime>

int main(int argc, char* argv[])
{
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
    EntityGroup->RuntimeBody[(MyIndex * EntityGroup->BlockSize)].Value
            = EntityGroup->RuntimeBody[(MyIndex * EntityGroup->BlockSize) + 2].Value = 30;
    AdvertisementBase SomeAd = AdvertisementBase();

    SomeAd.AddPositiveEffect(CostBase(0,20)); //Increases global attributes 0 and 2 by 20
    SomeAd.AddPositiveEffect(CostBase(2,20)); //Increases global attributes 2 and 2 by 20

    SomeAd.AddNegativeEffect(CostBase(1, 50)); //Reduces global attributes 1 and 3 by 50
    SomeAd.AddNegativeEffect(CostBase(3, 50)); //Reduces global attributes 1 and 3 by 50
    World::get()->pushBackTemplateAd(&SomeAd);
    //Computes the transfer constant for entity instance 0
    EntityGroup->Prepare(0);

    auto t_start = std::chrono::high_resolution_clock::now();
    for(int i = 0; i < 1000000; i++)
        EntityGroup->ReturnScore(0, SomeAd);
    auto t_end = std::chrono::high_resolution_clock::now();

    printf(" %f \n",1000000.0f / std::chrono::duration<double, std::milli>(t_end-t_start).count());
    return 0;
}

