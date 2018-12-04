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
#include <Dy/Core/Resource/Resource_Deprecated/ShaderResource_Deprecated.h>

#include <Dy/Helper/IoHelper.h>
#include <Dy/Core/Resource/Information_Deprecated/ShaderInformation_Deprecated.h>
#include <Dy/Core/Resource/Resource_Deprecated/MaterialResource_Deprecated.h>
#include <Dy/Management/LoggingManager.h>


//!
//! Only this translate unit function.
//!

namespace
{

///
/// @brief Retrieve immutable debug string (string_view) for attribute variable type. (GL)
///
[[nodiscard]] std::string_view DyGetDebugStringOfAttributeVariableType(dy::EDyAttributeVariableType type)
{
  switch (type)
  {
  case dy::EDyAttributeVariableType::Matrix4:   return "Real Matrix4x4";
  case dy::EDyAttributeVariableType::Matrix3:   return "Real Matrix3x3";
  case dy::EDyAttributeVariableType::Matrix2:   return "Real Matrix2x2";
  case dy::EDyAttributeVariableType::Vector4:   return "Real Vector4";
  case dy::EDyAttributeVariableType::Vector3:   return "Real Vector3";
  case dy::EDyAttributeVariableType::Vector2:   return "Real Vector2";
  case dy::EDyAttributeVariableType::IVec4:     return "Integer Vector4";
  case dy::EDyAttributeVariableType::IVec3:     return "Integer Vector3";
  case dy::EDyAttributeVariableType::IVec2:     return "Integer Vector2";
  case dy::EDyAttributeVariableType::Integer:   return "Interger1";
  case dy::EDyAttributeVariableType::Float:     return "Float1";
  case dy::EDyAttributeVariableType::NoneError: return "None";
  default: return "Error";
  }
}

///
/// @brief Retrieve immutable debug string (string_view) for uniform variable type. (GL)
///
[[nodiscard]] std::string_view DyGetDebugStringOfUniformVariableType(dy::EDyUniformVariableType type)
{
  switch (type)
  {
  case dy::EDyUniformVariableType::Matrix4:   return "Real Matrix4x4";
  case dy::EDyUniformVariableType::Matrix3:   return "Real Matrix3x3";
  case dy::EDyUniformVariableType::Matrix2:   return "Real Matrix2x2";
  case dy::EDyUniformVariableType::Vector4:   return "Real Vector4";
  case dy::EDyUniformVariableType::Vector3:   return "Real Vector3";
  case dy::EDyUniformVariableType::Vector2:   return "Real Vector2";
  case dy::EDyUniformVariableType::IVec4:     return "Integer Vector4";
  case dy::EDyUniformVariableType::IVec3:     return "Integer Vector3";
  case dy::EDyUniformVariableType::IVec2:     return "Integer Vector2";
  case dy::EDyUniformVariableType::Integer:   return "Interger1";
  case dy::EDyUniformVariableType::Float:     return "Float1";
  case dy::EDyUniformVariableType::NoneError: return "None";
  case dy::EDyUniformVariableType::IntegerPointer:  return "Integer Ptr";
  case dy::EDyUniformVariableType::FloatPointer:    return "Float Ptr";
  case dy::EDyUniformVariableType::Texture1D:       return "Texture 1D";
  case dy::EDyUniformVariableType::Texture2D:       return "Texture 2D";
  default: return "Error";
  }
}

} /// ::unnamed namespace

//!
//! Implementation
//!

namespace {

void DyPrintShaderErrorLog(GLuint shaderFragmentId) {
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

void DyPrintShaderProgramErrorLog(GLuint shaderProgramId) {
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

dy::EDyAttributeVariableType DyGlGetAttributeVariableTypeFrom(GLenum type) noexcept
{
  switch (type)
  {
  case GL_BOOL:                             return dy::EDyAttributeVariableType::Bool;
  case GL_FLOAT:      case GL_DOUBLE:       return dy::EDyAttributeVariableType::Float;
  case GL_FLOAT_VEC2: case GL_DOUBLE_VEC2:  return dy::EDyAttributeVariableType::Vector2;
  case GL_FLOAT_VEC3: case GL_DOUBLE_VEC3:  return dy::EDyAttributeVariableType::Vector3;
  case GL_FLOAT_VEC4: case GL_DOUBLE_VEC4:  return dy::EDyAttributeVariableType::Vector4;
  case GL_FLOAT_MAT2: case GL_DOUBLE_MAT2:  return dy::EDyAttributeVariableType::Matrix2;
  case GL_FLOAT_MAT3: case GL_DOUBLE_MAT3:  return dy::EDyAttributeVariableType::Matrix3;
  case GL_FLOAT_MAT4: case GL_DOUBLE_MAT4:  return dy::EDyAttributeVariableType::Matrix4;
  case GL_INT:                              return dy::EDyAttributeVariableType::Integer;
  case GL_INT_VEC2:                         return dy::EDyAttributeVariableType::IVec2;
  case GL_INT_VEC3:                         return dy::EDyAttributeVariableType::IVec3;
  case GL_INT_VEC4:                         return dy::EDyAttributeVariableType::IVec4;
  default: return dy::EDyAttributeVariableType::NoneError;
  }
}

dy::EDyUniformVariableType DyGlGetUniformVariableTypeFrom(GLenum type) noexcept
{
  switch (type)
  {
  case GL_BOOL:                             return dy::EDyUniformVariableType::Bool;
  case GL_FLOAT:      case GL_DOUBLE:       return dy::EDyUniformVariableType::Float;
  case GL_FLOAT_VEC2: case GL_DOUBLE_VEC2:  return dy::EDyUniformVariableType::Vector2;
  case GL_FLOAT_VEC3: case GL_DOUBLE_VEC3:  return dy::EDyUniformVariableType::Vector3;
  case GL_FLOAT_VEC4: case GL_DOUBLE_VEC4:  return dy::EDyUniformVariableType::Vector4;
  case GL_FLOAT_MAT2: case GL_DOUBLE_MAT2:  return dy::EDyUniformVariableType::Matrix2;
  case GL_FLOAT_MAT3: case GL_DOUBLE_MAT3:  return dy::EDyUniformVariableType::Matrix3;
  case GL_FLOAT_MAT4: case GL_DOUBLE_MAT4:  return dy::EDyUniformVariableType::Matrix4;
  case GL_INT:                              return dy::EDyUniformVariableType::Integer;
  case GL_INT_VEC2:                         return dy::EDyUniformVariableType::IVec2;
  case GL_INT_VEC3:                         return dy::EDyUniformVariableType::IVec3;
  case GL_INT_VEC4:                         return dy::EDyUniformVariableType::IVec4;
  case GL_SAMPLER_1D:                       return dy::EDyUniformVariableType::Texture1D;
  case GL_SAMPLER_2D:                       return dy::EDyUniformVariableType::Texture2D;
  case GL_SAMPLER_2D_ARRAY:                 return dy::EDyUniformVariableType::Texture2DArray;
  default: return dy::EDyUniformVariableType::NoneError;
  }
}

} /// unnamed namespace

namespace dy
{

CDyShaderResource_Deprecated::~CDyShaderResource_Deprecated()
{
  // Release heap resources
  if (this->mShaderProgramId) { glDeleteProgram(this->mShaderProgramId); }

  // Unbind previous and next level.
  if (this->__mLinkedShaderInformationPtr) { this->__mLinkedShaderInformationPtr->__pfResetShaderResourceLink(); }

  for (auto& [notUsed, materialPtr] : __mLinkedMaterialResourcePtrs)
  {
    materialPtr->__pfResetShaderResource();
  }
}

EDySuccess CDyShaderResource_Deprecated::pfInitializeResource(const DDyShaderInformation_Deprecated& shaderInformation)
{
  const auto& information = shaderInformation.GetInformation();
  this->mShaderName = information.mShaderName;

  std::vector<std::pair<EDyShaderFragmentType, uint32_t>> shaderFragmentIdList;
  // If failured, release resources.
  if (this->__pInitializeShaderFragments(information, shaderFragmentIdList) == DY_FAILURE)
  {
    return DY_FAILURE;
  }
  if (this->__pInitializeShaderProgram(shaderFragmentIdList) == DY_FAILURE)
  {
    return DY_FAILURE;
  }
  if (this->__pStoreAttributePropertiesOfProgram() == DY_FAILURE)
  {
    return DY_FAILURE;
  }
  if (this->__pStoreConstantUniformPropertiesOfProgram() == DY_FAILURE)
  {
    return DY_FAILURE;
  }
  if (this->__pStoreUniformBufferObjectPropertiesOfProgram() == DY_FAILURE)
  {
    return DY_FAILURE;
  }

  return DY_SUCCESS;
}

EDySuccess CDyShaderResource_Deprecated::__pInitializeShaderFragments(
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
    case EDyShaderFragmentType::Vertex:                 shaderFragmentId = glCreateShader(GL_VERTEX_SHADER);          break;
    case EDyShaderFragmentType::Hull:     shaderFragmentId = glCreateShader(GL_TESS_CONTROL_SHADER);    break;
    case EDyShaderFragmentType::Domain:  shaderFragmentId = glCreateShader(GL_TESS_EVALUATION_SHADER); break;
    case EDyShaderFragmentType::Geometry:               shaderFragmentId = glCreateShader(GL_GEOMETRY_SHADER);        break;
    case EDyShaderFragmentType::Pixel:                  shaderFragmentId = glCreateShader(GL_FRAGMENT_SHADER);        break;
    }

    // If shader creation had failed, revert and release all shader fragments.
    if (shaderFragmentId <= 0)
    {
      for (auto& [type, compiledShaderId] : shaderFragmentIdList) { glDeleteShader(compiledShaderId); }
      return DY_FAILURE;
    }

    // Read shader fragment code. if something is wrong, revert and release all shader fragments.
    if (!fragmentInformation.mIsEnabledRawLoadShaderCode)
    { // If must read from external shader file, let it do.
      const auto opShaderFragmentCode = DyReadBinaryFileAll(fragmentInformation.mShaderPath);
      if (!opShaderFragmentCode.has_value())
      {
        // @todo output error message;
        glDeleteShader(shaderFragmentId);
        for (auto&[type, compiledShaderId] : shaderFragmentIdList) { glDeleteShader(compiledShaderId); }
        return DY_FAILURE;
      }

      // Compile shader fragment.
      {
        const auto& codeString = opShaderFragmentCode.value();
        const char* codePtr = codeString.data();
        glShaderSource(shaderFragmentId, 1, &codePtr, nullptr);
        glCompileShader(shaderFragmentId);
      }
    }
    else
    { // If must read from internal string code, let it do.
      const auto codePtr = fragmentInformation.mShaderRawCode.data();
      glShaderSource(shaderFragmentId, 1, &codePtr, nullptr);
      glCompileShader(shaderFragmentId);
    }

#if !defined(NDEBUG)
    // Check shader fragment compiliation status only in debug mode.
    {
      int32_t err;
      if (glGetShaderiv(shaderFragmentId, GL_COMPILE_STATUS, &err); !err)
      {
        DyPrintShaderErrorLog(shaderFragmentId);
        glDeleteShader(shaderFragmentId);
        for (auto& [type, compiledShaderId] : shaderFragmentIdList) { glDeleteShader(compiledShaderId); }
        return DY_FAILURE;
      }
    }
#endif

    // Insert newly compiled shader fragment for __pInitilaizeShaderProgram.
    shaderFragmentIdList.emplace_back(fragmentInformation.mShaderType, shaderFragmentId);
  }

  return DY_SUCCESS;
}

EDySuccess CDyShaderResource_Deprecated::__pInitializeShaderProgram(const std::vector<std::pair<EDyShaderFragmentType, uint32_t>>& shaderFragmentIdList)
{
  // Create GlShaderProgram, and attach fragments to link them.
  this->mShaderProgramId = glCreateProgram();
  for (auto& [shaderFragmentType, shaderFragmentId] : shaderFragmentIdList) {
    glAttachShader(this->mShaderProgramId, shaderFragmentId);
  }
  glLinkProgram(this->mShaderProgramId);
  for (auto& [shaderFragmentType, shaderFragmentId] : shaderFragmentIdList) {
    glDetachShader(this->mShaderProgramId, shaderFragmentId);
    glDeleteShader(shaderFragmentId);
  }

  // Check shader program linking status only in debug mode.
#if !defined(NDEBUG)
  {
    GLint isLinked = 0;
    glGetProgramiv(this->mShaderProgramId, GL_LINK_STATUS, &isLinked);
    if (!isLinked) {
      DyPrintShaderProgramErrorLog(this->mShaderProgramId);
      return DY_FAILURE;
    }
  }
#endif

  return DY_SUCCESS;
}

EDySuccess CDyShaderResource_Deprecated::__pStoreAttributePropertiesOfProgram() noexcept
{
  // GL(native) code
  int32_t activatedAttributeCount = 0;
  glGetProgramiv(this->mShaderProgramId, GL_ACTIVE_ATTRIBUTES, &activatedAttributeCount);
  int32_t attributeBufferLength = 0;
  glGetProgramiv(this->mShaderProgramId, GL_ACTIVE_ATTRIBUTE_MAX_LENGTH, &attributeBufferLength);
  this->mAttributeVariableLists.resize(activatedAttributeCount);

  // Retrieve attirbute variable information.
  GLchar* attributeName = new GLchar[attributeBufferLength];
  std::memset(attributeName, '\0', attributeBufferLength);
  for (int32_t i = 0; i < activatedAttributeCount; ++i)
  {
    GLsizei attributelength = 0;
    GLint   attributeSize = 0;
    GLenum  attributeType = GL_NONE;
    glGetActiveAttrib(this->mShaderProgramId, i, attributeBufferLength, &attributelength, &attributeSize, &attributeType, attributeName);
    if (std::strcmp(attributeName, "gl_VertexID") == 0) { continue; }
    const auto attributeLocation = glGetAttribLocation(this->mShaderProgramId, attributeName);
    // Output log of attribute variables.
    const auto storeType = DyGlGetAttributeVariableTypeFrom(attributeType);

    // Integrity check.
    if (storeType == EDyAttributeVariableType::NoneError)
    {
      MDY_LOG_CRITICAL_D("{} | Failed to retrieve attribute information. Shader name : {}, Attribute varaible name : {}",
                         "CDyShaderResource_Deprecated::__pStoreAttributePropertiesOfProgram", this->mShaderName, attributeName);
      this->mAttributeVariableLists.clear();
      return DY_FAILURE;
    }
    if (attributeLocation < 0)
    {
      MDY_LOG_CRITICAL_D("{} | Failed to retrieve attribute location. Shader name : {} | Attribute Variable Name : {}",
                         "CDyShaderResource_Deprecated::__pStoreAttributePropertiesOfProgram", this->mShaderName, attributeLocation);
      this->mAttributeVariableLists.clear();
      return DY_FAILURE;
    }

    this->mAttributeVariableLists[i].mVariableName = attributeName;
    this->mAttributeVariableLists[i].mVariableSlotSize = attributeSize;
    this->mAttributeVariableLists[i].mVariableType = storeType;
    this->mAttributeVariableLists[i].mVariableLocation = attributeLocation;
  }
  delete[] attributeName;

  // Output activated attirbute variable information on console and file in debug_mode.
  for (const auto& variable : this->mAttributeVariableLists)
  {
    MDY_LOG_DEBUG_D("{} | Shader attribute information | Name : {} | Slotsize : {} | Type : {} | Location : {}",
                    this->mShaderName,
                    variable.mVariableName, variable.mVariableSlotSize,
                    DyGetDebugStringOfAttributeVariableType(variable.mVariableType).data(), variable.mVariableLocation);
  }
  return DY_SUCCESS;
}

EDySuccess CDyShaderResource_Deprecated::__pStoreConstantUniformPropertiesOfProgram() noexcept
{
  // GL(native) code
  int32_t activatedUniformCount = 0;
  glGetProgramiv(this->mShaderProgramId, GL_ACTIVE_UNIFORMS, &activatedUniformCount);
  int32_t uniformBufferLength = 0;
  glGetProgramiv(this->mShaderProgramId, GL_ACTIVE_UNIFORM_MAX_LENGTH, &uniformBufferLength);
  this->mPlainUniformVariableLists.resize(activatedUniformCount);

  // Retrieve uniform variable information.
  GLchar* uniformName = new GLchar[uniformBufferLength];
  std::memset(uniformName, '\0', uniformBufferLength);
  for (int32_t i = 0; i < activatedUniformCount; ++i)
  {
    GLsizei uniformLength = 0;
    GLint   uniformSize = 0;
    GLenum  glUniformType = GL_NONE;
    glGetActiveUniform(this->mShaderProgramId, i, uniformBufferLength, &uniformLength, &uniformSize, &glUniformType, uniformName);
    // -1 When uniform buffer object member variable is detected.
    // We must check uniform buffer object is exist, or error.
    const auto uniformLocation = glGetUniformLocation(this->mShaderProgramId, uniformName);
    // Output log of attribute variables.
    const auto storeType = DyGlGetUniformVariableTypeFrom(glUniformType);

    // Integrity check.
    if (storeType == EDyUniformVariableType::NoneError)
    {
      MDY_LOG_CRITICAL_D("{} | Failed to retrieve uniform information. Shader name : {}, Uniform variable name : {}",
                         "CDyShaderResource_Deprecated::__pStoreAttributePropertiesOfProgram", this->mShaderName, uniformName);
      this->mPlainUniformVariableLists.clear();
      return DY_FAILURE;
    }
    if (uniformLocation < 0)
    {
      const GLchar* uboNameLastPtr = std::strchr(uniformName, '.');
      if (MDY_CHECK_ISNULL(uboNameLastPtr))
      {
        MDY_LOG_CRITICAL_D("{} | Failed to retrieve uniform variable location. Shader name : {}, Uniform variable name : {}",
                           "CDyShaderResource_Deprecated::__pStoreAttributePropertiesOfProgram", this->mShaderName, uniformName);
        this->mPlainUniformVariableLists.clear();
        return DY_FAILURE;
      }
      // If UBO is exist, just continue until finding next plain uniform variable.
      continue;
    }

    this->mPlainUniformVariableLists[i].mVariableName = uniformName;
    this->mPlainUniformVariableLists[i].mVariableSlotSize = uniformSize;
    this->mPlainUniformVariableLists[i].mVariableType = storeType;
    this->mPlainUniformVariableLists[i].mVariableLocation = uniformLocation;
  }
  delete[] uniformName;

  // Output activated attirbute variable information on console and file in debug_mode.
  for (const auto& variable : this->mPlainUniformVariableLists)
  {
    MDY_LOG_DEBUG_D("{} | Shader uniform variable information | Name : {} | Slotsize : {} | Type : {} | Location : {}",
                    this->mShaderName,
                    variable.mVariableName, variable.mVariableSlotSize,
                    DyGetDebugStringOfUniformVariableType(variable.mVariableType).data(), variable.mVariableLocation);
  }
  return DY_SUCCESS;
}

EDySuccess CDyShaderResource_Deprecated::__pStoreUniformBufferObjectPropertiesOfProgram() noexcept
{
  // GL(native) code
  TI32 activatedUboCount = 0;
  glGetProgramiv(this->mShaderProgramId, GL_ACTIVE_UNIFORM_BLOCKS, &activatedUboCount);
  this->mUniformBufferObjectList.resize(activatedUboCount);

  TI32 uboNameMaxLength = 0;
  glGetProgramiv(this->mShaderProgramId, GL_ACTIVE_UNIFORM_BLOCK_MAX_NAME_LENGTH, &uboNameMaxLength);

  // Retrieve uniform variable information.
  GLchar* uniformName = new GLchar[uboNameMaxLength];
  std::memset(uniformName, '\0', uboNameMaxLength);
  for (int32_t i = 0; i < activatedUboCount; ++i)
  {
    GLsizei uniformLength = 0;
    glGetActiveUniformBlockName(this->mShaderProgramId, i, uboNameMaxLength, &uniformLength, uniformName);

    if (uniformLength <= 0) { continue; }
    this->mUniformBufferObjectList[i].mUboSpecifierName = uniformName;
  }
  delete[] uniformName;

  // Output activated attirbute variable information on console and file in debug_mode.
  for (const auto& variable : this->mUniformBufferObjectList)
  {
    MDY_LOG_DEBUG_D("{} | Shader UBO information | Buffer name : {}", this->mShaderName, variable.mUboSpecifierName);
  }
  return DY_SUCCESS;
}

void CDyShaderResource_Deprecated::__pfSetMaterialResourceLink(CDyMaterialResource_Deprecated* ptr) const noexcept
{
  auto [it, result] = this->__mLinkedMaterialResourcePtrs.try_emplace(ptr, ptr);
  if (!result) {
    MDY_LOG_CRITICAL_D("{} | Unexpected error occurred. | Shader Name : {}", "CDyShaderResource_Deprecated::__pfSetMaterialResourceLink", this->mShaderName);
    MDY_UNEXPECTED_BRANCH();
  }
}

void CDyShaderResource_Deprecated::UseShader() noexcept
{
  glUseProgram(mShaderProgramId);
}

void CDyShaderResource_Deprecated::UpdateUniformVariables()
{
  MDY_NOT_IMPLEMENTED_ASSERT();
}

void CDyShaderResource_Deprecated::UnuseShader() noexcept
{
  glUseProgram(0);
}

} /// ::dy namespace
