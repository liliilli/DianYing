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

#include <array>

#include <assimp/vector3.h>
#include <nlohmann/json_fwd.hpp>

#include "Data_Vector2.h"

///
/// @struct DDyVector3
/// @brief Float type 3-element vector struct.
///
struct DDyVector3 final {
  float X = 0.f;
  float Y = 0.f;
  float Z = 0.f;

  DDyVector3() = default;

  explicit DDyVector3(const float value)        noexcept : X{value}, Y{value}, Z{value} {};
  explicit DDyVector3(const DDyVector2& value)  noexcept : X{value.X}, Y{value.Y}, Z{0.0f} {};
  DDyVector3(const float x, const float y, const float z) noexcept : X(x), Y(y), Z(z) {};
  DDyVector3(const DDyVector3& value)                     noexcept = default;

  //!
  //! Constructor and assign operator for dependencies.
  //!

  DDyVector3(const aiVector2D& value) noexcept : X{value.x}, Y{value.y} {}
  DDyVector3(const aiVector3D& value) noexcept : X{value.x}, Y{value.y}, Z{value.z} {}

  DDyVector3& operator=(const aiVector2D& value) noexcept
  {
    this->X = value.x;
    this->Y = value.y;
    this->Z = 0.0f;
    return *this;
  }

  DDyVector3& operator=(const aiVector3D& value) noexcept
  {
    this->X = value.x;
    this->Y = value.y;
    this->Z = value.z;
    return *this;
  }

  DDyVector3& operator=(const DDyVector3& value) noexcept = default;

  auto& operator[](std::size_t index)
  {
    switch (index)
    {
    case 0: return this->X;
    case 1: return this->Y;
    case 2: return this->Z;
    default: throw std::out_of_range("DDyVector2 range is out of bound.");
    }
  }

  const auto& operator[](std::size_t index) const
  {
    switch (index)
    {
    case 0: return this->X;
    case 1: return this->Y;
    case 2: return this->Z;
    default: throw std::out_of_range("DDyVector2 range is out of bound.");
    }
  }

  //!
  //! Conversion operators for dependencies.
  //!

  explicit operator aiVector2D() const noexcept
  {
    return aiVector2D{this->X, this->Y};
  }

  operator aiVector3D() const noexcept
  {
    return aiVector3D{this->X, this->Y, this->Z};
  }

  //!
  //! Methods
  //!

  ///
  /// @brief Return sequence data of this instance.
  ///
  [[nodiscard]] std::array<float, 3> Data() const noexcept
  {
    return {this->X, this->Y, this->Z};
  }

  ///
  /// @brief Returns the length of this vector.
  /// @return Length of this DDyVector3.
  ///
  [[nodiscard]]  float GetLength() const noexcept
  {
    return std::sqrtf(GetSquareLength());
  }

  ///
  /// @brief Return squared length of this vector.
  /// @return Squared length of this DDyVector3.
  ///
  [[nodiscard]]  float GetSquareLength() const noexcept
  {
    return this->X * this->X + this->Y * this->Y + this->Z * this->Z;
  }

  ///
  /// @brief Return new DDyVector3 instance of normalized input vector.
  /// @return Normalized DDyVector3 vector.
  ///
  [[nodiscard]]  DDyVector3 Normalize() const noexcept
  {
    const auto length = this->GetLength();
    return {this->X / length, this->Y / length, this->Z / length};
  }

  //!
  //! Operators
  //!

  friend DDyVector3 operator+(DDyVector3 lhs, const DDyVector3& rhs) noexcept
  {
    lhs.X += rhs.X;
    lhs.Y += rhs.Y;
    lhs.Z += rhs.Z;
    return lhs;
  }

  friend DDyVector3 operator-(DDyVector3 lhs, const DDyVector3& rhs) noexcept
  {
    lhs.X -= rhs.X;
    lhs.Y -= rhs.Y;
    lhs.Z -= rhs.Z;
    return lhs;
  }

  /// DDyVector3 $$ v = (x, y, z) $$ and value $$ a $$
  /// $$ av $$.
  friend DDyVector3 operator*(DDyVector3 lhs, const float rhs) noexcept
  {
    lhs.X *= rhs;
    lhs.Y *= rhs;
    lhs.Z *= rhs;
    return lhs;
  }

  /// If lhs and rhs are DDyVector3, element multiplication happens.
  friend DDyVector3 operator*(DDyVector3 lhs, const DDyVector3& rhs) noexcept
  {
    lhs.X *= rhs.X;
    lhs.Y *= rhs.Y;
    lhs.Z *= rhs.Z;
    return lhs;
  }

  /// If rhs has 0 value, this function just do nothing.
  friend DDyVector3 operator/(DDyVector3 lhs, const float rhs) noexcept
  {
    lhs.X /= rhs;
    lhs.Y /= rhs;
    lhs.Z /= rhs;
    return lhs;
  }

  /// If rhs vector has any 0 value, this function just do nothing.
  friend DDyVector3 operator/(DDyVector3 lhs, const DDyVector3& rhs) noexcept
  {
    lhs.X /= rhs.X;
    lhs.Y /= rhs.Y;
    lhs.Z /= rhs.Z;
    return lhs;
  }

  DDyVector3& operator+=(const DDyVector3& value) noexcept
  {
    this->X += value.X;
    this->Y += value.Y;
    this->Z += value.Z;
    return *this;
  }

  DDyVector3& operator-=(const DDyVector3& value) noexcept
  {
    this->X -= value.X;
    this->Y -= value.Y;
    this->Z -= value.Z;
    return *this;
  }

  DDyVector3& operator*=(const float value) noexcept
  {
    this->X *= value;
    this->Y *= value;
    this->Z *= value;
    return *this;
  }

  DDyVector3& operator*=(const DDyVector3& value) noexcept
  {
    this->X *= value.X;
    this->Y *= value.Y;
    this->Z *= value.Z;
    return *this;
  }

  /// If lhs and rhs are DDyVector3, element multiplication happens.
  DDyVector3& operator/=(const float value) 
  {
    this->X /= value;
    this->Y /= value;
    this->Z /= value;
    return *this;
  }

  /// If rhs vector has any 0 value, this function just do nothing.
  DDyVector3& operator/=(const DDyVector3& value) 
  {
    this->X /= value.X;
    this->Y /= value.Y;
    this->Z /= value.Z;
    return *this;
  }

  friend bool operator<(const DDyVector3& lhs, const DDyVector3& rhs) noexcept
  {
    return lhs.GetSquareLength() < rhs.GetSquareLength();
  }

  friend bool operator>(const DDyVector3& lhs, const DDyVector3& rhs) noexcept
  {
    return rhs < lhs;
  }

  friend bool operator<=(const DDyVector3& lhs, const DDyVector3& rhs) noexcept
  {
    return !(rhs < lhs);
  }

  friend bool operator>=(const DDyVector3& lhs, const DDyVector3& rhs) noexcept
  {
    return !(lhs < rhs);
  }

  friend bool operator==( const DDyVector3& lhs,  const DDyVector3& rhs) noexcept;
  friend bool operator!=( const DDyVector3& lhs,  const DDyVector3& rhs) noexcept;

  //!
  //! Static functions
  //!

  ///
  /// @brief Do dot product of (x, y, z) R^3 vector.
  /// @return Dot product float value.
  ///
  [[nodiscard]] static  float Dot(const DDyVector3& lhs, const DDyVector3& rhs) noexcept
  {
    return lhs.X * rhs.X + lhs.Y * rhs.Y + lhs.Z * rhs.Z;
  }

  /// @brief Cross product of (x, y, z) R^3 vector.
  /// @return Cross product float value.
  [[nodiscard]] static  DDyVector3 Cross(const DDyVector3& lhs, const DDyVector3& rhs) noexcept
  {
    return
    {
        lhs.Y * rhs.Z - rhs.Y * lhs.Z,
        lhs.Z * rhs.X - rhs.Z * lhs.X,
        lhs.X * rhs.Y - rhs.X * lhs.Y
    };
  }

  /// @brief
  /// @param[in] lhs
  /// @param[in] rhs
  /// @param[in] value
  /// @return
  [[nodiscard]] static  DDyVector3 Lerp(const DDyVector3& lhs, const DDyVector3& rhs, float value) noexcept
  {
    return lhs * (1.0f - value) + rhs * value;
  }

  /// @brief
  /// @param[in] lhs
  /// @param[in] rhs
  /// @return
  [[nodiscard]] static  DDyVector3 CompareMaxLength(const DDyVector3& lhs, const DDyVector3& rhs) noexcept
  {
    return (lhs >= rhs) ? lhs : rhs;
  }

  /// @brief
  /// @param[in] lhs
  /// @param[in] rhs
  /// @return
  [[nodiscard]] static  DDyVector3 CompareMinLength(const DDyVector3& lhs, const DDyVector3& rhs) noexcept
  {
    return (lhs < rhs) ? lhs : rhs;
  }

  /// @brief Return {0, 0, 1} front DDyVector3 vector.
  static DDyVector3 FrontZ() noexcept
  {
    static DDyVector3 ret{0, 0, 1};
    return ret;
  }

  /// @brief Return {1, 0, 0} right DDyVector3 vector.
  static DDyVector3 RightX() noexcept
  {
    static DDyVector3 ret{1, 0, 0};
    return ret;
  }

  /// @brief Return {0, 1, 0} up DDyVector3 vector.
  static DDyVector3 UpY() noexcept
  {
    static DDyVector3 ret{0, 1, 0};
    return ret;
  }
};

void to_json(nlohmann::json& j, const DDyVector3& p);
void from_json(const nlohmann::json& j, DDyVector3& p);
