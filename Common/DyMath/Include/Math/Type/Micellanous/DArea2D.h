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
#include <Math/Type/Math/DVector2.h>

namespace dy::math
{

/// @struct DArea2D
/// @brief 2-element area struct type that has start point and width and height as length.
template <typename TType, typename TExpression = void> 
struct DArea2D;

template <typename TType>
struct DArea2D<TType, std::enable_if_t<kIsIntegerType<TType>>>;

template <typename TType>
struct DArea2D<TType, std::enable_if_t<kIsRealType<TType>>>;

} /// ::dy::math namespace

namespace dy::math
{

template <typename TType>
struct MATH_NODISCARD DArea2D<TType, std::enable_if_t<kIsIntegerType<TType>>> final
{
public:
  using TValueType  = TType;

  DArea2D() = default;
  DArea2D(const DVector2<TValueType>& iStartPoint, const DVector2<TValueType>& iLength);
  DArea2D(TValueType x, TValueType y, TValueType width, TValueType height);

  template <typename TAnotherType>
  explicit operator DArea2D<TAnotherType>() const noexcept;

  /// @brief Get area of instance.
  TReal GetArea() const noexcept;

  /// @brief Set start point.
  void SetStartPoint(const DVector2<TValueType>& iStartPoint);

  /// @brief Get start point.
  const DVector2<TValueType>& GetStartPoint() const noexcept;

  /// @brief Get end point from this instance.
  DVector2<TValueType> GetEndPoint() const noexcept;

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

  /// @brief Set (width, height) as vector2 into area.
  void SetWh(const DVector2<TValueType>& wh);

  /// @brief Get (width, height) as vector2.
  DVector2<TValueType> GetWh() const noexcept;

private:
  /// @brief Relocate position to be satisfied width and heigth will be positive.
  void RelocatePosition();

  DVector2<TValueType> mStartPoint  = kMinValueOf<TValueType>;
  DVector2<TValueType> mLength      = kMaxValueOf<TValueType>; // RangeOf may lead to overflow.
};

template <typename TType>
struct MATH_NODISCARD DArea2D<TType, std::enable_if_t<kIsRealType<TType>>> final
{
public:
  using TValueType  = TType;

  DArea2D() = default;
  DArea2D(const DVector2<TValueType>& iStartPoint, const DVector2<TValueType>& iLength);
  DArea2D(TValueType x, TValueType y, TValueType width, TValueType height);

  template <typename TAnotherType>
  explicit operator DArea2D<TAnotherType>() const noexcept;

  /// @brief Get area of instance.
  TReal GetArea() const noexcept;

  /// @brief Set start point.
  void SetStartPoint(const DVector2<TValueType>& iStartPoint);

  /// @brief Get start point.
  const DVector2<TValueType>& GetStartPoint() const noexcept;

  /// @brief Get end point from this instance.
  DVector2<TValueType> GetEndPoint() const noexcept;

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
  
  /// @brief Set (width, height) as vector2 into area.
  void SetWh(const DVector2<TValueType>& wh);

  /// @brief Get (width, height) as vector2.
  DVector2<TValueType> GetWh() const noexcept;

  /// @brief Check value has NaN.
  bool HasNaN() const noexcept;

  /// @brief Check value has Infinity.
  bool HasInfinity() const noexcept;

  /// @brief Check values are normal value, neither NaN nor Inf.
  bool HasOnlyNormal() const noexcept;

private:
  /// @brief Relocate position to be satisfied width and heigth will be positive.
  void RelocatePosition();

  DVector2<TValueType> mStartPoint  = kMinValueOf<TValueType>;
  DVector2<TValueType> mLength      = kMaxValueOf<TValueType>; // RangeOf may lead to overflow.
};

} /// ::dy::math namespace
#include <Math/Type/Inline/DArea2D/DArea2DInt.inl>
#include <Math/Type/Inline/DArea2D/DArea2DReal.inl>