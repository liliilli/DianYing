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

#include <Math/Common/Inline/TGlobalTypes.inl>
#include <Math/Type/Math/DVector3.h>

namespace dy::math
{

/// @struct DArea3D
/// @brief 2-element area struct type that has start point and width and height as length.
template <typename TType, typename TExpression = void> 
struct DArea3D;

template <typename TType>
struct DArea3D<TType, std::enable_if_t<kIsIntegerType<TType>>>;

template <typename TType>
struct DArea3D<TType, std::enable_if_t<kIsRealType<TType>>>;

} /// ::dy::math namespace

namespace dy::math
{

template <typename TType>
struct MATH_NODISCARD DArea3D<TType, std::enable_if_t<kIsIntegerType<TType>>> final
{
public:
  using TValueType  = TType;

  DArea3D() = default;
  DArea3D(const DVector3<TValueType>& iStartPoint, const DVector3<TValueType>& iLength);
  DArea3D(
    TValueType x, TValueType y, TValueType z, 
    TValueType width, TValueType height, TValueType depth);

  template <typename TAnotherType>
  explicit operator DArea3D<TAnotherType>() const noexcept;

  /// @brief Get area of instance.
  TReal GetArea() const noexcept;

  /// @brief Set start point.
  void SetStartPoint(const DVector3<TValueType>& iStartPoint);

  /// @brief Get start point.
  const DVector3<TValueType>& GetStartPoint() const noexcept;
  /// @brief Get end point from this instance.
  DVector3<TValueType> GetEndPoint() const noexcept;

  /// @brief Set width. 
  /// If width is negative value, start point will be moved.
  void SetWidth(TValueType width) noexcept;
  /// @brief Get width value. 
  TValueType GetWidth() const noexcept;

  /// @brief Set height. 
  /// If height is negative value, start point will be moved.
  void SetHeight(TValueType height) noexcept;
  /// @brief Get height value.
  TValueType GetHeight() const noexcept;
  
  /// @brief Set depth. 
  /// If depth is negative value, start point will be moved.
  void SetDepth(TValueType depth) noexcept;
  /// @brief Get depth value.
  TValueType GetDepth() const noexcept;
  
  /// @brief Set (width, height, depth) as vector3 into area.
  void SetWhd(const DVector3<TValueType>& whd);
  /// @brief Get (width, height, depth) as vector3.
  DVector3<TValueType> GetWhd() const noexcept;

private:
  /// @brief Relocate position to be satisfied width, height and depth will be positive.
  void RelocatePosition();

  DVector3<TValueType> mStartPoint = kMinValueOf<TValueType>;
  DVector3<TValueType> mLength     = kMaxValueOf<TValueType>; // RangeOf may lead to overflow.
};

template <typename TType>
struct MATH_NODISCARD DArea3D<TType, std::enable_if_t<kIsRealType<TType>>> final
{
public:
  using TValueType  = TType;

  DArea3D() = default;
  DArea3D(const DVector3<TValueType>& iStartPoint, const DVector3<TValueType>& iLength);
  DArea3D(
    TValueType x, TValueType y, TValueType z, 
    TValueType width, TValueType height, TValueType depth);

  template <typename TAnotherType>
  explicit operator DArea3D<TAnotherType>() const noexcept;

  /// @brief Get area of instance.
  TReal GetArea() const noexcept;

  /// @brief Set start point.
  void SetStartPoint(const DVector3<TValueType>& iStartPoint);

  /// @brief Get start point.
  const DVector3<TValueType>& GetStartPoint() const noexcept;
  /// @brief Get end point from this instance.
  DVector3<TValueType> GetEndPoint() const noexcept;

  /// @brief Set width. 
  /// If width is negative value, start point will be moved.
  void SetWidth(TValueType width) noexcept;
  /// @brief Get width value. 
  TValueType GetWidth() const noexcept;

  /// @brief Set height. 
  /// If height is negative value, start point will be moved.
  void SetHeight(TValueType height) noexcept;
  /// @brief Get height value.
  TValueType GetHeight() const noexcept;
  
  /// @brief Set depth. 
  /// If depth is negative value, start point will be moved.
  void SetDepth(TValueType depth) noexcept;
  /// @brief Get depth value.
  TValueType GetDepth() const noexcept;
    
  /// @brief Set (width, height, depth) as vector3 into area.
  void SetWhd(const DVector3<TValueType>& whd);
  /// @brief Get (width, height, depth) as vector3.
  DVector3<TValueType> GetWhd() const noexcept;
  
  /// @brief Check value has NaN.
  bool HasNaN() const noexcept;
  /// @brief Check value has Infinity.
  bool HasInfinity() const noexcept;
  /// @brief Check values are normal value, neither NaN nor Inf.
  bool HasOnlyNormal() const noexcept;

private:
  /// @brief Relocate position to be satisfied width, height and depth will be positive.
  void RelocatePosition();

  DVector3<TValueType> mStartPoint = kMinValueOf<TValueType>;
  DVector3<TValueType> mLength     = kMaxValueOf<TValueType>; // RangeOf may lead to overflow.
};

} /// ::dy::math namespace
#include <Math/Type/Inline/DArea3D/DArea3DInt.inl>
#include <Math/Type/Inline/DArea3D/DArea3DReal.inl>