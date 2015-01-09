#ifndef TOBJECT_HEADER
#define TOBJECT_HEADER
#include <cstring>

struct StringInterface
{
private:
    const char *Value;
public:
    StringInterface(const char* str){ this->Value = str; }
    StringInterface() { Value = ""; }
    const char *c_str()
    {
        return Value;
    }
    bool operator==(const StringInterface &rhs) const {
        return 0==strcmp(Value,rhs.Value);
    }
};
//TObject does not require the object to specify the way that its going to be addressed. Minimum of 0 bytes of padding
#pragma pack(1)
struct TObject
{
    virtual StringInterface GetName() const = 0;

    bool operator==(const TObject &other) const
    {
        return other.GetName() == GetName();
    }
};
//The T notates its method of access. Whether using TAddress or some other varient, it is required
//TObject_Adr requires the object to specify the way that its going to be addressed. Minimum of 1 byte of padding
#pragma pack(1)
template <class T>
struct TObject_Adr : public TObject
{
protected:
    T Addr;
public:
    T ReturnAddress() { return Addr; }

    bool operator==(const TObject_Adr &other) const
    {
        bool same_name = (*this==((TObject)other));
        bool same_address = (Addr == other.Addr);
        return same_name &&  same_address;
    }
};
#endif
