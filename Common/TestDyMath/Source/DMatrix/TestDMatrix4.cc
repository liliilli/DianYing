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
#include <Math/Type/Math/DMatrix4.h>

namespace dy::math
{

TEST_CASE("DMatrix4 can operate.", "[DMatrix4]")
{
  SECTION("Check initialized value are matched.")
  {
    DMatrix4<TReal> matrix = 
    {
      3, 4, 5, 6,
      6, 7, 8, 9,
      9, 10, 11, 12,
      12, 13, 14, 15
    };

    REQUIRE(matrix[0][0] == 3.f);
    REQUIRE(matrix[1][0] == 4.f);
    REQUIRE(matrix[2][0] == 5.f);
    REQUIRE(matrix[0][1] == 6.f);
    REQUIRE(matrix[3][0] == 6.f);

    REQUIRE(matrix[1][1] == 7.f);
    REQUIRE(matrix[2][1] == 8.f);
    REQUIRE(matrix[0][2] == 9.f);
    REQUIRE(matrix[3][1] == 9.f);

    REQUIRE(matrix[1][2] == 10.f);
    REQUIRE(matrix[2][2] == 11.f);
    REQUIRE(matrix[0][3] == 12.f);
    REQUIRE(matrix[3][2] == 12.f);

    REQUIRE(matrix[1][3] == 13.f);
    REQUIRE(matrix[2][3] == 14.f);
    REQUIRE(matrix[3][3] == 15.f);
  }

  SECTION("Check mutliplicated values are matched.")
  {
    const DVector4<TReal> vector = {5, 10, 15, 20};
    const DMatrix4<TReal> matrix = 
    {
      3, 4, 5, 6,
      6, 7, 8, 9,
      9, 10, 11, 12,
      12, 13, 14, 15
    };

    const auto vm = vector * matrix;
    const auto mv = matrix * vector;

    REQUIRE(vm == DVector4<TReal>(450, 500, 550, 600));
    REQUIRE(mv == DVector4<TReal>(250, 400, 550, 700));

    // 5 3
    const auto mm_ = matrix * DMatrix4<TReal>
    {
      2, 3, 4, 5, 
      5, 6, 7, 8,
      8, 9, 10, 11,
      11, 12, 13, 14
    };
    REQUIRE(mm_[0] == DVector4<TReal>(132, 210, 288, 366));
    REQUIRE(mm_[1] == DVector4<TReal>(150, 240, 330, 420));
    REQUIRE(mm_[2] == DVector4<TReal>(168, 270, 372, 474));
    REQUIRE(mm_[3] == DVector4<TReal>(186, 300, 414, 528));
  }

  SECTION("Check determinant and inversion.")
  {
    const DMatrix4<TReal> matrix = 
    {
      3, 4, 5, 6,
      6, 8, 8, 9,
      9, 10, 11, 12,
      12, 13, 14, 16
    };;

    const auto isInvertible = matrix.IsInvertible();
    REQUIRE(isInvertible == true);

    const auto inverse = matrix.Inverse();
    REQUIRE(inverse[0] == DVector4<TReal>(-5.f/12, -0.5f, 0.25f, 0.5f));
  }
}

} /// ::dy::math namespace
