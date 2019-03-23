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

### Log

19-03-19 Make Project, Add TypeList.

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