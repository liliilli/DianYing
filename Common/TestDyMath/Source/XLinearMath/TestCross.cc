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

TEST_CASE("Cross function operates properly.", "[MathCross]") 
{
  SECTION("Vector3, Integer types")
  {
    const DVector3<TI16> lhs = {20, -5, 8};
    const DVector3<TI8>  rhs = {16, 8, -10};

    const auto result = Cross(lhs, rhs);
    REQUIRE(result[0] == -14);
    REQUIRE(result[1] == 328);
    REQUIRE(result[2] == 240);
  }

  SECTION("Vector3, Real types")
  {
    const DVector3<TF32> lhs = {1.5f, 8.8f, -7.62f};
    const DVector3<TF32> rhs = {-3.65f, 1.2f, 4.0f};

    const auto result = Cross(lhs, rhs);
    REQUIRE(IsNearlyEqual(result[0], 44.344f, 0.01f) == true);
    REQUIRE(IsNearlyEqual(result[1], 21.813f, 0.01f) == true);
    REQUIRE(IsNearlyEqual(result[2], 33.92f, 0.01f) == true);
  }
}

} /// ::dy::math namespace
