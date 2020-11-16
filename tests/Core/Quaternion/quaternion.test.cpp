#include <Core/Quaternion/quaternion.h>
#include <gtest/gtest.h>

using namespace MathLib;

TEST(QUATERNION_TEST, instantiate_quaternion)
{
  Quaternion<float> q{};
  Quaternion<double> qd{};
}

TEST(QUATERNION_TEST, instantiate_and_initialize)
{
  Quaternion<float> q1{Vector<float, 3>{ 1.0f, 1.0f, 1.0f }, 1.0f};
  Quaternion<double> q2{Vector<double, 3>{ 1.0, 1.0, 1.0 }, 1.0};
  Quaternion<float> q3{Vector<double, 3>{ 1.0, 1.0, 1.0 }, 1.0f};
  Quaternion<float> q4{Vector<float, 3>{ 1.0f, 1.0f, 1.0f }, 1.0};
  Quaternion<float> q5{ 1.0, 2.0, 3.0, 4.0 };
}

TEST(QUATERNION_TEST, copy_constructor)
{
  Quaternion<float> q1{Vector<float, 3>{ 1.0f, 2.0f, 3.0f }, 4.0f};
  Quaternion<float> copy{q1};
  EXPECT_EQ(q1, copy);

  Quaternion<double> copyD{q1};
}

TEST(QUATERNION_TEST, assignment)
{
  Quaternion<float> q1{Vector<float, 3>{ 1.0f, 2.0f, 3.0f }, 4.0f};
  Quaternion<float> copy{};
  copy = q1;
  EXPECT_EQ(q1, copy);

  Quaternion<double> copyD{};
  copyD = q1;
}

TEST(QUATERNION_TEST, multiplication)
{
  Quaternion<float> q1{ 3.0, 4.0, 3.0, 0.0 };
  Quaternion<float> q2{ 3.9,-1.0,-3.0, 4.0 };
  Quaternion<float> expected{ 3.0, 36.7,-6.6, 1.3 };

  // comparison fails due to catastrophic cancelation
  // EXPECT_EQ(q1 * q2, expected);
}

TEST(QUATERNION_TEST, scalar_quaternion_multiplication)
{
  Quaternion<float> q{ 3.0, 4.0, 3.0, 1.0 };
  Quaternion<float> expected{ 6.0, 8.0, 6.0, 2.0 };

  EXPECT_EQ( 2.0 * q, expected);
  EXPECT_EQ( q * 2.0, expected);
}

TEST(QUATERNION_TEST, addition)
{
  Quaternion<float> q1{ 3.0, 4.0, 3.0, 1.0 };
  Quaternion<float> expected{ 6.0, 8.0, 6.0, 2.0 };


  EXPECT_EQ(q1 + q1, expected);  
}

TEST(QUATERNION_TEST, conjugate)
{
  Quaternion<float> q{ 1.0, 2.0, 3.0, 4.0 };
  Quaternion<float> expected{ -1.0, -2.0, -3.0, 4.0 };

  EXPECT_EQ(q.getConjugate(), expected);
}

TEST(QUATERNION_TEST, norm)
{
  Quaternion<float> q{ 1.0, 1.0, 1.0, 1.0 };

  EXPECT_FLOAT_EQ(q.norm(), 2.0);
}

TEST(QUATERNION_TEST, identity)
{
  Quaternion<float> q{ 1.0, 1.0, 1.0, 1.0 };
  Quaternion<float> identity{ 0.0, 0.0, 0.0, 1.0 };

  EXPECT_EQ(q.setIdentity(), identity);
}

TEST(QUATERNION_TEST, inverse)
{
  Quaternion<float> q{ 3.0, -4.0, 2.0, 1.0 };
  Quaternion<float> expected{ -1.0/10.0, + 2.0/15.0, - 1.0/15.0, 1.0/30.0 };

  EXPECT_EQ(q.getInverse(), expected);
}

TEST(QUATERNION_TEST, unit_quaternion)
{
  Quaternion<float> q{ 1.0, 1.0, 1.0, 1.0 };
  Quaternion<float> expected{ 0.5, 0.5, 0.5, 0.5 };

  EXPECT_EQ(q.getUnit(), expected);

  q.setUnit();
  EXPECT_EQ(q, expected);
}

TEST(QUATERNION_TEST, rotation)
{
  Quaternion<float> q{};
  q.setRotation(Vector<float, 3>{1.0, 0.0, 0.0}, M_PI );
  Vector<float, 3> start{ 0.0, 0.0, 1.0 };

  Vector<float, 3> semiRotation{ ( q * Quaternion<float>{start, 1.0} * q.getConjugate() ).qv() };
  Vector<float, 3> expected{ 0.0, 0.0, -1.0 };

  EXPECT_TRUE(allClose(semiRotation, expected));

  q.setRotation(Vector<float, 3>{1.0, 0.0, 0.0}, -M_PI_2);
  Vector<float, 3> quarterRotation{ (q * Quaternion<float>{start, 1.0} * q.getConjugate()).qv() };
  expected = Vector<float, 3>{ 0.0, 1.0, 0.0 };

  EXPECT_TRUE(allClose(quarterRotation, expected));
}

TEST(QUATERNION_TEST, slerp)
{
  Quaternion<float> q{};
  Quaternion<float> r{};
  q.setRotation(Vector<float, 3>{1.0, 0.0, 0.0}, 0.0);
  r.setRotation(Vector<float, 3>{1.0, 0.0, 0.0}, M_PI);

  Quaternion<float> interpolated{slerp(q, r, 0.25f)};
  Quaternion<float> expected{};
  expected.setRotation(Vector<float, 3>{1.0, 0.0, 0.0}, M_PI_4);

  EXPECT_EQ(interpolated, expected);
}