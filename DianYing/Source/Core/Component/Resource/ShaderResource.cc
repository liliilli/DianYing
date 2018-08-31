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
#include <Dy/Core/Component/Resource/ShaderResource.h>

#include <Dy/Helper/IoHelper.h>
#include <Dy/Core/Component/Information/ShaderInformation.h>
#include <Dy/Core/Component/Resource/MaterialResource.h>

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

CDyShaderResource::~CDyShaderResource()
{
  // Release heap resources
  if (mShaderProgramId)
  {
    glDeleteProgram(mShaderProgramId);
  }

  // Unbind previous and next level.
  if (__mPrevLevelPtr)
  {
    __mPrevLevelPtr->__pfSetNextLevel(nullptr);
  }
  for (auto& [notUsed, materialPtr] : __mBindMaterialPtrs)
  {
    materialPtr->__pfResetShaderPtr();
  }
}

EDySuccess CDyShaderResource::pfInitializeResource(const CDyShaderInformation& shaderInformation)
{
  const auto& information = shaderInformation.GetInformation();
  mShaderName = information.mShaderName;

  std::vector<std::pair<EDyShaderFragmentType, uint32_t>> shaderFragmentIdList;
  if (__pInitializeShaderFragments(information, shaderFragmentIdList) == DY_FAILURE)
  {
    return DY_FAILURE;
  }
  if (__pInitializeShaderProgram(shaderFragmentIdList) == DY_FAILURE)
  {
    return DY_FAILURE;
  }

  glGenVertexArrays(1, &mTemporalVertexArray);
  return DY_SUCCESS;
}

EDySuccess CDyShaderResource::__pInitializeShaderFragments(
    const PDyShaderConstructionDescriptor& shaderConstructionDescriptor,
    std::vector<std::pair<EDyShaderFragmentType, uint32_t>>& shaderFragmentIdList)
{
  const auto& container = shaderConstructionDescriptor.mShaderFragments;
  for (const auto& fragmentInformation : container)
  {
    GLuint shaderFragmentId = 0;

    // Get OpenGL Shader type
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

    // If shader creation had failed, revert and release all shader fragments.
    if (shaderFragmentId <= 0)
    {
      for (auto& [type, compiledShaderId] : shaderFragmentIdList)
      {
        glDeleteShader(compiledShaderId);
      }
      return DY_FAILURE;
    }

    // Read shader fragment code. if something is wrong, revert and release all shader fragments.
    const auto opShaderFragmentCode = DyReadBinaryFileAll(fragmentInformation.mShaderPath);
    if (!opShaderFragmentCode.has_value())
    {
      // @todo output error message;
      glDeleteShader(shaderFragmentId);
      for (auto& [type, compiledShaderId] : shaderFragmentIdList)
      {
        glDeleteShader(compiledShaderId);
      }
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
        glDeleteShader(shaderFragmentId);
        for (auto& [type, compiledShaderId] : shaderFragmentIdList)
        {
          glDeleteShader(compiledShaderId);
        }
        return DY_FAILURE;
      }
    }
#endif

    // Insert newly compiled shader fragment for __pInitilaizeShaderProgram.
    shaderFragmentIdList.emplace_back(fragmentInformation.mShaderType, shaderFragmentId);
  }

  return DY_SUCCESS;
}

EDySuccess CDyShaderResource::__pInitializeShaderProgram(const std::vector<std::pair<EDyShaderFragmentType, uint32_t>>& shaderFragmentIdList)
{
  // Create GlShaderProgram, and attach fragments to link them.
  mShaderProgramId = glCreateProgram();
  for (auto& [shaderFragmentType, shaderFragmentId] : shaderFragmentIdList) {
    glAttachShader(mShaderProgramId, shaderFragmentId);
  }
  glLinkProgram(mShaderProgramId);
  for (auto& [shaderFragmentType, shaderFragmentId] : shaderFragmentIdList) {
    glDetachShader(mShaderProgramId, shaderFragmentId);
    glDeleteShader(shaderFragmentId);
  }

  // Check shader program linking status only in debug mode.
#if !defined(NDEBUG)
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

void CDyShaderResource::UseShader() noexcept
{
  glUseProgram(mShaderProgramId);
}

void CDyShaderResource::UpdateUniformVariables()
{
  assert(false && "NOT IMPLEMENTED");
}

void CDyShaderResource::BindShader() noexcept
{
  glBindVertexArray(mTemporalVertexArray);
}

void CDyShaderResource::UnbindShader() noexcept
{
  glBindVertexArray(0);
}

void CDyShaderResource::UnuseShader() noexcept
{
  glUseProgram(0);
}

} /// ::dy namespace
