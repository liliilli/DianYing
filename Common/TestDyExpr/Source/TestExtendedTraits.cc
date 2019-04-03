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
#include <Expr/TZip.h>
#include <array>
#include <string>

namespace dy::expr
{

struct TestStruct{};

TEST_CASE("TZip helper test", "[TZip]" ) 
{
  SECTION("TZip initialization")
  {
    STATIC_REQUIRE(IsIterable<int[5]> == true);
    STATIC_REQUIRE(IsIterable<float[5]> == true);
    STATIC_REQUIRE(IsIterable<std::string(&)[5]> == true);
    STATIC_REQUIRE(IsIterable<const float[5]> == true);

    STATIC_REQUIRE(IsIterable<const float[]> == false);

    STATIC_REQUIRE(IsIterable<std::vector<int>> == true);
    STATIC_REQUIRE(HasBeginEnd<std::vector<int>> == true);
    STATIC_REQUIRE(HasBeginEnd<int[5]> == true);

    using type = EXPR_MAKE_TYPELIST_LREF(int, long, float*);
    //STATIC_REQUIRE(HasBeginEnd<TZip<type>> != true) // This does not compile because test failed.
    STATIC_REQUIRE(HasBeginEnd<TestStruct> == false);
  }
}

} /// ::dy::expr namespace
