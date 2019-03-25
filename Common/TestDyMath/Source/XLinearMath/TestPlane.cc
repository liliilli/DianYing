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
#include <Math/Type/Shape/DPlane.h>
#include <Math/Type/Math/DVector3.h>
#include <Math/Utility/XMath.h>

namespace dy::math
{

TEST_CASE("DPlane API test.", "[DPlane]") 
{
  SECTION("Test1")
  {
    const DPlane<TF32> plane = {1, 2, 0, 3};
    REQUIRE(IsNearlyEqual(plane.GetNormalVector().GetLength(), 1.0f, 0.05f));

    const DVector3<TF32> pos = {3, 4, 0};
    REQUIRE(IsNearlyEqual(plane.GetSignedDistanceFrom(pos), 6.26f, 0.05f));
    REQUIRE(plane.CheckPointStatusOnPlane(pos) == EPosPlaneState::FrontNormal);
    REQUIRE(IsNearlyEqual(
      plane.GetClosestPoint(pos), 
      pos - 6.26f * DVector3<TF32>(1, 2, 0).Normalize(),
      0.05f));
  }
}

} /// ::dy::math namespace
