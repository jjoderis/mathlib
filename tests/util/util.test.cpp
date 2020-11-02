#include <gtest/gtest.h>
#include <util/util.h>

using namespace MathLib::Util;

TEST(UTIL_TEST, is_close_on_float)
{
    float start = 0.1f;
    float sum = 0.0f;

    for(int i = 0; i < 10; ++i)
    {
        sum += start;
    }

    EXPECT_TRUE(isClose(1.0f, sum));
    EXPECT_FALSE(isClose(1.0f, 1.00001f));
    EXPECT_TRUE(isClose(0.0f, sum - 1.0f, (std::numeric_limits<float>::epsilon() * sum)));
}

TEST(UTIL_TEST, is_close_on_double)
{
    double start = 0.1;
    double sum = 0.0;

    for(int i = 0; i < 10; ++i)
    {
        sum += start;
    }

    EXPECT_TRUE(isClose(1.0, sum));
    EXPECT_FALSE(isClose(1.0, 1.000000000001));
    EXPECT_TRUE(isClose(0.0, sum - 1.0, std::numeric_limits<double>::epsilon() * sum));
}