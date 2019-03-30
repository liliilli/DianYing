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
  
template <typename TLeft, typename TRight>
DVector2<GetBiggerType<TLeft, TRight>>
ExtractMin(const DVector2<TLeft>& lhs, const DVector2<TRight>& rhs) noexcept
{
  using Type = GetBiggerType<TLeft, TRight>;
  return 
  {
    std::min(Type(lhs.X), Type(rhs.X)),
    std::min(Type(lhs.Y), Type(rhs.Y))
  };
}

template <typename TLeft, typename TRight>
DVector3<GetBiggerType<TLeft, TRight>>
ExtractMin(const DVector3<TLeft>& lhs, const DVector3<TRight>& rhs) noexcept
{
  using Type = GetBiggerType<TLeft, TRight>;
  return 
  {
    std::min(Type(lhs.X), Type(rhs.X)),
    std::min(Type(lhs.Y), Type(rhs.Y)),
    std::min(Type(lhs.Z), Type(rhs.Z)),
  };
}

template <typename TLeft, typename TRight>
DVector4<GetBiggerType<TLeft, TRight>>
ExtractMin(const DVector4<TLeft>& lhs, const DVector4<TRight>& rhs) noexcept
{
  using Type = GetBiggerType<TLeft, TRight>;
  return 
  {
    std::min(Type(lhs.X), Type(rhs.X)),
    std::min(Type(lhs.Y), Type(rhs.Y)),
    std::min(Type(lhs.Z), Type(rhs.Z)),
    std::min(Type(lhs.W), Type(rhs.W)),
  };
}
 
template <typename TLeft, typename TRight>
DVector2<GetBiggerType<TLeft, TRight>>
ExtractMax(const DVector2<TLeft>& lhs, const DVector2<TRight>& rhs) noexcept
{
  using Type = GetBiggerType<TLeft, TRight>;
  return 
  {
    std::max(Type(lhs.X), Type(rhs.X)),
    std::max(Type(lhs.Y), Type(rhs.Y))
  };
}

template <typename TLeft, typename TRight>
DVector3<GetBiggerType<TLeft, TRight>>
ExtractMax(const DVector3<TLeft>& lhs, const DVector3<TRight>& rhs) noexcept
{
  using Type = GetBiggerType<TLeft, TRight>;
  return 
  {
    std::max(Type(lhs.X), Type(rhs.X)),
    std::max(Type(lhs.Y), Type(rhs.Y)),
    std::max(Type(lhs.Z), Type(rhs.Z)),
  };
}

template <typename TLeft, typename TRight>
DVector4<GetBiggerType<TLeft, TRight>>
ExtractMax(const DVector4<TLeft>& lhs, const DVector4<TRight>& rhs) noexcept
{
  using Type = GetBiggerType<TLeft, TRight>;
  return 
  {
    std::max(Type(lhs.X), Type(rhs.X)),
    std::max(Type(lhs.Y), Type(rhs.Y)),
    std::max(Type(lhs.Z), Type(rhs.Z)),
    std::max(Type(lhs.W), Type(rhs.W)),
  };
}

} /// ::dy::math namespace