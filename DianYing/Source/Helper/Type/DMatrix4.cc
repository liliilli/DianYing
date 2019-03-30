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
#include <Dy/Helper/Type/DMatrix4x4.h>

#include <glm/gtc/matrix_transform.hpp>
#include <foundation/PxTransform.h>
#include <nlohmann/json.hpp>

#include <Dy/Helper/Type/DMatrix2x2.h>
#include <Dy/Helper/Type/DMatrix3x3.h>
#include <Dy/Helper/Type/DQuaternion.h>

namespace dy
{

DMat4 FMat4::CreateWithScale(TReal x, TReal y, TReal z)
{
  DMat4 result = DMat4::Identity();
  result[0] *= x;
  result[1] *= y;
  result[2] *= z;

  return result;
}

DMat4 FMat4::CreateWithScale(const DVec3& vec)
{
  DMat4 result = DMat4::Identity();
  result[0] *= vec[0];
  result[1] *= vec[1];
  result[2] *= vec[2];

  return result;
}

DMat4 FMat4::CreateWithTranslation(TReal x, TReal y, TReal z)
{
  DMat4 result = DMat4::Identity();
  result[3][0] = x;
  result[3][1] = y;
  result[3][2] = z;

  return result;
}

DMat4 FMat4::CreateWithTranslation(const DVec3& vec)
{
  DMat4 result = DMat4::Identity();
  result[3][0] = vec[0];
  result[3][1] = vec[1];
  result[3][2] = vec[2];

  return result;
}

DMat4 FMat4::Rotate(const DMat4& matrix, const DVec3& angle, bool isDegree)
{
  return matrix * DQuat(angle, isDegree).ToMatrix4();
}

DMat4 FMat4::Rotate(const DMat4& matrix, const DQuat& quat)
{
  return matrix * quat.ToMatrix4();
}

DMat4 FMat4::Scale(const DMat4& matrix, const DVec3& scale)
{
  return DMat4{matrix[0] * scale.X, matrix[1] * scale.Y, matrix[2] * scale.Z, matrix[3]};
}

DMat4 FMat4::Locate(const DMat4& matrix, const DVec3& position)
{
  auto result = matrix;
  result[3][0] = position.X;
  result[3][1] = position.Y;
  result[3][2] = position.Z;

  return result;
}

DMat4 FMat4::Translate(const DMat4& matrix, const DVec3& offsetPos)
{
  auto result = matrix;
  result[3][0] += offsetPos.X;
  result[3][1] += offsetPos.Y;
  result[3][2] += offsetPos.Z;
  return result;
}

DMat4 FMat4::CreateMat4(const glm::mat4& mat) noexcept
{
  return 
  {
    FVec4::CreateVec4(mat[0]),
    FVec4::CreateVec4(mat[1]),
    FVec4::CreateVec4(mat[2]),
    FVec4::CreateVec4(mat[3])
  };
}

DMat4 FMat4::CreateMat4(const physx::PxTransform& transform) noexcept
{
  auto quatMatrix = FQuat::CreateQuat(transform.q).ToMatrix4();
  quatMatrix[3][0] = transform.p.x;
  quatMatrix[3][1] = transform.p.y;
  quatMatrix[3][2] = transform.p.z;

  return quatMatrix;
}

} /// ::dy namespace

namespace dy::math
{
  
void to_json(nlohmann::json& j, const DMat4& p)
{
  j = nlohmann::json
  {
    {"00", p[0][0]}, {"01", p[0][1]}, {"02", p[0][2]}, {"03", p[0][3]},
    {"10", p[1][0]}, {"11", p[1][1]}, {"12", p[1][2]}, {"13", p[1][3]},
    {"20", p[1][0]}, {"21", p[2][1]}, {"22", p[2][2]}, {"23", p[2][3]},
    {"30", p[1][0]}, {"31", p[3][1]}, {"32", p[3][2]}, {"33", p[3][3]},
  };
}

void from_json(const nlohmann::json& j, DMat4& p)
{
  p[0][0] = j["00"].get<float>(); p[0][1] = j["01"].get<float>(); p[0][2] = j["02"].get<float>(); p[0][3] = j["03"].get<float>();
  p[1][0] = j["10"].get<float>(); p[1][1] = j["11"].get<float>(); p[1][2] = j["12"].get<float>(); p[1][3] = j["13"].get<float>();
  p[2][0] = j["20"].get<float>(); p[2][1] = j["21"].get<float>(); p[2][2] = j["22"].get<float>(); p[2][3] = j["23"].get<float>();
  p[3][0] = j["30"].get<float>(); p[3][1] = j["31"].get<float>(); p[3][2] = j["32"].get<float>(); p[3][3] = j["33"].get<float>();
}

} /// ::dy::math namespace