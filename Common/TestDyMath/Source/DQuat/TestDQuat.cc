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
#include <Math/Type/Math/DQuat.h>

namespace dy::math
{

TEST_CASE("DQuaternion can operate.", "[DQuaternion]")
{
  SECTION("Check initialized value are matched.")
  {
    DQuaternion<TReal> matrix = {};

    REQUIRE(matrix.W() == 1);
    REQUIRE(matrix.X() == 0);
    REQUIRE(matrix.Y() == 0);
    REQUIRE(matrix.Z() == 0);
   
    matrix.SetAngle({45, 90, 0}, true);
    //matrix.AddAngle(EAxis::X, 45.0f);

    REQUIRE(IsNearlyEqual(matrix.W(), 0.707f));
    REQUIRE(IsNearlyEqual(matrix.X(), 0.f));
    REQUIRE(IsNearlyEqual(matrix.Y(), 0.707f));
    REQUIRE(IsNearlyEqual(matrix.Z(), 0.f));
  }

#ifdef false
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
#endif
}

} /// ::dy::math namespace
