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

#define CATCH_CONFIG_RUNTIME_STATIC_REQUIRE 
#include <Catch2/catch.hpp>
#include <Math/Common/XGlobalUtilites.h>
#include <Math/Common/Inline/TGlobalTypes.inl>
#include <Math/Common/TGlobalTypes.h>

namespace dy::math
{

TEST_CASE("Testing Global Utility API", "[GlobalUtility]")
{
  SECTION("Get coverable types")
  {
    STATIC_REQUIRE(std::is_same_v<GetCoverableTypeOf<EValueCategory::Character, 1>::Type, TChr8>);
    STATIC_REQUIRE(std::is_same_v<GetCoverableTypeOf<EValueCategory::Character, 8>::Type, TChr8>);
    STATIC_REQUIRE(std::is_same_v<GetCoverableTypeOf<EValueCategory::Character, 9>::Type, TChr16>);
    STATIC_REQUIRE(std::is_same_v<GetCoverableTypeOf<EValueCategory::Character, 16>::Type, TChr16>);
    STATIC_REQUIRE(std::is_same_v<GetCoverableTypeOf<EValueCategory::Character, 32>::Type, TChr32>);
    STATIC_REQUIRE(std::is_same_v<GetCoverableTypeOf<EValueCategory::Character, 33>, expr::TNull>);
  }

  SECTION("Get bigger types")
  {
    STATIC_REQUIRE(std::is_same_v<GetBiggerType<TI8, TI32>, TI32>);
    STATIC_REQUIRE(std::is_same_v<GetBiggerType<TI16, TI16>, TI16>);
    STATIC_REQUIRE(std::is_same_v<GetBiggerType<TU64, TU8>, TU64>);
    STATIC_REQUIRE(std::is_same_v<GetBiggerType<TF32, TF64>, TF64>);
  }

  SECTION("Get bigger types between real and integer type")
  {
    STATIC_REQUIRE(std::is_same_v<GetBiggerType<TI8, TF32>, TF32>);
    STATIC_REQUIRE(std::is_same_v<GetBiggerType<TI16, TF64>, TF64>);
    STATIC_REQUIRE(std::is_same_v<GetBiggerType<TF32, TI32>, TF32>);
    STATIC_REQUIRE(std::is_same_v<GetBiggerType<TF32, TF64>, TF64>);
  }
}

} /// ::dy::math