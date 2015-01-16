// Fill out your copyright notice in the Description page of Project Settings.

#include "DOTPluginPrivatePCH.h"
#include "Entity.h"

AEntity::AEntity(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	Power = 0;
	EntityType = 0;
	Death = false;
	Lock = 0;
	WeighedSum = 1;
}

void AEntity::IncLock()
{

}

TArray<EntityAttribute*>  AEntity::getGoals()
{
	return Attributes;
}

TArray<EntityAttribute*>* AEntity::getGoalsPtr()
{
	return &Attributes;
}

void AEntity::LockForTurns(AAdvertisement* Next)
{
	int32 count = (int32)(((float)Next->TimeConstant + 1) * (1 / WeighedSum));
	Lock = count;
	this->Queue = Next;
}

void AEntity::CheckDead()
{
	for (int32 i = 0; i < Attributes.Num(); i++)
	{
		if (Attributes[i]->Value < 0)
			Death = true;
	};
	Death = false;
}

void AEntity::Tick_()
{
	for (int32 i = 0; i < Attributes.Num(); i++)
	{
		Attributes[i]->Value -= Attributes[i]->Delta;
	};
}

void AEntity::ExecuteAdvertisment(AAdvertisement* Ad)
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

void AEntity::ExecuteQueue()
{
	ExecuteAdvertisment(Queue);
	PreAdUpdate();
}

FString AEntity::ReturnNextName()
{
	return this->Queue->AdName;
}

void AEntity::DeleteAttribute(FString Name)
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

AEntity* AEntity::createUpdateCopy()
{
	AEntity* ToReturn = GetWorld()->SpawnActor<AEntity>(AEntity::StaticClass());
	ToReturn->SelfAdvertisment = SelfAdvertisment;
	ToReturn->BroadcastedAdvertisement = BroadcastedAdvertisement;
	ToReturn->Attributes = Attributes;
	return ToReturn;
}

void AEntity::createUpdateCopy(AEntity *out)
{
	out->SelfAdvertisment = SelfAdvertisment;
	out->BroadcastedAdvertisement = BroadcastedAdvertisement;
	out->ViableAds = ViableAds;
	out->UsedAttributes = UsedAttributes;
	for (int32 i = 0; i < Attributes.Num(); i++)
	{
		Attributes[i]->createCopy(out->Attributes[i]);
	}
}

AEntity* AEntity::createCopy()
{
	AEntity* ToReturn = GetWorld()->SpawnActor<AEntity>(AEntity::StaticClass());
	ToReturn->Position = Position;
	ToReturn->Name = Name;
	ToReturn->SelfAdvertisment = SelfAdvertisment;
	ToReturn->BroadcastedAdvertisement = BroadcastedAdvertisement;
	ToReturn->Attributes = Attributes;
	return ToReturn;
}

void AEntity::createCopy(AEntity *out)
{
	out->Position = Position;
	out->Name = Name;
	out->SelfAdvertisment = SelfAdvertisment;
	out->BroadcastedAdvertisement = BroadcastedAdvertisement;
	out->Attributes = Attributes;
}

void AEntity::PreAdUpdate()
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

float AEntity::ComputeInference(AAdvertisement* Ad)
{
	float Average = 0;
	for (int32 i = 0; i < Ad->Infer.Num(); i++)
	{
		Average += ReturnScore(Ad->GetInferByIndex(i));
	}

	return Average / Ad->Infer.Num();
}

float AEntity::ReturnScore(AAdvertisement* Ad)
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