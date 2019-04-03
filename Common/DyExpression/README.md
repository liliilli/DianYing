## Dy(DianYing) Expression Library

Expression Library is a tool library for template & Compile time common helper APIs. It allows a user or a developer of `Dy` Series to use compile time expression codes.

`Expr` Called Expression library uses C++1z (C++17). `Expr` is header only library, so user can use this library by connecting include path to this.

## Usage

This library is header-only library. Just add the path of include files into your project.

## Features

### TypeList

`Typelist` is for consturcting list that contains types in compile time.
`Typelist` can be created using macro or `STypeList` container's API function.

``` c++
/// Same to STypeList::MakeList<std::size_t*, long, float, void, int>;
EXPR_MAKE_TYPELIST(std::size_t*, long, float, void, int);

/// Same to STypeList::MakeListExt<ETypeRef::LValue, long, float, void, int>;
EXPR_MAKE_TYPELIST_LREF(long, float, void, int);

/// Same to STypeList::MakeListExt<ETypeRef::RValue, long, float, void, int>;
EXPR_MAKE_TYPELIST_RREF(long, float, void, int);
```

Created TypeList type can do these below things.

1. Getting the length of TypeList.
2. Getting specified type using index. 
3. Checking given type is in TypeList.
4. Getting the index of given type when exist in TypeList.
5. Append another type into TypeList.
6. Getting satisfied type using static functor that has one type parameter.

#### Example Code

``` c++
using TestType2 = EXPR_MAKE_TYPELIST(std::size_t*, long, float, void, int);

// Getting the length of TypeList.
static_assert(STypeList::LengthOf<TestType2> == 5, "Test Failed");

// Getting arbitary type using index.
static_assert(std::is_same_v<STypeList::At<2, TestType2>, float>, "Test failed");

// Checking given type is in TypeList.
// Getting the index of given type when exist in TypeList.
static_assert(
  STypeList::GetIndexOf<std::size_t&, TestType2> == std::nullopt, "Test failed");
static_assert(
  STypeList::GetIndexOf<int, TestType2> == 4, "Test failed");

// Append another type into TypeList.
using TestType1 = STypeList::MakeList<void, int>;
static_assert(
  std::is_same_v<
    STypeList::Append<TestType1, int, long>,
    STypeList::MakeList<void, int, int, long>>,
  "Test Failed");
```

``` c++
// Getting satisfied type using static functor that has one type parameter.
namespace dy::expr::typelist::gettypeif
{

template <std::size_t TBit>
struct Bit
{
  static constexpr std::size_t kByte = TBit / 8;
};

template <typename TType>
struct Functor0
{
  static constexpr bool kResult = sizeof(TType) == Bit<32>::kByte;
};

template <typename TType>
struct Functor1
{
  static constexpr size_t div = 15 % 8;
  static constexpr bool kResult = sizeof(TType) > ((15 - div) / 8);
};

using TestType = STypeList::MakeList<char, short, int, long int>;
static_assert(
  std::is_same_v<
    STypeList::GetTypeIf<TestType, Functor0>,
    int>,
  "Test Failed");

static_assert(
  std::is_same_v<
    STypeList::GetTypeIf<TestType, Functor1>,
    short>,
  "Test Failed");
}
```

### TNull

`TNull` is failure type of all compile time processing.

``` c++
struct TNull final {};
```

### TZip

`TZip` provides heterogeneous types binding and iteration of these. All types (static, dynamic) are must be same size(length) to use this API.
If any of size are different to others, there is UB.

``` c++
SECTION("TZip begin, end and ranged for loop")
{
  int   value[5]  = {11, 12, 13, 14, 15};
  float value2[5] = {2.f, 3.f, 5.f, 7.f, 11.f};
  std::vector<std::string> value3 = {"Hello", "World", "My", "Name", "Neu."};

  // Use `Zip`.
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
```

### Compile/Runtime CRC32 Hash

User can create CRC32 Hash value on compile-time or runtime.
If given argument is characters literal or created on compile time, CRC32 Hash function `ToHashCrc32` returns uint32_t value in compile time.
Otherwise, `ToHashCrc32` function call will be deferred into runtime.

``` c++
/// Just use `ToHashCrc32(const char*)` or `ToHashCrc32(std::string_view)` function.
static_assert(ToHashCrc32("Hello world!") == 0x1B851995);
```

### String Switch/Case Helper Function

String can be used in switch/case statement using `string::Input` and `string::Case` function.

`string::Case` function must be processed compile-time when using in `case` statement.
`string::Input` function can be also processed in runtime.

This functions has dependencies on `TCrc32`, creates crc32 hash values.

``` c++
// ..
std::string keyword;
std::cin >> keyword;

switch (string::Input(keyword))
{
	case string::Case("Input_DefaultVao"):       buffer += Buffer_Input_DefaultVao; break;
	case string::Case("Input_UboCamera"):        buffer += Buffer_Input_UboCamera; break;
	case string::Case("Input_SkinAnimation"):    buffer += Buffer_Input_SkinAnimation; break;
	case string::Case("Input_DefaultTexture2D"): buffer += Buffer_Input_DefaultTexture2D; break;
	case string::Case("Input_UboDirLight"):      buffer += Buffer_Input_UboDirLight; break;
	case string::Case("Input_UStrPointLight"):   buffer += Buffer_Input_UStrPointLight; break;
	case string::Case("Output_OpaqueStream"):    buffer += Buffer_Output_OpaqueStream; break;
	case string::Case("Etc_Miscellaneous"):      buffer += Buffer_Etc_Miscellaneous; break;
	default: break;
}
// ..
```

### Extended `type_traits`

Extended type_trait API provides more complicated compile time type cabatility checking.

* `IsIterable` : Check type is iterable. this checks given type is c-array, or have begin(), and() so forward iterable.
* `HasBeginEnd` : Check given type has begin(), and() function.

``` c++
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
  STATIC_REQUIRE(HasBeginEnd<TZip<type>> == true);
  STATIC_REQUIRE(HasBeginEnd<TestStruct> == false);
}
```

### Common MACROes

DyExpr provides helper macroes that used on Dy~ Projects.
To see what macroes are implemented, see `XMacroes.h` file.

### Log

19-03-19 Make Project, Add TypeList.
19-03-25 Add TZip, Extended type_traits.
19-04-01 Add Crc32 Hash value creation function.
19-04-03 Add Common macroes.

## Copyright

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