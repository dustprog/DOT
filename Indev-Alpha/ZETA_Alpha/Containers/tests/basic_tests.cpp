#include "gtest/gtest.h"
#include "NibbleContainer.h"
class NibbleContainerFixture : public ::testing::Test {
public:
    virtual void SetUp() {
    }
    virtual void TearDown() {
    }
};
TEST_F(NibbleContainerFixture, construction_tests) {
    NContainer g;
    ASSERT_EQ(0,g.size()) << " Expect default constructed NContainer to have size of 0";
    ASSERT_EQ(g.end(),g.begin()) << " Expect empty NContainer's begin() to equal end() ";
}
//TEST_F(NibbleContainerFixture, test_name5) {
//    ASSERT_TRUE(false);
//}
