#ifndef GUARD_DY_HELPER_TYPE_VECTORINT4_H
#define GUARD_DY_HELPER_TYPE_VECTORINT4_H
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
#include <nlohmann/json_fwd.hpp>

#include <Dy/Helper/System/Macroes.h>
#include <Dy/Helper/GlobalType.h>
#include <Dy/Helper/Type/DVector4.h>

namespace dy
{

/// @struct DVectorInt4
/// @brief TI32 type 4-element vector struct.
struct DVectorInt4 final {
  TI32 X = 0;
  TI32 Y = 0;
  TI32 Z = 0;
  TI32 W = 0;

  DVectorInt4() = default;

  DVectorInt4(_MIN_ const TI32 x, _MIN_ const TI32 y, _MIN_ const TI32 z, _MIN_ const TI32 w) noexcept :
      X{x}, Y{y}, Z{z}, W{w} {}
  explicit DVectorInt4(_MIN_ const TI32 value) noexcept :
      X{value}, Y{value}, Z{value}, W{value} {}
  explicit DVectorInt4(_MIN_ const DVector4& value) noexcept :
      X{static_cast<decltype(this->X)>(value.X)},
      Y{static_cast<decltype(this->Y)>(value.Y)},
      Z{static_cast<decltype(this->Z)>(value.Z)},
      W{static_cast<decltype(this->W)>(value.W)}
  {};

  //!
  //! Constructor and assign operator for dependencies.
  //!

  DVectorInt4(const glm::ivec4& value) noexcept : X{value.x}, Y{value.y}, Z{value.z}, W{value.w} {}
  DVectorInt4(const DVectorInt4& value) noexcept = default;

  DVectorInt4& operator=(const DVectorInt4& value) noexcept = default;
  DVectorInt4& operator=(_MIN_ const glm::ivec4& value) noexcept
  {
    this->X = value.x;
    this->Y = value.y;
    this->Z = value.z;
    this->W = value.w;
    return *this;
  }

  auto& operator[](_MIN_ const std::size_t index)
  {
    switch (index)
    {
    case 0: return this->X;
    case 1: return this->Y;
    case 2: return this->Z;
    case 3: return this->W;
    default: throw std::out_of_range("DVectorInt4 range is out of bound.");
    }
  }

  const auto& operator[](_MIN_ const std::size_t index) const
  {
    switch (index) {
    case 0: return this->X;
    case 1: return this->Y;
    case 2: return this->Z;
    case 3: return this->W;
    default: throw std::out_of_range("DVectorInt4 range is out of bound.");
    }
  }

  //!
  //! Conversion operators for dependencies.
  //!

  explicit operator glm::vec4() const noexcept
  {
    return glm::vec4{this->X, this->Y, this->Z, this->W};
  }

  operator glm::ivec4() const noexcept
  {
    return glm::ivec4{this->X, this->Y, this->Z, this->W};
  }

  operator DVector4() const noexcept
  {
    return DVector4{
        static_cast<TF32>(this->X), 
        static_cast<TF32>(this->Y),
        static_cast<TF32>(this->Z),
        static_cast<TF32>(this->W)};
  }

#if defined(false)
  DVectorInt4(const DirectX::XMINT2& value) noexcept : X{value.x}, Y{value.y} {}
  DVectorInt4(DirectX::FXMVECTOR value) noexcept
  {
    DirectX::XMINT2 xmVector = {};
    DirectX::XMStoreInt2(&xmVector.x, value);
    this->X = xmVector.x;
    this->Y = xmVector.y;
  }

  DVectorInt4& operator=(const DirectX::XMFLOAT2& value) noexcept
  {
    this->X = value.x;
    this->Y = value.y;
    return *this;
  }

  operator DirectX::XMFLOAT2() const noexcept
  {
    return DirectX::XMFLOAT2{this->X, this->Y};
  }

  operator DirectX::XMVECTOR() const noexcept
  {
    auto xmVector = static_cast<DirectX::XMFLOAT2>(*this);
    return XMLoadFloat2(&xmVector);
  }
#endif /// End defined(_WIN32)

  //!
  //! Methods
  //!

  /// @brief Return start pointer of instance. 
  MDY_NODISCARD const TI32* Data() const noexcept { return &this->X; }

  //!
  //! Operators
  //!

  friend DVectorInt4 operator+(_MIN_ DVectorInt4& lhs, _MIN_ const DVectorInt4& rhs) noexcept {
    lhs.X += rhs.X; lhs.Y += rhs.Y; lhs.Z += rhs.Z; lhs.W += rhs.W;
    return lhs;
  }

  friend DVectorInt4 operator-(_MIN_ DVectorInt4& lhs, _MIN_ const DVectorInt4& rhs) noexcept {
    lhs.X -= rhs.X; lhs.Y -= rhs.Y; lhs.Z -= rhs.Z; lhs.W -= rhs.W;
    return lhs;
  }

  ///
  /// DVectorInt4 $$ v = (x, y) $$ and value $$ a $$
  /// $$ av $$.
  ///
  friend DVectorInt4 operator*(_MIN_ DVectorInt4& lhs, _MIN_ const TI32 rhs) noexcept {
    lhs.X *= rhs; lhs.Y *= rhs; lhs.Z *= rhs; lhs.W *= rhs;
    return lhs;
  }

  ///
  /// If lhs and rhs are DVectorInt4, element multiplication happens.
  ///
  friend DVectorInt4 operator*(_MIN_ DVectorInt4& lhs, _MIN_ const DVectorInt4& rhs) noexcept {
    lhs.X *= rhs.X; lhs.Y *= rhs.Y; lhs.Z *= rhs.Z; lhs.W *= rhs.W;
    return lhs;
  }

  ///
  /// If rhs has 0 value, this function just do nothing.
  ///
  friend DVectorInt4 operator/(_MIN_ DVectorInt4& lhs, _MIN_ const TI32 rhs) noexcept {
    if (rhs == 0)
    { DyPushLogDebugCritical("DVectorInt4 could not be divided by {0}.", rhs); }
    else
    { lhs.X /= rhs; lhs.Y /= rhs; lhs.Z /= rhs; lhs.W /= rhs; }

    return lhs;
  }

  ///
  /// If rhs vector has any 0 value, this function just do nothing.
  ///
  friend DVectorInt4 operator/(_MIN_ DVectorInt4 lhs, _MIN_ const DVectorInt4& rhs) noexcept {
    if (rhs.X == 0 || rhs.Y == 0 || rhs.Z == 0 || rhs.W == 0)
    {
      DyPushLogDebugCritical(
          "DVectorInt4 could not be devided by 0 included DVectorInt4, ({0}, {1}, {2}, {3})", 
          rhs.X, rhs.Y, rhs.Z, rhs.W);
    }
    else
    {
      lhs.X /= rhs.X; lhs.Y /= rhs.Y; lhs.Z /= rhs.Z; lhs.W /= rhs.W;
    }

    return lhs;
  }

  DVectorInt4& operator+=(_MIN_ const DVectorInt4& value) noexcept
  {
    this->X += value.X; this->Y += value.Y; this->Z += value.Z; this->W += value.W;
    return *this;
  }

  DVectorInt4& operator-=(_MIN_ const DVectorInt4& value) noexcept
  {
    this->X -= value.X; this->Y -= value.Y; this->Z -= value.Z; this->W -= value.W;
    return *this;
  }

  DVectorInt4& operator*=(_MIN_ const TI32 value) noexcept
  {
    this->X *= value; this->Y *= value; this->Z *= value; this->W *= value;
    return *this;
  }

  DVectorInt4& operator*=(_MIN_ const DVectorInt4& value) noexcept
  {
    this->X *= value.X; this->Y *= value.Y; this->Z *= value.Z; this->W *= value.W;
    return *this;
  }

  ///
  /// If lhs and rhs are DVectorInt4, element multiplication happens.
  ///
  DVectorInt4& operator/=(const TI32 value) noexcept
  {
    if (value == 0)
    { DyPushLogDebugCritical("DVectorInt4 could not be divided by {0}.", value); }
    else
    { this->X /= value; this->Y /= value; this->W /= value; this->Z /= value; }

    return *this;
  }

  ///
  /// If rhs vector has any 0 value, this function just do nothing.
  ///
  DVectorInt4& operator/=(_MIN_ const DVectorInt4& value) noexcept
  {
    if (value.X == 0 || value.Y == 0 || value.Z == 0 || value.W == 0)
    {
      DyPushLogDebugCritical(
          "DVectorInt4 could not be devided by 0 included DVectorInt4, ({0}, {1}, {2}, {3})", 
          value.X, value.Y, value.Z, value.W);
    }
    else
    { this->X /= value.X; this->Y /= value.Y; this->Z /= value.Z; this->W /= value.W; }

    return *this;
  }

  /// @brief Compare each element of two vectors and return if there are all matched. 
  friend bool operator==(const DVectorInt4& lhs, const DVectorInt4& rhs) noexcept
  {
    return lhs.X == rhs.X 
        && lhs.Y == rhs.Y
        && lhs.Z == rhs.Z
        && lhs.W == rhs.W;
  }

public:
  /// @brief Check if this DVectorInt4 is all zero or nearly equal to zero.
  MDY_NODISCARD bool IsAllZero() const noexcept
  {
    return this->X == 0 && this->Y == 0 && this->Z == 0 && this->W == 0;
  }
};

void to_json  (_MINOUT_ nlohmann::json& j,    _MIN_ const DVectorInt4& p);
void from_json(_MIN_ const nlohmann::json& j, _MOUT_ DVectorInt4& p);

} /// ::dy namespace

#endif /// GUARD_DY_HELPER_TYPE_VECTORINT4_H