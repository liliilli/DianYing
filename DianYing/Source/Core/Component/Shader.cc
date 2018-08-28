#include <precompiled.h>
///
/// MIT License
/// Copyright (c) 2018 Jongmin Yun
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
#include <Dy/Core/Component/Shader.h>
#include <Dy/Helper/IoHelper.h>

namespace {

void PrintShaderErrorLog(GLuint shaderFragmentId) {
  GLint max_length = 0;
  glGetShaderiv(shaderFragmentId, GL_INFO_LOG_LENGTH, &max_length);

  std::vector<GLchar> info_log(max_length);
  glGetShaderInfoLog(shaderFragmentId, max_length, &max_length, &info_log[0]);
#if defined(_WIN32)
  MessageBox(nullptr, std::wstring(info_log.begin(), info_log.end()).c_str(), L"Error", MB_OK | MB_ICONERROR);
#else
  std::cout << "Failed to compile shader. Log : {}", &info_log[0];
#endif
}

void PrintShaderProgramErrorLog(GLuint shaderProgramId) {
  GLint max_length = 0;
  glGetProgramiv(shaderProgramId, GL_INFO_LOG_LENGTH, &max_length);

  std::vector<GLchar> info_log(max_length);
  glGetProgramInfoLog(shaderProgramId, max_length, &max_length, &info_log[0]);
#if defined(_WIN32)
  MessageBox(nullptr, std::wstring(info_log.begin(), info_log.end()).c_str(), L"Error", MB_OK | MB_ICONERROR);
#else
  std::cout << "Failed to compile shader. Log : {}", &info_log[0];
#endif
}

} /// unnamed namespace

namespace dy
{

CDyShaderComponent::~CDyShaderComponent()
{
  glDeleteProgram(mShaderProgramId);
}

EDySuccess CDyShaderComponent::pInitializeShaderProgram(const PDyShaderConstructionDescriptor& shaderConstructionDescriptor)
{
  std::vector<std::pair<EDyShaderFragmentType, uint32_t>> shaderFragmentIdList;

  MDY_CALL_ASSERT_SUCCESS(pInitializeShaderFragments(shaderConstructionDescriptor, shaderFragmentIdList));
  MDY_CALL_ASSERT_SUCCESS(pInitializeShaderProgram(shaderFragmentIdList));

  glGenVertexArrays(1, &mTemporalVertexArray);
  return DY_SUCCESS;
}

EDySuccess CDyShaderComponent::pInitializeShaderFragments(
    const PDyShaderConstructionDescriptor& shaderConstructionDescriptor,
    std::vector<std::pair<EDyShaderFragmentType, uint32_t>>& shaderFragmentIdList)
{
  const auto& container = shaderConstructionDescriptor.mShaderFragments;
  for (const auto& fragmentInformation : container)
  {
    GLuint shaderFragmentId = 0;
    switch (fragmentInformation.mShaderType)
    {
    case EDyShaderFragmentType::Vertex:
      shaderFragmentId = glCreateShader(GL_VERTEX_SHADER);
      break;
    case EDyShaderFragmentType::TesselationControl:
      shaderFragmentId = glCreateShader(GL_TESS_CONTROL_SHADER);
      break;
    case EDyShaderFragmentType::TesselationEvaluation:
      shaderFragmentId = glCreateShader(GL_TESS_EVALUATION_SHADER);
      break;
    case EDyShaderFragmentType::Geometry:
      shaderFragmentId = glCreateShader(GL_GEOMETRY_SHADER);
      break;
    case EDyShaderFragmentType::Pixel:
      shaderFragmentId = glCreateShader(GL_FRAGMENT_SHADER);
      break;
    }

    const auto opShaderFragmentCode = DyReadBinaryFileAll(fragmentInformation.mShaderPath);
    if (!opShaderFragmentCode.has_value())
    {
      return DY_FAILURE;
    }

    // Compile shader fragment.
    {
      const auto& codeString  = opShaderFragmentCode.value();
      const char* codePtr     = codeString.data();
      glShaderSource(shaderFragmentId, 1, &codePtr, nullptr);
      glCompileShader(shaderFragmentId);
    }

#if !defined(NDEBUG)
    // Check shader fragment compiliation status only in debug mode.
    {
      int32_t err;
      if (glGetShaderiv(shaderFragmentId, GL_COMPILE_STATUS, &err); !err)
      {
        PrintShaderErrorLog(shaderFragmentId);
        for (auto& [type, fragmentId] : shaderFragmentIdList)
        {
          glDeleteShader(fragmentId);
        }
        return DY_FAILURE;
      }
    }
#endif
    shaderFragmentIdList.emplace_back(fragmentInformation.mShaderType, shaderFragmentId);
  }

  return DY_SUCCESS;
}

EDySuccess CDyShaderComponent::pInitializeShaderProgram(const std::vector<std::pair<EDyShaderFragmentType, uint32_t>>& shaderFragmentIdList)
{
  mShaderProgramId = glCreateProgram();
  for (auto& [shaderFragmentType, shaderFragmentId] : shaderFragmentIdList) {
    glAttachShader(mShaderProgramId, shaderFragmentId);
  }
  glLinkProgram(mShaderProgramId);
  for (auto& [shaderFragmentType, shaderFragmentId] : shaderFragmentIdList) {
    glDetachShader(mShaderProgramId, shaderFragmentId);
  }

#if !defined(NDEBUG)
  // Check shader program linking status only in debug mode.
  {
    GLint isLinked = 0;
    glGetProgramiv(mShaderProgramId, GL_LINK_STATUS, &isLinked);
    if (!isLinked) {
      PrintShaderProgramErrorLog(mShaderProgramId);
      return DY_FAILURE;
    }
  }
#endif
  return DY_SUCCESS;
}

void CDyShaderComponent::UseShader()
{

}

void CDyShaderComponent::UpdateUniformVariables()
{

}

void CDyShaderComponent::BindShader()
{
  glUseProgram(mShaderProgramId);
}

void CDyShaderComponent::UnbindShader()
{
  glUseProgram(0);
}

void CDyShaderComponent::TempRender()
{
  glBindVertexArray(mTemporalVertexArray);
  glDrawArrays(GL_TRIANGLES, 0, 3);
  glBindVertexArray(0);
}
} /// ::dy namespace
