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

template <typename TLeft, typename TRight, typename = void>
struct GetBiggerType_T;

template <typename TLeft, typename TRight> 
struct GetBiggerType_T<
  TLeft, TRight,
  std::enable_if_t<kCategoryOf<TLeft> == kCategoryOf<TRight>>> final
{
  static constexpr auto kCategory   = kCategoryOf<TLeft>;
  static constexpr auto kLeftSize   = sizeof(TLeft) * 8;
  static constexpr auto kRightSize  = sizeof(TRight) * 8;
  static constexpr auto kBiggerSize = kLeftSize > kRightSize ? kLeftSize : kRightSize;

  using Type = typename GetCoverableTypeOf<kCategory, kBiggerSize>::Type;
};

template <typename TLeftType, typename TRightType>
struct GetBiggerType_T<
  TLeftType, TRightType, 
  std::enable_if_t<
    (kCategoryOf<TLeftType> == EValueCategory::Real)
  ^ (kCategoryOf<TRightType> == EValueCategory::Real)>> final
{
  static constexpr EValueCategory kLeftCategory   = kCategoryOf<TLeftType>;
  static constexpr EValueCategory kRightCategory  = kCategoryOf<TRightType>;
  static constexpr auto kLeftSize  = sizeof(TLeftType) * 8;
  static constexpr auto kRightSize = sizeof(TRightType) * 8;
  static constexpr auto kBiggerSize = kLeftSize > kRightSize ? kLeftSize : kRightSize;

  using Type = std::conditional_t<
    kLeftCategory == EValueCategory::Real,
    typename GetCoverableTypeOf<kLeftCategory, kBiggerSize>::Type,
    typename GetCoverableTypeOf<kRightCategory, kBiggerSize>::Type>;
};

} /// ::dy::math namespace