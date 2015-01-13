#include "EntityBase.h"
float IEntityGroup::ReturnScore(short Index, AdvertisementBase Ad)
{
    int BitMask;
    UNibble LocalIndex;
    bool NotValid = true;
    unsigned short ConvIndex;
    int CurrentScore, FutureScore, Length;
    float t, k, Inferscore, WeightedSumSigmoid;
    Gradient grad;

    ConvIndex = Index * BlockSize;
    t = k = Inferscore = 1;
    //How long will this advertisment take us? Storing a temporary value since this is used by both WeightedSigmoid and Length
    float fLength =  (float)TransferConstant[Index].Sum  / WS_PERCISION;

    Length = 1 / fLength;
    Length *= (int)(float)Ad.TimeConstant + 1;

    //Store a temporary copy of our effect
    for(const CostBase & CurrentEffect : Ad.GetPositiveEffects())
    {
        //Find what attribute we're talking about
        LocalIndex = ReferenceTable[CurrentEffect.GlobalAdr];
        grad = TemplateBody[LocalIndex];

        LocalIndex += ConvIndex;
        //Current is the score before starting the ad, Future would be what it is after

        CurrentScore = (float)RuntimeBody[LocalIndex].Value;
        FutureScore = CurrentScore + (float)(CurrentEffect.Value) + (-grad.Delta) * Length;

        //Does it go over the maximum value? If so, set it to max
        BitMask =    (grad.Max - (int)FutureScore) >> (sizeof(int) * CHAR_BIT - 1);
        FutureScore =       ((grad.Max)&BitMask) | ((FutureScore)&~BitMask);
        NotValid = ((false)&~BitMask) | ((true)&BitMask); //Our results have been vailidated, and this advertisement will help atleast one attribute. This should be done during a precompute stage however, and not this late

        //Compute a sigmoid. Opinion is utilized as another way to see how heavily an NPC weighs certain attributes
        t += (31.0f / (float)RuntimeBody[LocalIndex].Opinion) * Scoring::PositiveScore(CurrentScore, FutureScore, grad); //Average positive return val
    }
    if (NotValid) //All of our attributes are already at their maximum value
        return -std::abs(t); //Make sure that advertisement isn't used

    //Take the average score
    t /= (float)(Ad.GetPositiveEffects().size() + 1);


    //Compute a quick sigmoid
    WeightedSumSigmoid = Ad.TimeConstant;

    WeightedSumSigmoid = WeightedSumSigmoid / (1 + WeightedSumSigmoid);
    WeightedSumSigmoid *= fLength;

    //Are we going to die before we complete this advertisement?
    t /= WeightedSumSigmoid;


    for (const CostBase &  CurrentEffect : Ad.GetNegativeEffects())
    {
        //Find what attribute we're talking about
        LocalIndex = ReferenceTable[CurrentEffect.GlobalAdr];
        grad = TemplateBody[LocalIndex];

        LocalIndex += ConvIndex;
        //Current is the score before starting the ad, Future would be what it is after

        CurrentScore = RuntimeBody[LocalIndex].Value;

        //How much will this hurt us?
        FutureScore = CurrentScore - (float)(CurrentEffect.Value) + (grad.Delta) * Length;

        //If this would kill us, just return the negative absolute value of our positive score. Theres no way to avoid branching here
        if (FutureScore <= 0)
            return -std::abs(t);


        //Compute a sigmoid. Opinion is utilized as another way to see how heavily an NPC weighs certain attributes
        k += (31.0f / (float)RuntimeBody[LocalIndex].Opinion) * Scoring::NegativeScore((float)CurrentScore, (float)FutureScore, grad); //Average negative return val
    }
    k /= (float)(Ad.GetNegativeEffects().size() + 1);

    /*
        if(Ad->Infer.size() > 0)
        {
            Inferscore = ComputeInference(Ad);
        }
        */
    return Inferscore + (t * k);
}
TContainer<IndirectAd> IEntityGroup::Plan(short Index, AdvertisementBase Goal)
{
    TContainer<IndirectAd> SolutionSet;

    //The summation of all advertisements we've confirmed so far
    TContainer<CostBase> NeededAttributes = TContainer<CostBase>(BlockSize);
    int m = 0;
    for (int i = BlockSize * Index; i < BlockSize * (Index+1); i++)
    {
        NeededAttributes[m].Value = RuntimeBody[i].Value;
        m++;
    }


    //TOOD: Get local advertisements from the world structure
    TContainer<AdvertisementBase*> LocalAds;

    bool Finished = false; //When this is true, all of the needs for my objective have been met.
    while (!Finished)
    {
        int i = 0;
        //The value cannot get greater than -1
        float Lowest = 1;
        UNibble index = 0;

        //Cycle through all the local goals. See what score is the lowest from after this advertisement -> Destination. Basic path finding
        for (AdvertisementBase * CurrentAd : LocalAds)
        {
            float AverageScore = 0;

            auto NeededTemp = NeededAttributes;

            //Go through every positive cost. Do not worry about negative effects right now. Thats managed as error correcting
            for (const CostBase & CurrentCost : CurrentAd->GetPositiveEffects())
            {
                //How much does this add increase our attributes by?
                NeededTemp[ReferenceTable[CurrentCost.GlobalAdr]].Value += CurrentCost.Value;
            }
            //Does this ad let us meet the requirements?
            for (const CostBase & DestinationCost : Goal.GetNegativeEffects())
            {
                //TODO: Retrieve Gradient function
                Gradient grad = TemplateBody[ReferenceTable[DestinationCost.GlobalAdr]];
                grad.Q3 = DestinationCost.Value; //Anything that would throw us over the value is rated to be negative

                AverageScore += Scoring::PositiveScore_Linear((float)NeededTemp[ReferenceTable[DestinationCost.GlobalAdr]].Value, (float)DestinationCost.Value, grad); //Find the score between our destinatin cost, and our temporary attribute value
            }
            //Divide by the full size
            AverageScore /= Goal.GetNegativeEffects().size();

            if (AverageScore < Lowest)
            {
                //Its our new lowest score, so lets change the value of Lowest
                Lowest = AverageScore;
                index = i;
            }
            if (AverageScore <= 0)
            {
                //We've finished the chain. More than half of the needs are met. They rest are computed during runtime to allow for some dynamic response.
                index = i;
                Finished = true;
                break;
            }
            i++;
        }

        /*Queue is not an order dependent list. Infact, after running the plan function, it is suggested that Queue be ran through a more extensive
            scheduling function in order to retain realism.*/
        IndirectAd ad;
        ad.Address = LocalAds[index]->ReturnAddress();
        ad.TimeConstant = LocalAds[index]->TimeConstant;
        SolutionSet.push_back(ad);
        //Make sure that our summation is kept up to date
        for (auto Cost : LocalAds[index]->GetPositiveEffects())
        {
            NeededAttributes[ReferenceTable[Cost.GlobalAdr]].Value += Cost.Value;
        }
    }
    return SolutionSet;

}
void IEntityGroup::ExecuteQueue()
{

    //Ordering of entity Queues:
    //Index 0 = The NPC's next plot objective
    //Lower indicides are things that the NPC is more likely to actually want (ie: Spatialy closer advertisements that it meets the requirements for)
    //Higher indicides are eveything else

    //Go through every entity in the group
    for(int i = 0; i < ToUpdate.size(); i++)
    {

        float HighestScore = 0;
        int Index = 0;
         //Go through all the advertisements on their queue
        for(int j = 0; j < ToUpdate[i].Ad.size(); j++)
        {

            AdvertisementBase Ad = ToUpdate[i].Ad[j];
            float temp = ReturnScore(ToUpdate[i].Index, Ad);
            //If its higher than our current score, set i to this index
            if(temp > HighestScore)
                Index = j;
        }
        //Index is 0 when we're currently continuing our plot queue. We haven't changed our mind about what advertisement we want
        if(Index !=0)
        {
            AdvertisementBase *Ad = World::get()->getTemplateAd(ToUpdate[i].Ad[Index]);
            this->ReduceAdChain(ToUpdate[i].Index, Ad);
            Ad->BeginUse();
        }

        else
        {
            //TODO: Plan
            AdvertisementBase Ad = *AdQueue.Interface[ToUpdate[i].Index].Next;
            Plan(ToUpdate[i].Index,Ad);
        }
    }
}
IndexScore IEntityGroup::ComputeInference(short Index, AdvertisementBase *Ad)
{
    IndexScore _indexscore;
    _indexscore.Score = 0;
    /*
        for(int i = 0; i < Advert.Inferences.size(); i++)
        {
        float temp = ReturnScore(Advert.Inferences[i], ReferenceTable);
        if(_indexscore.Score < temp)
        {
        _indexscore.Score = temp;
        _indexscore.Index = i;
        }
        }
        */
    return _indexscore;
}
