#ifndef ENTITYBASE_HEADER
#define ENTITYBASE_HEADER
#define WS_PERCISION 100
//The higher this value, the more inaccurate the score will be
#define SCORE_APPROX_PERCISION 2
#include "InterfaceBase.h"
#include "AdvertisementBase.h"
#include <list>
#include "../MathUtility/Sigmoid.h"
#include "../../ZETA_Alpha/Containers/GenericContainer.h"
#include <ZETA_Alpha/Containers/Node.h>
#define LearningRate 10
#pragma pack(1)
struct IEntityGroup
{
    IEntityGroup(){ BlockSize = 0;}
    //Prebaked
    //The entire reference table from this node to the origin
    TContainer_NULL<UNibble> ReferenceTable; //Size is irrelavent and only available through World
    //A list of all gradients created by this leaf node
    TContainer_NULL<Gradient> TemplateBody; //Size is equal to BlockSize
    //TContainer<Tag> TagBody;

    //Used as a timing mechanism for threads
    TContainer_Short<DeathTimeConstant> Timing;
    //Recalculated per entity after said entity completes an advertisement. This value takes time into consideration when deriving a score.
    TContainer_NULL<WeighedSum> TransferConstant;
    //Attributes that are modified at runtime
    TContainer_Short<AttributeBase> RuntimeBody;
    //A list of objectives that the entity has
    TContainer_NULL<DirectAdvertisementQueue> AdQueue;
    //Contains a list of all entities that need to be updated in the next pass
    TContainer_Short<ShortQueue> ToUpdate;

    //The amount of all attributes up this this point inclusive. (TemplateBody.size())
    short BlockSize;
    //Returns the index of the new instance
    short CreateNewInstance()
    {
        short Size = Timing.size();
        TransferConstant.resize(Size, Size + 1);
        Timing.resize(Size + 1);
        AdQueue.resize(Size, Size + 1);
        RuntimeBody.resize((Size + 1) * BlockSize); //Block size is the total amount of attributes thats present at that section (TemplateBody.size())
        return Size;
    }
    //Extends the length of an NPC's add chain by one, and replaces their next item in queue.
    void ReduceAdChain(short Index, AdvertisementBase *Ad)
    {
        AdQueue[Index].Indirect.pop_back();
        AdQueue[Index].Next = Ad;
    }

    void ExecuteQueue();
#ifndef NO_PREPARE
    //Computes the transfer constant. Utilized directly by Critic, and indirectly as the Weighted Sum. See DOT's wiki for more details
    void Prepare(short Index)
    {
        float WeighedSumtemp = 0;
        for (int i = 0; i < BlockSize; i++)
        {
            short temp;
            Gradient grad = TemplateBody[i];
            if (RuntimeBody[(Index * BlockSize) + i].Value > 0)
                temp = grad.Max / (RuntimeBody[(Index * BlockSize) + i].Value);
            else
                break;
            //Are we going to die before we finish this activity?
            WeighedSumtemp += (Scoring::TimeAspect(temp, grad.Delta, RuntimeBody[(Index * BlockSize) + i].Value));
        }
        WeighedSumtemp /= BlockSize;
        TransferConstant[Index] = (int)(WeighedSumtemp * WS_PERCISION);
    }
#endif
    IndexScore ComputeInference(short Index, AdvertisementBase *Ad);

    float ReturnScore(short Index, AdvertisementBase Ad);

    /*At this point it is already assumbed that we do indeed want this goal
    We're looking for the lowest value for the distance between our selected local advertisement, and our destination advertisement.
    This attempts to solve a very simple multivariable optimization problem. It, however, is only an approximation. During runtime, the AI must still
    calculate whether or not it wishes to follow its suggested plan.
    */
    TContainer<IndirectAd> Plan(short Index, AdvertisementBase Goal);


    void AddAttribute(const UNibble GlobalAdr, const Gradient grad = Gradient()) { ReferenceTable[GlobalAdr] = BlockSize; TemplateBody.push_back(BlockSize,GlobalAdr); BlockSize++; }
    //Warning: Deletes all data within the local reference table
    void SetGlobalAttributeBufferSize(short Size) { ReferenceTable.Interface = new UNibble[Size]; }

    //Deleted names since they would be deleted anyway during DOT's prebake phase
#ifndef NO_TAGS //WARNING: LACKING OPTIMIZATION. CURRENTLY A WIP. NOT VIABLE UNTIL BETA
	//indices of tags in tag list per entity 
    TContainer_NULL<TContainer<short>> TagIndicesPerEntity;

	//Life objectives for all tags allowed in this entity group
    TContainer<AdvertisementBase> TagObjectives; //This data will rarely get accessed. Perhaps we should move it somewhere else?

    //The attributes that the tags wish to modify
    TContainer_Short<CostBase> DesiredAttributes;

    //Coefficients of emotional importance. NULL, since it will always be the same size as Desired Attributes
    TContainer_NULL<SNibble> Coefficients;

	//calculates the sum per the equation
    float f_sum(short Index)
	{
		float f_sum = 0.0f;
		for (int i = 0; i < DesiredAttributes.size(); i++)
		{
            //R = the emotional signal produced by the desire for this attribute
            float r = RuntimeBody[ReferenceTable[DesiredAttributes[i].GlobalAdr] + (BlockSize * Index)];

            r = ((float)Coefficients[i] / 128) * Scoring::PositiveScore(r, 0, DesiredAttributes[i].Value, TemplateBody[ReferenceTable[DesiredAttributes[i].GlobalAdr]].Max);

			f_sum += r;
		}
		return f_sum;
    }

	//Value for X_New. Weighted sum must be recalcalted per attribute. However its base value is done once per itteration, (Usually via PreAdUpdate). f_sum is calculated once per itteration.
    float computeEmotionalConstant(float WeighedSum, float X_old, float f_sum)
    {
        //Set AttributeBase::Opinion equal to X_new. This is a neural network system, where the amount of neurons per tag are equal to DesiredAttributes.size()
		return X_old + (LearningRate * f_sum * WeighedSum);
	}
#endif
};
#endif
