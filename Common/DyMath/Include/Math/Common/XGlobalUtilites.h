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

namespace dy::math
{

/// @brief Convert TSourceType value to TTargetType.
/// This is wrapping function of static_cast<> for readability.
template <typename TTargetType, typename TSourceType>
TTargetType Cast(TSourceType iValue);

} /// ::dy::math namespace
#include <Math/Common/Inline/XGlobalUtilities/XGlobalUtilities.inl>

#include <Math/Common/Inline/XGlobalUtilities/GetCoverableTypeOf.inl>
namespace dy::math
{

/// @brief Get proper type that covers given bit range with category.
///
/// If found, it returns wrapped type as TValueCategory<Returned>,
/// that can give actual type of wrapped type as TValueCategory<Returned>::Type.
///
/// If not found, it just returns expr::TNull as failure type.
///
/// @tparam ECategory Category enumeration value.
/// @tparam VBitSize  Bit size to find proper type to cover it.
template <EValueCategory ECategory, std::size_t VBitSize>
using GetCoverableTypeOf = typename GetCoverableTypeOf_T<ECategory, VBitSize>::Type;

} /// ::dy::math namespace

#include <Math/Common/Inline/XGlobalUtilities/GetBiggerType.inl>
namespace dy::math
{

/// @brief Get big type by comparing given two arithmetic types.
///
/// If given types have same category, returned type also will be same category,
/// but have bigger size type comparing a pair of type.
///
/// But if given types does not hve same category but one type is Real,
/// Real type that have coverable size type will be returned.
///
/// @tparam TLeft Actual type.
/// @tparam TRight Actual type.
template <typename TLeft, typename TRight>
using GetBiggerType = typename GetBiggerType_T<TLeft, TRight>::Type;

} /// ::dy::math namespace

#include <Math/Common/Inline/XGlobalUtilities/GetRealTypeOf.inl>
namespace dy::math
{

/// @brief Get real type that covers given TType bits.
/// If not covered, abort compile as failure error.
///
/// @tparam TType Input type.
template <typename TType>
using GetRealTypeOf = typename GetRealTypeOf_T<TType>::Type::Type;

} /// ::dy::math namespace