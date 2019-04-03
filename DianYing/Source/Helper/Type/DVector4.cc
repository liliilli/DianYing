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
#include <Dy/Helper/Type/DVector4.h>


#include <Dy/Helper/Type/DMatrix4x4.h>
#include <Dy/Management/MLog.h>

namespace dy 
{

DVec4 FVec4::CreateVec4(const glm::vec4& vec) noexcept
{
  return DVec4{vec.x, vec.y, vec.z, vec.w};
}

DVec4 FVec4::CreateVec4(const glm::ivec4& vec) noexcept
{
  using Type = DVec4::TValueType;
  return DVec4{Type(vec.x), Type(vec.y), Type(vec.z), Type(vec.w)};
}

DIVec4 FVec4::CreateIVec4(const glm::ivec4& vec) noexcept
{
  return DIVec4{vec.x, vec.y, vec.z, vec.w};
}

} /// ::dy namespace

namespace dy::math 
{

void to_json(nlohmann::json& oJson, const DVec4& iVector)
{
  oJson = nlohmann::json
  { 
    {"X", iVector.X}, {"Y", iVector.Y}, {"Z", iVector.Z}, {"W", iVector.W}
  };
}

void from_json(const nlohmann::json& iJson, DVec4& oVector)
{
  json::GetValueFromTo(iJson, "X", oVector.X);
  json::GetValueFromTo(iJson, "Y", oVector.Y);
  json::GetValueFromTo(iJson, "Z", oVector.Z);
  json::GetValueFromTo(iJson, "W", oVector.W);
}

void to_json(nlohmann::json& oJson, const DIVec4& iVector)
{
  oJson = nlohmann::json
  { 
    {"X", iVector.X}, {"Y", iVector.Y}, {"Z", iVector.Z}, {"W", iVector.W}
  };
}

void from_json(const nlohmann::json& iJson, DIVec4& oVector)
{
  json::GetValueFromTo(iJson, "X", oVector.X);
  json::GetValueFromTo(iJson, "Y", oVector.Y);
  json::GetValueFromTo(iJson, "Z", oVector.Z);
  json::GetValueFromTo(iJson, "W", oVector.W);
}

} /// ::dy::math namespace