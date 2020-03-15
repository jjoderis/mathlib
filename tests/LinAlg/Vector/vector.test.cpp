#include <LinAlg/Vector/vector.h>
#include <gtest/gtest.h>

class VectorTest: public ::testing::Test
{
    protected:

};

TEST(VECTOR_TEST, instantiate_vector)
{
    LinAlg::Vector<float, 3> a{};
}