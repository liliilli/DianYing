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

TEST_CASE("IsNearlyEqual function operates properly.", "[IsNearlyEqual]") 
{
  SECTION("Vector2")
  {
    const DVector2<TI32> lhs = {5, 4};
    const DVector2<TF32> rhs = {5.1f, 4.1f};

    const auto result = IsNearlyEqual(lhs, rhs, 0.1f);
    REQUIRE(result == true);
  }

  SECTION("Vector3")
  {
    const DVector3<TI32> lhs = {-5, 4, 3};
    const DVector3<TF32> rhs = {-5.1f, 4.101f, 3.005f};

    const auto result = IsNearlyEqual(lhs, rhs, 0.1f);
    REQUIRE(result == false);
  }

  SECTION("Vector4")
  {
    const DVector4<TI32> lhs = {-5, 4, 3, 2};
    const DVector4<TF32> rhs = {-5.1f, 4.100f, 3.005f, 2.099f};

    const auto result = IsNearlyEqual(lhs, rhs, 0.1f);
    REQUIRE(result == true);
  }
}

} /// ::dy::math namespace
