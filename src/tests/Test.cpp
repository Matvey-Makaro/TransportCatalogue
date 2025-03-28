#include "gtest/gtest.h"

using namespace std;


TEST(Test, Test1) 
{
  EXPECT_EQ(1, 1);
}

TEST(Test, Test2) 
{
  EXPECT_EQ(1, 0);
}