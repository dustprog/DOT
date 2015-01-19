#pragma once
#include <vector>
#include <unordered_set>
#include <stdint.h>

#include <ZETA_Alpha/Containers/Node.h>

#include <DOT_Alpha/Interface/InterfaceBase.h>
#include <DOT_Alpha/Managers/BranchPredictor.h>
#include "../DOTbuilder/InterfaceBuilder.h"
struct TAddress;
template <class T>
struct QueueObject;
struct IEntityGroup;

/**
 \brief The World : central manager class, stores Entities, Advertiements, etc.
*/
class World {
public:
    typedef TNode<TContainer<AttributeBase> > TemplateTreeNode;
private:

    TNode<IEntityGroup*> m_simulated_entities;
    TContainer_Short<AdvertisementBase> m_template_ads; //Should be ordered by frequency of usage (DONE)
    TemplateTreeNode *m_template_tree_root;
    static World *m_instance;
    MatrixXf m_simulated_ads;
    TContainer_Short<short> *m_simulated_ads_refences;
    TContainer_Short<RuntimeAddress> *m_search_queue;
    NNSearchF *m_searchtree;
    //Constructor and destructor are private to prevent their use in user code.
    World() : m_template_tree_root(nullptr)
    {
        m_simulated_ads.resize(0,SPATIAL_DIM);
    }
    ~World() {}
public:
    static World *get() {
        if(!m_instance)
            m_instance = new World;
        return m_instance;
    }
    void resetWorld();
    void simulationStep();
    void simulateGroup_CPU(IEntityGroup *Group);
    //Call this to create a copy of an advertisement template
    void createNewAdInstance(short AdIndex, VectorXf vec)
    {
        m_simulated_ads.resize(m_simulated_ads.rows() + 1,SPATIAL_DIM);
        //This for loop will unroll for obvious reasons
        for(int i = 0; i < SPATIAL_DIM; i++)
           m_simulated_ads(m_simulated_ads.rows(), i) = vec[i];

        m_simulated_ads_refences->push_back(AdIndex);
    }
    //Call this after you no longer are setting up your initial world state
    void finalizeAdInstaces()
    {
        m_searchtree = NNSearchF::create(m_simulated_ads,std::numeric_limits<NNSearchF::Index>::max(),NNSearchF::SearchType::KDTREE_CL_PT_IN_NODES);
    }
    //Runtime address can store as of Alpha 1.41E14 entities. This equates to roughly 1.2TB of entity data. If you require more than 1.2TB of entity data, increase ADDR_SIZE by factors of 3
    void addToSearchQueue(const RuntimeAddress EntityAddress)
    {
        m_search_queue->push_back(EntityAddress);
    }

    void executeSearch(UNibble SearchSize = 65)
    {

        for(int i = 0; i < m_search_queue->size(); i++)
        {
            VectorXi indicies(SearchSize);
            //this->m_searchtree->knn(/* Retrieve position from the m_simulated_entities*/nullptr,indicies,nullptr,SearchSize);
            //Add the returned value back to the local ads reference for each entity instance. TODO: Create function within IEntityGroup for this
            //Finalize
        }
        //This can be multithreaded, but it seems there is no performance need for it. I performed 10million searches in a second on a GTX 760.
    }

    /*
     * Template tree management and access
     */
    int initializeTemplateTree();
    int initializeTemplateTreeString(const char *);
    TemplateTreeNode *getTemplateTreeRoot() { return m_template_tree_root; }
    /*
     * Entity template managment interface
     */
    void instantiateEntity(IEntityGroup *et);

    /*
     * Simulated entity managment interface
     */
    //uint32_t numSimulatedEntities() { return m_simulated_entities.size(); }

    uint32_t numAds() { return m_template_ads.size(); }

    AdvertisementBase *getTemplateAd(unsigned short Index) { return &m_template_ads[Index]; }
    void pushBackTemplateAd(AdvertisementBase *Ad) { m_template_ads.push_back(*Ad); Ad->SetAddress(m_template_ads.size() - 1); }
    //TODO
    Item *getTemplateItem(unsigned short Index) { return new Item; }

};
