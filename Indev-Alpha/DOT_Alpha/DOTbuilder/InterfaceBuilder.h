#ifndef INTERFACEBUILDER_H
#define INTERFACEBUILDER_H
#include "../Interface/EntityBase.h"
#include <ZETA_Alpha/Manager/SearchTreeManager.h>
#include <boost/container/map.hpp>
using namespace boost;
using namespace container;

struct EntityTemplate
{
public:
    StringInterface Name;
    StringInterface Parent;
    TContainer_Short<StringInterface> Attributes;
    TContainer_Short<StringInterface> Tags;
    unsigned int instances;

    EntityTemplate() {}
    static EntityTemplate _create(const char* Name) { auto e = new EntityTemplate(); e->Name = StringInterface(Name); return e->_deRef(); }
    EntityTemplate _deRef() {return *this; }
    EntityTemplate _inheritsFrom(const char* Name){ this->Parent = StringInterface(Name); return *this; }
    EntityTemplate _addAttribute(const char *Name){ this->Attributes.push_front(StringInterface(Name)); return *this; }
    EntityTemplate _addTag(const char* Name) {this->Attributes.push_back(StringInterface(Name)); return *this; }
    EntityTemplate _instance(unsigned int amt) {this->instances = amt; return *this; }
};
struct AttributeTemplate
{
public:
    Gradient Grad;
    short Value;
    SNibble Opinion;
    StringInterface Name;

    AttributeTemplate() {}
    static AttributeTemplate _create(const  char *Name) { auto e = new AttributeTemplate(); e->Name = StringInterface(Name); return e->_deRef(); }
    AttributeTemplate _deRef() {return *this; }
    AttributeTemplate _setGradient(Gradient grad = Gradient()) { this->Grad = grad; return *this; }
    AttributeTemplate _setValue(short val = 50) { this->Value = val; return *this; }
    AttributeTemplate _setOpinion(SNibble Opinion = 31U) {this->Opinion = Opinion; return *this; }
};
struct TagTemplate
{
public:
    TContainer_Short<StringInterface> Attributes;
    TContainer_NULL<unsigned int> ValueOverride;
    TContainer_NULL<SNibble> Coefficients;
    TContainer_Short<StringInterface> LifeObjectives;
    StringInterface Name;

    TagTemplate() {}
    static TagTemplate _create(const  char *Name) { auto e = new TagTemplate(); e->Name = StringInterface(Name); return e->_deRef(); }
    TagTemplate _deRef() {return *this; }
    TagTemplate _addAttribute(const char *Name, SNibble CoefficientOfImportance = 1, unsigned int Override = 0) { this->Attributes.push_front(StringInterface(Name)); this->ValueOverride.push_back(Attributes.size() - 1, Override); this->Coefficients.push_back(Attributes.size() - 1, CoefficientOfImportance); return *this; }
    TagTemplate _addLiveObjective(const char *Name) { this->LifeObjectives.push_back(StringInterface(Name)); return *this; }
};
struct CostTemplate
{
public:
    unsigned int cost;
    StringInterface Name;

    CostTemplate(unsigned int cost=20, StringInterface Name = StringInterface("Default")) {this->cost = cost; this->Name = Name; }
};
struct AdvertisementTemplate
{
public:

    StringInterface Name;
    TContainer_Short<CostTemplate> Positive;
    TContainer_Short<CostTemplate> Negative;
    UNibble TimeConstant;
    UNibble PolyUsability;


    AdvertisementTemplate() {}
    static AdvertisementTemplate _create(const  char *Name) { auto e = new AdvertisementTemplate(); e->Name = StringInterface(Name); return e->_deRef(); }
    AdvertisementTemplate _deRef() {return *this; }
    AdvertisementTemplate _addPositiveCost(const char* cost, unsigned int amount) { this->Positive.push_back(CostTemplate(amount,StringInterface(cost))); return *this; }
    AdvertisementTemplate _addNegativeCost(const char* cost, unsigned int amount) { this->Negative.push_back(CostTemplate(amount,StringInterface(cost))); return *this; }
    AdvertisementTemplate _setTimeConstant(UNibble TC) { this->TimeConstant = TC; return *this; }
    AdvertisementTemplate _setPolyUsability(UNibble PU) { this->PolyUsability = PU; return *this; }
};

//Look up tables.
typedef boost::container::map<const char*, unsigned short> AttributeTable;
typedef boost::container::map<const char*, unsigned short> AdvertisementTable;
typedef boost::container::map<const char*, unsigned short> TagTable;
typedef boost::container::map<const char*, TAddress> EntityGroupTable;

struct IEntityGroup;
//Ouput from THE GLORIOUS COMPILER!
struct WorldCompiled
{

    TContainer_Short<AttributeBase> Attributes;
    TContainer_Short<AdvertisementBase> AdInstances;
    TNode<IEntityGroup *> EntityTemplateTree;

};
//THE GLORIOUS COMPILER!
struct DB
{
private:
    static DB *m_instance;

    AttributeTable *attr_Table;
    TContainer_Short<AttributeTemplate> *attr_list;


    AdvertisementTable *ad_table;
    TContainer_Short<AdvertisementTemplate> *ad_list;

    TagTable *tag_table;
    TContainer_Short<TagTemplate> *tag_list;

    EntityGroupTable *entity_Table;
    TNode<EntityTemplate> *entity_Tree;

    DB() {}

    void AddAttribute(AttributeTemplate Template){ attr_Table->at(Template.Name.c_str()) = attr_list->size(); attr_list->push_back(Template); }

    void AddAdvertisement(AdvertisementTemplate Template){ ad_table->at(Template.Name.c_str()) = ad_list->size(); ad_list->push_back(Template); }

    void AddTag(TagTemplate Template){ tag_table->at(Template.Name.c_str()) = tag_list->size(); tag_list->push_back(Template); }

    void AddEntityGroup(EntityTemplate Template) {
        if(entity_Tree->Children.size() == 0)
        {
            TAddress Zero = TAddress(1);
            NContainerIterator itt = Zero.begin();
            itt.write(0);

            entity_Table->at(Template.Name.c_str()) = Zero;
            entity_Tree->SetData(Template);
        }
        else
        {
           TAddress temp = entity_Table->at(Template.Parent.c_str());

           TNode<EntityTemplate> *temp_node = SManager::GetNode<EntityTemplate>(entity_Tree,temp);

            //Set the address to the next node in the tree
            auto itt = temp.end();
            itt.write(temp_node->Children.size());

            //Add the node to the tree
            temp_node->Children.push_back(new TNode<EntityTemplate>(Template));

            //Push the address back into the map
            entity_Table->at(Template.Name.c_str()) = temp;

        }
    }
public:
    static DB *get() {
           if(!m_instance)
               m_instance = new DB();
           return m_instance;
    }


    //Finalizes an object
    void Add(AttributeTemplate temp) {AddAttribute(temp);}
    void Add(EntityTemplate temp) { AddEntityGroup(temp);}
    void Add(AdvertisementTemplate temp) { AddAdvertisement(temp);}
    void Add(TagTemplate temp) { AddTag(temp);}
    //TODO: This will probably be very slow :(
    WorldCompiled Compile()
    {
        WorldCompiled _compiled;
        //Compile the list of all of our attributes
        for(int i = 0; i < attr_list->size(); i++)
        {
            AttributeTemplate attr_temp= (*attr_list)[i];
            _compiled.Attributes.push_back(AttributeBase(attr_temp.Grad,attr_temp.Value, attr_temp.Opinion));
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
        bool done = false;
        TNode<EntityTemplate> node = *entity_Tree;

        //The system starts at node(0) and climbs down the first branch summing all attributes along the way. After child.size <= 0, referencetablesum and GradientTableSum are subtracted from.
        TContainer<TContainer_Short<short>> ReferenceTableSum;
        TContainer<TContainer_Short<Gradient>> GradientTableSum;
        while(!done)
        {

           while(node.Children.size() > 0)
           {
                ReferenceTableSum.push_back(TContainer_Short<short>());
                GradientTableSum.push_back(TContainer_Short<Gradient>());
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

                //Expand the vector so that we can properly convert global coordinates to local coordinates
                TContainer_Short<UNibble> ExpandedVector;
                //Bring the vector back to our TContainer
                ReferenceTableSum[ReferenceTableSum.size() - 1].Interface = &ReferenceVector[0];

                //Expansion
                ExpandedVector.resize(ReferenceTableSum[ReferenceTableSum.size() - 1].end().Index);

                for(UNibble i = 0; i < CurrentSize; i++)
                {
                    //Determine our local coordinates
                    ExpandedVector[ReferenceTableSum[ReferenceTableSum.size() - 1].Interface[i]] = i;
                    //Set our gradient buffer value
                    GradientTableSum[GradientTableSum.size() - 1].push_back((*attr_list)[ReferenceTableSum[ReferenceTableSum.size() - 1].Interface[i]].Grad);
                }

                //TODO: Finalize IEntityGroup with Tags
           }
       }

       return _compiled;
    }


    void SuperSecretDeepNeuralNetFunctionThatMayOrMayNotHintToSomethingInBeta(){ /*OH HI THERE*/ }
};
struct Example
{
public:
    void Try()
    {
        EntityTemplate newT = EntityTemplate::_create("William")
                ._inheritsFrom("Bob")
                ._addAttribute("Food")
                ._addTag("Blacksmith");
        AttributeTemplate newAT = AttributeTemplate::_create("Food")
                ._setGradient(Gradient(1U,5U,50U,70U,110U))
                ._setValue(60)
                ._setOpinion(12U);

        DB::get()->Add(newT);
        DB::get()->Add(newAT);
    }
};


#endif // INTERFACEBUILDER_H
