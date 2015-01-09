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

//Expermenting with Data Oriented Design
#pragma pack(1)
struct IEntityGroup
{
    IEntityGroup(){}
    //Prebaked
    //The entire reference table from this node to the origin
    TContainer<UNibble> ReferenceTable;
    //A list of all gradients created by this leaf node
    TContainer<Gradient> TemplateBody;
    //TContainer<Tag> TagBody;

    //Used as a timing mechanism for threads
    TContainer_Short<DeathTimeConstant> Timing;
    //Recalculated per entity after said entity completes an advertisement. This value takes time into consideration when deriving a score.
    TContainer_Short<WeighedSum> TransferConstant;
    //Attributes that are modified at runtime
    TContainer_Short<AttributeBase> RuntimeBody;
    //A list of objectives that the entity has
    TContainer_Short<DirectAdvertisementQueue> AdQueue;
    //Contains a list of all entities that need to be updated in the next pass
    TContainer_Short<ShortQueue> ToUpdate;

    //The amount of all attributes up this this point inclusive. (ReferenceTableSum.size())
    short BlockSize;
    //Returns the index of the new instance
    short CreateNewInstance()
    {
        short Size = RuntimeBody.size();
        TransferConstant.resize(Size + 1);
        Timing.resize(Size + 1);
        AdQueue.resize(Size + 1);
        RuntimeBody.resize((Size + 1) * BlockSize); //Block size is the total amount of attributes thats present at that section (ReferenceTableSum.size())
        return Size + 1;
    }
    //Extends the length of an NPC's add chain by one, and replaces their next item in queue.
    void ReduceAdChain(short Index, AdvertisementBase *Ad)
    {
        AdQueue[Index].Indirect.pop_back();
        AdQueue[Index].Next = Ad;
    }

    void ExecuteQueue();

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

    IndexScore ComputeInference(short Index, AdvertisementBase *Ad);

    float ReturnScore(short Index, AdvertisementBase *Ad);

    /*At this point it is already assumbed that we do indeed want this goal
    We're looking for the lowest value for the distance between our selected local advertisement, and our destination advertisement.
    This attempts to solve a very simple multivariable optimization problem. It, however, is only an approximation. During runtime, the AI must still
    calculate whether or not it wishes to follow its suggested plan.
    */
    TContainer<IndirectAd> Plan(short Index, AdvertisementBase *Goal);
};


#endif
