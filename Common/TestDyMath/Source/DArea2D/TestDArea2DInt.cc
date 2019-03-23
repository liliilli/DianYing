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
#include <Math/Type/Math/DArea2D.h>
#include <Math/Type/Math/DVector2.h>

namespace dy::math
{

TEST_CASE("Integer area 2D can be operated.", "[DArea2D]")
{
  SECTION("Check initialized vector values are matched.")
  {
    const DVector2<TI32> intStart{-10, -5};
    const DVector2<TI32> intLength{20, 10};
    const DArea2D<TI32> intArea{intStart, intLength};

    REQUIRE(intArea.GetStartPoint() == intStart);
    REQUIRE(intArea.GetWidth() == intLength[0]);
    REQUIRE(intArea.GetHeight() == intLength[1]);
    REQUIRE(intArea.GetEndPoint() == (intStart + intLength));
  }

  SECTION("Check initialized plain values are matched.")
  {
    const DArea2D<TI32> intArea{-10, -5, 20, 10};

    REQUIRE(intArea.GetStartPoint() == DVector2<TI32>{-10, -5});
    REQUIRE(intArea.GetWidth() == 20);
    REQUIRE(intArea.GetHeight() == 10);
    REQUIRE(intArea.GetEndPoint() == DVector2<TI32>{10, 5});
  }

  SECTION("Check negative width, length operated.")
  {
    const DVector2<TI32> intStart{-10, -5};
    const DVector2<TI32> intLength{-10, 7};
    const DArea2D<TI32> intArea{intStart, intLength};

    REQUIRE(intArea.GetStartPoint() == DVector2<TI32>{-20, -5});
    REQUIRE(intArea.GetWidth() == 10);
    REQUIRE(intArea.GetHeight() == 7);
    REQUIRE(intArea.GetEndPoint() == DVector2<TI32>{-10, 2});
  }

  SECTION("Check getting area operated.")
  {
    const DVector2<TI16> intStart{-20, -30};
    const DVector2<TI32> intLength{20, 30};
    const DArea2D<TI32> intArea{static_cast<DVector2<TI32>>(intStart), intLength};

    REQUIRE(intArea.GetArea() == TReal(20 * 30));
  }

  SECTION("Check set width, height operated.")
  {
    const DVector2<TI16> intStart{-20, -30};
    const DVector2<TI32> intLength{20, 30};

    DArea2D<TI32> intArea{static_cast<DVector2<TI32>>(intStart), intLength};
    REQUIRE(intArea.GetEndPoint() == DVector2<TI32>{0, 0});
    
    intArea.SetWidth(30);
    REQUIRE(intArea.GetEndPoint() == DVector2<TI32>{10, 0});

    intArea.SetHeight(-5);
    REQUIRE(intArea.GetEndPoint() == DVector2<TI32>{10, -30});
  }
}

} /// ::dy::math namespace
