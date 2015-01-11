#pragma once
#include <vector>
#include <unordered_set>
#include <stdint.h>

#include <ZETA_Alpha/Containers/Node.h>

#include <DOT_Alpha/Interface/InterfaceBase.h>
#include <DOT_Alpha/Managers/BranchPredictor.h>
#
struct TAddress;
template <class T>
struct IEntityBase;

struct QueueObject;
struct EntityTemplate;
struct EntityInstance;
struct IEntityGroup;
/**
 \brief The World : central manager class, stores Entities, Advertiements, etc.
*/
class World {
public:
    typedef TNode<TContainer<AttributeBase> > TemplateTreeNode;
private:

    std::vector<IEntityGroup *> m_simulated_entities;
    TemplateTreeNode *m_template_tree_root;
    static World *m_instance;
    //Constructor and destructor are private to prevent their use in user code.
    World() : m_template_tree_root(nullptr)
    {}
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
    uint32_t numSimulatedEntities() { return m_simulated_entities.size(); }

};
