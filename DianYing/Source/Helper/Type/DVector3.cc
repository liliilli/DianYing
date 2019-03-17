#include <precompiled.h>
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

/// Header file
#include <Dy/Helper/Type/DVector3.h>

#include <nlohmann/json.hpp>
#include <Dy/Helper/Math/Math.h>
#include <Dy/Helper/Type/DMatrix3x3.h>
#include <Dy/Helper/Library/HelperJson.h>

namespace dy 
{

DVector3& DVector3::operator=(const aiVector2D& iVector) noexcept
{
  this->X = iVector.x; this->Y = iVector.y; this->Z = 0.0f;
  return *this;
}

DVector3& DVector3::operator=(const aiVector3D& iVector) noexcept
{
  this->X = iVector.x;
  this->Y = iVector.y;
  this->Z = iVector.z;
  return *this;
}

TF32* DVector3::Data() noexcept
{
  return &this->X;
}

const TF32* DVector3::Data() const noexcept
{
  return &this->X;
}

DVector3 DVector3::MultiplyMatrix(const DMatrix3x3& matrix) const noexcept
{
  return DVector3
  {
    this->X * matrix[0][0] + this->Y * matrix[0][1] + this->Z * matrix[0][2],
    this->X * matrix[1][0] + this->Y * matrix[1][1] + this->Z * matrix[1][2],
    this->X * matrix[2][0] + this->Y * matrix[2][1] + this->Z * matrix[2][2]
  };
}

bool DVector3::IsAllZero(const DVector3& vector) noexcept 
{
  return math::IsAllZero(vector);
}

bool DVector3::IsAllZero() const noexcept 
{
  return math::IsAllZero(*this);
}

bool operator==(_MIN_ const DVector3& lhs, _MIN_ const DVector3& rhs) noexcept
{
  return lhs.X == rhs.X
      && lhs.Y == rhs.Y
      && lhs.Z == rhs.Z;
}

bool operator!=(_MIN_ const DVector3& lhs, _MIN_ const DVector3& rhs) noexcept
{
  return !(lhs == rhs);
}

std::string DVector3::ToString() const noexcept
{
  return MakeStringU8("({:1}, {:1}, {:1})", this->X, this->Y, this->Z);
}

void to_json(_MINOUT_ nlohmann::json& j, _MIN_ const DVector3& p)
{
  j = nlohmann::json{ {"X", p.X}, {"Y", p.Y}, {"Z", p.Z} };
}

void from_json(_MIN_ const nlohmann::json& j, _MOUT_ DVector3& p)
{
  json::GetValueFromTo(j, "X", p.X);
  json::GetValueFromTo(j, "Y", p.Y);
  json::GetValueFromTo(j, "Z", p.Z);
}

} /// ::dy namespace