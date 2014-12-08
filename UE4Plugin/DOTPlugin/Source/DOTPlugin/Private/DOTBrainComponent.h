#pragma once

#include "HelperFunction.h"

#include "DOTBrainComponent.generated.h"

UCLASS()
class UDOTBrainComponent : public UActorComponent
{
	GENERATED_BODY()

		UDOTBrainComponent(const FObjectInitializer& ObjectInitializer);

protected: //changed to acoid possible circular reference issues.
	class ADOTAdvertisement* Queue;
	
private:
	void ExecuteNegativeEffects(int32 type, int32 Delta);
	
public:
	//Have we started moving?
	bool HasStartedObjective; 
	
	float RecentTime;
	
	//Unload on game restart
	//Create Interface IDOTControlledAI
	//add function IsDead()
	//let any actor implement this interface and make component check for it
	//on owning actor IDOTControlledAI* interface = Cast<IDOTControlledAI*>(GetOwner())
	//interface->IsDead()
	//this way we can let every actor to decided where/how it want to become dead.

	//in anycase renamed it to something more meaningful (;.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = FeroNPC)
	bool IsDead;
	
	//Use for actions that take a particular amount of time
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = FeroNPC)
	int32 Lock;

	int16 Power; //The entity's rank in society

	int16 EntityType;

	FString Link;

	int32 LinkNumber; //How many others have inherited?

	FString Name;

	TArray<EntityAttribute*> Attributes;

	TArray<int32> UsedAttributes;

	TArray<int32> ViableAds;

	TArray<class ADOTAdvertisement*> SelfAdvertisment;

	TArray<class ADOTAdvertisement*> BroadcastedAdvertisement;

	float WeighedSum;
	FVector2D Position;

	bool IsDead()
	{
		return Death;
	};

	int32 Update() //Returns 0 when the entity is still locked, returns 1 when lock ends.
	{
		Lock -= 1;
		//Other update code goes here

		return 0;
	};	

	int16 ReturnRank() //The closer this is to 0, the higher the NPC's rank
	{
		return Power;
	}

	int16 ReturnLockVal()
	{
		return this->Lock;
	};	

	bool IsLocked()
	{
		return (Lock > 1);
	}

	int16 ReturnLock()
	{
		return this->Lock;
	}
	
	void IncLock();
	TArray<EntityAttribute*> GetGoals();
	TArray<EntityAttribute*>* GetGoalsPtr();
	void LockForTurns(class ADOTAdvertisement* Next);
	void CheckDead();	
	void Tick_();
	void ExecuteAdvertisment(class ADOTAdvertisement* Ad);
	void ExecuteQueue();	
	FString ReturnNextName();
	void DeleteAttribute(FString Name);

	AActor* CreateUpdateCopy();
	void CreateUpdateCopy(AActor* out);
	AActor* CreateCopy();
	void CreateCopy(AActor* out);

	void PreAdUpdate();
	float ComputeInference(class ADOTAdvertisement* Ad);
	float ReturnScore(class ADOTAdvertisement* Ad);
	
};