#ifndef GUARD_DY_HELPER_TYPE_VECTORINT2_H
#define GUARD_DY_HELPER_TYPE_VECTORINT2_H
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
#include <Dy/Helper/Type/Vector2.h>

namespace dy
{

/// @struct DDyVectorInt2
/// @brief TI32 type 2-element vector struct.
struct DDyVectorInt2 final 
{
  TI32 X = 0;
  TI32 Y = 0;

  DDyVectorInt2() = default;
  DDyVectorInt2(const TI32 x, const TI32 y) noexcept : X{x}, Y{y} {}
  explicit DDyVectorInt2(const TI32 value) noexcept : X{value}, Y{value} {}
  explicit DDyVectorInt2(const DDyVector2& value) noexcept 
    : X{static_cast<decltype(this->X)>(value.X)},
      Y{static_cast<decltype(this->Y)>(value.Y)}
  {};

  //!
  //! Constructor and assign operator for dependencies.
  //!

  DDyVectorInt2(const glm::ivec2& value) noexcept : X{value.x}, Y{value.y} {}
  DDyVectorInt2(const DDyVectorInt2& value) noexcept = default;

  DDyVectorInt2& operator=(const DDyVectorInt2& value) noexcept = default;
  DDyVectorInt2& operator=(_MIN_ const glm::ivec2& value) noexcept
  {
    this->X = value.x;
    this->Y = value.y;
    return *this;
  }

  auto& operator[](_MIN_ const std::size_t index)
  {
    switch (index)
    {
    case 0: return this->X;
    case 1: return this->Y;
    default: throw std::out_of_range("DDyVectorInt2 range is out of bound.");
    }
  }

  const auto& operator[](_MIN_ const std::size_t index) const
  {
    switch (index) {
    case 0: return this->X;
    case 1: return this->Y;
    default: throw std::out_of_range("DDyVectorInt2 range is out of bound.");
    }
  }

  //!
  //! Conversion operators for dependencies.
  //!

  operator aiVector2D() const noexcept
  {
    return aiVector2D{static_cast<ai_real>(this->X), static_cast<ai_real>(this->Y)};
  }

  operator glm::vec2() const noexcept
  {
    return glm::vec2{this->X, this->Y};
  }

  operator glm::ivec2() const noexcept
  {
    return glm::ivec2{this->X, this->Y};
  }

  operator DDyVector2() const noexcept
  {
    return DDyVector2{static_cast<float>(this->X), static_cast<float>(this->Y)};
  }

#if defined(false)
  DDyVectorInt2(const DirectX::XMINT2& value) noexcept : X{value.x}, Y{value.y} {}
  DDyVectorInt2(DirectX::FXMVECTOR value) noexcept
  {
    DirectX::XMINT2 xmVector = {};
    DirectX::XMStoreInt2(&xmVector.x, value);
    this->X = xmVector.x;
    this->Y = xmVector.y;
  }

  DDyVectorInt2& operator=(const DirectX::XMFLOAT2& value) noexcept
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

  /// @brief Return one-dimensional data chunk of DDyVectorInt2.
  FORCEINLINE std::array<TI32, 2> Data() const noexcept
  {
    return {this->X, this->Y};
  }

  /// @brief Return squared length of this vector.
  /// @return Squared length of this DDyVectorInt2.
  FORCEINLINE TI32 GetSquareLength() const noexcept
  {
    return this->X * this->X + this->Y * this->Y;
  }

  /// @brief Returns the length of this vector.
  /// @return Length of this DDyVectorInt2.
  FORCEINLINE float GetLength() const noexcept
  {
    return std::sqrtf(static_cast<float>(this->GetSquareLength()));
  }

  //!
  //! Operators
  //!

  friend DDyVectorInt2 operator+(_MIN_ DDyVectorInt2 lhs, _MIN_ const DDyVectorInt2& rhs) noexcept {
    lhs.X += rhs.X;
    lhs.Y += rhs.Y;
    return lhs;
  }

  friend DDyVectorInt2 operator-(_MIN_ DDyVectorInt2 lhs, _MIN_ const DDyVectorInt2& rhs) noexcept {
    lhs.X -= rhs.X;
    lhs.Y -= rhs.Y;
    return lhs;
  }

  ///
  /// DDyVectorInt2 $$ v = (x, y) $$ and value $$ a $$
  /// $$ av $$.
  ///
  friend DDyVectorInt2 operator*(_MIN_ DDyVectorInt2 lhs, _MIN_ const TI32 rhs) noexcept {
    lhs.X *= rhs;
    lhs.Y *= rhs;
    return lhs;
  }

  ///
  /// If lhs and rhs are DDyVectorInt2, element multiplication happens.
  ///
  friend DDyVectorInt2 operator*(_MIN_ DDyVectorInt2 lhs, _MIN_ const DDyVectorInt2& rhs) noexcept {
    lhs.X *= rhs.X;
    lhs.Y *= rhs.Y;
    return lhs;
  }

  ///
  /// If rhs has 0 value, this function just do nothing.
  ///
  friend DDyVectorInt2 operator/(_MIN_ DDyVectorInt2 lhs, _MIN_ const TI32 rhs) noexcept {
    if (rhs == 0)
    {
      DyPushLogDebugCritical("DDyVectorInt2 could not be divided by {0}.", rhs);
    }
    else
    {
      lhs.X /= rhs;
      lhs.Y /= rhs;
    }

    return lhs;
  }

  ///
  /// If rhs vector has any 0 value, this function just do nothing.
  ///
  friend DDyVectorInt2 operator/(_MIN_ DDyVectorInt2 lhs, _MIN_ const DDyVectorInt2& rhs) noexcept {
    if (rhs.X == 0 || rhs.Y == 0)
    {
      DyPushLogDebugCritical( "DDyVectorInt2 could not be devided by 0 included DDyVectorInt2, ({0}, {1})", rhs.X, rhs.Y);
    }
    else
    {
      lhs.X /= rhs.X;
      lhs.Y /= rhs.Y;
    }

    return lhs;
  }

  DDyVectorInt2& operator+=(_MIN_ const DDyVectorInt2& value) noexcept
  {
    this->X += value.X;
    this->Y += value.Y;
    return *this;
  }

  DDyVectorInt2& operator-=(_MIN_ const DDyVectorInt2& value) noexcept
  {
    this->X -= value.X;
    this->Y -= value.Y;
    return *this;
  }

  DDyVectorInt2& operator*=(_MIN_ const TI32 value) noexcept
  {
    this->X *= value;
    this->Y *= value;
    return *this;
  }

  DDyVectorInt2& operator*=(_MIN_ const DDyVectorInt2& value) noexcept
  {
    this->X *= value.X;
    this->Y *= value.Y;
    return *this;
  }

  ///
  /// If lhs and rhs are DDyVectorInt2, element multiplication happens.
  ///
  DDyVectorInt2& operator/=(const TI32 value) noexcept
  {
    if (value == 0)
    {
      DyPushLogDebugCritical("DDyVectorInt2 could not be divided by {0}.", value);
    }
    else
    {
      this->X /= value;
      this->Y /= value;
    }

    return *this;
  }

  ///
  /// If rhs vector has any 0 value, this function just do nothing.
  ///
  DDyVectorInt2& operator/=(_MIN_ const DDyVectorInt2& value) noexcept
  {
    if (value.X == 0 || value.Y == 0)
    {
      DyPushLogDebugCritical("DDyVectorInt2 could not be devided by 0 included DDyVectorInt2, ({0}, {1})", value.X, value.Y);
    }
    else
    {
      this->X /= value.X;
      this->Y /= value.Y;
    }

    return *this;
  }

  ///
  /// @brief Compare length of two vectors and return if they are same length.
  /// @param[in] lhs
  /// @param[in] rhs
  /// @return Equal flag.
  ///
  friend bool operator==(const DDyVectorInt2& lhs, const DDyVectorInt2& rhs) noexcept
  {
    return lhs.GetSquareLength() == rhs.GetSquareLength();
  }

private:
  friend bool operator<(const DDyVectorInt2& lhs, const DDyVectorInt2& rhs) noexcept
  {
    return lhs.GetSquareLength() < rhs.GetSquareLength();
  }

  friend bool operator>(const DDyVectorInt2& lhs, const DDyVectorInt2& rhs) noexcept
  {
    return rhs < lhs;
  }

  friend bool operator<=(const DDyVectorInt2& lhs, const DDyVectorInt2& rhs) noexcept
  {
    return !(rhs < lhs);
  }

  friend bool operator>=(const DDyVectorInt2& lhs, const DDyVectorInt2& rhs) noexcept
  {
    return !(lhs < rhs);
  }

public:
  ///
  /// @brief Check if this DDyVectorInt2 is all zero or nearly equal to zero.
  ///
  MDY_NODISCARD bool IsAllZero() const noexcept
  {
    return this->X == 0 && this->Y == 0;
  }

  //!
  //! Static functions
  //!
public:
  [[nodiscard]] static const DDyVectorInt2& CompareMaxLength(
    const DDyVectorInt2& lhs, 
    const DDyVectorInt2& rhs) noexcept
  {
    return (lhs >= rhs) ? lhs : rhs;
  }

  ///
  /// @brief Compare two DDyVectorInt2 and return max DDyVectorInt2 instance.
  ///
  [[nodiscard]] static FORCEINLINE const DDyVectorInt2& CompareMinLength(const DDyVectorInt2& lhs, const DDyVectorInt2& rhs) noexcept
  {
    return (lhs < rhs) ? lhs : rhs;
  }
};

void to_json  (_MINOUT_ nlohmann::json& j, _MIN_ const DDyVectorInt2& p);
void from_json(_MIN_ const nlohmann::json& j, _MOUT_ DDyVectorInt2& p);

} /// ::dy namespace

namespace std
{

template <> struct hash<dy::DDyVectorInt2>
{
  size_t operator()(const dy::DDyVectorInt2& iVertex) const 
  {
    return ((hash<TI32>()(iVertex.X) 
          ^ (hash<TI32>()(iVertex.Y) << 1)) >> 1);
  }
};

} /// ::std namespace

#endif /// GUARD_DY_HELPER_TYPE_VECTORINT2_H