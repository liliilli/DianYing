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

#include <Expr/TTypeList.h>

namespace dy::expr::typelist::make
{

static_assert(
  std::is_same_v<STypeList::MakeListExt<ETypeRef::None, void>, TTypeListTail<void>>, 
  "Test failed");
static_assert(
  std::is_same_v<EXPR_MAKE_TYPELIST(int), TTypeListTail<int>>, 
  "Test failed");
static_assert(
  std::is_same_v<EXPR_MAKE_TYPELIST(void, int), TTypeList<void, TTypeListTail<int>>>, 
  "Test failed");
static_assert(
  std::is_same_v<
    EXPR_MAKE_TYPELIST(long, void, int), 
    TTypeList<long, TTypeList<void, TTypeListTail<int>>>
  >, 
  "Test failed");

} /// ::dy::expr::typelist::make namespace

namespace dy::expr::typelist::length
{

static_assert(
  STypeList::LengthOf<EXPR_MAKE_TYPELIST(void)> == 1, 
  "Test failed");

static_assert(
  STypeList::LengthOf<EXPR_MAKE_TYPELIST(void, int)> == 2, 
  "Test failed");

static_assert(
  STypeList::LengthOf<EXPR_MAKE_TYPELIST(std::size_t*, long, float, void, int)> == 5, 
  "Test failed");

} /// :: dy::expr::typelist::length namespace

namespace dy::expr::typelist::type
{

static_assert(
  std::is_same_v<
    STypeList::At<0, EXPR_MAKE_TYPELIST(void)>,
    void
  >,
  "Test failed");

static_assert(
  std::is_same_v<
    STypeList::At<1, EXPR_MAKE_TYPELIST(void, int)>,
    int
  >,
  "Test failed");

static_assert(
  std::is_same_v<
    STypeList::At<2, EXPR_MAKE_TYPELIST(std::size_t*, long, float, void, int)>,
    float
  >,
  "Test failed");

} /// ::dy::expr::typelist::type namespace

namespace dy::expr::typelist::hastype
{

using TestType0 = EXPR_MAKE_TYPELIST(void);
using TestType1 = EXPR_MAKE_TYPELIST(void, int);
using TestType2 = EXPR_MAKE_TYPELIST(std::size_t*, long, float, void, int);

static_assert(STypeList::HasType<void, TestType0> == true, "Test failed");
static_assert(STypeList::HasType<int , TestType0> == false, "Test failed");

static_assert(STypeList::HasType<long, TestType1> == false, "Test failed");
static_assert(STypeList::HasType<int , TestType1> == true, "Test failed");

static_assert(STypeList::HasType<long, TestType2> == true, "Test failed");
static_assert(STypeList::HasType<std::size_t*, TestType2> == true, "Test failed");
static_assert(STypeList::HasType<std::size_t&, TestType2> == false, "Test failed");

} /// ::dy::expr::typelist::hastype namespace

namespace dy::expr::typelist::getindex
{

using TestType0 = EXPR_MAKE_TYPELIST(void);
using TestType1 = EXPR_MAKE_TYPELIST(void, int);
using TestType2 = EXPR_MAKE_TYPELIST(std::size_t*, long, float, void, int);

static_assert(STypeList::GetIndexOf<void, TestType0>.has_value() == true, "Test failed");
static_assert(*STypeList::GetIndexOf<void, TestType0> == 0, "Test failed");
static_assert(STypeList::GetIndexOf<int, TestType0>.has_value() == false, "Test failed");

static_assert(STypeList::GetIndexOf<int, TestType1>.value() == 1, "Test failed");
static_assert(STypeList::GetIndexOf<long, TestType1> == std::nullopt, "Test failed");

static_assert(STypeList::GetIndexOf<void, TestType2> == 3, "Test failed");
static_assert(STypeList::GetIndexOf<int, TestType2> == 4, "Test failed");
static_assert(STypeList::GetIndexOf<std::size_t&, TestType2> == std::nullopt, "Test failed");

} /// ::dy::expr::typelist::getindex namespace

namespace dy::expr::typelist::append
{

using TestType0 = STypeList::MakeListExt<ETypeRef::None, void>;
static_assert(
  std::is_same_v<
    STypeList::Append<TestType0, int, long>,
    STypeList::MakeListExt<ETypeRef::None, void, int, long>>,
  "Test Failed");

using TestType1 = STypeList::MakeListExt<ETypeRef::None, void, int>;
static_assert(
  std::is_same_v<
    STypeList::Append<TestType1, int, long>,
    STypeList::MakeListExt<ETypeRef::None, void, int, int, long>>,
  "Test Failed");

} /// ::dy::expr::typelist::append namespace

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

using TestType = STypeList::MakeListExt<ETypeRef::None, char, short, int, long int>;
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

} /// ::dy::expr::typelist::gettypeif namespace


namespace dy::expr::typelist::make_ref
{

static_assert(
  std::is_same_v<STypeList::MakeListExt<ETypeRef::LValue, int, long>, 
  TTypeList<int&, TTypeListTail<long&>>>, 
  "Test failed");
static_assert(
  std::is_same_v<EXPR_MAKE_TYPELIST_LREF(int), TTypeListTail<int&>>, 
  "Test failed");
static_assert(
  std::is_same_v<EXPR_MAKE_TYPELIST(void, int), TTypeList<void, TTypeListTail<int>>>, 
  "Test failed");
static_assert(
  std::is_same_v<
    EXPR_MAKE_TYPELIST_RREF(long, std::array<int, 3>, int), 
    TTypeList<long&&, TTypeList<std::array<int, 3>&&, TTypeListTail<int&&>>>
  >, 
  "Test failed");

} /// ::dy::expr::typelist::make namespace

