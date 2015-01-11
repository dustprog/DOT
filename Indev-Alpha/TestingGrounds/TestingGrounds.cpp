// TestingGrounds.cpp : Defines the entry point for the console application.
//
#include <cstdio>
#include "../ZETA_Alpha/Manager/SearchTreeManager.h"
#include "../ZETA_Alpha/Containers/SearchTree.h"
#include "../DOT_Alpha/Interface/EntityBase.h"
//#include <qt5/QtCore/QCoreApplication>
//#include "../DOT_Alpha/Managers/PythonBinding.h"
int main(int argc, char* argv[])
{
    //QCoreApplication a(argc, argv);
    //sm.Read(z,a);
    printf("%d\n",sizeof(IEntityGroup));

    IEntityGroup *EntityGroup = new IEntityGroup();
    EntityGroup->TemplateBody.resize(0,4);
    EntityGroup->TemplateBody[0] = (Gradient());
    EntityGroup->TemplateBody[1] = (Gradient());
    EntityGroup->TemplateBody[2] = (Gradient());
    EntityGroup->TemplateBody[3] = (Gradient());


    auto ReferenceTable = new TContainer_Short<UNibble>();

    //A very simple look up table for global to local attributes
    ReferenceTable->push_back(0);
    ReferenceTable->push_back(1);
    ReferenceTable->push_back(2);
    ReferenceTable->push_back(3);

    EntityGroup->ReferenceTable = *ReferenceTable;
    EntityGroup->BlockSize = ReferenceTable->size();

    EntityGroup->CreateNewInstance();


    AdvertisementBase SomeAd = AdvertisementBase();

    SomeAd.AddPositiveEffect(CostBase(0, 5)); //Effects global attribute 0 by 5 (Increase)
    SomeAd.AddPositiveEffect(CostBase(2, 7));
    SomeAd.AddNegativeEffect(CostBase(1, 5)); //Effects global attribute 1 by 5 (Decrease)
    SomeAd.AddNegativeEffect(CostBase(3, 5));

    EntityGroup->Prepare(0);

    float True = EntityGroup->ReturnScore(0, SomeAd);
    printf(" %f \n",True);
    return 0;
}

