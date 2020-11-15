#include <gtest/gtest.h>
#include <util/util.h>
#include <Core/Vector/vector.h>
#include <Core/Matrix/matrix.h>

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

TEST(UTIL_TEST, degToRad)
{
    EXPECT_TRUE(isClose(degToRad(180.0), M_PI));
}

TEST(UTIL_TEST, radToDeg) {
    EXPECT_TRUE(isClose(radToDeg(M_PI), 180.0));
}

TEST(UTIL_TEST, degToRad_Vector) {
    EXPECT_TRUE(MathLib::allClose(MathLib::Vector<double, 3>{0.0, M_PI, 2 * M_PI}, degToRad(MathLib::Vector<double, 3>{0.0, 180.0, 360.0})));
}

TEST(UTIL_TEST, radToDeg_Vector) {
    EXPECT_TRUE(MathLib::allClose(radToDeg(MathLib::Vector<double, 3>{0.0, M_PI, 2 * M_PI}), MathLib::Vector<double, 3>{0.0, 180.0, 360.0}));
}

TEST(UTIL_TEST, degToRad_Matrix) {
    EXPECT_TRUE(
        MathLib::allClose(
            MathLib::Matrix<double, 2, 3>{
                0.0, 0.5 * M_PI, M_PI,
                M_PI, 1.5 * M_PI, 2.0 * M_PI
            },
            degToRad(
                MathLib::Matrix<double, 2, 3>{
                    0.0, 90.0, 180.0,
                    180.0, 270.0, 360.0
                }
            )
        )
    );
}

TEST(UTIL_TEST, radToDeg_Matrix) {
    EXPECT_TRUE(
        MathLib::allClose(
            MathLib::Matrix<double, 2, 3>{
                0.0, 90.0, 180.0,
                180.0, 270.0, 360.0
            },
            radToDeg(
                MathLib::Matrix<double, 2, 3>{  
                    0.0, 0.5 * M_PI, M_PI,
                    M_PI, 1.5 * M_PI, 2.0 * M_PI
                }
            )
        )
    );
}

