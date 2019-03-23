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
#include <Math/Type/Math/DArea3D.h>
#include <Math/Type/Math/DVector3.h>

namespace dy::math
{

TEST_CASE("Integer area 3D can be operated.", "[DArea3D]")
{
  SECTION("Check initialized vector values are matched.")
  {
    const DVector3<TI32> start{-10, -5, -7};
    const DVector3<TI32> length{20, 10, 6};
    const DArea3D<TI32> intArea{start, length};

    REQUIRE(intArea.GetStartPoint() == start);
    REQUIRE(intArea.GetWidth() == length[0]);
    REQUIRE(intArea.GetHeight() == length[1]);
    REQUIRE(intArea.GetDepth() == length[2]);
    REQUIRE(intArea.GetEndPoint() == (start + length));
    REQUIRE(intArea.GetArea() == 20 * 10 * 6);
  }

  SECTION("Check initialized plain values are matched.")
  {
    const DArea3D<TI32> intArea{-10, -5, 20, 10, 5, 8};

    REQUIRE(intArea.GetStartPoint() == DVector3<TI32>{-10, -5, 20});
    REQUIRE(intArea.GetWidth() == 10);
    REQUIRE(intArea.GetHeight() == 5);
    REQUIRE(intArea.GetDepth() == 8);
    REQUIRE(intArea.GetEndPoint() == DVector3<TI32>{0, 0, 28});
    REQUIRE(intArea.GetArea() == 10 * 5 * 8);
  }

  SECTION("Check negative width, length operated.")
  {
    const DVector3<TI32> start{-10, -5, -7};
    const DVector3<TI32> length{-20, -10, 6};
    const DArea3D<TI32> intArea{start, length};

    REQUIRE(intArea.GetStartPoint() == DVector3<TI32>(-30, -15, -7));
    REQUIRE(intArea.GetWidth() == -length[0]);
    REQUIRE(intArea.GetHeight() == -length[1]);
    REQUIRE(intArea.GetDepth() == length[2]);
    REQUIRE(intArea.GetEndPoint() == start + DVector3<TI32>(0, 0, 6));
    REQUIRE(intArea.GetArea() == 20 * 10 * 6);;
  }

  SECTION("Check set width, height operated.")
  {
    const DVector3<TI32> start{-10, -5, -7};
    const DVector3<TI32> length{-20, -10, 6};
    DArea3D<TI32> intArea{start, length};

    REQUIRE(intArea.GetEndPoint() == DVector3<TI32>{-10, -5, -1});
    
    intArea.SetWidth(30);
    REQUIRE(intArea.GetEndPoint() == DVector3<TI32>{0, -5, -1});

    intArea.SetHeight(-5);
    REQUIRE(intArea.GetEndPoint() == DVector3<TI32>{0, -15, -1});
  }
}

} /// ::dy::math namespace
