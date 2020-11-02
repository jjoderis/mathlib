#include <Core/Vector/vector.h>
#include <gtest/gtest.h>

using namespace MathLib;
class VectorTest: public ::testing::Test
{
    protected:

    Vector<int, 3> TestVec{ 1, 2, 3 };
    Vector<int, 3> TestVecNeg{ -1, -2, -3 };
    Vector<int, 3> TestVecX2{ 2, 4, 6 };
};

TEST(VECTOR_TEST, instantiate_vector)
{
    Vector<float, 3> a{};
}

TEST(VECTOR_TEST, instantiate_and_initialize_vector)
{
    Vector<float, 3> a{ 1.0, 0.0, 0.0 };
}

TEST_F(VectorTest, instantiate_and_initialize_with_smaller_vector)
{
    Vector<int, 4> a{ TestVec, 4 };
    Vector<int, 4> expected{ 1, 2, 3, 4 };
    EXPECT_EQ(a, expected);
}

TEST_F(VectorTest, instantiate_and_initialize_with_bigger_vector)
{
    Vector<int, 2> a{ TestVec };
    Vector<int, 2> expected{ 1, 2 };
    EXPECT_EQ(a, expected);
}

TEST_F(VectorTest, const_subsript_operator)
{
    EXPECT_EQ(TestVec(0), 1);
    EXPECT_EQ(TestVec(1), 2);
    EXPECT_EQ(TestVec(2), 3); 
}

TEST_F(VectorTest, subsript_operator)
{
    TestVec(0) = 10;
    EXPECT_EQ(TestVec(0), 10);
}

TEST_F(VectorTest, const_at_function)
{
    EXPECT_EQ(TestVec.at(0), 1);
    EXPECT_EQ(TestVec.at(1), 2);
    EXPECT_EQ(TestVec.at(2), 3);
}

TEST_F(VectorTest, at_function)
{
    TestVec.at(0) = 10;
    EXPECT_EQ(TestVec.at(0), 10);
}

TEST_F(VectorTest, set_function)
{
    TestVec.set(0, 10);
    EXPECT_EQ(TestVec.at(0), 10);
}

TEST_F(VectorTest, copy_constructor)
{
    Vector<int, 3> copy{ TestVec };

    EXPECT_EQ(copy, TestVec);
}

TEST_F(VectorTest, assignment_operator)
{
    Vector<int, 3> copy{};
    copy = TestVec;

    EXPECT_EQ(copy, TestVec);
}

TEST_F(VectorTest, copy_constructor_other_type)
{
    Vector<double, 3> orig{ 1.0, 2.0, 3.0 };
    Vector<int, 3> copy{ orig };

    EXPECT_EQ(copy, TestVec);
}

TEST_F(VectorTest, assignment_operator_other_type)
{
    Vector<double, 3> orig{ 1.0, 2.0, 3.0 };
    Vector<int, 3> copy{};
    copy = orig;

    EXPECT_EQ(copy, TestVec);
}

TEST_F(VectorTest, vector_vector_addition_assignment)
{
    EXPECT_EQ(TestVec += TestVec, TestVecX2);
}

TEST_F(VectorTest, vector_vector_addition)
{
    EXPECT_EQ(TestVec + TestVec, TestVecX2);
}

TEST_F(VectorTest, vector_vector_subtraction_assignment)
{
    EXPECT_EQ(TestVecX2 -= TestVec, TestVec);
}

TEST_F(VectorTest, vector_vector_subtraction)
{
    EXPECT_EQ(TestVecX2 - TestVec, TestVec);
}

TEST_F(VectorTest, vector_scalar_multiplication_assignment)
{
    EXPECT_EQ(TestVec *= 2, TestVecX2);
}

TEST_F(VectorTest, vector_scalar_division_assignment)
{
    EXPECT_EQ(TestVecX2 /= 2, TestVec);
}

TEST_F(VectorTest, scalar_vector_multiplication)
{
    EXPECT_EQ(2 * TestVec, TestVecX2);
}

TEST_F(VectorTest, vector_scalar_multiplication)
{
    EXPECT_EQ(TestVec * 2, TestVecX2);
}

TEST_F(VectorTest, vector_scalar_division)
{
    EXPECT_EQ(TestVecX2 / 2, TestVec);
}

TEST_F(VectorTest, vector_negation_override)
{
    EXPECT_EQ(TestVec.negate(), TestVecNeg);
}

TEST_F(VectorTest, vector_negation)
{
    EXPECT_EQ(-TestVec, TestVecNeg);
}

TEST_F(VectorTest, dot_product)
{
    float res = dot(TestVec, TestVec);

    EXPECT_FLOAT_EQ(res, 14);
}

TEST_F(VectorTest, dot_product_operator)
{
    float res = TestVec * TestVec;

    EXPECT_FLOAT_EQ(res, 14);
}

TEST_F(VectorTest, norm)
{
    Vector<int, 3> vec{ 1, 2, 2 };

    EXPECT_FLOAT_EQ(vec.norm(), 3);
}

TEST_F(VectorTest, outstream_overload)
{
    testing::internal::CaptureStdout();
    std::cout << TestVec;
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_EQ(output, std::string{"[ 1, 2, 3 ]"});
}

TEST_F(VectorTest, cross_product)
{
    Vector<int, 3> u{ 1, 0, 0 };
    Vector<int, 3> v{ 0, 1, 0 };

    Vector<int, 3> expected{ 0, 0, 1 };

    ASSERT_EQ(cross(u, v), expected);
}

TEST_F(VectorTest, normalize)
{
    Vector<float, 3> vec{ 1.0, 2.0, 3.0 };
    vec.normalize();

    ASSERT_FLOAT_EQ(vec.norm(), 1.0);
}

TEST_F(VectorTest, angleTo)
{
    Vector<int, 3> vec1{ 1, 0, 0 };
    Vector<int, 3> vec2{ 0, 1, 0 };

    ASSERT_FLOAT_EQ(180*vec1.angleTo(vec1)/M_PI, 0.0);
    ASSERT_FLOAT_EQ(180*vec1.angleTo(vec2)/M_PI, 90.0);
}
