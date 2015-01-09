#ifndef BRANCHPREDICTOR_H
#define BRANCHPREDICTOR_H
#include <DOT_Alpha/Interface/EntityBase.h>
#define UNSIGNED_SHORT_MAX 65535
struct Prediction
{
    //How often did this advertisement occur in our prediction?
    unsigned short Amt;
    //Which index is it stored at globally
    unsigned short Index;
};
#ifndef OVERLOAD_PREDICTION
/*static bool EqualPredictions(Prediction i, Prediction j)
{
    return (i.Amt == j.Amt);
}
static bool GreaterPredictions(Prediction i, Prediction j)
{
    return (i.Amt > j.Amt);
}*/
static bool EqualPredictionIndexes(Prediction i, Prediction j)
{
    return (i.Index == j.Index);
}
static bool GreaterPredictionIndexes(Prediction i, Prediction j)
{
    return (i.Index < j.Index);
}
#endif
/*
    Short predictor is utilized to figure out the top most used advertisements by a group of AIs. If food is being used more than its being produced,
    this class would allow a meta AI to either spawn more food, or direct entities in different directions.

    Its a work in progress, but will be needed for Spatial Meta Groups in beta.
*/
//Uses a MapReduce implementation
struct ShortPredictor
{
private:
    //Removes duplicates from the Predictions. Akin to a reduction, however its only 1 dimensional
    void ErrorCorrect()
    {
        std::vector<Prediction> PredictionVector(Indicies.Interface, Indicies.Interface + Indicies.size());

        std::vector<Prediction>::iterator it;
        //Sort the prediction vector first
        std::sort(PredictionVector.begin(), PredictionVector.end(),GreaterPredictionIndexes);
        //Make sure there are no duplicate indexes
        it = std::unique(PredictionVector.begin(), PredictionVector.end(), EqualPredictionIndexes);
        //Resize our prediction vector to the outputted itterator
        int Size = std::distance(PredictionVector.begin(), it);
        PredictionVector.resize(Size);
        //Copy the vector back to indicies
        Indicies.Interface = &PredictionVector[0];
        Indicies.Size = (UNibble)Size;
    }

public:
    TContainer<Prediction> Indicies;
    ShortPredictor()
    {
        Indicies.resize(255);//The 0th index is always the maximum possible value a short can be
        Indicies[0].Amt =UNSIGNED_SHORT_MAX; //This is done so that the first index is always ignored by the sort

    }

    //Computes local advertisements for a set of entities. Utilizing the Manhatten distance, it determines which advertisements are the most viable decisions and rejects the rest
    void Localize()
    {
        //TODO: Link to KD-Tree
    }

    //Returns the 255 most used indexes by IEntityGroup's queue. This reduces the need to copy every advertisement to the GPU
    void Map(IEntityGroup *ToComputeOver)
    {

        TContainer<unsigned short> map; //The array is not 2 dimenasional to avoid the need for cache blocking. Cache blocking is done before hand by IEntityGroup
        unsigned short AmtOfAdsInWorld; // = GetAmtAds(); This is the total number of advertisement templates and NOT the total number of instances.
        map.resize(AmtOfAdsInWorld); //Make sure every value is set to zero
        for(int i = 0; i < ToComputeOver->ToUpdate.size(); i++)
        {
            for(int j = 0; i < ToComputeOver->ToUpdate[i].Ad.size(); j++)
            {
                unsigned short temp = ToComputeOver->ToUpdate[i].Ad[j];
                map[temp]++;
                //Approximated sort function. Computes in O(N) time. It does not care that much about order, rather it pulls the first 255 top
                for(int k = Indicies.size() - 2; k >= 0; k--)
                {
                    signed short BitMask = (Indicies[k].Amt - map[temp]) >> 15;
                    Indicies[k + 1].Index = (temp)&~BitMask; //If our Indicies at point k is greater than map[temp], set Index{k+1] (Therefore the previous index) to map[temp]
                    Indicies[k + 1].Amt = (map[temp])&~BitMask;
                    k = (0)&~BitMask; //If we've found out point, set k equal to zero so we leave the for loop
                }
            }
        }
        ErrorCorrect();
    }

    //Takes all computed maps from a chain and reduces into a one dimensional array.
    void Reduce(TContainer<TContainer<short>> Map)
    {
        for(int i = 0; i < Map.size(); i++)
        {

        }
    }
};

#endif // BRANCHPREDICTOR_H
