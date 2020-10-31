#include <Core/Matrix/matrix.h>
#include <gtest/gtest.h>

class MatrixTest : public ::testing::Test
{
protected :
    Core::Matrix<int, 2, 3> testMat
    {    
         1, 2, 3, 
         4, 5, 6 
    };

    Core::Matrix<int, 2, 3> testMatX2
    {    
         2, 4, 6, 
         8, 10, 12 
    };

    Core::Matrix<int, 2, 3> testMatNeg
    {    
         -1, -2, -3, 
         -4, -5, -6 
    };
};

TEST_F(MatrixTest, instantiate_matrix) {
    Core::Matrix<int, 2, 2> a{};
}

TEST_F(MatrixTest, instantiate_and_initialize_matrix){
    const int* test = testMat.raw();

    EXPECT_EQ(test[0], 1);
    EXPECT_EQ(test[1], 4);
    EXPECT_EQ(test[2], 2);
    EXPECT_EQ(test[3], 5);
    EXPECT_EQ(test[4], 3);
    EXPECT_EQ(test[5], 6);
}

TEST_F(MatrixTest, instantiate_and_initialize_with_vectors)
{
    Core::Matrix<int, 2, 3> test
    {
        Core::Vector<int, 2> {1, 4},
        Core::Vector<int, 2> {2, 5},
        Core::Vector<int, 2> {3, 6},
    };

    const int* raw = test.raw();
    
    EXPECT_EQ(raw[0], 1);
    EXPECT_EQ(raw[1], 4);
    EXPECT_EQ(raw[2], 2);
    EXPECT_EQ(raw[3], 5);
    EXPECT_EQ(raw[4], 3);
    EXPECT_EQ(raw[5], 6);
}

TEST_F(MatrixTest, const_subscript_operator)
{
    EXPECT_EQ(testMat(0, 0), 1);
    EXPECT_EQ(testMat(0, 1), 2);
    EXPECT_EQ(testMat(0, 2), 3);
    EXPECT_EQ(testMat(1, 0), 4);
    EXPECT_EQ(testMat(1, 1), 5);
    EXPECT_EQ(testMat(1, 2), 6);
}

TEST_F(MatrixTest, subscript_operator)
{
    testMat(0, 0) = 10;
    EXPECT_EQ(testMat(0, 0), 10);
}

TEST_F(MatrixTest, const_at_function)
{
    EXPECT_EQ(testMat.at(0, 0), 1);
    EXPECT_EQ(testMat.at(0, 1), 2);
    EXPECT_EQ(testMat.at(0, 2), 3);
    EXPECT_EQ(testMat.at(1, 0), 4);
    EXPECT_EQ(testMat.at(1, 1), 5);
    EXPECT_EQ(testMat.at(1, 2), 6);
}

TEST_F(MatrixTest, at_function)
{
    testMat.at(0, 0) = 10;
    EXPECT_EQ(testMat.at(0, 0), 10);
}

TEST_F(MatrixTest, set_function)
{
    testMat.set(0, 0, 10);
    EXPECT_EQ(testMat.at(0, 0), 10);
}

TEST_F(MatrixTest, equality_operators)
{
    Core::Matrix<int, 2, 3> same
    {    
         1, 2, 3, 
         4, 5, 6 
    };

    Core::Matrix<int, 2, 3> different
    {    
         1, 3, 4, 
         4, 5, 6 
    };

    EXPECT_TRUE(testMat == same);
    EXPECT_FALSE(testMat == different);
    EXPECT_FALSE(testMat != same);
    EXPECT_TRUE(testMat != different);
}

TEST_F(MatrixTest, copy_constructor)
{
    Core::Matrix<int, 2, 3> copy{ testMat };

    EXPECT_EQ(testMat, copy);
}

TEST_F(MatrixTest, assignment_operator)
{
    Core::Matrix<int, 2, 3> copy{};
    copy = testMat;

    EXPECT_EQ(testMat, copy);
}

TEST_F(MatrixTest, copy_constructor_other_type)
{
    Core::Matrix<double, 2, 3> orig{ 1.0, 2.0, 3.0, 4.0, 5.0, 6.0 };
    Core::Matrix<int, 2, 3> copy{ orig };

    EXPECT_EQ(copy, testMat);
}

TEST_F(MatrixTest, assignment_operator_other_type)
{
    Core::Matrix<double, 2, 3> orig{ 1.0, 2.0, 3.0, 4.0, 5.0, 6.0 };
    Core::Matrix<int, 2, 3> copy{};
    copy = orig;

    EXPECT_EQ(copy, testMat);
}

TEST_F(MatrixTest, instantiate_and_initialize_with_bigger_matrix)
{
    Core::Matrix<int, 3, 3> orig{ 1, 2, 3, 4, 5, 6, 7, 8, 9 };
    Core::Matrix<int, 2, 2> sub{orig};
    Core::Matrix<int, 2, 2> expected{ 1, 2, 4, 5 };

    EXPECT_EQ(sub, expected);
}

TEST_F(MatrixTest, instantiate_and_initialize_with_smaller_matrix)
{
    Core::Matrix<int, 3, 4> inc{ testMat };
    Core::Matrix<int, 3, 4> expected{
        1, 2, 3, 0,
        4, 5, 6, 0,
        0, 0, 0, 1
    };

    EXPECT_EQ(inc, expected);
}

TEST_F(MatrixTest, matrix_matrix_addition_assignment)
{
    EXPECT_EQ(testMat += testMat, testMatX2);
}

TEST_F(MatrixTest, matrix_matrix_addition)
{
    EXPECT_EQ(testMat + testMat, testMatX2);
}

TEST_F(MatrixTest, matrix_matrix_subtraction_assignment)
{
    EXPECT_EQ(testMatX2 -= testMat, testMat);
}

TEST_F(MatrixTest, matrix_matrix_subtraction)
{
    EXPECT_EQ(testMatX2 - testMat, testMat);
}

TEST_F(MatrixTest, matrix_scalar_multiplication_assignment)
{
    EXPECT_EQ(testMat *= 2, testMatX2);
}

TEST_F(MatrixTest, matrix_scalar_division_assignment)
{
    EXPECT_EQ(testMatX2 /= 2, testMat);
}

TEST_F(MatrixTest, matrix_scalar_multiplication)
{
    EXPECT_EQ(testMat * 2, testMatX2);
}

TEST_F(MatrixTest, scalar_matrix_multiplication)
{
    EXPECT_EQ(2 * testMat, testMatX2);
}

TEST_F(MatrixTest, matrix_scalar_division)
{
    EXPECT_EQ(testMatX2 / 2, testMat);
}

TEST_F(MatrixTest, matrix_negation_override)
{
    EXPECT_EQ(testMat.negate(), testMatNeg);
}

TEST_F(MatrixTest, matrix_negation)
{
    EXPECT_EQ(-testMat, testMatNeg);
}

TEST_F(MatrixTest, matrix_matrix_product)
{
    Core::Matrix<int, 3, 2> operand{
         1, 2,
         3, 4, 
         5, 6 
    };

    Core::Matrix<int, 2, 2> expected{
        22, 28,
        49, 64
    };
    
    EXPECT_EQ(testMat * operand, expected);
}

TEST_F(MatrixTest, matrix_vector_product)
{
    Core::Vector<int, 3> operand{ 1, 2 ,3 };
    Core::Vector<int, 2> expected{ 14, 32 };

    EXPECT_EQ(testMat * operand, expected);
}

TEST_F(MatrixTest, set_identity)
{
    Core::Matrix<int, 3, 3> mat{
        1, 2, 3,
        4, 5, 6,
        7, 8, 9
    };

    Core::Matrix<int, 3, 3> identity{
        1, 0, 0,
        0, 1, 0,
        0, 0, 1
    };

    EXPECT_EQ(mat.setIdentity(), identity);
}

TEST_F(MatrixTest, transpose_self_if_square)
{
    Core::Matrix<int, 3, 3> mat{
        1, 2, 3,
        4, 5, 6,
        7, 8, 9
    };

    Core::Matrix<int, 3, 3> expected{
        1, 4, 7,
        2, 5, 8,
        3, 6, 9
    };

    EXPECT_EQ(mat.transpose(), expected);
}

TEST_F(MatrixTest, transpose)
{
    Core::Matrix<int, 3, 2> expected{
        1, 4,
        2, 5,
        3, 6
    };

    EXPECT_EQ(transpose(testMat), expected);
}

TEST_F(MatrixTest, trace)
{
    Core::Matrix<int, 3, 3> mat{
        1, 2, 3,
        4, 5, 6,
        7, 8, 9
    };

    EXPECT_EQ(mat.trace(), 15);
}

TEST_F(MatrixTest, outstream_overload)
{
    testing::internal::CaptureStdout();
    std::cout << testMat;
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_EQ(output, std::string{"[ [ 1, 2, 3 ], [ 4, 5, 6 ] ]"}); 
}



