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

template <typename TTargetType, typename TSourceType>
TTargetType Cast(TSourceType iValue)
{
  return static_cast<TTargetType>(iValue);
}

template <EValueCategory TCategory, size_t BitSize>


template <
  typename TLeftType, 
  typename TRightType,
  typename std::enable_if_t<
      kCategoryOf<TLeftType> != EValueCategory::Character
  &&  kCategoryOf<TRightType> != EValueCategory::Character> = 0
>
struct TBigType
{
  using Type = std::conditional_t<
    kCategoryOf<TLeftType>  
};

} /// ::dy::math namespace