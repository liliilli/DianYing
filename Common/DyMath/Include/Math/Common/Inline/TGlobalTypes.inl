#pragma once
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
#include <Math/Common/TGlobalTypes.h>

namespace dy::math
{

/// @enum EValueCategory
/// @brief Value category of actual types.
enum class EValueCategory
{
  Character = 0,
  Signed = 1,
  Unsigned = 2,
  Real = 3
};

/// @struct TValueCategory
/// @brief Value category type.
template <typename TType> struct TValueCategory;

}
#include <Math/Common/Inline/TGlobalTypes/TValueCategory.inl>

namespace dy::math
{

/// @brief These types are Character types.
using TCharacterTypes = EXPR_MAKE_TYPELIST(
  TValueCategory<TChr8>, 
  TValueCategory<TChr16>, 
  TValueCategory<TChr32>, 
  TValueCategory<TWChr>);

/// @brief These types are Signed integer types.
using TSignedTypes = EXPR_MAKE_TYPELIST(
  TValueCategory<TI8>,
  TValueCategory<TI16>,
  TValueCategory<TI32>,
  TValueCategory<TI64>);

/// @brief These types are Unsigned integer types.
using TUnsignedTypes = EXPR_MAKE_TYPELIST(
  TValueCategory<TU8>,
  TValueCategory<TU16>,
  TValueCategory<TU32>,
  TValueCategory<TU64>);

/// @brief These types are Real types.
using TRealTypes = EXPR_MAKE_TYPELIST(
  TValueCategory<TF32>,
  TValueCategory<TF64>);

}
#include <Math/Common/Inline/TGlobalTypes/TTypeListOf.inl>

namespace dy::math
{

/// @brief Get proper type list from given category value in compile time.
/// @tparam ECategory Category value.
template <EValueCategory ECategory>
using TTypeListOf = typename TTypeListOf_T<ECategory>::Type;

/// @brief Get category of given Type.
/// @tparam TType Actual arithmetic type.
template <typename TType>
constexpr EValueCategory kCategoryOf = TValueCategory<TType>::kValue;

/// @brief Get type's byte size.
/// @tparam TType Actual arithmetic type.
template <typename TType>
constexpr size_t kTypeSizeOf = TValueCategory<TType>::kTypeSize;

/// @brief Check given type is integer type (Signed + Unsigned)
/// @tparam TType Actual arithmetic type.
template <typename TType>
constexpr bool kIsIntegerType = 
     kCategoryOf<TType> == EValueCategory::Signed
  || kCategoryOf<TType> == EValueCategory::Unsigned;

/// @brief Check given type is real type.
/// @tparam TType Actual arithmetic type.
template <typename TType>
constexpr bool kIsRealType = kCategoryOf<TType> == EValueCategory::Real;

} /// ::dy::math namespace