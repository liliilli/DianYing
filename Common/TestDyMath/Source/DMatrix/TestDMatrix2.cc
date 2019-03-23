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
#include <Math/Type/Math/DMatrix2.h>

namespace dy::math
{

TEST_CASE("DMatrix2 can operate.", "[DMatrix2]")
{
  SECTION("Check initialized value are matched.")
  {
    // 3 5  
    // 4 6
    DMatrix2<TReal> matrix = {3, 5, 4, 6};

    REQUIRE(matrix[0][0] == 3.f);
    REQUIRE(matrix[0][1] == 4.f);
    REQUIRE(matrix[1][0] == 5.f);
    REQUIRE(matrix[1][1] == 6.f);

    matrix = {DVector2<TReal>{2.0f, 3.0f}, DVector2<TReal>{4.0f, 5.0f}};

    REQUIRE(matrix[0][0] == 2.f);
    REQUIRE(matrix[0][1] == 3.f);
    REQUIRE(matrix[1][0] == 4.f);
    REQUIRE(matrix[1][1] == 5.f);
  }

  SECTION("Check mutliplicated values are matched.")
  {
    const DVector2<TReal> vector = {5, 10};
    const DMatrix2<TReal> matrix = {3, 5, 4, 6};

    const auto vm = vector * matrix;
    const auto mv = matrix * vector;

    REQUIRE(vm[0] == 55);
    REQUIRE(vm[1] == 85);
    REQUIRE(mv[0] == 65);
    REQUIRE(mv[1] == 80);

    // 6 4
    // 5 3
    const auto mm_ = matrix * DMatrix2<TReal>{6, 4, 5, 3};
    REQUIRE(mm_[0][0] == 43);
    REQUIRE(mm_[0][1] == 54);
    REQUIRE(mm_[1][0] == 27);
    REQUIRE(mm_[1][1] == 34);
  }

  SECTION("Check unary operators operates.")
  {
    DMatrix2<TReal> matrix = {3, 5, 4, 6};
    const auto matrix2 = DMatrix2<TReal>{6, 4, 5, 3};

    matrix *= matrix2;
    REQUIRE(matrix[0][0] == 43);
    REQUIRE(matrix[0][1] == 54);
    REQUIRE(matrix[1][0] == 27);
    REQUIRE(matrix[1][1] == 34);

    matrix += matrix;
    REQUIRE(matrix[0][0] == 86);
    REQUIRE(matrix[0][1] == 108);
    REQUIRE(matrix[1][0] == 54);
    REQUIRE(matrix[1][1] == 68);

    matrix /= (matrix - matrix / 2);
    REQUIRE(matrix[0][0] == 2);
    REQUIRE(matrix[0][1] == 2);
    REQUIRE(matrix[1][0] == 2);
    REQUIRE(matrix[1][1] == 2);
  }

  SECTION("Check determinant and inversion.")
  {
    DMatrix2<TReal> matrix = {3, 5, 4, 6};

    const auto isInvertible = matrix.IsInvertible();
    REQUIRE(isInvertible == true);

    const auto det = matrix.GetDeterminant();
    REQUIRE(det == -2);

    const auto inverse = matrix.Inverse();
    REQUIRE(inverse[0][0] == -3);
    REQUIRE(inverse[0][1] == 2);
    REQUIRE(inverse[1][0] == +2.5f);
    REQUIRE(inverse[1][1] == -1.5f);
  }
}

} /// ::dy::math namespace
