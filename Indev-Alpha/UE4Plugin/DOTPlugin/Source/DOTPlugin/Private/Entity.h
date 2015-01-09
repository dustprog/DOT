#pragma once

#include "GameFramework/Character.h"
#include "HelperFunction.h"
#include "Advertisement.h"
#include "Entity.generated.h"

UCLASS()
class AEntity : public ACharacter
{
	GENERATED_BODY()

	AEntity(const FObjectInitializer& ObjectInitializer);

protected:
	AAdvertisement* Queue;
	
private:
	void ExecuteNegativeEffects(int32 type, int32 Delta);
	
public:
	//Have we started moving?
	bool HasStartedObjective; 
	
	float recent_time;
	
	//Unload on game restart
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = FeroNPC)
	bool Death;
	
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
	TArray<AAdvertisement*> SelfAdvertisment;
	TArray<AAdvertisement*> BroadcastedAdvertisement;
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
	TArray<EntityAttribute*> getGoals();
	TArray<EntityAttribute*>* getGoalsPtr();
	void LockForTurns(AAdvertisement* Next);
	void CheckDead();	
	void Tick_();
	void ExecuteAdvertisment(AAdvertisement* Ad);	
	void ExecuteQueue();	
	FString ReturnNextName();
	void DeleteAttribute(FString Name);

	AEntity* createUpdateCopy();
	void createUpdateCopy(AEntity *out);
	AEntity* createCopy();
	void createCopy(AEntity *out);

	void PreAdUpdate();
	float ComputeInference(AAdvertisement* Ad);
	float ReturnScore(AAdvertisement* Ad);
	
};