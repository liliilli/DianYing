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
#include <Math/Type/Math/DVector2.h>
#include <Math/Type/Math/DVector3.h>
#include <Math/Utility/XLinearMath.h>
#include <Math/Utility/XMath.h>

namespace dy::math
{

TEST_CASE("Dot function operates properly.", "[MathDot]") 
{
  SECTION("Vector2, Integer types")
  {
    const DVector2<TI32> lhs = {5, 4};
    const DVector2<TI16> rhs = {-16, 8};

    const auto result = Dot(lhs, rhs);
    REQUIRE(result == -48.f);
  }

  SECTION("Vector2, Real types")
  {
    const DVector2<TF32> lhs = {1.2f, 1.0f};
    const DVector2<TF32> rhs = {4.4f, -7.6f};

    const auto result = Dot(lhs, rhs);
    REQUIRE(IsNearlyEqual(result, -2.32f, 0.01f) == true);
  }

  SECTION("Vector3, Integer types")
  {
    const DVector3<TI16> lhs = {20, -5, 8};
    const DVector3<TI8>  rhs = {16, 8, -10};

    const auto result = Dot(lhs, rhs);
    REQUIRE(result == 200.f);
  }

  SECTION("Vector3, Real types")
  {
    const DVector3<TF32> lhs = {1.6f, 8.8f, -7.225f};
    const DVector3<TF64> rhs = {7.11, 9.15, 10.1134};

    const auto result = Dot(lhs, rhs);
    REQUIRE(IsNearlyEqual(result, 18.826685f, 0.01f) == true);
  }
}

} /// ::dy::math namespace
