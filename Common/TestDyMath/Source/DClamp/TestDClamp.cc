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
#include <Math/Type/Micellanous/DClamp.h>
#include <Math/Utility/XMath.h>

namespace dy::math
{

TEST_CASE("DClamp can operate.", "[DClamp]")
{
  SECTION("Check initialized values are clamped.")
  {
    DClamp<TF32, 0, 1> _0 = 2.0f;
    REQUIRE(_0 == 1.0f);

    _0 = -1.0f;
    REQUIRE(_0 == 0.0f);

    DClamp<TI32, -16, 2288> _1 = 2243;
    REQUIRE(_1 == 2243);

    _1 = 2289;
    REQUIRE(_1 == 2288);
  }

  SECTION("Check value can do arithmetic process.")
  {
    DClamp<TF32, -10, 10> _0 = 1.0f;
    _0 += 2.14159f;
    _0 -= 2.14159f;
    _0 *= 2.14159f;
    _0 /= 2.14159f;
    _0 += 2.14159f;

    REQUIRE(IsNearlyEqual(_0, 3.14159f, 0.01f) == true);

    decltype(_0) _1 = _0 * 5.0f;
    REQUIRE(_1 == 10.0f);
  }

  SECTION("Check value can do comparison process.")
  {
    DClamp<TF32, -10, 10> _0 = 1.0f;
    DClamp<TF32, -10, 10> _1 = 5.0f;

    REQUIRE(_0 < _1);
    REQUIRE(_0 != _1);

    _0 += 5.0f;

    REQUIRE(_0 >= _1);
  }

  SECTION("Check value can interact with vary range but same type clamps.")
  {
    DClamp<TF32, -10, 50> _0 = 9.0f;
    DClamp<TF32, -20, 20> _1 = 16.3f;

    _0 += _1;
    REQUIRE(_0 == 25.3f);
    _0 -= _1;
    REQUIRE(_0 == 9.0f);
    _0 *= _1;
    REQUIRE(_0 == 50.0f);
    _0 /= _1;
    REQUIRE(IsNearlyEqual(_0, 3.0674f, 0.01f));
    _0 += _1;
    REQUIRE(_0 >= _1);
  }
}

} /// ::dy::math namespace
