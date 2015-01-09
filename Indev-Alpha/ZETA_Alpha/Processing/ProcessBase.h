#ifndef PROCESSBASE_H
#define PROCESSBASE_H
struct VoidContainer
{
 private:
    void* Data;
public:
    template<class T>
    void Set(T data)
    {
        this->Data = (void*)data;
    }
    template<class T>
    T Get() { return (T)Data; }
};

class ProcessBase
{
private:


    UNibble Mode;
    void *Atomic;
    void **Input;
    void End(void *Finished) { Mode = ProcessModes::Finished; Atomic = Finished; }
public:
    enum ProcessModes
    {
        NotStarted = 0,
        Started = 1,
        Finished = 2,
    };
    void SetInputBuffer(void **In) { Mode = ProcessModes::NotStarted; Input= In; }
    void Begin() { Mode = ProcessModes::Started; }

    virtual void Compute(){}

    //This is returning an error because I cannot get C++14 to function properly in QtCreator :( We may need to change IDEs
    void operator() ()
    {
        Compute();
    }

	//Copy this output to a seperate object!!
    template<class T>
    T GetOutput() { if(Mode == ProcessModes::Finished) return (T)Atomic; else return nullptr; }

    UNibble GetMode() { return this->Mode; }
};

class ProcessBase_Const : public ProcessBase
{
private:
    void *Const;
public:
    using ProcessBase::operator();
    ProcessBase_Const(void *Const)
	{
		this->Const = Const;
	}

    virtual void Compute(){}
};
#endif // PROCESSBASE_H
