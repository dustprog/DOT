#ifndef INTERFACEBUILDER_H
#define INTERFACEBUILDER_H
#include <ZETA_Alpha/Manager/SearchTreeManager.h>
#include "../Interface/InterfaceBase.h"
#include "../Interface/EntityBase.h"
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
    static EntityTemplate _create(const char* Name) { auto e = new EntityTemplate(); e->Name = StringInterface(Name);
                                                      e->Attributes = e->Tags =  TContainer_Short<StringInterface>(0);
                                                      return e->_deRef(); }
    EntityTemplate _deRef() {return *this; }
    EntityTemplate _inheritsFrom(const char* Name){ this->Parent = StringInterface(Name); return *this; }
    EntityTemplate _addAttribute(const char *Name){ this->Attributes.push_back(StringInterface(Name)); return *this; }
    EntityTemplate _addTag(const char* Name) {this->Tags.push_back(StringInterface(Name)); return *this; }
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
    TContainer<StringInterface> Attributes;
    TContainer<unsigned int> ValueOverride;
    TContainer<SNibble> Coefficients;
    TContainer<StringInterface> LifeObjectives;
    StringInterface Name;

    TagTemplate() {}
    static TagTemplate _create(const  char *Name) { auto e = new TagTemplate(); e->Name = StringInterface(Name);
                                                    e->Attributes=  TContainer<StringInterface>(0); e->ValueOverride =  TContainer<unsigned int>(0);
                                                    e->Coefficients =  TContainer<SNibble>(0); e->LifeObjectives =  TContainer<StringInterface>(0);
                                                    return e->_deRef(); }
    TagTemplate _deRef() {return *this; }
    TagTemplate _addAttribute(const char *Name, SNibble CoefficientOfImportance = 1, unsigned int Override = 0) { this->Attributes.push_back(StringInterface(Name)); this->ValueOverride.push_back(Override); this->Coefficients.push_back(CoefficientOfImportance); return *this; }
    TagTemplate _addLifeObjective(const char *Name) { this->LifeObjectives.push_back(StringInterface(Name)); return *this; }
};
struct TagCompiled
{
public:
    TContainer<CostBase> Attributes;
    TContainer_NULL<SNibble> Coefficients;
    TContainer<unsigned short> LifeObjectives;
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
    static AdvertisementTemplate _create(const  char *Name) { auto e = new AdvertisementTemplate(); e->Name = StringInterface(Name);
                                                              e->Positive = e->Negative =  TContainer_Short<CostTemplate>(0);
                                                              return e->_deRef(); }
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

//Ouput from THE GLORIOUS COMPILER!
struct WorldCompiled
{

    TContainer_Short<AttributeBase> Attributes;
    TContainer_Short<Gradient> Gradients;
    TContainer_Short<AdvertisementBase> AdInstances;
    TContainer_Short<TagCompiled> TagInstaces;
    TNode<IEntityGroup> EntityTemplateTree;

};
//THE GLORIOUS COMPILER!
struct DB
{
public:
    static DB *get() {
           if(!m_instance)
               m_instance = new DB;
           return m_instance;
    }

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

    DB(){
        attr_Table = new AttributeTable();
        attr_list = new TContainer_Short<AttributeTemplate>(0);

        ad_table = new AdvertisementTable();
        ad_list= new TContainer_Short<AdvertisementTemplate>(0);

        tag_table = new TagTable();
        tag_list= new TContainer_Short<TagTemplate>(0);

        entity_Table = new EntityGroupTable();
        entity_Tree= new TNode<EntityTemplate>();
    }
    ~DB() {}

    void AddAttribute(AttributeTemplate Template){ attr_Table->insert(AttributeTable::value_type(Template.Name.c_str(), attr_list->size())); attr_list->push_back(Template); }

    void AddAdvertisement(AdvertisementTemplate Template){ ad_table->insert(AdvertisementTable::value_type(Template.Name.c_str(), ad_list->size())); ad_list->push_back(Template); }

    void AddTag(TagTemplate Template){ tag_table->insert(TagTable::value_type(Template.Name.c_str(), tag_list->size())); tag_list->push_back(Template); }

    void AddEntityGroup(EntityTemplate Template) {
        if(entity_Tree->Children.size() == 0)
        {
            TAddress Zero = TAddress(1);
            NContainerIterator itt = Zero.begin();
            itt.write(0);
            entity_Table->insert(EntityGroupTable::value_type(Template.Name.c_str(), Zero));
            entity_Tree->SetData(Template);
            entity_Tree->Children = TContainer<TNode<EntityTemplate>*>(0);
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
            temp_node->Children.back()->Children = TContainer<TNode<EntityTemplate>*>(0);
            //Push the address back into the map
            entity_Table->insert(EntityGroupTable::value_type(Template.Name.c_str(), temp));

        }
    }

public:

    //Finalizes an object
    void Add(AttributeTemplate temp) {AddAttribute(temp);}
    void Add(EntityTemplate temp) { AddEntityGroup(temp);}
    void Add(AdvertisementTemplate temp) { AddAdvertisement(temp);}
    void Add(TagTemplate temp) { AddTag(temp);}
    //TODO: This will probably be very slow :(

    WorldCompiled Compile();


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
