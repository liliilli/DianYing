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
#include <Math/Utility/XLinearMath.h>
#include <Math/Utility/XMath.h>

namespace dy::math
{

TEST_CASE("Lerp function operates properly.", "[MathLerp]") 
{
  SECTION("OneType, Integer Integer")
  {
    const auto lhs = 0;
    const auto rhs = 10;

    REQUIRE(Lerp(lhs, rhs, 0.5f) == 5.f);

    const TI16 lhs16 = 10;
    const TI64 rhs64 = 90;
    REQUIRE(Lerp(lhs16, rhs64, 0.8f) == 74);
  }

  SECTION("OneType, Real Real")
  {
    const TF32 lhs = -1.5f;
    const TF64 rhs = 1.5f;

    REQUIRE(Lerp(lhs, rhs, 0.25f) == -0.75);
  }

  SECTION("OneType, Integer Real")
  {
    const TI32  lhs = -40;
    const TReal rhs = 62.5f;

    REQUIRE(Lerp(lhs, rhs, 0.35f) == -4.125f);
  }

  SECTION("TwoType, Integer Integer")
  {
    const DVector2<TI32> lhs = {0, 10};
    const DVector2<TI16> rhs = {-10, 0};

    const auto result = Lerp(lhs, rhs, 0.5f);

    REQUIRE(result == DVector2<TReal>{-5.f, 5.f});
  }

  SECTION("ThreeType, Integer Real")
  {
    const DVector3<TI32> lhs = {0, 10, -20};
    const DVector3<TF32> rhs = {-10.f, 0.f, 120.f};

    const auto result = Lerp(lhs, rhs, 0.25f);

    REQUIRE(result == DVector3<TReal>{-2.5f, 7.5f, 15.f});
  }

  SECTION("FourType, Real Real")
  {
    const DVector4<TF32> lhs = {7.f, -8.f, -20.f, 13.f};
    const DVector4<TF32> rhs = {-10.f, 0.f, 80.f, 64.f};

    const auto result = Lerp(lhs, rhs, 0.3f);

    REQUIRE(IsNearlyEqual(result, DVector4<TReal>{1.9f, -5.6f, 10.f, 28.3f}, 0.05f) == true);
  }
}

} /// ::dy::math namespace
