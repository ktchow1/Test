#include<gtest/gtest.h>

void test_gtest(int argc, char* argv[])
{
    testing::InitGoogleTest(&argc, argv);
    RUN_ALL_TESTS();
}

TEST(sample_suite0, sample_case0)
{
    std::uint32_t x = 123;
    EXPECT_EQ(x, 123U);
    EXPECT_EQ(x*1, 123U);
    EXPECT_EQ(x+1-1, 123U);
}

TEST(sample_suite0, sample_case1)
{
    std::uint32_t x = 123;
    EXPECT_EQ(123U, 123U);
    EXPECT_EQ(124U-1, 123U);
    EXPECT_EQ((x+x)/2, 123U);
}

TEST(sample_suite0, sample_case2)
{
    std::uint32_t x = 123;
    EXPECT_EQ(123U, 123U);
    EXPECT_EQ(124U, 123U);
    EXPECT_EQ(x, 123U);
}




