#ifndef THREADMANAGER_H
#define THREADMANAGER_H
#include "../Processing/ProcessBase.h"
#include "../Containers/GenericContainer.h"
#include <boost/thread.hpp>

using namespace boost;
struct ThreadManagerBase
{
protected:
    //A back log of all processes that are available
    TContainer<ProcessBase*> Backlog;

    TContainer<UNibble> Queue;


public:
    /*This does not queue up the action. Rather this adds the ability for the thread manager to call this action.
     *Returns the function index (Where its stored within the backlog). This is required when queueing a process*/
    UNibble AddToBacklog(ProcessBase *Process)
    {
        Backlog.push_back(Process);
        return Backlog.size() - 1;
    }
    void AddToQueue(UNibble Task)
    {
        Queue.push_back(Task);
    }

    virtual void Update() = 0;

};
struct ThreadManager_CPU : ThreadManagerBase
{
    /*Calls an update step for the threads. Fires off all threads for the queue. Everything beyond this point is managed internally by the OS.
     *Output from processes can be retrieved by accessing your process structure */
    void Update()
    {
        for(int i = 0; i < Queue.size();)
        {
            switch(Backlog[Queue[i]]->GetMode())
            {
               case ProcessBase::ProcessModes::Finished:
                    Backlog.Remove(Queue[i]);
                    break;
                case ProcessBase::ProcessModes::Started:
                    i++;
                    break;
                case ProcessBase::ProcessModes::NotStarted:
                    boost::thread t(*Backlog[Queue[i]]);
                    Backlog[Queue[i]]->Begin();
                    i++;
                    break;

            }
        }
    }
};

#endif // THREADMANAGER_H
