// TestingGrounds.cpp : Defines the entry point for the console application.
//
#include <cstdio>
#include "../ZETA_Alpha/Manager/SearchTreeManager.h"
#include "../DOT_Alpha/DOTbuilder/InterfaceBuilder.cpp"
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


    //DOTbuild Test
    EntityTemplate newT = EntityTemplate::_create("Entity")
            ._addAttribute("Food")
            ._addAttribute("Wealth")
            ._addAttribute("Wealth")
            ._addAttribute("Wealth")
            ._addTag("Blacksmith");

    AttributeTemplate newAT = AttributeTemplate::_create("Food")
            ._setGradient(Gradient(1U,5U,50U,70U,110U))
            ._setValue(60)
            ._setOpinion(12U);

    AttributeTemplate newAT2 = AttributeTemplate::_create("Wealth")
            ._setGradient(Gradient(1U,5U,50U,70U,110U))
            ._setValue(60)
            ._setOpinion(12U);

    TagTemplate newTag = TagTemplate::_create("Blacksmith")
            ._addAttribute("Wealth",1,90);



    DB::get()->Add(newT);
    DB::get()->Add(newAT);
    DB::get()->Add(newAT2);
    DB::get()->Add(newTag);

    WorldCompiled _compiled = DB::get()->Compile();

    TNode<IEntityGroup> node = _compiled.EntityTemplateTree;

    IEntityGroup temp =node.getData();
    temp.CreateNewInstance();
    temp.Prepare(0);
    temp.ReturnScore(0,SomeAd);
    return 0;
}

