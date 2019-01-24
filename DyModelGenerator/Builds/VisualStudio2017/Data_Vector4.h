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
#include "Data_Vector2.h"
#include "Data_Vector3.h"

/// @struct DDyVector4
/// @brief Float type 4-element vector struct.
struct DDyVector4 final
{
  float X = 0.f;
  float Y = 0.f;
  float Z = 0.f;
  float W = 1.f;

  DDyVector4() = default;
  DDyVector4(const DDyVector4&) = default;
  DDyVector4& operator=(const DDyVector4&) = default;

  explicit DDyVector4(const float value) noexcept : X{value}, Y{value}, Z{value}, W{value} {};
  explicit DDyVector4(const DDyVector2& value) noexcept : X{value.X}, Y{value.Y}, Z{0.0f}, W{1.0f} {};
  explicit DDyVector4(const DDyVector3& value) noexcept : X{value.X}, Y{value.Y}, Z{value.Z}, W{1.0f} {};
  DDyVector4(const float x, const float y, const float z) noexcept : X(x), Y(y), Z(z), W{1.0f} {};
  DDyVector4(const float x, const float y, const float z, const float w) noexcept : X(x), Y(y), Z(z), W{w} {};
  DDyVector4(const DDyVector3& iXyz, const float w) : X{iXyz.X}, Y{iXyz.Y}, Z{iXyz.Z}, W{w} {};

  //!
  //! Constructor and assign operator for dependencies.
  //!

  DDyVector4(const aiVector2D& value) noexcept : X{value.x}, Y{value.y} {}
  DDyVector4(const aiVector3D& value) noexcept : X{value.x}, Y{value.y}, Z{value.z} {}
  DDyVector4(const std::array<float, 4>& value) noexcept : X{value[0]}, Y{value[1]}, Z{value[2]}, W{value[3]} {}

  DDyVector4& operator=(const aiVector2D& value) noexcept
  {
    this->X = value.x;
    this->Y = value.y;
    this->Z = 0.0f;
    this->W = 1.0f;
    return *this;
  }

  DDyVector4& operator=(const aiVector3D& value) noexcept
  {
    this->X = value.x;
    this->Y = value.y;
    this->Z = value.z;
    this->W = 1.0f;
    return *this;
  }

  auto& operator[](std::size_t index)
  {
    switch (index)
    {
    case 0: return this->X;
    case 1: return this->Y;
    case 2: return this->Z;
    case 3: return this->W;
    default: throw std::out_of_range("DDyVector2 range is out of bound.");
    }
  }

  const auto& operator[](std::size_t index) const
  {
    switch (index) {
    case 0: return this->X;
    case 1: return this->Y;
    case 2: return this->Z;
    case 3: return this->W;
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

  explicit operator aiVector3D() const noexcept
  {
    return aiVector3D{this->X, this->Y, this->Z};
  }

  //!
  //! Methods
  //!

  ///
  /// @brief
  /// Return sequence data of this instance.
  ///
  [[nodiscard]] std::array<float, 4> Data() const noexcept
  {
    return {this->X, this->Y, this->Z, this->W};
  }

  //!
  //! Operators
  //!

  friend DDyVector4 operator+(DDyVector4 lhs, const DDyVector4& rhs) noexcept
  {
    lhs.X += rhs.X;
    lhs.Y += rhs.Y;
    lhs.Z += rhs.Z;
    lhs.W += rhs.W;
    return lhs;
  }

  friend DDyVector4 operator-(DDyVector4 lhs, const DDyVector4& rhs) noexcept
  {
    lhs.X -= rhs.X;
    lhs.Y -= rhs.Y;
    lhs.Z -= rhs.Z;
    lhs.W -= rhs.W;
    return lhs;
  }

  ///
  /// DDyVector4 $$ v = (x, y, z) $$ and value $$ a $$
  /// $$ av $$.
  ///
  friend DDyVector4 operator*(DDyVector4 lhs, const float rhs) noexcept
  {
    lhs.X *= rhs;
    lhs.Y *= rhs;
    lhs.Z *= rhs;
    lhs.W *= rhs;
    return lhs;
  }

  ///
  /// If lhs and rhs are DDyVector4, element multiplication happens.
  ///
  friend DDyVector4 operator*(DDyVector4 lhs, const DDyVector4& rhs) noexcept
  {
    lhs.X *= rhs.X;
    lhs.Y *= rhs.Y;
    lhs.Z *= rhs.Z;
    lhs.W *= rhs.W;
    return lhs;
  }

  ///
  /// If rhs has 0 value, this function just do nothing.
  ///
  friend DDyVector4 operator/(DDyVector4 lhs, const float rhs) noexcept
  {
    lhs.X /= rhs;
    lhs.Y /= rhs;
    lhs.Z /= rhs;
    lhs.W /= rhs;

    return lhs;
  }

  ///
  /// If rhs vector has any 0 value, this function just do nothing.
  ///
  friend DDyVector4 operator/(DDyVector4 lhs, const DDyVector4& rhs) noexcept
  {
    lhs.X /= rhs.X;
    lhs.Y /= rhs.Y;
    lhs.Z /= rhs.Z;
    lhs.W /= rhs.W;

    return lhs;
  }

  DDyVector4& operator+=(const DDyVector4& value) noexcept
  {
    this->X += value.X;
    this->Y += value.Y;
    this->Z += value.Z;
    this->W += value.W;
    return *this;
  }

  DDyVector4& operator-=(const DDyVector4& value) noexcept
  {
    this->X -= value.X;
    this->Y -= value.Y;
    this->Z -= value.Z;
    this->W -= value.W;
    return *this;
  }

  DDyVector4& operator*=(const float value) noexcept
  {
    this->X *= value;
    this->Y *= value;
    this->Z *= value;
    this->W *= value;
    return *this;
  }

  DDyVector4& operator*=(const DDyVector4& value) noexcept
  {
    this->X *= value.X;
    this->Y *= value.Y;
    this->Z *= value.Z;
    this->W *= value.W;
    return *this;
  }

  ///
  /// If lhs and rhs are DDyVector4, element multiplication happens.
  ///
  DDyVector4& operator/=(const float value) noexcept
  {
    this->X /= value;
    this->Y /= value;
    this->Z /= value;
    this->W /= value;

    return *this;
  }

  ///
  /// If rhs vector has any 0 value, this function just do nothing.
  ///
  DDyVector4& operator/=(const DDyVector4& value) noexcept
  {
    this->X /= value.X;
    this->Y /= value.Y;
    this->Z /= value.Z;
    this->W /= value.W;

    return *this;
  }

  friend bool operator==( const DDyVector4& lhs,  const DDyVector4& rhs) noexcept;
  friend bool operator!=( const DDyVector4& lhs,  const DDyVector4& rhs) noexcept;
};

void to_json(nlohmann::json& j, const DDyVector4& p);
void from_json(const nlohmann::json& j, DDyVector4& p);
