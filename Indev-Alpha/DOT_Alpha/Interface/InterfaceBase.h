#ifndef BASEINTERFACE_HEADER
#define BASEINTERFACE_HEADER
#include "TObject.h"
#include <list>
#include "../../ZETA_Alpha/Containers/TemplateTreeAddress.h"

#include "../../ZETA_Alpha/Containers/GenericContainer.h"

struct RuntimeAddress
{
    //Template tree address
    TAddress Addr;
    //Instance number
    unsigned short Index;
};
struct Gradient
{
    SNibble Delta : 6;
    //How many favtors lower the minimum is compared to Max
    UNibble LowFactor : 4;
    unsigned int Q1, Q3, Max : 10;
    //NOTE:Q1, Q3, and Max are all 10 bits each. Delta is 6 bits, and low factor is 4 bits. Total size = 5 bytes
    Gradient(SNibble Delta = 0U,UNibble LowFactor = 5U, unsigned int Q1 = 40, unsigned int Q3 = 60, unsigned int Max = 100)
    {
        this->Delta = Delta;
        this->Q1 = Q1;
        this->Q3 = Q3;
        this->Max = Max;
        this->LowFactor = LowFactor;
    }
};

struct OwnershipBase : public TObject_Adr<RuntimeAddress> //Alows the system to dictate who owns what
{
    bool OwnerSet;

    virtual StringInterface GetName() const override { return StringInterface("Default Owner"); }
    //NOTE: OwnerSet is 1 byte. Address is 5 bytes. Total size: 6 bytes
    OwnershipBase(bool OwnerSet = false, RuntimeAddress Address = RuntimeAddress())
    {
        this->OwnerSet = OwnerSet;
        this->Addr = Address;
    }
};


/*In this version of DOT, rather than the entity trying to keep its attribute as high as possible, it attempts to keep it between Q1 and Q3.
Gradient is retrieved by calling a master node */
#pragma pack(1)
struct AttributeBase
{
    unsigned int Value : 10;

    //Opinion is used in planning to see how much the NPC cares about a particular attribute. AttributeScore * (Opinion/16). If we figure out a better use for this padding then we'll change it
    unsigned int Opinion : 6;

    //NOTE: Opinion is 6 bits while Value is 10 bits. Total size = 2 bytes
    AttributeBase(unsigned int Value = 100U,unsigned int Opinion = 31U )
    {
        this->Opinion = Opinion;
        this->Value = Value;
    }

    AttributeBase(Gradient grad)
    {
        this->Value = (grad.Q1 + grad.Q3) / 2;
        this->Opinion = 31U;
    }
};
#pragma pack(1)
struct CostBase
{
    unsigned int GlobalAdr : 12;
    unsigned int Value : 12;

    //NOTE: Global is 12 bits while Value is 12 bits. Total size = 3 bytes
    CostBase(unsigned int GlobalAdr = 0, unsigned int Value = 100U)
    {
        this->GlobalAdr = GlobalAdr;
        this->Value = Value;
    }
};
#pragma pack(1)
struct AdvertisementBase : public TObject_Adr<unsigned short>
{
private:
    TContainer<CostBase> negative_effects;
    TContainer<CostBase> positive_effects;
public:
    //May be used by more than one entity at once
    UNibble Polyusability : 1;
    //Are we currently being utilized
    UNibble InUse : 1;
    //How long does this advertisement lock the entity?
    UNibble TimeConstant : 6;

    virtual StringInterface GetName() const override { return StringInterface("Default Advertisement"); }

    /*NOTE: Polyusability and InUse set are 1 bit each. TimeConstant is 6 bits.
    Positive and negative effects allocate 2 bytes by default. They take 2 bytes per effect. Total (Smallest) size: 5 bytes
    */
    AdvertisementBase(UNibble PolyUsage = 1, UNibble InUse = 0, UNibble TimeConstant = 1)
    {
        this->Polyusability = PolyUsage;
        this->InUse = InUse;
        this->TimeConstant = TimeConstant;
        negative_effects = positive_effects = TContainer<CostBase>();
    }
    void AddNegativeEffect(CostBase Attribute)
    {
        negative_effects.push_back(Attribute);
    }
    void AddPositiveEffect(CostBase Attribute)
    {
        positive_effects.push_back(Attribute);
    }
    const TContainer<CostBase> &GetPositiveEffects() const { return this->positive_effects; }
    const TContainer<CostBase> &GetNegativeEffects() const { return this->negative_effects; }

    virtual void BeginUse()
    {
        InUse = 1;
    }
    virtual void Execute(){}

    virtual void EndUse()
    {
        InUse = 0;
    }
};
//Every address in item must point to its correlating advertisement
struct Item
{
    short PickupEffect;
    short PassiveEffect;
    short UseEffect;
    short ThrowAwayEffect;
};
typedef short ItemAddress;
typedef TContainer<ItemAddress> Inventory;
//Used as a temporary reference to an advertisement
struct IndirectAd
{
    unsigned short Address;
    UNibble TimeConstant : 8;

    IndirectAd(short Adr= 0, UNibble T = 1) { this->Address  = Adr; this->TimeConstant = T; }
};

struct DeathTimeConstant
{
    UNibble Death : 1;
    UNibble TimeConstant : 7;
};

struct DirectAdvertisementQueue
{
    //A list of advertisements to be loaded on a later date
    TContainer<IndirectAd> Indirect;
};

struct WeighedSum
{
    signed int Sum : 16;
    WeighedSum(signed int S = 0) { Sum = S; }
};
//Index poitns to the entity this structure relates to. Ad points to all ads that interest this entity
struct ShortQueue
{
    unsigned short Index;
    TContainer<unsigned short> Ad;
};


#endif
