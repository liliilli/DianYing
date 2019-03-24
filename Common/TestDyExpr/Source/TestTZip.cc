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

TEST_CASE("TZip helper test", "[TZip]" ) 
{
  SECTION("TZip initialization")
  {
    int   value[5]  = {1, 2, 3, 4, 5};
    float value2[5] = {2.f, 3.f, 5.f, 7.f, 11.f};

    auto zip = Zip(value, value2);
    auto& vvv = zip.GetItem<0>();

    for (auto& item : vvv)
    {
      item += 10;
    }

    const int value3[5] = {11, 12, 13, 14, 15};
    for (std::size_t va = 0; va < 5; ++va)
    {
      REQUIRE(value[va] == value3[va]);
    }

    auto [fff, eee] = zip[0];
    REQUIRE(*fff == 11);
    REQUIRE(*eee == 2.f);
  }

  SECTION("TZip begin, end and ranged for loop")
  {
    int   value[5]  = {11, 12, 13, 14, 15};
    float value2[5] = {2.f, 3.f, 5.f, 7.f, 11.f};
    auto zip = Zip(value, value2);

    std::array<std::pair<int, float>, 5> result = {
      std::pair<int, float>{11, 2.f},
      std::pair<int, float>{12, 3.f},
      std::pair{13, 5.f},
      std::pair{14, 7.f},
      std::pair{15, 11.f},
    };

    int i = 0;
    for (auto it = zip.begin(); it != zip.end(); ++it)
    {
      const auto [lhs, rhs] = *it;
      REQUIRE(*lhs == result[i].first);
      REQUIRE(*rhs == result[i].second);
      ++i;
    }

    i = 0;
    for (auto& [lhs, rhs] : zip)
    {
      REQUIRE(*lhs == result[i].first);
      REQUIRE(*rhs == result[i].second);
      ++i;
    }
  }

  
  SECTION("TZip begin, end and ranged for loop")
  {
    int   value[5]        = {11, 12, 13, 14, 15};
    float value2[5]       = {2.f, 3.f, 5.f, 7.f, 11.f};
    std::vector<std::string> value3 = {"Hello", "World", "My", "Name", "Neu."};

    auto zip = Zip(value, value2, value3);

    std::array<std::tuple<int, float, std::string>, 5> result = {
      std::tuple{11, 2.f, "Hello"},
      {12, 3.f, "World"},
      {13, 5.f, "My"},
      {14, 7.f, "Name"},
      {15, 11.f, "Neu."},
    };

    int i = 0;
    for (auto it = zip.cbegin(); it != zip.cend(); ++it)
    {
      auto& [lhs, rhs, str] = *it;
      REQUIRE(*lhs == std::get<0>(result[i]));
      REQUIRE(*rhs == std::get<1>(result[i]));
      REQUIRE(*str == std::get<2>(result[i]));
      ++i;
    }

    i = 0;
    for (const auto& [lhs, rhs, str] : Zip(value, value2, value3))
    {
      REQUIRE(*lhs == std::get<0>(result[i]));
      REQUIRE(*rhs == std::get<1>(result[i]));
      REQUIRE(*str == std::get<2>(result[i]));
      ++i;
    }
  }
}

} /// ::dy::expr namespace
