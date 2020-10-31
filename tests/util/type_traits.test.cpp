#include <util/type_traits.h>
#include <gtest/gtest.h>

TEST(UTIL_TYPE_TRAITS_TEST, are_same)
{
    bool intIsSame = are_same<int>{};
    bool intAreSame = are_same<int, int, int , int>{};
    EXPECT_TRUE(intIsSame);
    EXPECT_TRUE(intAreSame);

    bool mixIsSame = are_same<int, int, int, bool>{};
    EXPECT_FALSE(mixIsSame);
}

TEST(UTIL_TYPE_TRAITS_TEST, are_arithmetic)
{
    bool intIsArithmetic = are_arithmetic<int>{};
    EXPECT_TRUE(intIsArithmetic);

    bool charPointerIsArithmetic = are_arithmetic<char*>{};
    EXPECT_FALSE(charPointerIsArithmetic);

    bool numbersAreArithmetic = are_arithmetic<int, double, float, short, unsigned int, char>{};
    EXPECT_TRUE(numbersAreArithmetic);

    bool mixIsArithmetic = are_arithmetic<int, double, float, char*>{};
    EXPECT_FALSE(mixIsArithmetic);
}