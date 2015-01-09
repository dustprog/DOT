#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "World.h"
#include "DOT_Alpha/Interface/EntityBase.h"

using namespace testing;
const char testingTemplate[] = {
R"(
{
        "TreeNodes" : [
        {
            "Name": "root",
            "Attributes": [ {} ],
            "Children": []
        }
    ]
})"
};
const char testingBadTemplate[] = {
"{ \"TreeNodes\" : ["
        "{"
        "\"Name\": \"root\","
        "\"Attributes\": [],"
        "\"Children\": []"
        "},"
        "{"
        "\"Name\": \"root2\","
        "\"Attributes\": [],"
        "\"Children\": []"
        "}"
    "]}"
};
const char testingTemplate_noName[] = {
"{ \"TreeNodes\" : ["
        "{"
        "\"Attributes\": [],"
        "\"Children\": []"
        "}"
    "]}"
};
const char testingTemplate_garbage[] = {
".qdqw.12 213 [/} .asda {}  :c cqqwd"
};

struct FakeEntity : public IEntityBase<TAddress>{
    StringInterface GetName() const { return StringInterface(""); }
    MOCK_METHOD2(ReturnScore, float(AdvertisementBase *,TContainer<UNibble> *));
    MOCK_METHOD2(ApproximateScore, float(AdvertisementBase *,TContainer<UNibble> *));
};

class WorldFixture : public ::testing::Test {
public:
    virtual void SetUp() {
    }
    virtual void TearDown() {
    }
};
TEST_F(WorldFixture, singleton_tests) {
    ASSERT_NE(World::get(),nullptr) << " The singleton world instance must exist ";
    ASSERT_EQ(World::get(),World::get()) << " The World::get should always return the same instance";
}

TEST_F(WorldFixture, simulated_entity_management_tests) {
    FakeEntity * fe1 = new FakeEntity;
    World::get()->resetWorld();
    ASSERT_EQ(0,World::get()->numSimulatedEntities()) << "Resetting the world should leave it empty of simulated entities";
    ASSERT_EQ(-1,World::get()->addSimulatedEntity(nullptr)) << "Adding a null entity should fail";
    ASSERT_EQ(0,World::get()->numSimulatedEntities()) << "Expected to have no simulated entities after failure";
    ASSERT_EQ(0,World::get()->addSimulatedEntity(fe1));
    ASSERT_EQ(1,World::get()->numSimulatedEntities()) << "Expected to have 1 simulated entity after adding it";
    ASSERT_EQ(-1,World::get()->addSimulatedEntity(fe1)) << "Adding the same entity twice should fail";
    ASSERT_EQ(1,World::get()->numSimulatedEntities());

    World::get()->resetWorld();
    ASSERT_EQ(0,World::get()->numSimulatedEntities()) << "Resetting the world should leave it empty of simulated entities";
    ASSERT_EQ(0,World::get()->addSimulatedEntity(fe1));
}

TEST_F(WorldFixture, template_tree_base_tests) {
    World::get()->resetWorld();
    ASSERT_EQ(World::get()->getTemplateTreeRoot(),nullptr)
            << "Expecting null template tree root after reset";

    ASSERT_EQ(-1,World::get()->initializeTemplateTreeString(testingBadTemplate))
            << "Expected template tree initialization to fail with multilpe root nodes";

    ASSERT_EQ(World::get()->initializeTemplateTreeString(testingTemplate_noName),-2)
            << "Expected initializing from template with missing name to fail name check";

    ASSERT_EQ(World::get()->initializeTemplateTreeString(testingTemplate_garbage),-3)
            << "Expected initializing from template with missing name to fail parsing";

    ASSERT_EQ(0,World::get()->initializeTemplateTreeString(testingTemplate))
            << "Expected template tree initialization to succeed";

    ASSERT_NE(World::get()->getTemplateTreeRoot(),nullptr)
            << "Expecting template tree root to exist after initialization";

    World::get()->resetWorld();
    ASSERT_EQ(World::get()->getTemplateTreeRoot(),nullptr)
            << "Expecting no template tree root after world reset";

    ASSERT_EQ(0,World::get()->initializeTemplateTreeString(testingTemplate));
    World::TemplateTreeNode *node = World::get()->getTemplateTreeRoot();
    ASSERT_GE(node->getData().size(),1)
            << "Initialized template has to have at leas one attribute";
}

TEST_F(WorldFixture, simulation_single_step_tests) {
    FakeEntity fe1;
    World::get()->resetWorld();
    World::get()->addSimulatedEntity(&fe1);

    EXPECT_CALL(fe1,ApproximateScore(_,_)).Times(AtLeast(1));
    World::get()->simulationStep();
}
