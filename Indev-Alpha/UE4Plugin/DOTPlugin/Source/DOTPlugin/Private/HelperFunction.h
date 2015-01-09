#ifndef HELPERFUNCTIONS_HEADER
#define HELPERFUNCTIONS_HEADER
#include <vector>
#include <algorithm>
#include "Engine.h"
#define _USE_MATH_DEFINES
#ifndef M_PI
#define M_PI           3.14159265358979323846
#endif
class Entity;
struct Group;

struct Gradient
{
	int32 Low, Q1, Q3;
	int32 Max;
	Gradient(int32 L = 20, int32 FirstQuartile = 30, int32 ThirdQuartile = 50, int32 Max = 100) //L = lower bound, H = upper bound
	{
		Low = L;
		Q1 = FirstQuartile;
		Q3 = ThirdQuartile;
		this->Max = Max;
	}

};

struct EntityAttribute
{
public:
	EntityAttribute(bool nullset = false)
	{
		if (!nullset)
		{
			this->Name = "Default";
			this->Value = 100;
			this->Delta = 1;
		}
	}

	EntityAttribute(FString n, int32 val, int32 d)
	{
		this->Name = n;
		Value = val;
		Delta = d;
	};

	EntityAttribute(FString n, int32 val, int32 d, Gradient g)
	{
		this->Name = n;
		Value = val;
		Delta = d;
		grad = g;
	};

	FString Name;
	int32 Value;
	int32 Delta;
	Gradient grad;

	void SetGradient(Gradient g)
	{
		this->grad = g;
	}

	EntityAttribute createCopy()
	{
		EntityAttribute ea;
		ea.Name = Name;
		ea.Value = Value;
		ea.Delta = Delta;
		ea.grad = grad;
		return ea;
	}

	void createCopy(EntityAttribute *ea)
	{
		ea->Name = Name;
		ea->Value = Value;
		ea->Delta = Delta;
		ea->grad = grad;
	}
};

struct Cost
{
public:
	Cost(FString StringType, int32 D)
	{
		this->StringT = StringType;
		this->DiminishingReturn = D;
	}

	Cost()
	{
		StringT = "Default";
		Index = -1;
		DiminishingReturn = 5;
	}

	Cost createCopy()
	{
		return Cost(StringT, DiminishingReturn);
	}

	FString StringT;
	int32 Index;
	int32 DiminishingReturn; //How badly does taking this advertisement hurt us?
};

struct Group
{
public:

	TArray<int32> LTL; //Local to local
	TArray<int32> VI_A; //Viable advertisements
	TArray<Group*> Nodes;

	int32 MinAdr, MaxAdr;
	int32 MaxAd, MinAd;

	void FillAttributes(TArray<int32> NodeAttributes_Global, TArray<int32> NodeAttributes_Local)
	{		
		NodeAttributes_Global.Sort();
		MinAdr = NodeAttributes_Global[0];
		MaxAdr = NodeAttributes_Global.Last();

		for (int32 i = 0; i < MaxAdr - MinAdr; i++)
		{

		}
	};
};

struct GroupManager
{
	Group *MasterNode;
	//Returns the SINGLE node that is linked to the address
	Group *ReturnNodeFromAddress(TArray<int16> Adr)
	{
		Group *temp;
		*temp = *MasterNode;
		for (int16 SingleAdr : Adr)
		{
			temp = (Group*)temp->Nodes[(int32)SingleAdr];
		}


		return temp;
	};

	//For chain retreival only
	TArray<Group*> *ReturnChain(TArray<int16> Adr)
	{
		TArray<Group*>* Chain = new TArray<Group*>();
		Chain->Add(MasterNode);
		for (int32 i = 0; i < Adr.Num(); i++)
		{
			//Chain->push_back((Group*)Chain->at(Chain->size() - 1)->Nodes[Adr.at(i)]);
		}

		for (int16 SingleAdr : Adr)
		{
			Chain->Add((Group*)Chain->Last()->Nodes[(int32)SingleAdr]);
		}

		return Chain;
	};
	//For single address retreival only
	int32 ReturnLocalAdr(int32 GlobalAdr, TArray<int16> EntityAdr)
	{
		TArray<Group*>* Chain = ReturnChain(EntityAdr);
		for (auto ChainSingle : *Chain)
		{
			if(ChainSingle->MaxAdr < GlobalAdr && ChainSingle->MinAdr > GlobalAdr)
				return ChainSingle->LTL[GlobalAdr - ChainSingle->MinAdr];
		}

	}
	//For single advertisement retreival only
	int32 ReturnLocalAdViability(int32 AdAdr, TArray<int16> EntityAdr)
	{
		TArray<Group*> *Chain = ReturnChain(EntityAdr);
		for (auto ChainSingle : *Chain)
		{
			if (ChainSingle->MaxAdr > AdAdr && ChainSingle->MinAdr < AdAdr)
				return ChainSingle->LTL[AdAdr - ChainSingle->MinAdr];
		}
	}
	//For multi address retreival only
	TArray<int32> ReturnLocalAdrGroup(TArray<int32> GlobalAdr, TArray<int16> EntityAdr)
	{
		TArray<Group*> *Chain = ReturnChain(EntityAdr);
		TArray<int32> LocalAdr = TArray<int32>();	

		for (auto GlobalAdrSingle : GlobalAdr) //For every global address we've given
		{
			for (auto ChainSingle : *Chain) //Make sure that we haven't hit the end of our chain
			{
				if (ChainSingle->MaxAdr < GlobalAdrSingle && ChainSingle->MinAdr > GlobalAdrSingle) //Are we within bounds?
				{
					LocalAdr.Add(ChainSingle->LTL[GlobalAdrSingle - ChainSingle->MinAdr]); //Yes! Add the index to the local adr array
					break;
				}
			}
		}
		return LocalAdr;
	};
	//Returns all local addresses for the entity specified. Returns a highly expanded vector. Needed for GPGPU
	TArray<int32> ReturnAttributeChain(TArray<int16> EntityAdr)
	{
		TArray<Group*> *Chain = ReturnChain(EntityAdr);
		TArray<int32> LocalAdr = TArray<int32>();

		for (int32 i = 0; i < Chain->Num(); i++)
		{
			int32 min, max;
			min = (*Chain)[i]->MinAdr;
			max = (*Chain)[i]->MaxAdr;

			for (int32 j = min; j < max; j++)
			{
				//Push back value
				LocalAdr.Add((*Chain)[i]->LTL[j]);
			}

			if (i != (Chain->Num() - 1)) //We aren't on the last chain
			{
				for (int32 j = max; j < (*Chain)[i+1]->MinAdr; j++)
				{
					LocalAdr.Add(-1); //So push back padding
				}
			}
		}		
		return LocalAdr;
	};
	//Slower than ReturnChainSizeInBytes_Compressed since the chain must be itterated through in a fully compressed state. Provides a more accurate result
	int32 ReturnChainSizeInBytes_HyperCompressed(TArray<int16> Adr)
	{
		TArray<Group*> *Chain = ReturnChain(Adr);
		int32 size = 0;

		for (auto ChainSingle : *Chain)
		{
			for (auto LTLSingle : ChainSingle->LTL)
			{
				if (LTLSingle != -1)
					size += 4;
			}
		}

		return size;
	};
	//Returns the amount of (Attribute Addresses + Padding) * sizeof(int). Does not fully decompress the chain, and therefore is only to be used exclusively. Fastest size gauge, with medium accuracy
	int32 ReturnChainSizeInBytes_Compressed(TArray<int16> Adr)
	{
		TArray<Group*> *Chain = ReturnChain(Adr);
		int32 size = 0;

		for (auto ChainSingle : *Chain)
		{
			size += ChainSingle->LTL.Num() * 4;
		}
		return size;
	};
	//Slowest size gauge. Required for GPGPU
	int32 ReturnChainSizeInBytes_Uncompressed(TArray<int16> Adr)
	{
		TArray<int32> vec = ReturnAttributeChain(Adr);
		return vec.Num() * 4;
	};
};

static int32 binarySearch(TArray<float> arr, int32 value, int32 min, int32 max){
	int32 pos = -1;

	while (max >= min && pos == -1) {
		int32 mid = min + (max - min) / 2;

		if (arr[mid] == value){
			pos = mid;
		}
		else if (arr[mid] < value){
			min = mid + 1;
		}
		else if (arr[mid] > value){
			max = mid - 1;
		}

	}
	return pos;
};

struct Monitization //Contains a set of helper functions for assisting the entity to figure out what helps it the most in its current eco state
{
	/*
	Erf positive is use for tasks with heigh weights. Tanh is slightly lower weighted than Erf. XPlusAbsX is the lowest ranked
	*/
	static float Attune(float input) //Is an increase from 30% to 60% worth just as much as an increase from 60% to 90%?
	{
		return 10 / input;
	};
	static float TimeAspect(float inputscore, int32 Delta, int32 EntityValue) //How much time is this process going to take? Are we going to die completing it?
	{
		inputscore *= Delta; //How much food do we need during this time?
		inputscore = 1 - (inputscore / EntityValue); //The more food it takes to complete a task, the lower timeaspect becomes
		return inputscore;
	};
	static float CostAttune_Erf_Positive(int32 Cost, int32 EntityValue) //This function is a sigmoid graph. At 0 cost, this function returns 1. At max cost, this function returns near 0
	{

		if (Cost > EntityValue)
			return -1;
		float var = (((float)Cost / (float)EntityValue) * 4) - 2; //This is what we feed into Error Function (http://mathworld.wolfram.com/Erf.html)
		var = (float)erf((M_PI / 2) * var);
		var += 1;
		var = 1 - var;

		//The higher the cost goes, the closer this value gets to 0. It never becomes negative. Look at the wolfram link for more information. All that is done here is compressions and scaling. (Just so the value returned is (0,1])
		return var;
	}
	static float CostAttune_Tanh_Positive(int32 Cost, int32 EntityValue)
	{
		if (Cost > EntityValue)
			return -1;
		float var = (((float)Cost / (float)EntityValue) * 4) - 2;

		var = tanh(var);
		var += 1;
		var = 1 - var;

		return var;
	}
	static float CostAttune_XPlusAbsX_Positive(int32 Cost, int32 EntityValue)
	{
		if (Cost > EntityValue)
			return -1;
		float var = (((float)Cost / (float)EntityValue) * 4) - 2;

		var = var / (1 + abs(var));
		var += 1;
		var = 1 - var;

		return var;
	};
	static double AutoWeightedScore_Negative(int32 L, float inputscore, int32 Cost, int32 EntityValue, Gradient grad = Gradient(20, 30, 50, 60)) //L = score1 (The value we're trying to lerp) This is also quite overpowered
	{

		if (L <= grad.Low) //We're only using Erf
		{
			return (inputscore * CostAttune_Erf_Positive(Cost, EntityValue));
		}
		else if (L <= grad.Q1)
		{
			float t = ((float)grad.Q1 + (float)grad.Q3) / (float)2 - (float)L; //Distance from the First Quartile point
			if (t < 1)
				t = 1;
			t = ((grad.Q3 - grad.Q1) - t) / (grad.Q3 - grad.Q1); //The further the number is from the First Quartil, the smaller it gets. We're comparing against the interquartile range
			return inputscore* ((1 - t) * CostAttune_Tanh_Positive(Cost, EntityValue)) + ((t)* CostAttune_Erf_Positive(Cost, EntityValue));
		}
		else if (L <= grad.Q3)
		{
			return (inputscore * CostAttune_Tanh_Positive(Cost, EntityValue));
		}
		return (inputscore * CostAttune_XPlusAbsX_Positive(Cost, EntityValue));
	}
	static float StaticWeightedScore_Negative(int32 L, float inputscore, int32 Cost, int32 EntityValue)
	{
		if (L< 20) //It needs to be weighed highly
			return Monitization::CostAttune_Erf_Positive(Cost, EntityValue);
		else if (L < 50) //Weighed in a medium matter
			return Monitization::CostAttune_Tanh_Positive(Cost, EntityValue);
		else //Unweighed
			return Monitization::CostAttune_XPlusAbsX_Positive(Cost, EntityValue);
	}
	//Feel free to add any more models you want :D We max out at about 40 models or so, so no need to hold back

	static float Score(int32 input1, int32 input2, int32 Delta, int32 EntityValue, float Completion) //Provides your base score. This is just an upper level for Attune and TimeAspect. Does nothing differently than managing attune and time aspect individually
	{
		float temp = Monitization::Attune((float)input1) - Monitization::Attune((float)input2);
		temp *= Completion;
		return TimeAspect(temp, Delta, EntityValue);
	}
	static float PrecomputedTimeAspectScore(int32 input1, int32 input2, float Pre)
	{
		float temp = Monitization::Attune((float)input1) - Monitization::Attune((float)input2);
		return temp / Pre;
	};
};
#endif