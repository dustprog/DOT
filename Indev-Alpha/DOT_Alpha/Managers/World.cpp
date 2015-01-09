#include "World.h"

#include "../Interface/AdvertisementBase.h"
#include "DOT_Alpha/Interface/EntityBase.h"

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <unordered_map>

World *World::m_instance=nullptr;


/**
 * @brief This is the main world simulation method
 *
 * It will process all simulated entities
 */
void World::simulationStep()
{

    //Find chain to simulate
    //Retrieve reference table (No longer needed -Louis)
    //For every group in this chain simulateGroup()
    //Some groups are offloaded to GPU, while other groups remain on the CPU. Perhaps every other is done on the GPU? Yeah sounds good -Louis
    //We should avoid using less than 2 threads when going through the groups on the CPU. Dedicate a thread per group even
    //At most, going through an entire chain would take 9 passes. This is of course depedent on how many entities are in a single group
}
void World::simulateGroup_CPU(IEntityGroup Group)
{
    Group.ExecuteQueue();
}
/**
 * @brief World::instantiateAttributes
 * @param et - Entity Group
 */
void World::instantiateEntity(IEntityGroup *et)
{
    et->CreateNewInstance();
}
/**
 * @brief Since the World is a singleton, this method allows us to cleanup all World elements
 */
void World::resetWorld()
{

    m_simulated_entities.clear();
    delete m_template_tree_root;
    m_template_tree_root = nullptr;
}
/**
 * @brief This method is responsible for setting up this World's template tree
 */
int World::initializeTemplateTree() {
    m_template_tree_root = new TemplateTreeNode;
    return 0;
}
#include <sstream>
int World::initializeTemplateTreeString(const char *dat)
{
    std::istringstream stream(dat);

    using namespace boost::property_tree;
    ptree pt2;
    try {
        read_json(stream,pt2);
    }
    catch(const boost::property_tree::ptree_error &ex) {
        //std::string s= ex.what();
        //fprintf(stderr,"%s\n",s.c_str());
        return -3;
    }
    // Find the tree root:
    bool root_found=false;
    ptree root_node;
    std::unordered_map<std::string,ptree> name_to_tree_node_map;
    // visit tree nodes, and verify the correctnes
    for(const ptree::value_type &v : pt2.get_child("TreeNodes")) {
        std::string name = v.second.get("Name","");
        if(name.empty())
            return -2;
        name_to_tree_node_map[name] = v.second;
        if(v.second.get<std::string>("Parent","")=="") {
            if(root_found==true)
                return -1;
            root_found=true;
            root_node=v.second;
        }
    }
    m_template_tree_root = new TemplateTreeNode();
    TContainer<AttributeBase> attributes;
    attributes.push_back(AttributeBase());
    m_template_tree_root->SetData(attributes);
    return 0;
}
