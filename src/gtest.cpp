#include<gtest/gtest.h>

void test_gtest(int argc, char* argv[])
{
    testing::InitGoogleTest(&argc, argv);
    RUN_ALL_TESTS();
}

TEST(sample_suite0, case_boolean)
{
    std::uint32_t x = 123U;
    EXPECT_TRUE (x==123U);
    EXPECT_FALSE(x!=123U);
}

TEST(sample_suite0, case_integer)
{
    std::uint32_t x = 123;
    EXPECT_EQ(x, 123U);
    EXPECT_NE(x, 124U);
    EXPECT_LT(x, 124U);
    EXPECT_LE(x, 123U);
    EXPECT_GT(x, 122U);
    EXPECT_GE(x, 123U);    
    EXPECT_EQ(x*x, 123U*123U);
}

TEST(sample_suite0, case_double)
{
    double x = 1.23;
    EXPECT_NEAR(x+0.0001, 1.23, 0.001);
    EXPECT_NEAR(x-0.0001, 1.23, 0.001);
//  EXPECT_NEAR(x+0.0011, 1.23, 0.001);
//  EXPECT_NEAR(x-0.0011, 1.23, 0.001);
}

TEST(sample_suite0, case_string)
{
    char x[] = "ABC-DEF-GHIJ";
    EXPECT_STREQ(x, "ABC-DEF-GHIJ");
    EXPECT_STRNE(x, "ABC-DEF-GHIX");
    EXPECT_STRCASEEQ(x, "abc-DEF-ghij");
    EXPECT_STRCASENE(x, "xbc-DEF-ghij");
}

class my_fixture : public ::testing::Test // beware the character case "testing::Test"
{
public:
    my_fixture()    { /* std::cout << "*** my_fixture() ";      */ }
   ~my_fixture()    { /* std::cout << ">> ~my_fixture() ***\n"; */ }
    void SetUp()    { /* std::cout << ">> SetUp() ";            */ }
    void TearDown() { /* std::cout << ">> TearDown()";          */ }
};

TEST_F(my_fixture, sample_case0)
{
    std::uint32_t x = 123;
    EXPECT_EQ(x, 123U);
}

TEST_F(my_fixture, sample_case1)
{
    std::uint32_t x = 123;
    EXPECT_EQ(123U, 123U);
}




