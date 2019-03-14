#ifndef GUARD_DY_HELPER_TYPE_VECTOR3_H
#define GUARD_DY_HELPER_TYPE_VECTOR3_H
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

#include <array>

#include <assimp/vector3.h>
#include <glm/glm.hpp>
#include <nlohmann/json_fwd.hpp>
#include <foundation/PxVec3.h>

#include <Dy/Helper/Type/DVector2.h>

//!
//! Forward declaration
//!

namespace dy
{
class DMatrix3x3;
} /// ::dy namespace

//!
//! Implementation
//! 

namespace dy 
{

/// @struct DVector3
/// @brief Float type 3-element vector struct.
struct DVector3 final 
{
  TF32 X = 0.f, Y = 0.f, Z = 0.f;

  DVector3() = default;

  explicit DVector3(TF32 value) noexcept : X{value}, Y{value}, Z{value} {};
  explicit DVector3(const DVector2& value) noexcept : X{value.X}, Y{value.Y}, Z{0.0f} {};
  DVector3(TF32 x, TF32 y, TF32 z) noexcept : X(x), Y(y), Z(z) {};
  DVector3(const DVector3& value) noexcept = default;

  DVector3(const aiVector3D& value) noexcept : X{value.x}, Y{value.y}, Z{value.z} {}
  DVector3(const glm::vec2& value)  noexcept : X{value.x}, Y{value.y} {};
  DVector3(const glm::vec3& value)  noexcept : X{value.x}, Y{value.y}, Z{value.z} {};
  DVector3(const physx::PxVec3& value) noexcept : X{value.x}, Y{value.y}, Z{value.z} {};

  DVector3& operator=(const aiVector2D& iVector) noexcept;
  DVector3& operator=(const aiVector3D& iVector) noexcept;

  DVector3& operator=(const glm::vec2& value) noexcept
  {
    this->X = value.x;
    this->Y = value.y;
    this->Z = 0.0f;
    return *this;
  }

  DVector3& operator=(const glm::vec3& value) noexcept
  {
    this->X = value.x;
    this->Y = value.y;
    this->Z = value.z;
    return *this;
  }

  DVector3& operator=(const physx::PxVec3& value) noexcept
  {
    this->X = value.x;
    this->Y = value.y;
    this->Z = value.z;
    return *this;
  }

  DVector3& operator=(const DVector3& value) noexcept = default;

  auto& operator[](std::size_t index)
  {
    switch (index)
    {
    case 0: return this->X;
    case 1: return this->Y;
    case 2: return this->Z;
    default: throw std::out_of_range("DVector2 range is out of bound.");
    }
  }

  const auto& operator[](std::size_t index) const
  {
    switch (index)
    {
    case 0: return this->X;
    case 1: return this->Y;
    case 2: return this->Z;
    default: throw std::out_of_range("DVector2 range is out of bound.");
    }
  }

  //!
  //! Conversion operators for dependencies.
  //!

  explicit operator glm::vec2() const noexcept
  {
    return glm::vec2{this->X, this->Y};
  }

  explicit operator aiVector2D() const noexcept
  {
    return aiVector2D{this->X, this->Y};
  }

  operator glm::vec3() const noexcept
  {
    return glm::vec3{this->X, this->Y, this->Z};
  }

  operator aiVector3D() const noexcept
  {
    return aiVector3D{this->X, this->Y, this->Z};
  }

  operator physx::PxVec3() const noexcept
  {
    return physx::PxVec3{this->X, this->Y, this->Z};
  }

#if defined(_WIN32)
  explicit DVector3(const DirectX::XMFLOAT2& value) noexcept : X{value.x}, Y{value.y}, Z{0.0f} {}
  DVector3(const DirectX::XMFLOAT3& value) noexcept : X{value.x}, Y{value.y}, Z{value.z} {}
  DVector3(DirectX::FXMVECTOR value) noexcept
  {
    DirectX::XMFLOAT3 xmVector = {};
    DirectX::XMStoreFloat3(&xmVector, value);
    this->X = xmVector.x;
    this->Y = xmVector.y;
    this->Z = xmVector.z;
  }

  DVector3& operator=(const DirectX::XMFLOAT2& value) noexcept
  {
    this->X = value.x;
    this->Y = value.y;
    this->Z = 0.0f;
    return *this;
  }

  DVector3& operator=(const DirectX::XMFLOAT3& value) noexcept
  {
    this->X = value.x;
    this->Y = value.y;
    this->Z = value.z;
    return *this;
  }

  explicit operator DirectX::XMFLOAT2() const noexcept
  {
    return DirectX::XMFLOAT2{this->X, this->Y};
  }

  operator DirectX::XMFLOAT3() const noexcept
  {
    return DirectX::XMFLOAT3{this->X, this->Y, this->Z};
  }

  operator DirectX::XMVECTOR() const noexcept
  {
    auto xmVector = static_cast<DirectX::XMFLOAT3>(*this);
    return XMLoadFloat3(&xmVector);
  }
#endif /// End defined(_WIN32)

  //!
  //! Methods
  //!

  /// @brief Return sequence data of this instance.
  MDY_NODISCARD TF32* Data() noexcept;

  /// @brief Return sequence data of this instance.
  MDY_NODISCARD const TF32* Data() const noexcept;

  ///
  /// @brief Returns the length of this vector.
  /// @return Length of this DVector3.
  ///
  [[nodiscard]] FORCEINLINE float GetLength() const noexcept
  {
    return std::sqrtf(GetSquareLength());
  }

  ///
  /// @brief Return squared length of this vector.
  /// @return Squared length of this DVector3.
  ///
  [[nodiscard]] FORCEINLINE float GetSquareLength() const noexcept
  {
    return this->X * this->X + this->Y * this->Y + this->Z * this->Z;
  }

  ///
  /// @brief Return new DVector3 instance of normalized input vector.
  /// @return Normalized DVector3 vector.
  ///
  [[nodiscard]] FORCEINLINE DVector3 Normalize() const noexcept
  {
    const auto length = this->GetLength();
    return {this->X / length, this->Y / length, this->Z / length};
  }

  //!
  //! Operators
  //!

  friend DVector3 operator+(DVector3 lhs, const DVector3& rhs) noexcept
  {
    lhs.X += rhs.X;
    lhs.Y += rhs.Y;
    lhs.Z += rhs.Z;
    return lhs;
  }

  friend DVector3 operator-(DVector3 lhs, const DVector3& rhs) noexcept
  {
    lhs.X -= rhs.X;
    lhs.Y -= rhs.Y;
    lhs.Z -= rhs.Z;
    return lhs;
  }

  ///
  /// DVector3 $$ v = (x, y, z) $$ and value $$ a $$
  /// $$ av $$.
  ///
  friend DVector3 operator*(DVector3 lhs, const float rhs) noexcept
  {
    lhs.X *= rhs;
    lhs.Y *= rhs;
    lhs.Z *= rhs;
    return lhs;
  }

  ///
  /// If lhs and rhs are DVector3, element multiplication happens.
  ///
  friend DVector3 operator*(DVector3 lhs, const DVector3& rhs) noexcept
  {
    lhs.X *= rhs.X;
    lhs.Y *= rhs.Y;
    lhs.Z *= rhs.Z;
    return lhs;
  }

  ///
  /// If rhs has 0 value, this function just do nothing.
  ///
  friend DVector3 operator/(DVector3 lhs, const float rhs) noexcept
  {
    if (rhs == 0.0f)
    {
      DyPushLogDebugCritical("DVector3 could not be divided by {0}.", rhs);
    }
    else
    {
      lhs.X /= rhs;
      lhs.Y /= rhs;
      lhs.Z /= rhs;
    }

    return lhs;
  }

  ///
  /// If rhs vector has any 0 value, this function just do nothing.
  ///
  friend DVector3 operator/(DVector3 lhs, const DVector3& rhs) noexcept
  {
    if (rhs.X == 0.0f || rhs.Y == 0.0f || rhs.Z == 0.0f)
    {
      DyPushLogDebugCritical("DVector3 could not be devided by 0 included DVector3, ({0}, {1}, {2})", rhs.X, rhs.Y, rhs.Z);
    }
    else
    {
      lhs.X /= rhs.X;
      lhs.Y /= rhs.Y;
      lhs.Z /= rhs.Z;
    }

    return lhs;
  }

  DVector3& operator+=(const DVector3& value) noexcept
  {
    this->X += value.X;
    this->Y += value.Y;
    this->Z += value.Z;
    return *this;
  }

  DVector3& operator-=(const DVector3& value) noexcept
  {
    this->X -= value.X;
    this->Y -= value.Y;
    this->Z -= value.Z;
    return *this;
  }

  DVector3& operator*=(const float value) noexcept
  {
    this->X *= value;
    this->Y *= value;
    this->Z *= value;
    return *this;
  }

  DVector3& operator*=(const DVector3& value) noexcept
  {
    this->X *= value.X;
    this->Y *= value.Y;
    this->Z *= value.Z;
    return *this;
  }

  ///
  /// If lhs and rhs are DVector3, element multiplication happens.
  ///
  DVector3& operator/=(const float value) noexcept
  {
    if (value == 0.0f)
    {
      DyPushLogDebugCritical("DVector3 could not be divided by {0}.", value);
    }
    else
    {
      this->X /= value;
      this->Y /= value;
      this->Z /= value;
    }

    return *this;
  }

  ///
  /// If rhs vector has any 0 value, this function just do nothing.
  ///
  DVector3& operator/=(const DVector3& value) noexcept
  {
    if (value.X == 0.0f || value.Y == 0.0f || value.Z == 0.0f)
    {
      DyPushLogDebugCritical("DVector3 could not be devided by 0 included DVector3, ({0}, {1}, {2})", value.X, value.Y, value.Z);
    }
    else
    {
      this->X /= value.X;
      this->Y /= value.Y;
      this->Z /= value.Z;
    }

    return *this;
  }

  ///
  /// @brief Multiply with DMatrix3x3 as (v^T * M)
  ///
  DVector3 MultiplyMatrix(const dy::DMatrix3x3& matrix) const noexcept;

  friend bool operator<(const DVector3& lhs, const DVector3& rhs) noexcept
  {
    return lhs.GetSquareLength() < rhs.GetSquareLength();
  }

  friend bool operator>(const DVector3& lhs, const DVector3& rhs) noexcept
  {
    return rhs < lhs;
  }

  friend bool operator<=(const DVector3& lhs, const DVector3& rhs) noexcept
  {
    return !(rhs < lhs);
  }

  friend bool operator>=(const DVector3& lhs, const DVector3& rhs) noexcept
  {
    return !(lhs < rhs);
  }

  friend bool operator==(_MIN_ const DVector3& lhs, _MIN_ const DVector3& rhs) noexcept;
  friend bool operator!=(_MIN_ const DVector3& lhs, _MIN_ const DVector3& rhs) noexcept;

  //!
  //! Static functions
  //!

  ///
  /// @brief Do dot product of (x, y, z) R^3 vector.
  /// @return Dot product float value.
  ///
  [[nodiscard]] static FORCEINLINE float Dot(const DVector3& lhs, const DVector3& rhs) noexcept
  {
    return lhs.X * rhs.X + lhs.Y * rhs.Y + lhs.Z * rhs.Z;
  }

  ///
  /// @brief Cross product of (x, y, z) R^3 vector.
  /// @return Cross product float value.
  ///
  [[nodiscard]] static FORCEINLINE DVector3 Cross(const DVector3& lhs, const DVector3& rhs) noexcept
  {
    return
    {
        lhs.Y * rhs.Z - rhs.Y * lhs.Z,
        lhs.Z * rhs.X - rhs.Z * lhs.X,
        lhs.X * rhs.Y - rhs.X * lhs.Y
    };
  }

  ///
  /// @brief
  /// @param[in] lhs
  /// @param[in] rhs
  /// @param[in] value
  /// @return
  ///
  [[nodiscard]] static FORCEINLINE DVector3 Lerp(const DVector3& lhs, const DVector3& rhs, float value) noexcept
  {
    return lhs * (1.0f - value) + rhs * value;
  }

  ///
  /// @brief
  /// @param[in] lhs
  /// @param[in] rhs
  /// @return
  ///
  [[nodiscard]] static FORCEINLINE DVector3 CompareMaxLength(const DVector3& lhs, const DVector3& rhs) noexcept
  {
    return (lhs >= rhs) ? lhs : rhs;
  }

  ///
  /// @brief
  /// @param[in] lhs
  /// @param[in] rhs
  /// @return
  ///
  [[nodiscard]] static FORCEINLINE DVector3 CompareMinLength(const DVector3& lhs, const DVector3& rhs) noexcept
  {
    return (lhs < rhs) ? lhs : rhs;
  }

  ///
  /// @brief Check if vector DVector3 is all zero or nearly equal to zero.
  ///
  [[nodiscard]] static bool IsAllZero(const DVector3& vector) noexcept;

  ///
  /// @brief Return {0, 0, 1} front DVector3 vector.
  ///
  static DVector3 FrontZ() noexcept
  {
    static DVector3 ret{0, 0, 1};
    return ret;
  }

  ///
  /// @brief Return {1, 0, 0} right DVector3 vector.
  ///
  static DVector3 RightX() noexcept
  {
    static DVector3 ret{1, 0, 0};
    return ret;
  }

  ///
  /// @brief Return {0, 1, 0} up DVector3 vector.
  ///
  static DVector3 UpY() noexcept
  {
    static DVector3 ret{0, 1, 0};
    return ret;
  }

  ///
  /// @brief Check if this DVector3 is all zero or nearly equal to zero.
  ///
  [[nodiscard]] bool IsAllZero() const noexcept;
};

void to_json(_MINOUT_ nlohmann::json& j, _MIN_ const DVector3& p);
void from_json(_MIN_ const nlohmann::json& j, _MOUT_ DVector3& p);

} /// ::dy namespace

#endif /// GUARD_DY_HELPER_TYPE_VECTOR3_H
