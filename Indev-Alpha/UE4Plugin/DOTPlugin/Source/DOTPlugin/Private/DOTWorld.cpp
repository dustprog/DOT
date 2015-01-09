// Fill out your copyright notice in the Description page of Project Settings.

#include "DOTPluginPrivatePCH.h"
#include "DOTWorld.h"

ADOTWorld::ADOTWorld(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	ExternUpdate = ExternLoad = false;
}

void ADOTWorld::AddEntity()
{
	//spawn an entity and push it to the end of the array
	Entities->Add(GetWorld()->SpawnActor<AEntity>(AEntity::StaticClass()));
}

void ADOTWorld::AddAdvertisement()
{
	Ads->Add(GetWorld()->SpawnActor<AAdvertisement>(AAdvertisement::StaticClass()));
}

void ADOTWorld::AddSimulationEntity()
{
	SimulationEntities->Add(GetWorld()->SpawnActor<AEntity>(AEntity::StaticClass()));
}

void ADOTWorld::AddSimulationAdvertisement()
{
	SimulationAds->Add(GetWorld()->SpawnActor<AAdvertisement>(AAdvertisement::StaticClass()));
}

bool ADOTWorld::Update_No_LOD(int32 rad)
{
	if (SimulationEntities->Num() == 0)
		return true; //Everyone is dead

	for (AEntity* SimulationEntity : *SimulationEntities)
	{
		if (!SimulationEntity->IsDead())
			int32 k = UpdateEntity(SimulationEntity - (*SimulationEntities)[0], rad);
	}

	return false;
}

AEntity* ADOTWorld::CopyEntityForSim(FString Name, FVector2D position, int32 i)
{
	InheritsFromEntity(Name)->createCopy((*SimulationEntities)[i]); //Copy the entity from "Entities" to simulational entities
	int32 m = PrevInheritanceEntity(Name);
	(*SimulationEntities)[i]->Link = Name;
	(*SimulationEntities)[i]->Name = Name + ", Poly " + FString::FromInt(m);
	(*SimulationEntities)[i]->LinkNumber = m;
	(*SimulationEntities)[i]->Position = position;
	return (*SimulationEntities)[SimulationEntities->Num() - 1];
}

AAdvertisement* ADOTWorld::CopyAdForSim(FString Name, FVector2D position, int32 i)
{
	InheritsFromAd(Name)->createCopy((*SimulationAds)[i]); //Copy the entity from "Ads" to simulational Ads
	int32 m = PrevInheritanceAd(Name);
	(*SimulationAds)[i]->Link = Name;
	(*SimulationAds)[i]->AdName = Name + ", Poly " + FString::FromInt(m);
	(*SimulationAds)[i]->LinkNumber = m;
	(*SimulationAds)[i]->Position = position;
	return (*SimulationAds)[SimulationAds->Num() - 1];
}

AEntity* ADOTWorld::ReturnEntity(FString Name)
{
	for (int32 i = 0; i < Entities->Num(); i++)
		if (Name == (*Entities)[i]->Name)
			return (*Entities)[i];

	return nullptr;
}

int32 ADOTWorld::EntityIndex(FString Name)
{
	for (int32 i = 0; i < Entities->Num(); i++)
		if (Name == (*Entities)[i]->Name)
			return i;
	return -1;
}

AEntity* ADOTWorld::ReturnSimulationalEntity(FString Name)
{
	for (int32 i = 0; i < SimulationEntities->Num(); i++)
		if (Name == (*SimulationEntities)[i]->Name)
			return (*SimulationEntities)[i];
	return nullptr;
}

int32 ADOTWorld::SimulationalEntityIndex(FString Name)
{
	for (int32 i = 0; i < SimulationEntities->Num(); i++)
		if (Name == (*SimulationEntities)[i]->Name)
			return i;
	return -1;
}

AAdvertisement* ADOTWorld::ReturnAdvertisement(FString Name)
{
	for (int32 i = 0; i < Ads->Num(); i++)
		if (Name == (*Ads)[i]->AdName)
			return (*Ads)[i];
	return nullptr;
}

int32 ADOTWorld::AdIndex(FString Name)
{
	for (int32 i = 0; i < Ads->Num(); i++)
		if (Name ==(*Ads)[i]->AdName)
			return i;
	return -1;
}

AAdvertisement* ADOTWorld::ReturnSimulationalAdvertisement(FString Name)
{
	for (int32 i = 0; i < SimulationAds->Num(); i++)
		if (Name == (*SimulationAds)[i]->AdName)
			return (*SimulationAds)[i];
	return nullptr;
}

int32 ADOTWorld::SimulationAdIndex(FString Name)
{
	for (int32 i = 0; i < SimulationAds->Num(); i++)
		if (Name == (*SimulationAds)[i]->AdName)
			return i;
	return -1;
}

int32 ADOTWorld::AttributeIndex(FString Name)
{
	for (int32 i = 0; i < this->Attributes->Num(); i++)
		if (Name == (*Attributes)[i].Name)
			return i;
	return -1;
}

bool ADOTWorld::AttributeExist(FString Name)
{
	for (int32 i = 0; i < Entities->Num(); i++)
	{
		for (int32 j = 0; j < (*Entities)[i]->Attributes.Num(); j++)
		{
			if ((*Entities)[i]->Attributes[j]->Name == Name)
				return true;
		}
	}
	return false;
}

void ADOTWorld::ResizeEntityAttributes()
{
	for (int32 i = 0; i < Entities->Num(); i++)
	{
		(*Entities)[i]->UsedAttributes.Add(-1);
	}
}

void ADOTWorld::BuildAdvertisement(AAdvertisement *ad)
{
	for (int32 j = 0; j < ad->negative_effects.Num(); j++)
	{
		ad->negative_effects[j].Index = this->AttributeIndex(ad->negative_effects[j].StringT); //Make sure the ads are trying to effect an attribute that actually exists
	}

	for (int32 j = 0; j < ad->positive_effects.Num(); j++)
	{
		ad->positive_effects[j].Index = this->AttributeIndex(ad->positive_effects[j].StringT); //Make sure the ads are trying to effect an attribute that actually exists
	}

	for (int32 j = 0; j < ad->Infer.Num(); j++)
	{
		BuildAdvertisement(ad->Infer[j]);
	}
}

void ADOTWorld::BuildAdvertisementLinks()
{
	for (int32 i = 0; i < Ads->Num(); i++)
	{
		BuildAdvertisement((*Ads)[i]);
	}

	for (int32 k = 0; k < Entities->Num(); k++)
	{		
		auto Ads2 = (*Entities)[k]->SelfAdvertisment;
		for (int32 i = 0; i < Ads2.Num(); i++)
		{
			BuildAdvertisement(Ads2[i]);
		}

		auto Ads3 = (*Entities)[k]->BroadcastedAdvertisement;
		for (int32 i = 0; i < Ads3.Num(); i++)
		{
			BuildAdvertisement(Ads3[i]);
		}
	}
}

void ADOTWorld::Build()
{
	BuildAdvertisementLinks();
	for (int32 i = 0; i < Entities->Num(); i++) //For every entity
	{
		for (int32 j = 0; j < Ads->Num(); j++) //Cycle through every ad, and see if this entity is compadible with that ad
		{
			bool tempbool = true;
			for (int32 k = 0; k < (*Ads)[j]->positive_effects.Num(); k++)
				//Search through all of their attributes, and see if there are any mismatches
				if ((*Entities)[i]->UsedAttributes[(*Ads)[j]->positive_effects[k].Index] == -1) //The attribute was not found
				{
				tempbool = false; //The advertisement isn't compadible with this entity
				break;
				}
			if (tempbool)
			{
				for (int32 k = 0; k < (*Ads)[j]->negative_effects.Num(); k++)
					if ((*Entities)[i]->UsedAttributes[(*Ads)[j]->negative_effects[k].Index] == -1) //The attribute was not found
					{
					tempbool = false;
					break;
					}
			}
			if (tempbool)
				(*Entities)[i]->ViableAds.Add(j); //There were no mismatches, therefore the ad is viable
		};
	}
	for (int32 i = 0; i < Entities->Num(); i++)
	{
		(*Entities)[i]->ViableAds.Sort();
	}
	PrepareForSimulation();
}

void ADOTWorld::PrepareForSimulation()
{
	for (int32 i = 0; i < SimulationEntities->Num(); i++)
	{
		ReturnEntity((*SimulationEntities)[i]->Link)->createUpdateCopy((*SimulationEntities)[i]);
	}

	for (int32 i = 0; i < SimulationAds->Num(); i++)
	{
		AAdvertisement *ad = ReturnAdvertisement((*SimulationAds)[i]->Link);
		if (ad != nullptr)
			ad->createUpdateCopy((*SimulationAds)[i]);
		else
			SimulationAds->RemoveAt(i);

		(*SimulationAds)[i]->LinkAddress = AdIndex((*SimulationAds)[i]->Link);
	}
}

int32 ADOTWorld::UpdateEntity(int32 EntityIndex, int32 rad)
{
	AdList* A;
	TArray<float> Scores;
	bool b = true;			
	bool q = false; 			
	int32 intf = 0;
	switch ((*SimulationEntities)[EntityIndex]->ReturnLockVal())
	{
	case 1: //The lock is over
		(*SimulationEntities)[EntityIndex]->ExecuteQueue();
		(*SimulationEntities)[EntityIndex]->IncLock();
		return 0;
		break;
	case 0: //The lock ended last itt, give me a new task		
		A = FindLocalAds(EntityIndex, rad);
		for (int32 i = 0; i < (*SimulationEntities)[EntityIndex]->SelfAdvertisment.Num(); i++)
		{
			A->Add((*SimulationEntities)[EntityIndex]->SelfAdvertisment[i]);
		}
		for (int32 i = 0; i < A->Num();) //Either its not in use, or its in use, but sharable between people
		{
			if ((*A)[i]->IsInUse() && !(*A)[i]->UsableByMoreThanOne)
				A->RemoveAt(i); //So we erase it from the vector, and get the next highest
			else if ((*A)[i]->OwnerSet)
			{
				if ((*A)[i]->GetOwnership() != (*SimulationEntities)[EntityIndex])
				{
					A->RemoveAt(i);
				}
				else
					i++;
			}
			else
				i++;
		}
		for (int32 i = 0; i < A->Num();)
		{
			float ftemp = (*SimulationEntities)[EntityIndex]->ReturnScore((*A)[i]);

			Scores.Add(ftemp);
			i++;
		}

		int32 index;
		FMath::Max<float>(Scores, &index);
		if (!(*A)[index]->UsableByMoreThanOne)
			(*A)[index]->SetOwner((*SimulationEntities)[EntityIndex]); //If it isn't sharable, set the owner to ourself. We could eventually set this up as a job application idealogy 

		(*SimulationEntities)[EntityIndex]->LockForTurns((*A)[index]);
		(*A)[index]->BeginUse();
		return 1;
	default: //incriment the itt
		(*SimulationEntities)[EntityIndex]->Update();
		return 0;
		break;
	}
}

AdList* ADOTWorld::FindLocalAds(int32 EntityIndex, int32 rad)
{
	AdList* TempList = new AdList();
	AEntity *t = (*SimulationEntities)[EntityIndex];
	FVector2D temp = t->Position;
	for (int32 i = 0; i < SimulationAds->Num(); i++)
	{
		if (!t->ViableAds.Find((*SimulationAds)[i]->LinkNumber)) //This ad doesn't aplpy 
			i++;
		if (i >= SimulationAds->Num())
			break;
		if (rad > FVector2D::Distance((*SimulationAds)[i]->Position,temp)) //Simple bounding sphere
			TempList->Add((*SimulationAds)[i]);
	}

	return TempList;
}

AAdvertisement* ADOTWorld::ReturnAdvertisementShared(FString Name)
{
	for (int32 i = 0; i < Ads->Num(); i++)
		if (Name == (*Ads)[i]->AdName)
			return (*Ads)[i];
	return nullptr;
}

AAdvertisement* ADOTWorld::ReturnAdvertisementShared(FString Name, AEntity *et)
{
	for (int32 i = 0; i < et->SelfAdvertisment.Num(); i++)
		if (Name == et->SelfAdvertisment[i]->AdName)
			return et->SelfAdvertisment[i];
	for (int32 i = 0; i < et->BroadcastedAdvertisement.Num(); i++)
		if (Name == et->BroadcastedAdvertisement[i]->AdName)
			return et->BroadcastedAdvertisement[i];

	return nullptr;
}

AEntity* ADOTWorld::ReturnEntityShared(FString Name)
{
	for (int32 i = 0; i < Entities->Num(); i++)
		if (Name == (*Entities)[i]->Name)
			return (*Entities)[i];
	return nullptr;
}

int32 ADOTWorld::DeleteEntityAdvertisement(FString Name, AEntity *et)
{
	int32 ret = 0;
	for (int32 i = 0; i < et->SelfAdvertisment.Num(); i++){
		if (Name == et->SelfAdvertisment[i]->AdName)
		{
			et->SelfAdvertisment.RemoveAt(i);
			ret = 1;
			break;
		}
	}
	for (int32 i = 0; i < et->BroadcastedAdvertisement.Num(); i++){
		if (Name == et->BroadcastedAdvertisement[i]->AdName)
		{
			et->BroadcastedAdvertisement.RemoveAt(i);
			ret = 1;
			break;
		}
	}
	return ret;
}

int32 ADOTWorld::PrevInheritanceEntity(FString Name)
{
	int32 j = 0;
	for (int32 i = 0; i < SimulationEntities->Num(); i++)
		if ((*SimulationEntities)[i]->Link == Name)
			j++;
	return j;
}

int32 ADOTWorld::PrevInheritanceAd(FString Name)
{
	int32 j = 0;
	for (int32 i = 0; i < SimulationAds->Num(); i++)
		if ((*SimulationAds)[i]->Link == Name)
			j++;
	return j;
}