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
#include <Math/Type/Micellanous/DBounds2D.h>
#include <Math/Utility/XMath.h>

namespace dy::math
{

TEST_CASE("Bounds 2D can be operated.", "[DBounds2D]")
{
  SECTION("Check initialized integer values are matched.")
  {
    const DVector2<TI32> intStart{-10, 10};
    const DVector2<TI32> intLength{20, -5};
    const DBounds2D<TI32> intBounds{intStart, intLength};

    REQUIRE(intBounds.GetArea() == 450);
    REQUIRE(intBounds.GetDiagonal() == DVector2<TI32>(30, 15));
    REQUIRE(intBounds.GetMaximumPoint() == DVector2<TI32>(20, 10));
    REQUIRE(intBounds.GetMinimumPoint() == DVector2<TI32>(-10, -5));
  }

  SECTION("Check initialized real values are matched.")
  {
    const DVector2<TReal> p1{-28.3, 16.2};
    const DVector2<TReal> p2{-18.6, 7.7};
    const DBounds2D<TReal> bounds{p1, p2};

    REQUIRE(bounds.GetArea() == 82.45f);
    REQUIRE(IsNearlyEqual(bounds.GetDiagonal(), DVector2<TReal>(9.7f, 8.5f)));
    REQUIRE(bounds.GetMaximumPoint() == DVector2<TReal>(-18.6f, 16.2f));
    REQUIRE(bounds.GetMinimumPoint() == DVector2<TF32>(-28.3f, 7.7f));;

    const DBounds2D<TReal> bounds2 = {{-28.3f, 7.7f}, {-18.6f, 16.2f}};
    REQUIRE(bounds == bounds2);
  }

  SECTION("Check operator+ operated properly.")
  {
    const DVector2<TI32> intStart{-10, 10};
    const DVector2<TI32> intLength{20, -5};
    const DBounds2D<TI32> intBounds{intStart, intLength};

    const DVector2<TReal> p1{-28.3, 16.2};
    const DVector2<TReal> p2{-18.6, 7.7};
    const DBounds2D<TReal> bounds{p1, p2};

    const auto final = intBounds + bounds;
    REQUIRE(IsNearlyEqual(final.GetMaximumPoint(), DVector2<TReal>(20, 16.2f)));
    REQUIRE(IsNearlyEqual(final.GetMinimumPoint(), DVector2<TReal>(-28.3f, -5.f)));
  }
}

} /// ::dy::math namespace
