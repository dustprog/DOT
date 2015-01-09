// Fill out your copyright notice in the Description page of Project Settings.

#include "DOTPluginPrivatePCH.h"
#include "Entity.h"
#include "DOTAdvertisement.h"

ADOTAdvertisement::ADOTAdvertisement(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{	
	Mesh = ObjectInitializer.CreateDefaultSubobject<UStaticMeshComponent>(this, TEXT("Mesh"));
	RootComponent = Mesh;
	TimeConstant = 0;
	AdName = FString(TEXT("Default"));
	UsableByMoreThanOne = true;
	MyOwner.InUse = false;
}

ADOTAdvertisement* ADOTAdvertisement::CreateCopy()
{
	AAdvertisement* ToReturn = GetWorld()->SpawnActor<AAdvertisement>(AAdvertisement::StaticClass());
	ToReturn->Position = Position;
	ToReturn->AdName = AdName;
	ToReturn->negative_effects = negative_effects;
	ToReturn->positive_effects = positive_effects;
	ToReturn->OwnerSet = OwnerSet;
	ToReturn->UsableByMoreThanOne = UsableByMoreThanOne;
	ToReturn->MyOwner = MyOwner;
	ToReturn->Infer = Infer;
	return ToReturn;
}

void ADOTAdvertisement::CreateCopy(AAdvertisement* out)
{
	out->Position = Position;
	out->AdName = AdName;
	out->negative_effects = negative_effects;
	out->positive_effects = positive_effects;
	out->OwnerSet = OwnerSet;
	out->UsableByMoreThanOne = UsableByMoreThanOne;
	out->Infer = Infer;
	out->MyOwner = MyOwner;
}

void ADOTAdvertisement::CreateUpdateCopy(AAdvertisement* out)
{
	out->negative_effects = negative_effects;
	out->positive_effects = positive_effects;
	out->OwnerSet = OwnerSet;
	out->UsableByMoreThanOne = UsableByMoreThanOne;
	out->MyOwner = MyOwner;
	out->Infer = Infer;
	out->TimeConstant = TimeConstant;
}

void ADOTAdvertisement::AddNewPositiveCost(int32 index, int32 Delta)
{
	this->positive_effects.Add(Cost());
}

void ADOTAdvertisement::AddNewNegativeCost(int32 index, int32 Delta)
{
	this->negative_effects.Add(Cost());
}

void ADOTAdvertisement::SetCallableByMultiEntities(bool b)
{
	this->UsableByMoreThanOne = b;
}

void ADOTAdvertisement::BeginUse()
{
	MyOwner.InUse = true;
}

void ADOTAdvertisement::Execute()
{
	//why is this here? Where does it come from?
}

void ADOTAdvertisement::EndUse()
{
	MyOwner.InUse = false;
}

void ADOTAdvertisement::SetOwner(AEntity *e)
{
	this->MyOwner.e = e;
	OwnerSet = true;
}