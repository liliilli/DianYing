///
/// MIT License
/// Copyright (c) 2018-2019 Jongmin Yun
///
/// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
/// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
/// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
/// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
/// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
/// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
/// SOFTWARE.
///

#include <Catch2/catch.hpp>
#define MATH_ENABLE_SIMD
#include <Math/Type/Math/DVector4.h>

namespace dy::math
{

TEST_CASE("SIMD Integer vectors can be initialized and assigned.", "[DVector4Simd]") 
{

  SECTION("Check initilaized vector values are matched.")
  {
    DVector4<TI32> vector{0, 1, 2, 3};
    REQUIRE( vector.X == 0 );
    REQUIRE( vector.Y == 1 );
    REQUIRE( vector.Z == 2 );
    REQUIRE( vector.W == 3 );
  }

  SECTION("Check initilaized vector values has arithmetic operators")
  {
    DVector4<TI32> vector{0, 1, 2, 3};
    vector += DVector4<TI32>(-5, -4, -3, -2);
    REQUIRE(vector[0] == -5); REQUIRE(vector[1] == -3); REQUIRE(vector[2] == -1); REQUIRE(vector[3] == +1);

    vector -= DVector4<TI32>(-5, -4, -3, -2);
    REQUIRE(vector[0] == 0); REQUIRE(vector[1] == 1); REQUIRE(vector[2] == 2); REQUIRE(vector[3] == 3);

    vector *= 3;
    REQUIRE(vector[0] == 0); REQUIRE(vector[1] == 3); REQUIRE(vector[2] == 6); REQUIRE(vector[3] == 9);
    
    const auto homolen  = vector.GetHomogeneousLength();
    const auto square   = vector.GetSquareLength();
    REQUIRE(square == 9 + 36 + 81);
    REQUIRE(homolen == std::sqrt(9.f + 36.f));
    const auto len = vector.GetLength();
    REQUIRE(len == std::sqrt(9 + 36 + 81.f));

    const auto real = vector.Normalize();
    REQUIRE(real.GetSquareLength() == 1.0f);
  }
}

}
