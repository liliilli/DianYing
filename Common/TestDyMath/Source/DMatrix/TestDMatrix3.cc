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
#include <Math/Utility/XMath.h>
#include <Math/Type/Math/DMatrix3.h>

namespace dy::math
{

TEST_CASE("DMatrix3 can operate.", "[DMatrix3]")
{
  SECTION("Check initialized value are matched.")
  {
    DMatrix3<TReal> matrix = 
    {
      3, 4, 5, 
      6, 7, 8,
      9, 10, 11
    };

    REQUIRE(matrix[0][0] == 3.f);
    REQUIRE(matrix[1][0] == 4.f);
    REQUIRE(matrix[2][0] == 5.f);
    REQUIRE(matrix[0][1] == 6.f);
    REQUIRE(matrix[1][1] == 7.f);
    REQUIRE(matrix[2][1] == 8.f);
    REQUIRE(matrix[0][2] == 9.f);
    REQUIRE(matrix[1][2] == 10.f);
    REQUIRE(matrix[2][2] == 11.f);

    matrix = {
      DVector3<TReal>{2.0f, 3.0f, 4.0f}, 
      DVector3<TReal>{5.0f, 6.0f, 7.0f},
      DVector3<TReal>{8.0f, 9.0f, 10.0f}
    };


    REQUIRE(matrix[0][0] == 2.f);
    REQUIRE(matrix[0][1] == 3.f);
    REQUIRE(matrix[0][2] == 4.f);
    REQUIRE(matrix[1][0] == 5.f);
    REQUIRE(matrix[1][1] == 6.f);
    REQUIRE(matrix[1][2] == 7.f);
    REQUIRE(matrix[2][0] == 8.f);
    REQUIRE(matrix[2][1] == 9.f);
    REQUIRE(matrix[2][2] == 10.f);
;
  }

  SECTION("Check mutliplicated values are matched.")
  {
    const DVector3<TReal> vector = {5, 10, 15};
    const DMatrix3<TReal> matrix = 
    {
      3, 4, 5, 
      6, 7, 8,
      9, 10, 11
    };

    const auto vm = vector * matrix;
    const auto mv = matrix * vector;

    REQUIRE(vm[0] == 5*3 + 6*10 + 9*15);
    REQUIRE(vm[1] == 5*4 + 7*10 + 10*15);
    REQUIRE(vm[2] == 5*5 + 8*10 + 11*15);
    REQUIRE(mv[0] == 3*5 + 4*10 + 5*15);
    REQUIRE(mv[1] == 6*5 + 7*10 + 8*15);
    REQUIRE(mv[2] == 9*5 + 10*10 + 11*15);

    // 5 3
    const auto mm_ = matrix * DMatrix3<TReal>
    {
      2, 3, 4,
      5, 6, 7,
      8, 9, 10
    };
    REQUIRE(mm_[0] == DVector3<TReal>(66, 111, 156));
    REQUIRE(mm_[1] == DVector3<TReal>(78, 132, 186));
    REQUIRE(mm_[2] == DVector3<TReal>(90, 153, 216));
  }

  SECTION("Check unary operators operates.")
  {
    DMatrix3<TReal> matrix = 
    {
      3, 4, 5, 
      6, 7, 8,
      9, 10, 11
    };
    const auto matrix2 = DMatrix3<TReal>
    {
      2, 3, 4,
      5, 6, 7,
      8, 9, 10
    };

    matrix *= matrix2;
    REQUIRE(matrix[0] == DVector3<TReal>(66, 111, 156));
    REQUIRE(matrix[1] == DVector3<TReal>(78, 132, 186));
    REQUIRE(matrix[2] == DVector3<TReal>(90, 153, 216));;

    matrix += matrix;
    matrix /= (matrix - matrix / 2);
    REQUIRE(matrix[0][0] == 2); REQUIRE(matrix[0][1] == 2); REQUIRE(matrix[0][2] == 2);
    REQUIRE(matrix[1][0] == 2); REQUIRE(matrix[1][1] == 2); REQUIRE(matrix[1][2] == 2);
    REQUIRE(matrix[2][0] == 2); REQUIRE(matrix[2][1] == 2); REQUIRE(matrix[2][2] == 2);
  }

  SECTION("Check determinant and inversion.")
  {
    DMatrix3<TReal> matrix = 
    {
      3, 4, 5, 
      6, 7, 8,
      9, 10, 12
    };

    const auto isInvertible = matrix.IsInvertible();
    REQUIRE(isInvertible == true);

    const auto inverse = matrix.Inverse();
    REQUIRE(inverse[0][0] == -4.0f/3); REQUIRE(inverse[0][1] == 0); REQUIRE(inverse[0][2] == +1.0f);
    REQUIRE(inverse[1][0] == -2.0f/3); REQUIRE(inverse[1][1] == 3); REQUIRE(inverse[1][2] == -2.0f);
    REQUIRE(inverse[2][0] == 1); REQUIRE(inverse[2][1] == -2); REQUIRE(inverse[2][2] == +1.0f);
  }
}

} /// ::dy::math namespace
