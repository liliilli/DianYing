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

#include <Math/Common/XGlobalMacroes.h>
#include <Math/Type/Math/DVector3.h>

namespace dy::math
{

/// @enum EPosPlaneState
/// @brief Point's state for comparing status of position with plane.
enum class EPosPlaneState
{
  FrontNormal,  // Point is in front of plane normal.
  BehindNormal, // Point is bahind of plane normal.
  OnPlane       // Point is on plane.
};

/// @struct DPlane
/// @brief Plane type.
/// Plane type just can only store real number type. \n
///
///          . <= Normal (A, B, C) \n
///     /----|----\ \n
///    /     |     \ \n
///   /      '      \ \n
///  /---------------\ <= Plane \n
/// \n
template <typename TType>
struct MATH_NODISCARD DPlane final
{
  static_assert(kIsRealType<TType> == true, "DPlane only supports real type.");
  using TValueType = TType;

  /// @brief a, b, c will be normalized values as |a, b, c| == 1
  DPlane() = default;
  DPlane(TValueType a, TValueType b, TValueType c, TValueType d);
  DPlane(const DVector3<TValueType>& normalizedVector, TValueType d);

  /// @brief Set new normal vector to plane.
  /// Vector must be normalized when input.
  void SetNormalVector(const DVector3<TValueType>& iNormalVector);

  /// @brief Get normal vector.
  /// Returned normal vector does not guarantees that length is 1.
  DVector3<TValueType> GetNormalVector() const noexcept;

  /// @brief Check given point is on plane, on front (pos), on behind (neg).
  EPosPlaneState CheckPointStatusOnPlane(const DVector3<TValueType>& iPoint) const noexcept;
  
  /// @brief Get given point's signed distance from plane.
  TReal GetSignedDistanceFrom(const DVector3<TType>& iPoint) const noexcept;

  /// @brief Get given point's signed distance from plane.
  TReal GetUnsignedDistanceFrom(const DVector3<TType>& iPoint) const noexcept;

  /// @brief Get closest on-plane point of given point.
  DVector3<TValueType> GetClosestPoint(const DVector3<TValueType>& iPoint) const noexcept;

private:
  /// @brief Normal value.
  DVector3<TValueType> mNormal = DVector3<TValueType>::UnitY(); 
  /// @brief D value.
  TValueType mD;
};

} /// ::dy::math namespace
#include <Math/Type/Inline/DPlane/DPlane.inl>
