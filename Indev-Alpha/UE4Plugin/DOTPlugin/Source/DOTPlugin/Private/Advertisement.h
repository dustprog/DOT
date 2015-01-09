// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "AdvertisementInferface.h"
#include "Advertisement.generated.h"

/**
 * 
 */
UCLASS()
class AAdvertisement : public AActor, public IAdvertisementInferface
{
	GENERATED_BODY()

	// Constructor declaration
	AAdvertisement(const FObjectInitializer& ObjectInitializer);

public:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Advertisement)
	TArray<AAdvertisement*> Infer;

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

	AAdvertisement* createCopy();
	void createCopy(AAdvertisement* out);
	void createUpdateCopy(AAdvertisement* out);

	AAdvertisement* GetInferByIndex(int32 index) { return Infer[index]; }

	FVector GetPosition()
	{
		return this->Mesh->RelativeLocation;
	};	
};
