#include "pch.h"
#include <Dy/Helper/Type/Matrix3.h>
#include <glm/glm.hpp>

#define MDY_TEST_CATEGORY_MATRIX3x3 DyTestDDyMatrix3x3

TEST(MDY_TEST_CATEGORY_MATRIX3x3, TestCreation)
{
  const auto matrix = dy::DDyMatrix3x3{
      0, 1, 2,
      3, 4, 5,
      6, 7, 8
  };

  EXPECT_FLOAT_EQ(matrix[0][0], 0);
  EXPECT_FLOAT_EQ(matrix[1][0], 1);
  EXPECT_FLOAT_EQ(matrix[2][0], 2);
  EXPECT_FLOAT_EQ(matrix[0][1], 3);
  EXPECT_FLOAT_EQ(matrix[1][1], 4);
  EXPECT_FLOAT_EQ(matrix[2][1], 5);
  EXPECT_FLOAT_EQ(matrix[0][2], 6);
  EXPECT_FLOAT_EQ(matrix[1][2], 7);
  EXPECT_FLOAT_EQ(matrix[2][2], 8);
}

TEST(MDY_TEST_CATEGORY_MATRIX3x3, TestConversion)
{
  const auto matrix = dy::DDyMatrix3x3{
      0, 1, 2,
      3, 4, 5,
      6, 7, 8
  };

  {
    const glm::mat3 convertedMatrix = matrix;
    EXPECT_FLOAT_EQ(convertedMatrix[0][0], 0);
    EXPECT_FLOAT_EQ(convertedMatrix[1][0], 1);
    EXPECT_FLOAT_EQ(convertedMatrix[2][0], 2);
    EXPECT_FLOAT_EQ(convertedMatrix[0][1], 3);
    EXPECT_FLOAT_EQ(convertedMatrix[1][1], 4);
    EXPECT_FLOAT_EQ(convertedMatrix[2][1], 5);
    EXPECT_FLOAT_EQ(convertedMatrix[0][2], 6);
    EXPECT_FLOAT_EQ(convertedMatrix[1][2], 7);
    EXPECT_FLOAT_EQ(convertedMatrix[2][2], 8);
  }
}