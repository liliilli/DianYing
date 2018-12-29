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
#include <Dy/Core/Resource/Resource/FDyShaderResource.h>
#include <Dy/Core/Resource/Information/FDyShaderInformation.h>
#include <Dy/Core/Rendering/Wrapper/PDyGLShaderFragmentDescriptor.h>
#include <Dy/Core/Rendering/Wrapper/FDyGLWrapper.h>
#include <Dy/Management/Helper/SDyProfilingHelper.h>

//!
//! Forward declaration
//!

namespace
{

void DyPrintShaderErrorLog(_MIN_ TU32 shaderFragmentId)
{
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

void DyPrintShaderProgramErrorLog(_MIN_ TU32 shaderProgramId)
{
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

} /// ::unnamed namespace

//!
//! Implementation
//!

namespace dy
{

FDyShaderResource::FDyShaderResource(_MIN_ const FDyShaderInformation& information) :
    mBinderShader{information.GetSpecifierName()}
{
  this->mSpecifierName = information.GetSpecifierName();

  { // Create shader program.
    auto optFragmentList    = this->pCreateShaderFragments(information.GetShaderFragmentList());
    MDY_ASSERT(optFragmentList.has_value() == true, "OpenGL Shader Fragment compilation failed.");

    auto optShaderProgramId = this->pInitializeShaderProgram(optFragmentList.value());
    MDY_ASSERT(optShaderProgramId.has_value() == true, "OpenGL shader program compilation failed.");
    this->mShaderProgramId = optShaderProgramId.value();

    this->pDeleteShaderFragments(optFragmentList.value());
  }

  { // Get attribute, uniform, ubo information from shader program.
    this->pStoreAttributeProperties();
    this->pStoreUniformProperties();
    this->pStoreUniformBufferObjectProperties();
  }

  SDyProfilingHelper::IncreaseOnBindShaderCount(1);
}

FDyShaderResource::~FDyShaderResource()
{
  SDyProfilingHelper::DecreaseOnBindShaderCount(1);
  FDyGLWrapper::DeleteShaderProgram(this->mShaderProgramId);
}

TU32 FDyShaderResource::GetShaderProgramId() const noexcept
{
  return this->mShaderProgramId;
}

void FDyShaderResource::UseShader() const noexcept
{
  MDY_ASSERT(this->mShaderProgramId > 0, "Shader program must be valid.");
  FDyGLWrapper::UseShaderProgram(this->mShaderProgramId);
}

void FDyShaderResource::DisuseShader() const noexcept
{
  FDyGLWrapper::DisuseShaderProgram();
}

std::optional<TFragmentList>
FDyShaderResource::pCreateShaderFragments(_MIN_ const FDyShaderInformation::TShaderFragmentList& vector)
{
  TFragmentList resultList {};

  for (const auto& fragmentInformation : vector)
  {
    PDyGLShaderFragmentDescriptor fragDesc;
    fragDesc.mType      = fragmentInformation.mShaderType;
    fragDesc.mPtrBuffer = fragmentInformation.mShaderFragmentCode.data();

    // Create shader fragment.
    const auto optShaderFragmentId = FDyGLWrapper::CreateShaderFragment(fragDesc);
    MDY_ASSERT(optShaderFragmentId.has_value() == true, "Shader fragment compilation must be succeeded.");
    const auto shaderFragmentId = optShaderFragmentId.value();

    // Check shader fragment compiliation status only in debug mode.
#if defined(NDEBUG) == false
    {
      TI32 err;
      if (glGetShaderiv(shaderFragmentId, GL_COMPILE_STATUS, &err); !err)
      {
        DyPrintShaderErrorLog(shaderFragmentId);
        MDY_UNEXPECTED_BRANCH_BUT_RETURN(std::nullopt);
      }
    }
#endif

    // Insert newly compiled shader fragment for __pInitilaizeShaderProgram.
    resultList.emplace_back(fragmentInformation.mShaderType, shaderFragmentId);
  }

  return resultList;
}

std::optional<TU32> FDyShaderResource::pInitializeShaderProgram(_MIN_ const TFragmentList& fragmentList)
{
  auto optProgramId = FDyGLWrapper::CreateShaderProgram(fragmentList);
  MDY_ASSERT(optProgramId.has_value() == true, "Unexpected error occurred.");

  // Check shader program linking status only in debug mode.
#if defined(NDEBUG) == false
  {
    TI32 isLinked = 0;
    glGetProgramiv(optProgramId.value(), GL_LINK_STATUS, &isLinked);
    if (isLinked == 0) {
      DyPrintShaderProgramErrorLog(optProgramId.value());
      return std::nullopt;
    }
  }
#endif
  return optProgramId;
}

void FDyShaderResource::pDeleteShaderFragments(const TFragmentList& fragmentList)
{
  for (const auto& [_, validFragmentId] : fragmentList)
  {
    FDyGLWrapper::DeleteShaderFragment(validFragmentId);
  }
}

void FDyShaderResource::pStoreAttributeProperties() noexcept
{
  const TU32 activatedAttrCount = FDyGLWrapper::QueryShaderProgramIV(this->mShaderProgramId, GL_ACTIVE_ATTRIBUTES);
  this->mAttributeVariableList.reserve(activatedAttrCount);

  // Retrieve attirbute variable information.
  for (TU32 i = 0; i < activatedAttrCount; ++i)
  {
    auto result = FDyGLWrapper::GetShaderProgramAttributeInfo(this->mShaderProgramId, i);
    MDY_ASSERT(result.has_value() == true, "Unexpected error occurred.");

    auto [specifier, length, size, type, locId] = result.value();
    if (specifier == "gl_VertexID") { continue; }

    this->mAttributeVariableList.emplace_back(specifier, size, type, locId);
  }

#if defined(_DEBUG) == true
  // Output activated attirbute variable information on console and file in debug_mode.
  for (const auto& variable : this->mAttributeVariableList)
  {
    MDY_LOG_DEBUG_D("{} | Shader attribute information | Name : {} | Slotsize : {} | Location : {}",
                    this->mSpecifierName, variable.mVariableName, variable.mVariableSlotSize, variable.mVariableLocation);
  }
#endif
}

void FDyShaderResource::pStoreUniformProperties() noexcept
{
  const TU32 activatedUniformCount = FDyGLWrapper::QueryShaderProgramIV(this->mShaderProgramId, GL_ACTIVE_UNIFORMS);
  this->mUniformVariableList.reserve(activatedUniformCount);

  // Retrieve uniform variable information.
  for (TU32 i = 0; i < activatedUniformCount; ++i)
  { // If process was failed because of uniform variable is UBO, just do next thing.
    auto result = FDyGLWrapper::GetShaderProgramUniformInfo(this->mShaderProgramId, i);
    if (result.has_value() == false) { continue; }

    auto [specifier, length, size, type, locId] = result.value();
    this->mUniformVariableList.emplace_back(specifier, size, type, locId);
  }

#ifdef false
  // Output activated attirbute variable information on console and file in debug_mode.
  for (const auto& variable : this->mPlainUniformVariableLists)
  {
    MDY_LOG_DEBUG_D("{} | Shader uniform variable information | Name : {} | Slotsize : {} | Type : {} | Location : {}",
                    this->mShaderName,
                    variable.mVariableName, variable.mVariableSlotSize,
                    DyGetDebugStringOfUniformVariableType(variable.mVariableType).data(), variable.mVariableLocation);
  }
#endif
}

void FDyShaderResource::pStoreUniformBufferObjectProperties() noexcept
{
  const TU32 activatedUboCount = FDyGLWrapper::QueryShaderProgramIV(this->mShaderProgramId, GL_ACTIVE_UNIFORM_BLOCKS);
  this->mUniformBufferObjectList.reserve(activatedUboCount);

  for (TU32 i = 0; i < activatedUboCount; ++i)
  { // If process was failed, just do next thing.
    const auto optResult = FDyGLWrapper::GetShaderProgramUniformBlockInfo(this->mShaderProgramId, i);
    if (optResult.has_value() == false) { continue; }

    this->mUniformBufferObjectList.emplace_back(optResult.value());
  }

#if defined(_DEBUG) == true
  // Output activated attirbute variable information on console and file in debug_mode.
  for (const auto& variable : this->mUniformBufferObjectList)
  {
    MDY_LOG_DEBUG_D("{} | Shader UBO information | Buffer name : {}", this->mSpecifierName, variable.mUboSpecifierName);
  }
#endif
}

} /// ::dy namespace