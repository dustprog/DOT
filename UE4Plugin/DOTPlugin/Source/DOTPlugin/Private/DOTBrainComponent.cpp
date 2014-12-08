// Fill out your copyright notice in the Description page of Project Settings.

#include "DOTPluginPrivatePCH.h"
#include "DOTAdvertisement.h"
#include "DOTBrainComponent.h"

UDOTBrainComponent::UDOTBrainComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	Power = 0;
	EntityType = 0;
	IsDead = false;
	Lock = 0;
	WeighedSum = 1;
}

void UDOTBrainComponent::IncLock()
{

}

TArray<EntityAttribute*>  UDOTBrainComponent::GetGoals()
{
	return Attributes;
}

TArray<EntityAttribute*>* UDOTBrainComponent::GetGoalsPtr()
{
	return &Attributes;
}

void UDOTBrainComponent::LockForTurns(class ADOTAdvertisement* Next)
{
	int32 count = (int32)(((float)Next->TimeConstant + 1) * (1 / WeighedSum));
	Lock = count;
	this->Queue = Next;
}

void UDOTBrainComponent::CheckDead()
{
	for (int32 i = 0; i < Attributes.Num(); i++)
	{
		if (Attributes[i]->Value < 0)
			IsDead = true;
	};
	IsDead = false;
}

void UDOTBrainComponent::Tick_()
{
	for (int32 i = 0; i < Attributes.Num(); i++)
	{
		Attributes[i]->Value -= Attributes[i]->Delta;
	};
}

void UDOTBrainComponent::ExecuteAdvertisment(class ADOTAdvertisement* Ad)
{
	float DeltaVal = 0;
	for (int32 i = 0; i < Ad->positive_effects.Num(); i++)
	{
		int32 LocalIndex = UsedAttributes[Ad->positive_effects[i].Index];
		float DeltaTemp = Attributes[LocalIndex]->Value;
		Attributes[LocalIndex]->Value += Ad->positive_effects[i].DiminishingReturn;
		if (Attributes[LocalIndex]->Value > Attributes[LocalIndex]->grad.Max)
			Attributes[LocalIndex]->Value = Attributes[LocalIndex]->grad.Max;
		DeltaTemp = (Attributes[LocalIndex]->Value - DeltaTemp) / Ad->positive_effects[i].DiminishingReturn;
		DeltaVal += DeltaTemp;
	}
	//negative effects does not exist. Besides it should be named PositiveEffects, NegativeEffects.
	DeltaVal /= Ad->positive_effects.Num();
	if (DeltaVal != 0)
	{
		for (int32 i = 0; i < Ad->negative_effects.Num(); i++)
		{
			int32 LocalIndex = UsedAttributes[Ad->negative_effects[i].Index];
			Attributes[LocalIndex]->Value -= DeltaVal * Ad->negative_effects[i].DiminishingReturn;
		}
	}
	int32 itt;
	int32 count = (((float)Ad->TimeConstant + 1) * (1 / WeighedSum));
	for (itt = 0; itt < count; itt++) //Update for how many times it takes to complete this task
		Tick_();
	Ad->EndUse();
	CheckDead();
}

void UDOTBrainComponent::ExecuteQueue()
{
	ExecuteAdvertisment(Queue);
	PreAdUpdate();
}

FString UDOTBrainComponent::ReturnNextName()
{
	return this->Queue->AdName;
}

void UDOTBrainComponent::DeleteAttribute(FString Name)
{
	int32 i;
	for (i = 0; i < Attributes.Num(); i++)
	{
		if (Attributes[i]->Name == Name)
		{
			Attributes.RemoveAt(i); 
			break;
		}
	}

	int32 j;
	for (j = 0; j < UsedAttributes.Num(); j++)
	{
		if (i == UsedAttributes[j])
		{
			UsedAttributes[j] = -1;
			break;
		}
	}
}

AActor* UDOTBrainComponent::CreateUpdateCopy()
{
	//Component is always attached to Actor, it's safe assumption that owner of this
	//component is some kind of actor.
	AActor* ToReturn = GetWorld()->SpawnActor<UDOTBrainComponent>(GetOwner()->StaticClass());
	
	//probabaly better wayt would interface with
	//GetDOTBrainComponent()
	//as FindComponentByClass() simply iterates trough array of componenets until it find one.
	UDOTBrainComponent* component = nullptr;
	if (ToReturn)
	{
		component = ToReturn->FindComponentByClass<UDOTBrainComponent>();
	}
	if (component)
	{
		component->SelfAdvertisment = SelfAdvertisment;
		component->BroadcastedAdvertisement = BroadcastedAdvertisement;
		component->Attributes = Attributes;
	}
	return ToReturn;
}

//not sure that is this function supposed to do, but we don't want to create copy of component but
//copy of actor which owns this component
//currently it doesn't seem to accomplish either.
//New actors must be explictly spawn trough GetWorld->SpawnActor
//and to actually update actor existing in world we need to have some kind reference to it
//or find it using TActorIterator
void UDOTBrainComponent::CreateUpdateCopy(AActor *out)
{
	UDOTBrainComponent* component = nullptr;
	if (out)
	{
		component = out->FindComponentByClass<UDOTBrainComponent>();
	}

	if (component)
	{
		component->SelfAdvertisment = SelfAdvertisment;
		component->BroadcastedAdvertisement = BroadcastedAdvertisement;
		component->ViableAds = ViableAds;
		component->UsedAttributes = UsedAttributes;
		for (int32 i = 0; i < Attributes.Num(); i++)
		{
			Attributes[i]->createCopy(component->Attributes[i]);
		}
	}
}

AActor* UDOTBrainComponent::CreateCopy()
{
	AActor* ToReturn = GetWorld()->SpawnActor<UDOTBrainComponent>(GetOwner()->StaticClass());
	UDOTBrainComponent* component = nullptr;
	if (ToReturn)
	{
		component = ToReturn->FindComponentByClass<UDOTBrainComponent>();
	}
	if (component)
	{
		component->Position = Position;
		component->Name = Name;
		component->SelfAdvertisment = SelfAdvertisment;
		component->BroadcastedAdvertisement = BroadcastedAdvertisement;
		component->Attributes = Attributes;
	}
	return ToReturn;
}

void UDOTBrainComponent::CreateCopy(AActor *out)
{
	UDOTBrainComponent* component = nullptr;
	if (out)
	{
		component = out->FindComponentByClass<UDOTBrainComponent>();
	}
	if (component)
	{
		component->Position = Position;
		component->Name = Name;
		component->SelfAdvertisment = SelfAdvertisment;
		component->BroadcastedAdvertisement = BroadcastedAdvertisement;
		component->Attributes = Attributes;
	}
}

void UDOTBrainComponent::PreAdUpdate()
{
	WeighedSum = 0;
	for (int32 i = 0; i < Attributes.Num(); i++)
	{
		int16 temp;
		if (Attributes[i]->Value > 0)
			temp = Attributes[i]->grad.Max / (Attributes[i]->Value);
		else
			break;
		WeighedSum += Monitization::TimeAspect(temp, Attributes[i]->Delta, Attributes[i]->Value);
	}
	WeighedSum /= UsedAttributes.Num();
}

float UDOTBrainComponent::ComputeInference(class ADOTAdvertisement* Ad)
{
	float Average = 0;
	for (int32 i = 0; i < Ad->Infer.Num(); i++)
	{
		Average += ReturnScore(Ad->GetInferByIndex(i));
	}

	return Average / Ad->Infer.Num();
}

float UDOTBrainComponent::ReturnScore(class ADOTAdvertisement* Ad)
{
	int32 count = (int32)(((float)Ad->TimeConstant + 1) * (1 / WeighedSum));
	if (Ad->positive_effects.Num() == 0 && Ad->negative_effects.Num() == 0)
		return 0;

	float t = 1;
	int32 avgscore1 = 0;

	//Compute a quick sigmoid
	float m = Ad->TimeConstant;

	m = m / (1 + abs(m));
	m *= WeighedSum;
	bool check = true;

	for (int32 l = 0; l < Ad->positive_effects.Num(); l++)
	{
		int32 LocalIndex = UsedAttributes[Ad->positive_effects[l].Index];
		auto i = Ad->positive_effects[l];
		int32 score1, score2;
		score1 = score2 = 0;
		score2 = i.DiminishingReturn;
		score1 = Attributes[LocalIndex]->Value;
		score2 += score1; //Score2 = Future score

		if (score1 >= Attributes[LocalIndex]->grad.Max)
		{
			score2 = Attributes[LocalIndex]->grad.Max;
		}
		else
			check = false; //At least once, there was a difference 


		avgscore1 += score2; //Average input value

		t += Monitization::PrecomputedTimeAspectScore(score1, score2, m); //Average positive return val
	}
	if (check) //Everything is maxed out
		return -abs(t);
	avgscore1 /= Ad->positive_effects.Num() + 1;
	t /= Ad->positive_effects.Num() + 1;

	float k = 1;
	for (int32 i = 0; i < Ad->negative_effects.Num(); i++) //Goes through all the negative side effects and manages them for us
	{
		auto NegCost = Ad->negative_effects[i];
		int32 LocalIndex = UsedAttributes[NegCost.Index];
		if (NegCost.DiminishingReturn + (Attributes[LocalIndex]->Delta * count) >= Attributes[LocalIndex]->Value)
			return 0;

		k += (float)Monitization::AutoWeightedScore_Negative(avgscore1, t, NegCost.DiminishingReturn + (Attributes[LocalIndex]->Delta
			* count), this->Attributes[LocalIndex]->Value, this->Attributes[LocalIndex]->grad);
	}
	k /= Ad->negative_effects.Num() + 1;

	float inferm = 1;
	if (Ad->Infer.Num() > 0)
		inferm = ComputeInference(Ad);
	return inferm + (t * k);
}