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
#include <Math/Utility/XMath.h>

namespace dy::math
{

TEST_CASE("MapValue function operates properly.", "[MapValue]") 
{
  SECTION("Integer, MapValue")
  {
    //const TI32 value = 6;
    STATIC_REQUIRE(MapValue(6, -10, 7) == 6);
    STATIC_REQUIRE(MapValue(6, -10, 6) == -10);
    STATIC_REQUIRE(MapValue(5, 10, 15) == 10);
    STATIC_REQUIRE(MapValue(3, -10, -8) == -9);
  }

  SECTION("Real, MapValue")
  {
    REQUIRE(MapValue(kPi<TF32>, -kPi<TF32>, kPi<TF32>) == -kPi<TF32>);
  }
}

} /// ::dy::math namespace
