// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "AdvertisementInferface.h"
#include "DOTAdvertisement.generated.h"

/**
 * Added DOT prefix, to avoid possible name conflicts with other plugins or modules.
 */
UCLASS()
class ADOTAdvertisement : public AActor, public IAdvertisementInferface
{
	GENERATED_BODY()

	// Constructor declaration
	ADOTAdvertisement(const FObjectInitializer& ObjectInitializer);

public:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Advertisement)
	TArray<ADOTAdvertisement*> Infer;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Advertisement)
	UStaticMeshComponent* Mesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Advertisement)
	UCapsuleComponent* Component;

	UFUNCTION(BlueprintCallable, Category = "Advertisement")
	void AddNewPositiveCost(int32 index, int32 Delta);

	UFUNCTION(BlueprintCallable, Category = "Advertisement")
	void AddNewNegativeCost(int32 index, int32 Delta);

	UFUNCTION(BlueprintCallable, Category = "Advertisement")
	void SetCallableByMultiEntities(bool b);

	virtual void SetOwner(class AEntity *e) override;
	virtual void BeginUse() override;
	virtual void Execute() override;
	virtual void EndUse() override;

	ADOTAdvertisement* CreateCopy();
	void CreateCopy(ADOTAdvertisement* out);
	void CreateUpdateCopy(ADOTAdvertisement* out);

	ADOTAdvertisement* GetInferByIndex(int32 index) { return Infer[index]; }

	inline FVector GetPosition()
	{
		return this->Mesh->RelativeLocation;
	};	
};
