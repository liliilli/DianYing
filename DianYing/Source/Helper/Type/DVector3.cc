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
#include <Dy/Helper/Library/HelperJson.h>
#include <Dy/Helper/Library/HelperString.h>

namespace dy
{

std::string ToString(const DVec3& vec) noexcept
{
  return MakeStringU8("({:1}, {:1}, {:1})", vec.X, vec.Y, vec.Z);
}

std::string ToString(const DIVec3& vec) noexcept
{
  return MakeStringU8("({}, {}, {})", vec.X, vec.Y, vec.Z);
}

physx::PxVec3 ToPxVec3(const DVec3& vec) noexcept
{
  return physx::PxVec3{vec.X, vec.Y, vec.Z};
}

physx::PxVec3 ToPxVec3(const DIVec3& vec) noexcept
{
  return physx::PxVec3{
    static_cast<TF32>(vec.X), 
    static_cast<TF32>(vec.Y), 
    static_cast<TF32>(vec.Z)
  };
}

glm::vec3 ToGlmVec3(const DVec3& vec) noexcept
{
  return glm::vec3{vec.X, vec.Y, vec.Z};
}

glm::vec3 ToGlmVec3(const DIVec3& vec) noexcept
{
  return glm::vec3{TF32(vec.X), TF32(vec.Y), TF32(vec.Z)};
}

DVec3 CreateVec3(const physx::PxVec3& vec) noexcept
{
  return DVec3{vec.x, vec.y, vec.z};
}

DVec3 CreateVec3(const glm::vec3& vec) noexcept
{
  return DVec3{vec.x, vec.y, vec.z};
}

} /// ::dy::namespace

namespace dy::math
{

void to_json(nlohmann::json& j, const DVec3& p)
{
  j = nlohmann::json{ {"X", p.X}, {"Y", p.Y}, {"Z", p.Z} };
}

void from_json(const nlohmann::json& j, DVec3& p)
{
  json::GetValueFromTo(j, "X", p.X);
  json::GetValueFromTo(j, "Y", p.Y);
  json::GetValueFromTo(j, "Z", p.Z);
}

void to_json(nlohmann::json& j, const DIVec3& p)
{
  j = nlohmann::json{ {"X", p.X}, {"Y", p.Y}, {"Z", p.Z} };
}

void from_json(const nlohmann::json& j, DIVec3& p)
{
  json::GetValueFromTo(j, "X", p.X);
  json::GetValueFromTo(j, "Y", p.Y);
  json::GetValueFromTo(j, "Z", p.Z);
}

} /// ::dy::math namespace
