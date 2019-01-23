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

#include <assimp/matrix4x4.h>
#include "Data_Vector4.h"

///
/// @class DDyMatrix4x4
/// @brief
///
class DDyMatrix4x4 final
{
public:
  DDyMatrix4x4()  = default;
  ~DDyMatrix4x4() = default;

  DDyMatrix4x4(const DDyMatrix4x4& value) = default;
  DDyMatrix4x4(DDyMatrix4x4&& value) = default;
  DDyMatrix4x4& operator=(const DDyMatrix4x4& value) = default;
  DDyMatrix4x4& operator=(DDyMatrix4x4&& value) = default;
  DDyMatrix4x4(std::initializer_list<float>&) = delete;

  DDyMatrix4x4(const float _00, const float _01, const float _02, const float _03,
               const float _10, const float _11, const float _12, const float _13,
               const float _20, const float _21, const float _22, const float _23,
               const float _30, const float _31, const float _32, const float _33) :
      mMatrixValue{DDyVector4{_00, _10, _20, _30},
                   DDyVector4{_01, _11, _21, _31},
                   DDyVector4{_02, _12, _22, _32},
                   DDyVector4{_03, _13, _23, _33}} {}

  DDyMatrix4x4(const DDyVector4& column1, const DDyVector4& column2,
               const DDyVector4& column3, const DDyVector4& column4);

  DDyMatrix4x4(const aiMatrix4x4& aiMatrix) noexcept;
  DDyMatrix4x4& operator=(const aiMatrix4x4& value) noexcept;

  //!
  //! Conversion operators
  //!

  DDyVector4& operator[](std::size_t index) noexcept
  {
    return mMatrixValue[index];
  }

  const DDyVector4& operator[](std::size_t index) const noexcept
  {
    return mMatrixValue[index];
  }

  DDyMatrix4x4 operator*(const DDyMatrix4x4& rhs) const noexcept;

  DDyMatrix4x4& operator*=(const DDyMatrix4x4& rhs) noexcept;

  DDyMatrix4x4 operator/(const DDyMatrix4x4& rhs) const;

  DDyMatrix4x4& operator/=(const DDyMatrix4x4& rhs);

  DDyMatrix4x4 operator+(const DDyMatrix4x4& rhs) const noexcept;

  DDyMatrix4x4& operator+=(const DDyMatrix4x4& rhs) noexcept;

  DDyMatrix4x4 operator-(const DDyMatrix4x4& rhs) const noexcept;

  DDyMatrix4x4& operator-=(const DDyMatrix4x4& rhs) noexcept;

  friend bool operator==( const DDyMatrix4x4& lhs,  const DDyMatrix4x4& rhs) noexcept;
  friend bool operator!=( const DDyMatrix4x4& lhs,  const DDyMatrix4x4& rhs) noexcept;

  /// @brief
  DDyMatrix4x4 Transpose() const noexcept;

  /// @brief
  DDyMatrix4x4 Multiply(const DDyMatrix4x4& rhs) const noexcept;

  /// @brief
  DDyVector4 MultiplyVector(const DDyVector4& rhs) const noexcept;

  /// @brief
  static DDyMatrix4x4 IdentityMatrix() noexcept;

  /// @brief
  static DDyMatrix4x4 CreateWithScale(const DDyVector3& scaleVector);

  /// @brief
  static DDyMatrix4x4 CreateWithTranslation(const DDyVector3& translationPoint);

private:
  /// Identity matrix constructor
  explicit DDyMatrix4x4(bool);

  /// Column major
  std::array<DDyVector4, 4> mMatrixValue;
};

void to_json(nlohmann::json& j, const DDyMatrix4x4& p);
void from_json(const nlohmann::json& j, DDyMatrix4x4& p);
