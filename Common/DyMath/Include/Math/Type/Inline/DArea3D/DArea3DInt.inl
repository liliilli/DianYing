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

template <typename TType>
DArea3D<TType, std::enable_if_t<kIsIntegerType<TType>>>
::DArea3D(const DVector3<TValueType>& iStartPoint, const DVector3<TValueType>& iLength)
  : mStartPoint{iStartPoint},
    mLength{iLength}
{ 
  if constexpr (kCategoryOf<TType> == EValueCategory::Signed)
  {
    this->RelocatePosition();
  }
}

template <typename TType>
DArea3D<TType, std::enable_if_t<kIsIntegerType<TType>>>
::DArea3D(
  TValueType x, TValueType y, TValueType z, 
  TValueType width, TValueType height, TValueType depth)
  : mStartPoint{x, y, z},
    mLength{width, height, depth}
{
  if constexpr (kCategoryOf<TType> == EValueCategory::Signed)
  {
    this->RelocatePosition();
  }
}

template <typename TType>
template <typename TAnotherType>
DArea3D<TType, std::enable_if_t<kIsIntegerType<TType>>>
::operator DArea3D<TAnotherType>() const noexcept
{
  using AnotherType = typename DVector3<
    TAnotherType, 
    std::enable_if_t<kIsIntegerType<TAnotherType> || kIsRealType<TAnotherType>>>::TValueType; 

  using TVectorType = DVector3<AnotherType>;
  return DArea3D
  {
    Cast<TVectorType>(this->mStartPoint), 
    Cast<TVectorType>(this->mLength)
  };
}

template <typename TType>
TReal DArea3D<TType, std::enable_if_t<kIsIntegerType<TType>>>
::GetArea() const noexcept
{
  const auto& value = this->mLength;
  return static_cast<TReal>(value[0] * value[1] * value[2]);
}

template <typename TType>
void DArea3D<TType, std::enable_if_t<kIsIntegerType<TType>>>
::SetStartPoint(const DVector3<TValueType>& iStartPoint)
{
  this->mStartPoint = iStartPoint;
}

template <typename TType>
const DVector3<typename DArea3D<TType, std::enable_if_t<kIsIntegerType<TType>>>::TValueType>& 
DArea3D<TType, typename std::enable_if<kIsIntegerType<TType>>::type>
::GetStartPoint() const noexcept
{
  return this->mStartPoint;
}

template <typename TType>
DVector3<typename DArea3D<TType, std::enable_if_t<kIsIntegerType<TType>>>::TValueType> 
DArea3D<TType, typename std::enable_if<kIsIntegerType<TType>>::type>
::GetEndPoint() const noexcept
{
  return this->mStartPoint + this->mLength;
}

template <typename TType>
void DArea3D<TType, std::enable_if_t<kIsIntegerType<TType>>>
::SetWidth(TValueType width) noexcept
{
  this->mLength.X = width;
  if constexpr (kCategoryOf<TType> == EValueCategory::Signed)
  {
    this->RelocatePosition();
  }
}

template <typename TType>
typename DArea3D<TType, std::enable_if_t<kIsIntegerType<TType>>>::TValueType 
DArea3D<TType, typename std::enable_if<kIsIntegerType<TType>>::type>
::GetWidth() const noexcept
{
  return this->mLength.X;
}

template <typename TType>
void DArea3D<TType, std::enable_if_t<kIsIntegerType<TType>>>
::SetHeight(TValueType height) noexcept
{
  this->mLength.Y = height;
  if constexpr (kCategoryOf<TType> == EValueCategory::Signed)
  {
    this->RelocatePosition();
  }
}

template <typename TType>
typename DArea3D<TType, std::enable_if_t<kIsIntegerType<TType>>>::TValueType 
DArea3D<TType, typename std::enable_if<kIsIntegerType<TType>>::type>
::GetHeight() const noexcept
{
  return this->mLength.Y;
}

template <typename TType>
void DArea3D<TType, std::enable_if_t<kIsIntegerType<TType>>>
::SetDepth(TValueType depth) noexcept
{
  this->mLength.Z = depth;
  if constexpr (kCategoryOf<TType> == EValueCategory::Signed)
  {
    this->RelocatePosition();
  }
}

template <typename TType>
typename DArea3D<TType, std::enable_if_t<kIsIntegerType<TType>>>::TValueType 
DArea3D<TType, typename std::enable_if<kIsIntegerType<TType>>::type>
::GetDepth() const noexcept
{
  return this->mLength.Z;
}

template<typename TType>
void DArea3D<TType, std::enable_if_t<kIsIntegerType<TType>>>
::SetWhd(const DVector3<TValueType>& whd)
{
  this->mLength = whd;
  if constexpr (kCategoryOf<TType> == EValueCategory::Signed)
  {
    this->RelocatePosition();
  }
}

template<typename TType>
DVector3<TType> DArea3D<TType, std::enable_if_t<kIsIntegerType<TType>>>
::GetWhd() const noexcept
{
  return this->mLength;
}

template <typename TType>
void DArea3D<TType, std::enable_if_t<kIsIntegerType<TType>>>
::RelocatePosition()
{
  // When width is negative value.
  if (this->mLength.X < 0) 
  { 
    this->mStartPoint.X += this->mLength.X; 
    this->mLength.X = std::abs(this->mLength.X); 
  }
  // When height is negative value.
  if (this->mLength.Y < 0) 
  { 
    this->mStartPoint.Y += this->mLength.Y; 
    this->mLength.Y = std::abs(this->mLength.Y); 
  }
  if (this->mLength.Z < 0) 
  { 
    this->mStartPoint.Z += this->mLength.X; 
    this->mLength.Z = std::abs(this->mLength.Z); 
  }
}

} /// ::dy::math namespace