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
#include <Dy/Helper/Type/DVector2.h>

namespace dy
{

/// @struct DVectorInt2
/// @brief TI32 type 2-element vector struct.
struct DVectorInt2 final 
{
  TI32 X = 0;
  TI32 Y = 0;

  DVectorInt2() = default;
  DVectorInt2(const TI32 x, const TI32 y) noexcept : X{x}, Y{y} {}
  explicit DVectorInt2(const TI32 value) noexcept : X{value}, Y{value} {}
  explicit DVectorInt2(const DVector2& value) noexcept 
    : X{static_cast<decltype(this->X)>(value.X)},
      Y{static_cast<decltype(this->Y)>(value.Y)}
  {};

  //!
  //! Constructor and assign operator for dependencies.
  //!

  DVectorInt2(const glm::ivec2& value) noexcept : X{value.x}, Y{value.y} {}
  DVectorInt2(const DVectorInt2& value) noexcept = default;

  DVectorInt2& operator=(const DVectorInt2& value) noexcept = default;
  DVectorInt2& operator=(_MIN_ const glm::ivec2& value) noexcept
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
    default: throw std::out_of_range("DVectorInt2 range is out of bound.");
    }
  }

  const auto& operator[](_MIN_ const std::size_t index) const
  {
    switch (index) {
    case 0: return this->X;
    case 1: return this->Y;
    default: throw std::out_of_range("DVectorInt2 range is out of bound.");
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

  operator DVector2() const noexcept
  {
    return DVector2{static_cast<float>(this->X), static_cast<float>(this->Y)};
  }

#if defined(false)
  DVectorInt2(const DirectX::XMINT2& value) noexcept : X{value.x}, Y{value.y} {}
  DVectorInt2(DirectX::FXMVECTOR value) noexcept
  {
    DirectX::XMINT2 xmVector = {};
    DirectX::XMStoreInt2(&xmVector.x, value);
    this->X = xmVector.x;
    this->Y = xmVector.y;
  }

  DVectorInt2& operator=(const DirectX::XMFLOAT2& value) noexcept
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

  /// @brief Return one-dimensional data chunk of DVectorInt2.
  FORCEINLINE std::array<TI32, 2> Data() const noexcept
  {
    return {this->X, this->Y};
  }

  /// @brief Return squared length of this vector.
  /// @return Squared length of this DVectorInt2.
  FORCEINLINE TI32 GetSquareLength() const noexcept
  {
    return this->X * this->X + this->Y * this->Y;
  }

  /// @brief Returns the length of this vector.
  /// @return Length of this DVectorInt2.
  FORCEINLINE float GetLength() const noexcept
  {
    return std::sqrtf(static_cast<float>(this->GetSquareLength()));
  }

  //!
  //! Operators
  //!

  friend DVectorInt2 operator+(_MIN_ DVectorInt2 lhs, _MIN_ const DVectorInt2& rhs) noexcept {
    lhs.X += rhs.X;
    lhs.Y += rhs.Y;
    return lhs;
  }

  friend DVectorInt2 operator-(_MIN_ DVectorInt2 lhs, _MIN_ const DVectorInt2& rhs) noexcept {
    lhs.X -= rhs.X;
    lhs.Y -= rhs.Y;
    return lhs;
  }

  ///
  /// DVectorInt2 $$ v = (x, y) $$ and value $$ a $$
  /// $$ av $$.
  ///
  friend DVectorInt2 operator*(_MIN_ DVectorInt2 lhs, _MIN_ const TI32 rhs) noexcept {
    lhs.X *= rhs;
    lhs.Y *= rhs;
    return lhs;
  }

  ///
  /// If lhs and rhs are DVectorInt2, element multiplication happens.
  ///
  friend DVectorInt2 operator*(_MIN_ DVectorInt2 lhs, _MIN_ const DVectorInt2& rhs) noexcept {
    lhs.X *= rhs.X;
    lhs.Y *= rhs.Y;
    return lhs;
  }

  ///
  /// If rhs has 0 value, this function just do nothing.
  ///
  friend DVectorInt2 operator/(_MIN_ DVectorInt2 lhs, _MIN_ const TI32 rhs) noexcept {
    if (rhs == 0)
    {
      DyPushLogDebugCritical("DVectorInt2 could not be divided by {0}.", rhs);
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
  friend DVectorInt2 operator/(_MIN_ DVectorInt2 lhs, _MIN_ const DVectorInt2& rhs) noexcept {
    if (rhs.X == 0 || rhs.Y == 0)
    {
      DyPushLogDebugCritical( "DVectorInt2 could not be devided by 0 included DVectorInt2, ({0}, {1})", rhs.X, rhs.Y);
    }
    else
    {
      lhs.X /= rhs.X;
      lhs.Y /= rhs.Y;
    }

    return lhs;
  }

  DVectorInt2& operator+=(_MIN_ const DVectorInt2& value) noexcept
  {
    this->X += value.X;
    this->Y += value.Y;
    return *this;
  }

  DVectorInt2& operator-=(_MIN_ const DVectorInt2& value) noexcept
  {
    this->X -= value.X;
    this->Y -= value.Y;
    return *this;
  }

  DVectorInt2& operator*=(_MIN_ const TI32 value) noexcept
  {
    this->X *= value;
    this->Y *= value;
    return *this;
  }

  DVectorInt2& operator*=(_MIN_ const DVectorInt2& value) noexcept
  {
    this->X *= value.X;
    this->Y *= value.Y;
    return *this;
  }

  ///
  /// If lhs and rhs are DVectorInt2, element multiplication happens.
  ///
  DVectorInt2& operator/=(const TI32 value) noexcept
  {
    if (value == 0)
    {
      DyPushLogDebugCritical("DVectorInt2 could not be divided by {0}.", value);
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
  DVectorInt2& operator/=(_MIN_ const DVectorInt2& value) noexcept
  {
    if (value.X == 0 || value.Y == 0)
    {
      DyPushLogDebugCritical("DVectorInt2 could not be devided by 0 included DVectorInt2, ({0}, {1})", value.X, value.Y);
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
  friend bool operator==(const DVectorInt2& lhs, const DVectorInt2& rhs) noexcept
  {
    return lhs.GetSquareLength() == rhs.GetSquareLength();
  }

private:
  friend bool operator<(const DVectorInt2& lhs, const DVectorInt2& rhs) noexcept
  {
    return lhs.GetSquareLength() < rhs.GetSquareLength();
  }

  friend bool operator>(const DVectorInt2& lhs, const DVectorInt2& rhs) noexcept
  {
    return rhs < lhs;
  }

  friend bool operator<=(const DVectorInt2& lhs, const DVectorInt2& rhs) noexcept
  {
    return !(rhs < lhs);
  }

  friend bool operator>=(const DVectorInt2& lhs, const DVectorInt2& rhs) noexcept
  {
    return !(lhs < rhs);
  }

public:
  ///
  /// @brief Check if this DVectorInt2 is all zero or nearly equal to zero.
  ///
  MDY_NODISCARD bool IsAllZero() const noexcept
  {
    return this->X == 0 && this->Y == 0;
  }

  //!
  //! Static functions
  //!
public:
  [[nodiscard]] static const DVectorInt2& CompareMaxLength(
    const DVectorInt2& lhs, 
    const DVectorInt2& rhs) noexcept
  {
    return (lhs >= rhs) ? lhs : rhs;
  }

  ///
  /// @brief Compare two DVectorInt2 and return max DVectorInt2 instance.
  ///
  [[nodiscard]] static FORCEINLINE const DVectorInt2& CompareMinLength(const DVectorInt2& lhs, const DVectorInt2& rhs) noexcept
  {
    return (lhs < rhs) ? lhs : rhs;
  }
};

void to_json  (_MINOUT_ nlohmann::json& j, _MIN_ const DVectorInt2& p);
void from_json(_MIN_ const nlohmann::json& j, _MOUT_ DVectorInt2& p);

} /// ::dy namespace

namespace std
{

template <> struct hash<dy::DVectorInt2>
{
  size_t operator()(const dy::DVectorInt2& iVertex) const 
  {
    return ((hash<TI32>()(iVertex.X) 
          ^ (hash<TI32>()(iVertex.Y) << 1)) >> 1);
  }
};

} /// ::std namespace

#endif /// GUARD_DY_HELPER_TYPE_VECTORINT2_H