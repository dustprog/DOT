#ifndef TAGBASE_H
#define TAGBASE_H
#include "InterfaceBase.h"
#include "AdvertisementBase.h"
#include "../MathUtility/Sigmoid.h"
#include "../../ZETA_Alpha/Containers/GenericContainer.h"
#include <ZETA_Alpha/Containers/Node.h>
#define LearningRate 10
#define Nonrelative_Max 100
//http://www.wseas.us/e-library/conferences/2005salzburg/papers/492-271.pdf
struct CriticBase
{
    TContainer<CostBase> DesiredAttributes;
    TContainer<SNibble> Coefficients;
    //TOOD: Move into entity group as a DOD format
    static float f_sum(TContainer<UNibble> ReferenceTable, EntityStructure _entity)
    {
        float f_sum = 0.0f;
        for(int i = 0; i < DesiredAttributes.size(); i++)
        {
            float r = _entity.Attributes[ReferenceTable[DesiredAttributes[i].GlobalAdr]];

            r = ((float)Coefficients[i] / 128) * Scoring::PositiveScore(r,0,DesiredAttributes[i].Value,Nonrelative_Max);

            f_sum += r;
        }
        return f_sum;
    }
    //Value for X_New. Weighted sum must be recalcalted per attribute. However its base value is done once per itteration, (Usually via PreAdUpdate). f_sum is calculated once per itteration.
    static float computeConstant(float WeighedSum, float X_old, float f_sum = f_sum(EntityStructure*))
    {
        //Set AttributeBase::Opinion equal to X_new. This is a neural network system, where the amount of neurons per tag are equal to DesiredAttributes.size()
        return X_old + (LearningRate * f_sum * WeighedSum);
    }
};

#endif // TAGBASE_H
