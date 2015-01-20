#include "../Interface/EntityBase.h"
#include "InterfaceBuilder.h"

DB* DB::m_instance = nullptr;


WorldCompiled DB::Compile()
{
    WorldCompiled _compiled;
    //Compile the list of all of our attributes
    for(int i = 0; i < attr_list->size(); i++)
    {
        AttributeTemplate attr_temp= (*attr_list)[i];
        _compiled.Attributes.push_back(AttributeBase(attr_temp.Grad,attr_temp.Value, attr_temp.Opinion));
        _compiled.Gradients.push_back(attr_temp.Grad);
    }
    //Set up all of our advertisements and cross reference them to their correlating advertisements
    for(int i = 0; i < ad_list->size(); i++)
    {
        AdvertisementTemplate ad_temp= (*ad_list)[i];

        AdvertisementBase _actual_ad = AdvertisementBase(ad_temp.PolyUsability,0,ad_temp.TimeConstant);
        //Cross reference all of our positive attributes
        for(int j = 0; j < ad_temp.Positive.size(); j++)
        {
            _actual_ad.AddPositiveEffect(CostBase(attr_Table->at(ad_temp.Positive[i].Name.c_str()),ad_temp.Positive[i].cost));
        }
        //Now cross reference all of our negative ones
        for(int j = 0; j < ad_temp.Negative.size(); j++)
        {
            _actual_ad.AddNegativeEffect(CostBase(attr_Table->at(ad_temp.Negative[i].Name.c_str()),ad_temp.Negative[i].cost));
        }
        _compiled.AdInstances.push_back(_actual_ad);
    }
    //Make sure all of our tags are compiled before we start on entities
    for(int i = 0; i < tag_list->size(); i++)
    {
        TagTemplate tempTag = (*tag_list)[i];

        TagCompiled compTag;
        for(int j = 0; j < tempTag.Attributes.size(); j++)
        {
           CostBase cost;
           unsigned short Index = attr_Table->at(tempTag.Attributes[j].c_str());

           AttributeBase attrTemp = _compiled.Attributes[Index];
           //TODO: Eliminate branching
           cost.Value = tempTag.ValueOverride[j];
           //There was no override
           if( cost.Value  == 0)
                 cost.Value = attrTemp.Value;

            cost.GlobalAdr = Index;

            compTag.Attributes.push_back(cost);
            compTag.Coefficients.push_back(compTag.Attributes.size() - 1,tempTag.Coefficients[j]);
        }
        for(int j = 0; j < tempTag.LifeObjectives.size(); j++)
        {
            unsigned short AdPos = ad_table->at(tempTag.LifeObjectives[j].c_str());

            compTag.LifeObjectives.push_back(AdPos);
        }

        _compiled.TagInstaces.push_back(compTag);

    }
    bool done = false;
    TNode<EntityTemplate> node = *entity_Tree;

    //The system starts at node(0) and climbs down the first branch summing all attributes along the way. After child.size <= 0, referencetablesum and GradientTableSum are subtracted from.
    TContainer<TContainer_Short<short>> ReferenceTableSum;
    bool Check = false;
    //How far down the tree are we
    unsigned short Distance = 0;
    while(!done)
    {

       while(node.Children.size() > 0 || !Check)
       {
           //This is now our last route before we start backing up
           if(node.Children.size() <= 0 || (!&node.Children))
               Check = true;
            else
               Check = false;
            IEntityGroup group;
            ReferenceTableSum.push_back(TContainer_Short<short>());
            for(auto itt = node.getData().Attributes.begin(); itt != node.getData().Attributes.end(); itt++)
            {
                //Global coordinates
                ReferenceTableSum[ReferenceTableSum.size() - 1].push_back(attr_Table->at((*itt).c_str()));
            }
            //How large is our vector before we expand it?
            short CurrentSize = ReferenceTableSum[ReferenceTableSum.size() - 1].size();
            std::vector<short> ReferenceVector(ReferenceTableSum[ReferenceTableSum.size() - 1].Interface, ReferenceTableSum[ReferenceTableSum.size() - 1].Interface + ReferenceTableSum[ReferenceTableSum.size() - 1].size());

            //Sort the vector so that we know that our tree is being ordered in the correct way
            std::sort(ReferenceVector.begin(), ReferenceVector.end());

            //Bring the vector back to our TContainer
            ReferenceTableSum[ReferenceTableSum.size() - 1].Interface = &ReferenceVector[0];

            //Expansion
            group.SetGlobalAttributeBufferSize(ReferenceTableSum[ReferenceTableSum.size() - 1].end().Index);
            for(UNibble j = 0; j < ReferenceTableSum.size(); j++)
                for(UNibble i = 0; i < CurrentSize; i++)
                {
                    //Add all of our summed attributes into the group
                    group.AddAttribute(ReferenceTableSum[j].Interface[i], (*attr_list)[ReferenceTableSum[j].Interface[i]].Grad );
                }


            for(auto itt = node.getData().Tags.begin(); itt != node.getData().Tags.end(); itt++)
            {
                unsigned short tempTag = tag_table->at((*itt).c_str());
                group.AddTag(_compiled.TagInstaces[tempTag].Attributes,
                             _compiled.TagInstaces[tempTag].Coefficients,
                             _compiled.TagInstaces[tempTag].LifeObjectives);
            }
            for(unsigned int i = 0; i < node.getData().instances; i++)
            {
                group.CreateNewInstance();
            }

            //TODO: Organize in the compiled results
            _compiled.EntityTemplateTree.SetData(group);

            //If this was our last run before we start to retract, lets break
            if(Check)
                break;
            //Go to the next first child in the link
            node = *node.Children.front();
            Distance++;

       }
       //We've done a full circle, and there are no more subnodes below us.
       if(Distance == 0 && node.Children.size() == 0)
           break;
       //Our current node has no children. lets go back until we've found a child
       TNode<EntityTemplate> tempnode = *node.GetParent();
       tempnode.Children.Remove(0);
       node = tempnode;
       ReferenceTableSum.pop_back();
       Distance--;
   }

   return _compiled;
}
