// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/GameMode.h"
#include "DOTPluginPrivatePCH.h"
#include "Advertisement.h"
#include "Entity.h"
#include "DOTWorld.generated.h"

USTRUCT()
struct FAttributeRef
{
	GENERATED_USTRUCT_BODY()

public:
	FAttributeRef(FString Name)
	{
		this->Name = Name;
		Amount = 1;
	}
	FAttributeRef(){};
	FString Name;
	int32 Amount;

	static bool Compare(FAttributeRef i, FAttributeRef j) { return i.Amount < j.Amount; };
};

typedef TArray<AAdvertisement*> AdList;
typedef TArray<AEntity*> EntityList;
typedef TArray<FAttributeRef> AttributeList;

UCLASS()
class ADOTWorld : public AGameMode
{
	GENERATED_BODY()

	ADOTWorld(const FObjectInitializer& ObjectInitializer);	

	AdList *Ads;
	EntityList *Entities;
	AttributeList *Attributes;

	AdList* SimulationAds;
	EntityList* SimulationEntities;

	EntityAttribute* nullatr;

	GroupManager* ClusterManager;

public:
	bool ExternUpdate;
	bool ExternLoad;

	void AddEntity(AEntity* Entity_To_Add) { Entities->Add(Entity_To_Add); }

	void AddEntity();

	void AddAdvertisement(AAdvertisement* Advertisement_To_Add) { Ads->Add(Advertisement_To_Add); }

	void AddAdvertisement();

	void AddSimulationEntity();

	void AddSimulationAdvertisement();

	bool Update_No_LOD(int32 rad); //Updates all entities. Has no level of detail management (Our back propogation engine)

	AEntity *CopyEntityForSim(FString Name, FVector2D position, int32 i);

	AAdvertisement *CopyAdForSim(FString Name, FVector2D position, int32 i);

	AEntity *ReturnEntity(FString Name); //ONLY TO BE USED IN THE EDITOR!!
	
	int32 EntityIndex(FString Name); //ONLY TO BE USED IN THE EDITOR!!

	AEntity *ReturnSimulationalEntity(FString Name); //ONLY TO BE USED IN THE EDITOR!!

	int32 SimulationalEntityIndex(FString Name); //ONLY TO BE USED IN THE EDITOR!!

	AAdvertisement *ReturnAdvertisement(FString Name); //ONLY TO BE USED IN THE EDITOR!!

	int32 AdIndex(FString Name); //ONLY TO BE USED IN THE EDITOR!!

	AAdvertisement *ReturnSimulationalAdvertisement(FString Name); //ONLY TO BE USED IN THE EDITOR!!		

	int32 SimulationAdIndex(FString Name); //ONLY TO BE USED IN THE EDITOR!!

	int32 AttributeIndex(FString Name);

	bool AttributeExist(FString Name);

	void ResizeEntityAttributes();

	void BuildAdvertisement(AAdvertisement *ad);

	void BuildAdvertisementLinks();

	void Build();

	void PrepareForSimulation();

	int32 UpdateEntity(int32 EntityIndex, int32 rad); //Just a generic update function. Returns 0 when the entity is busy, returns 1 when the entity has been given a new task		

	AdList* FindLocalAds(int32 EntityIndex, int32 rad); //Looks to see how many advertisements are within walking distance of the entity

	AAdvertisement* ReturnAdvertisementShared(FString Name); //ONLY TO BE USED IN THE EDITOR!!

	AAdvertisement* ReturnAdvertisementShared(FString Name, AEntity *et); //ONLY TO BE USED IN THE EDITOR!!

	AEntity* ReturnEntityShared(FString Name); //ONLY TO BE USED IN THE EDITOR!!		

	int32 DeleteEntityAdvertisement(FString Name, AEntity *et);

private:

	AEntity* InheritsFromEntity(FString OtherEntity) { return ReturnEntity(OtherEntity); }

	AEntity* InheritsFrom_StaticEntity(FString OtherEntity) { return ReturnEntity(OtherEntity)->createCopy(); }

	AAdvertisement* InheritsFromAd(FString OtherAd) { return ReturnAdvertisement(OtherAd); }

	AAdvertisement* InheritsFrom_StaticAd(FString OtherAd) { return ReturnAdvertisement(OtherAd)->createCopy(); }

	int32 PrevInheritanceEntity(FString Name);
		
	int32 PrevInheritanceAd(FString Name);	
};
