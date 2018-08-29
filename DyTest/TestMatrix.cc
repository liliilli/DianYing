#include "pch.h"
#include <Dy/Helper/Type/Matrix2.h>
#include <Dy/Helper/Type/Vector2.h>

#define MDY_TEST_CATEGORY_MATRIX2x2 DyTestDDyMatrix2x2

TEST(MDY_TEST_CATEGORY_MATRIX2x2, TestCreation)
{
  const auto matrix = dy::DDyMatrix2x2{};
  //
  // DDyMatrix is column major.
  // [0,0] [1,0]
  // [0,1] [1,1]
  //
  EXPECT_FLOAT_EQ(matrix[0][0], 0);
  EXPECT_FLOAT_EQ(matrix[1][0], 0);
  EXPECT_FLOAT_EQ(matrix[0][1], 0);
  EXPECT_FLOAT_EQ(matrix[1][1], 0);
}

TEST(MDY_TEST_CATEGORY_MATRIX2x2, TestIdentity)
{
  const auto matrix = dy::DDyMatrix2x2::IdentityMatrix();
  //::IdentityMatrix();
  EXPECT_FLOAT_EQ(matrix[0][0], 1);
  EXPECT_FLOAT_EQ(matrix[1][0], 0);
  EXPECT_FLOAT_EQ(matrix[0][1], 0);
  EXPECT_FLOAT_EQ(matrix[1][1], 1);
}

TEST(MDY_TEST_CATEGORY_MATRIX2x2, TestFindingElement)
{
  auto matrix = dy::DDyMatrix2x2{};
  matrix[0][1] =  5;
  matrix[1][1] = 10;
  matrix[1][0] = -2;
  matrix[0][0] = -4;

  EXPECT_FLOAT_EQ(matrix[0][1],  5);
  EXPECT_FLOAT_EQ(matrix[1][1], 10);
  EXPECT_FLOAT_EQ(matrix[1][0], -2);
  EXPECT_FLOAT_EQ(matrix[0][0], -4);
}

TEST(MDY_TEST_CATEGORY_MATRIX2x2, TestTranspose)
{
  auto matrix = dy::DDyMatrix2x2{};
  matrix[0][1] =  5;
  matrix[1][1] = 10;
  matrix[1][0] = -2;
  matrix[0][0] = -4;

  matrix = matrix.Transpose();
  EXPECT_FLOAT_EQ(matrix[1][0],  5);
  EXPECT_FLOAT_EQ(matrix[1][1], 10);
  EXPECT_FLOAT_EQ(matrix[0][1], -2);
  EXPECT_FLOAT_EQ(matrix[0][0], -4);
}

TEST(MDY_TEST_CATEGORY_MATRIX2x2, TestElementMultiply)
{
  auto lhs = dy::DDyMatrix2x2{};
  lhs[0][0] = -4; lhs[1][0] = -2;
  lhs[0][1] =  5; lhs[1][1] = 10;

  auto rhs = dy::DDyMatrix2x2{};
  rhs[0][0] = -4; rhs[1][0] = -2;
  rhs[0][1] =  5; rhs[1][1] = 10;

  {
    const auto result = lhs * rhs;
    EXPECT_FLOAT_EQ(result[0][0],  16);
    EXPECT_FLOAT_EQ(result[1][0],   4);
    EXPECT_FLOAT_EQ(result[0][1],  25);
    EXPECT_FLOAT_EQ(result[1][1], 100);
  }
}

TEST(MDY_TEST_CATEGORY_MATRIX2x2, TestMatrixMultiply)
{
  auto lhs = dy::DDyMatrix2x2{};
  lhs[0][0] = -4; lhs[1][0] = -2;
  lhs[0][1] =  5; lhs[1][1] = 10;

  auto rhs = dy::DDyMatrix2x2{};
  rhs[0][0] = -4; rhs[1][0] = -2;
  rhs[0][1] =  5; rhs[1][1] = 10;

  {
    const auto result = lhs.Multiply(rhs);
    EXPECT_FLOAT_EQ(result[0][0],  6);
    EXPECT_FLOAT_EQ(result[1][0], -12);
    EXPECT_FLOAT_EQ(result[0][1],  30);
    EXPECT_FLOAT_EQ(result[1][1],  90);
  }
}

TEST(MDY_TEST_CATEGORY_MATRIX2x2, TestElementAddition)
{
  auto lhs = dy::DDyMatrix2x2{};
  lhs[0][0] = 2050; lhs[1][0] = 1322;
  lhs[0][1] = -239; lhs[1][1] = 6628;

  auto rhs = dy::DDyMatrix2x2{};
  rhs[0][0] = -347; rhs[1][0] = -192;
  rhs[0][1] = 1182; rhs[1][1] = -999;

  {
    const auto result = lhs + rhs;
    EXPECT_FLOAT_EQ(result[0][0], 1703);
    EXPECT_FLOAT_EQ(result[1][0], 1130);
    EXPECT_FLOAT_EQ(result[0][1],  943);
    EXPECT_FLOAT_EQ(result[1][1], 5629);
  }

  {
    lhs += rhs;
    EXPECT_FLOAT_EQ(lhs[0][0], 1703);
    EXPECT_FLOAT_EQ(lhs[1][0], 1130);
    EXPECT_FLOAT_EQ(lhs[0][1],  943);
    EXPECT_FLOAT_EQ(lhs[1][1], 5629);
  }
}

TEST(MDY_TEST_CATEGORY_MATRIX2x2, TestElementSubtraction)
{
  auto lhs = dy::DDyMatrix2x2{};
  lhs[0][0] = 2050; lhs[1][0] = 1322;
  lhs[0][1] = -239; lhs[1][1] = 6628;

  auto rhs = dy::DDyMatrix2x2{};
  rhs[0][0] = -347; rhs[1][0] = -192;
  rhs[0][1] = 1182; rhs[1][1] = -999;

  {
    const auto result = lhs - rhs;
    EXPECT_FLOAT_EQ(result[0][0], 2397);
    EXPECT_FLOAT_EQ(result[1][0], 1514);
    EXPECT_FLOAT_EQ(result[0][1], -1421);
    EXPECT_FLOAT_EQ(result[1][1], 7627);
  }

  {
    lhs -= rhs;
    EXPECT_FLOAT_EQ(lhs[0][0], 2397);
    EXPECT_FLOAT_EQ(lhs[1][0], 1514);
    EXPECT_FLOAT_EQ(lhs[0][1], -1421);
    EXPECT_FLOAT_EQ(lhs[1][1], 7627);
  }
}

TEST(MDY_TEST_CATEGORY_MATRIX2x2, TestElementDivision)
{
  auto lhs = dy::DDyMatrix2x2{};
  lhs[0][0] = 1000; lhs[1][0] = 640;
  lhs[0][1] = 2000; lhs[1][1] = 320;

  auto rhs = dy::DDyMatrix2x2{};
  rhs[0][0] = 50; rhs[1][0] = -480;
  rhs[0][1] = -200; rhs[1][1] = 240;

  {
    const auto result = lhs / rhs;
    EXPECT_FLOAT_EQ(result[0][0],  20);
    EXPECT_FLOAT_EQ(result[1][0], -1.334f);
    EXPECT_FLOAT_EQ(result[0][1], -10);
    EXPECT_FLOAT_EQ(result[1][1],  1.334f);
  }

  {
    lhs /= rhs;
    EXPECT_FLOAT_EQ(lhs[0][0],  20);
    EXPECT_FLOAT_EQ(lhs[1][0], -1.334f);
    EXPECT_FLOAT_EQ(lhs[0][1], -10);
    EXPECT_FLOAT_EQ(lhs[1][1],  1.334f);
  }
}

TEST(MDY_TEST_CATEGORY_MATRIX2x2, TestElementDivisionExceptionThrow)
{
  auto lhs = dy::DDyMatrix2x2{};
  lhs[0][0] = 1000; lhs[1][0] = 640;
  lhs[0][1] = 2000; lhs[1][1] = 320;

  auto rhs = dy::DDyMatrix2x2{};
  rhs[0][0] = 0; rhs[1][0] = -480;
  rhs[0][1] = -200; rhs[1][1] = 240;

  EXPECT_ANY_THROW(const auto result = lhs / rhs);
}

TEST(MDY_TEST_CATEGORY_MATRIX2x2, TestInversion)
{
  {
    auto lhs = dy::DDyMatrix2x2{};
    lhs[0][0] = 1; lhs[1][0] = 0;
    lhs[0][1] = 0; lhs[1][1] = 1;

    const auto invLhs = lhs.Inverse();
    EXPECT_FLOAT_EQ(invLhs[0][0], 1);
    EXPECT_FLOAT_EQ(invLhs[1][0], 0);
    EXPECT_FLOAT_EQ(invLhs[0][1], 0);
    EXPECT_FLOAT_EQ(invLhs[1][1], 1);
  }

  {
    auto lhs = dy::DDyMatrix2x2{};
    lhs[0][0] = 4; lhs[1][0] = 0;
    lhs[0][1] = 0; lhs[1][1] = 4;

    const auto invLhs = lhs.Inverse();
    EXPECT_FLOAT_EQ(invLhs[0][0], 0.25f);
    EXPECT_FLOAT_EQ(invLhs[1][0], 0);
    EXPECT_FLOAT_EQ(invLhs[0][1], 0);
    EXPECT_FLOAT_EQ(invLhs[1][1], 0.25f);
  }
}

TEST(MDY_TEST_CATEGORY_MATRIX2x2, TestInversionExceptionThrow)
{
  {
    auto lhs = dy::DDyMatrix2x2{};
    lhs[0][0] = 3; lhs[1][0] =  5;
    lhs[0][1] = 6; lhs[1][1] = 10;

    EXPECT_ANY_THROW(const auto invLhs = lhs.Inverse());
  }

  {
    auto lhs = dy::DDyMatrix2x2{};
    lhs[0][0] = 3; lhs[1][0] =  1;
    lhs[0][1] = 29; lhs[1][1] = 10;

    EXPECT_NO_THROW(const auto invLhs = lhs.Inverse());
  }
}

TEST(MDY_TEST_CATEGORY_MATRIX2x2, TestMultiplyWithVector)
{
  const auto lhs = dy::DDyMatrix2x2{
    3, 5,
    6, 10
  };
  const auto rhs = dy::DVector2{10, -10};

  {
    const auto result = lhs.MultiplyVector(rhs);
    EXPECT_FLOAT_EQ(result.X, -20);
    EXPECT_FLOAT_EQ(result.Y, -40);
  }

  {
    const auto result = rhs.MultiplyMatrix(lhs);
    EXPECT_FLOAT_EQ(result.X, -30);
    EXPECT_FLOAT_EQ(result.Y, -50);
  }
}
