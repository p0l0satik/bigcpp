#include <gtest/gtest.h>
#include <iostream>

TEST(First, FT){
    ASSERT_EQ(0, 1) << "1 is not equal 0";
}

int main(int argc, char *argv[])
{
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}