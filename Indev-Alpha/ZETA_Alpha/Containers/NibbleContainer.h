#ifndef NIBBLECONTAINER_H
#define NIBBLECONTAINER_H
#include <stdint.h>
#include <cassert>
#include <cstring>
#include <algorithm>
#include "generic_typedefs.h"

//Maximum amount of entites = 15^(2 * ADDR_SIZE) + MAX_VAL(unsigned short). Chains can only be 2 * ADDR_SIZE long however. Memory usage per address: ADDR_SIZE + 1 bytes
#define ADDR_SIZE 3
//Semi-octet size is the amount of bits that are stored in every Nibble
#define SEMIOCTET_SIZE 4
#define MODULO_SIZE (8 / SEMIOCTET_SIZE)
#ifndef NIBBLE_MATH
#define NIBBLE_MATH
#endif
#include "../Utility/Workarounds.h"
struct NInterface
{
    UNibble Low : SEMIOCTET_SIZE;
    UNibble High : SEMIOCTET_SIZE;
};
#if __cplusplus>=201103L
static_assert(sizeof(NInterface)==1,"size of the NInterface should be 1 byte");
#endif
struct NInterface_Signed
{
    SNibble Low : SEMIOCTET_SIZE;
    SNibble High : SEMIOCTET_SIZE;
};
struct NContainerIterator;
#pragma pack(1)
struct NContainer
{
    friend struct NContainerIterator;
private:
    /// Entries storage
    NInterface *Interface;
    /// Number of entries stored in container, each of SEMIOCTET_SIZE bits
    UNibble Size : 8;
public:
    UNibble size()
    {
        return Size;
    }
    void Resize(UNibble n)
    {
        NContainer temp(Size);
        UNibble tempp = n % MODULO_SIZE;
        UNibble actual_new_size = (n / 2) + tempp;
        memcpy(temp.Interface,Interface,Size*sizeof(NInterface));
        Interface = new NInterface[actual_new_size];
        UNibble n_old = std::min(n, Size);
        Size = n;
        memcpy(Interface,temp.Interface,n_old*sizeof(NInterface));
    }
    NContainer() : Size(0) {}
    NContainer(UNibble Length)
    {
        UNibble temp = Length % MODULO_SIZE;
        Interface = new NInterface[Length / 2 + temp]; //If Length is odd, allocate an extra position
        Size = Length;
    }
    NContainerIterator begin();
    bool operator == (const NContainer &rhs) {
        return memcmp(Interface,rhs.Interface,sizeof(NInterface)*ADDR_SIZE)==0;
    }
    NContainerIterator end();
};

#pragma pack(1)
struct NContainerIterator {
    NContainer *Owner;
    uint8_t Index;
    NContainerIterator(NContainer *owner, uint8_t pos=0) : Owner(owner),Index(pos) {}
    NContainerIterator &operator++() {
        assert(Index!=255); // check for wraparounds in debug builds
        ++Index;
        return *this;
    }
    NContainerIterator operator++(int) {
        assert(Index!=255); // check for wraparounds in debug builds
        NContainerIterator t(*this);
        Index++; return t;
    }
    NContainerIterator &operator--() {
        assert(Index!=0); // check for wraparounds in debug builds
        --Index;
        return *this;
    }
    NContainerIterator operator--(int) {
        assert(Index!=0); // check for wraparounds in debug builds
        NContainerIterator t(*this);
        --Index;
        return t;
    }
//    NContainerIterator &operator+=(int v) { Index+=v; return *this;}
//    NContainerIterator operator+(int v) { NContainerIterator t(*this); return t+=v;}
    UNibble operator *() const {
        bool part=Index & 1;
        NInterface &val(Owner->Interface[Index/2]);
        return part ? val.High : val.Low;
    }
    void write(UNibble v) {
        bool part=Index & 1;
        NInterface &val(Owner->Interface[Index/2]);
        if(part)
            val.High = v;
        else
            val.Low = v;
    }
    #ifdef _WINDOWS
    bool operator==(const NContainerIterator &other) const {
        if(other.Owner==nullptr) {
            return (Owner==nullptr) || (Index>=Owner->Size);
        }
        else if(Owner==nullptr) {
            return (other.Owner==nullptr) || (other.Index>=other.Owner->Size);
        }
        return (Owner==other.Owner) && (Index==other.Index);
    }
    bool operator!=(NContainerIterator &other) {
        return ! (*this==other);
    }
    #endif // _WINDOWS
    //#ifdef _LINUX

    bool operator==(const NContainerIterator &other) const {
        if(other.Owner==nullptr) {
            return (Owner==nullptr) || (Index>=Owner->Size);
        }
        else if(Owner==nullptr) {
            return (other.Owner==nullptr) || (other.Index>=other.Owner->Size);
        }
        return (Owner==other.Owner) && (Index==other.Index);
    }
    bool operator!=(NContainerIterator &other) {
        return ! (*this==other);
    }
    //#endif // _LINUX
};
inline NContainerIterator NContainer::begin() { return NContainerIterator(this,0); }
inline NContainerIterator NContainer::end() { return NContainerIterator(nullptr,0); }
struct NContainer_Signed
{
private:
    UNibble Index : 8;
    UNibble Size : 8;
    NInterface_Signed *Interface;
    void WriteContainerLow(SNibble val)
    {
        Interface[Index / 2].Low = val;
        IncrementWriteHead();
    };
    void WriteContainerHigh(SNibble val)
    {
        Interface[Index / 2].High = val;
        IncrementWriteHead();
    };
public:
    NInterface_Signed *ReturnPointer()
    {
        return Interface;
    }
    UNibble size()
    {
        return Size;
    }
    UNibble ReturnCurrentWriteHead()
    {
        return Index;
    }
    void IncrementWriteHead()
    {
        Index++;
    }
    void Resize(UNibble n)
    {
        NContainer_Signed *temp = new NContainer_Signed(Size);

        for (int i = 0; i < Size / 2; i++)
        {
            WriteContainerLow(Interface[i].Low);
            WriteContainerHigh(Interface[i].High);
        }
        SNibble tempp = n % MODULO_SIZE;
        Interface = new NInterface_Signed[n / 2 + tempp];
        Size = n;
        Index = 0;
        n = std::min(n, Size);

        for (int i = 0; i < Size / 2; i++)
        {
            WriteContainerLow(temp->Interface[i].Low);
            WriteContainerHigh(temp->Interface[i].High);
        }

    }
    NContainer_Signed(UNibble Length)
    {
        UNibble temp = Length % MODULO_SIZE;
        Interface = new NInterface_Signed[Length / 2 + temp]; //If Length is odd, allocate an extra position
        Index = 0;
        Size = Length;
    }
    void SetWriteHead(UNibble point)
    {
        Index = point;
    }
};

struct PointerInterface
{
    unsigned short Final;
    NInterface Adr[ADDR_SIZE];
};
#endif
