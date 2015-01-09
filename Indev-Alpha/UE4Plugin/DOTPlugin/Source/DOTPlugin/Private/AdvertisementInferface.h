#pragma once
#include "HelperFunction.h"
#include "AdvertisementInferface.generated.h"

UINTERFACE(MinimalAPI)
class UAdvertisementInferface : public UInterface
{
	GENERATED_UINTERFACE_BODY()
};

USTRUCT()
struct FOwnership //For obvious reasons. We could maybe implement theft (?)
{
	GENERATED_USTRUCT_BODY()

public:
	FOwnership(){};

	bool InUse;
	class AEntity *e;
};

class IAdvertisementInferface
{
	GENERATED_IINTERFACE_BODY()

public:	

	FOwnership MyOwner;
	int32 TimeConstant;
	FString AdName;
	TArray<Cost> negative_effects; //How badly does this effect us?	
	TArray<Cost> positive_effects; //What good does this do for us?
	int32 LinkNumber;
	int32 LinkAddress;
	bool UsableByMoreThanOne;
	bool OwnerSet;
	FVector2D Position;
	FString Link;	

	virtual void SetOwner(class AEntity *e) {};

	//Use this with "World"
	virtual class AEntity* GetOwnership()
	{
		if (OwnerSet)
			return MyOwner.e;
		return nullptr;
	};

	//Link this to a begin use behavior tree
	virtual void BeginUse() {};

	//Link this to an end use behavior tree
	virtual void EndUse() {};

	//Link this to an execute be
	virtual void Execute() {};

	bool IsInUse() { return MyOwner.InUse; }

	virtual FVector GetPosition() = 0;	
};