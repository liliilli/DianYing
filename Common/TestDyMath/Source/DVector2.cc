#include <Catch2/catch.hpp>
#include <Type/DVector2.h>

namespace dy::math
{

TEST_CASE("Integer vectors can be initialized and assigned.", "[DVector2]") 
{
  DVector2<TI32> vector{0, 1};

  SECTION("Check initilaized vector values are matched.")
  {
    REQUIRE( vector.X == 0 );
    REQUIRE( vector.Y == 1 );
  }

  SECTION("Check initilaized vector values are matched with operator[]")
  {
    REQUIRE( vector[0] == 0 );
    REQUIRE( vector[1] == 1 );
  }

  SECTION("Check not specified index will be asserted.")
  {
    REQUIRE_NOTHROW(vector[0]);
    REQUIRE_NOTHROW(vector[1]);
    REQUIRE_THROWS(vector[2]);
    REQUIRE_THROWS(vector[3]);
  }

  SECTION("Check reassigned values are matched.")
  {
    vector.X = 10;
    vector.Y += 39;

    REQUIRE(vector[0] == 10);
    REQUIRE(vector[1] == 40);
  }

  SECTION("Check reassigned values are matched.")
  {
    vector.X = 10;
    vector.Y += 39;

    REQUIRE(vector[0] == 10);
    REQUIRE(vector[1] == 40);
  }

  SECTION("Check Data() API returns pointer of first element.")
  {
    REQUIRE(vector.Data() == &vector.X);
    REQUIRE(vector.Data() != &vector.Y);
  }

  SECTION("Check squared length of vector when positive values.")
  {
    const auto squaredLength = Cast<TReal>(vector[0] * vector[0] + vector[1] * vector[1]);
    REQUIRE(vector.GetSquareLength() == squaredLength);
  }

  SECTION("Check squared length of vector when positive, and negative values.")
  {
    vector.X = -50;

    const auto squaredLength = Cast<TReal>(vector[0] * vector[0] + vector[1] * vector[1]);
    REQUIRE(vector.GetSquareLength() == squaredLength);
  }

  SECTION("Check length of vector when positive values.")
  {
    vector = DVector2<TI32>{10, 15};;

    const auto length = Cast<TReal>(std::sqrt(vector[0] * vector[0] + vector[1] * vector[1]));
    REQUIRE(vector.GetLength() == length);
  }

  SECTION("Check length of vector when negative values.")
  {
    vector = DVector2<TI32>{-16, -15};

    const auto length = Cast<TReal>(std::sqrt(vector[0] * vector[0] + vector[1] * vector[1]));
    REQUIRE(vector.GetLength() == length);
  }

  SECTION("Check normalized vector are matched.")
  {
    vector = DVector2<TI32>{3, 4};
    REQUIRE(vector.Normalize() == DVector2<TReal>{0.6f, 0.8f});
  }

  SECTION("Check conversion of integer to real.")
  {
    vector = DVector2<TI32>{3, 4};

    const auto realVector = static_cast<DVector2<TReal>>(vector);
    REQUIRE(realVector.X == 3.0f);
    REQUIRE(realVector.Y == 4.0f);
  }

  SECTION("Check mathmatical operators")
  {
    vector = DVector2<TI32>{16, 8};
    const auto anotherVector = DVector2<TI32>{-16, -8};

    REQUIRE((vector + anotherVector) == DVector2<TI32>{0, 0});
    REQUIRE((vector - anotherVector) == DVector2<TI32>{32, 16});
    REQUIRE((vector * anotherVector) == DVector2<TI32>{-256, -64});
    REQUIRE((vector / anotherVector) == DVector2<TI32>{-1, -1});

    vector = DVector2<TI32>{24, 12};
    REQUIRE((vector / anotherVector) == DVector2<TI32>{-1, -1});
  }

  SECTION("Check mathmatical unary operators")
  {
    vector = DVector2<TI32>{16, 8};
    const auto anotherVector = DVector2<TI32>{-16, -8};
    
    vector += anotherVector;
    REQUIRE(vector == DVector2<TI32>{0, 0});

    vector -= (anotherVector * 2);
    REQUIRE(vector == DVector2<TI32>{32, 16});
  }
}

}
