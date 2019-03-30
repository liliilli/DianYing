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
#include <Math/Type/Micellanous/DBounds3D.h>
#include <Math/Utility/XMath.h>

namespace dy::math
{

TEST_CASE("Bounds 3D can be operated.", "[DBounds3D]")
{
  SECTION("Check initialized integer values are matched.")
  {
    const DVector3<TI32> intStart{-10, 10, 4};
    const DVector3<TI32> intLength{20, -5, -8};
    const DBounds3D<TI32> intBounds{intStart, intLength};

    REQUIRE(intBounds.GetArea() == 5'400);
    REQUIRE(intBounds.GetDiagonal() == DVector3<TI32>(30, 15, 12));
    REQUIRE(intBounds.GetMaximumPoint() == DVector3<TI32>(20, 10, 4));
    REQUIRE(intBounds.GetMinimumPoint() == DVector3<TI32>(-10, -5, -8));
  }

  SECTION("Check initialized real values are matched.")
  {
    const DVector3<TReal> p1{-28.3, 16.2, 30.2};
    const DVector3<TReal> p2{-18.6, 7.7, 28.4};
    const DBounds3D<TReal> bounds{p1, p2};

    REQUIRE(bounds.GetArea() == 148.4101f);
    REQUIRE(IsNearlyEqual(bounds.GetDiagonal(), DVector3<TReal>(9.7f, 8.5f, 1.8f)));
    REQUIRE(bounds.GetMaximumPoint() == DVector3<TReal>(-18.6f, 16.2f, 30.2f));
    REQUIRE(bounds.GetMinimumPoint() == DVector3<TF32>(-28.3f, 7.7f, 28.4f));;

    const DBounds3D<TReal> bounds2 = {{-28.3f, 7.7f, 30.2f}, {-18.6f, 16.2f, 28.4f}};
    REQUIRE(bounds == bounds2);
  }

#ifdef false
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
#endif
}

} /// ::dy::math namespace
