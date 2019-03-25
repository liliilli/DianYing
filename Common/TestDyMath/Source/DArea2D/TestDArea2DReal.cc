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
#include <Math/Type/Micellanous/DArea2D.h>
#include <Math/Type/Math/DVector2.h>
#include <Math/Utility/XMath.h>

namespace dy::math
{

TEST_CASE("Integer DArea2D with real type can be operated.", "[DArea2DReal]")
{
  SECTION("Check initialized vector values are matched.")
  {
    const DVector2<TF32> start{-7.6f, -3.2f};
    const DVector2<TF32> length{19.9f, 28.3f};
    const DArea2D<TF32> area{start, length};

    REQUIRE(area.GetStartPoint() == start);
    REQUIRE(area.GetWidth() == length[0]);
    REQUIRE(area.GetHeight() == length[1]);
    REQUIRE(area.GetEndPoint() == (start + length));
  }

  SECTION("Check initialized plain values are matched.")
  {
    const DArea2D<TF64> area{-10.0, -30, -87.22, 66.45};

    REQUIRE(area.GetStartPoint() == DVector2<TF64>{-97.22, -30});
    REQUIRE(area.GetWidth() == 87.22);
    REQUIRE(area.GetHeight() == 66.45);
    REQUIRE(area.GetEndPoint() == DVector2<TF64>{-10, 36.45});
  }

  SECTION("Check getting area operated.")
  {
    const DVector2<TF32> start = {-20.45f, -30.662f};
    const DVector2<TF32> length = {45.55f, 10.23f};
    const DArea2D<TF32> area = {start, length};

    REQUIRE(IsNearlyEqual(area.GetArea(), 465.975f, 0.01f) == true);
  }

  SECTION("Check set width, height operated.")
  {
    const DVector2<TF32> intStart{-20.f, -30.f};
    const DVector2<TF64> intLength{20.f, 30.f};

    DArea2D<TF64> intArea{static_cast<DVector2<TF64>>(intStart), intLength};
    REQUIRE(intArea.GetEndPoint() == DVector2<TF64>{0, 0});
    
    intArea.SetWidth(30);
    REQUIRE(intArea.GetEndPoint() == DVector2<TF64>{10, 0});

    intArea.SetHeight(-5);
    REQUIRE(intArea.GetEndPoint() == DVector2<TF64>{10, -30});
  }
}

} /// ::dy::math namespace
