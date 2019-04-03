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
#include <Dy/Core/Resource/Resource/FResourceShader.h>
#include <Dy/Core/Resource/Information/FInformationShader.h>
#include <Dy/Core/Rendering/Wrapper/PDyGLShaderFragmentDescriptor.h>
#include <Dy/Core/Rendering/Wrapper/XGLWrapper.h>
#include <Dy/Management/Helper/SProfilingHelper.h>
#include <Dy/Helper/MCS/GLShaderParser.h>
#include <Dy/Helper/Library/HelperRegex.h>
#include <Dy/Core/Reflection/RReflection.h>

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

FResourceShader::FResourceShader(const FInformationShader& information) 
  : mBinderShader{information.GetSpecifierName()},
    mIsSupportingInstancing{information.IsInstantiable()}
{
  this->mSpecifierName = information.GetSpecifierName();

  // Section.
  { MDY_GRAPHIC_SET_CRITICALSECITON();
    // Create shader program.
    auto optFragmentList = this->pCreateShaderFragments(information.GetShaderFragmentList());
    MDY_ASSERT_MSG(optFragmentList.has_value() == true, "OpenGL Shader Fragment compilation failed.");

    auto optShaderProgramId = this->pInitializeShaderProgram(optFragmentList.value());
    MDY_ASSERT_MSG(optShaderProgramId.has_value() == true, "OpenGL shader program compilation failed.");
    this->mShaderProgramId = optShaderProgramId.value();

    this->pDeleteShaderFragments(optFragmentList.value());

    // Get attribute, uniform, ubo information from shader program.
    this->pStoreAttributeProperties();
    this->pStoreUniformProperties();
    this->pStoreUniformBufferObjectProperties();
  }

  // Setup common uniform value list.
  this->MDY_PRIVATE(TryClearUniformList)();
  this->MDY_PRIVATE(TryConstructDefaultUniformList)(*this);

  // Profiling.
  SProfilingHelper::IncreaseOnBindShaderCount(1);
}

std::optional<TFragmentList>
FResourceShader::pCreateShaderFragments(_MIN_ const FInformationShader::TShaderFragmentList& vector)
{
  TFragmentList resultList {};

  for (const auto& fragmentInformation : vector)
  {
    PDyGLShaderFragmentDescriptor fragDesc;
    fragDesc.mType      = fragmentInformation.mShaderType;
    fragDesc.mPtrBuffer = fragmentInformation.mShaderFragmentCode.data();

    // Create shader fragment.
    TU32 fragmentId = 0;
    const auto optShaderFragmentId = XGLWrapper::CreateShaderFragment(fragDesc);
    MDY_ASSERT_MSG(optShaderFragmentId.has_value() == true, "Shader fragment compilation must be succeeded.");
    fragmentId = optShaderFragmentId.value();

    // Check shader fragment compiliation status only in debug mode.
    #if defined(NDEBUG) == false
    {
      TI32 err;
      if (glGetShaderiv(fragmentId, GL_COMPILE_STATUS, &err); !err)
      {
        DyPrintShaderErrorLog(fragmentId);
        MDY_UNEXPECTED_BRANCH_BUT_RETURN(std::nullopt);
      }
    }
    #endif

    // Insert newly compiled shader fragment for __pInitilaizeShaderProgram.
    resultList.emplace_back(fragmentInformation.mShaderType, fragmentId);
  }

  return resultList;
}

std::optional<TU32> FResourceShader::pInitializeShaderProgram(_MIN_ const TFragmentList& fragmentList)
{
  std::optional<TU32> optProgramId = XGLWrapper::CreateShaderProgram(fragmentList);
  MDY_ASSERT_MSG(optProgramId.has_value() == true, "Unexpected error occurred.");

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

void FResourceShader::pDeleteShaderFragments(const TFragmentList& fragmentList)
{
  for (const auto& [_, validFragmentId] : fragmentList)
  {
    XGLWrapper::DeleteShaderFragment(validFragmentId);
  }
}

void FResourceShader::pStoreAttributeProperties() noexcept
{
  const TU32 activatedAttrCount = XGLWrapper::QueryShaderProgramIV(this->mShaderProgramId, GL_ACTIVE_ATTRIBUTES);
  this->mAttributeVariableList.reserve(activatedAttrCount);

  // Retrieve attirbute variable information.
  for (TU32 i = 0; i < activatedAttrCount; ++i)
  {
    auto result = XGLWrapper::GetShaderProgramAttributeInfo(this->mShaderProgramId, i);
    MDY_ASSERT_MSG(result.has_value() == true, "Unexpected error occurred.");

    auto [specifier, length, size, type, locId] = result.value();
    if (specifier == "gl_VertexID") { continue; }

    this->mAttributeVariableList.emplace_back(specifier, size, type, locId);
  }

#if defined(_DEBUG) == true
  // Output activated attirbute variable information on console and file in debug_mode.
  for (const auto& variable : this->mAttributeVariableList)
  {
    DyPushLogDebugDebug("{} | Shader attribute information | Name : {} | Slotsize : {} | Location : {}",
                    this->mSpecifierName, variable.mVariableName, variable.mVariableSlotSize, variable.mVariableLocation);
  }
#endif
}

void FResourceShader::pStoreUniformProperties() noexcept
{
  const TU32 activatedUniformCount = 
    XGLWrapper::QueryShaderProgramIV(this->mShaderProgramId, GL_ACTIVE_UNIFORMS);
  this->mUniformVariableList.reserve(activatedUniformCount);

  // Retrieve uniform variable information.
  for (TU32 i = 0; i < activatedUniformCount; ++i)
  { // If process was failed because of uniform variable is UBO, just do next thing.
    auto result = XGLWrapper::GetShaderProgramUniformInfo(this->mShaderProgramId, i);
    if (result.has_value() == false) { continue; }

    auto [specifier, length, size, type, locId] = result.value();

    // Check whether this is structurized uniform variable specifier.
    static MDY_SET_IMMUTABLE_STRING(kStructurized, R"dy((\w+)[\[\d+\]]*[.](\w+))dy");
    if (regex::IsMatched(specifier, kStructurized) == true)
    {
      // If regex is matched to array version, 
      static MDY_SET_IMMUTABLE_STRING(kArrayVersion, R"dy((\w+)\[(\d+)\]*[.](\w+))dy");
      if (regex::IsMatched(specifier, kArrayVersion) == true)
      {
        const auto optValue = regex::GetMatchedKeywordFrom(specifier, kArrayVersion);
        const auto prefix   = (*optValue)[0];
        const auto number   = static_cast<size_t>(std::stoi((*optValue)[1]));
        const auto postfix  = (*optValue)[2];
        MDY_ASSERT_FORCE(reflect::RUniformReflection::IsSubNameExist(prefix) == true);

        // Check prefix structure is in map.
        if (Contains(this->mUniformStructVarListMap, prefix) == false)
        {
          this->mUniformStructVarListMap.try_emplace(
            prefix, 
            std::pair(prefix, std::vector<DUniformStructVarInformation>{})
          );
        }

        // Resize of structure item.
        auto& [_, uniformStructVarList] = this->mUniformStructVarListMap[prefix];
        if (number >= uniformStructVarList.size()) 
        { 
          uniformStructVarList.resize(number + 1); 
        }

        // Verification check.
        const auto& reflectData = reflect::RUniformReflection::GetDataOfSubName(prefix);
        MDY_ASSERT_FORCE(reflectData.IsAliasExist(postfix) == true);
        const auto reflectType = reflectData.GetTypeOf(reflectData.GetVarNameOf(postfix));
        // Structure arrayed each component type should be converted to item type.
        type = ToUniformItemType(type);
        MDY_ASSERT_FORCE(reflect::ToUniformVariableType(reflectType) == type);

        uniformStructVarList[number].mMemberValues.emplace_back(postfix, size, type, locId);
      }
      else
      {
        const auto optValue = regex::GetMatchedKeywordFrom(specifier, kStructurized);
        const auto prefix   = (*optValue)[0];
        const auto postfix  = (*optValue)[1];

        // Check prefix structure is in map.
        if (Contains(this->mUniformStructVarItemMap, prefix) == false)
        {
          this->mUniformStructVarItemMap.try_emplace(
            prefix, 
            std::pair(prefix, DUniformStructVarInformation{})
          );
        }
        auto& [_, uniformStructVarList] = this->mUniformStructVarItemMap[prefix];

        // Verification check.
        const auto& reflectData = reflect::RUniformReflection::GetDataOfSubName(prefix);
        MDY_ASSERT_FORCE(reflectData.IsAliasExist(postfix) == true);
        const auto reflectType = reflectData.GetTypeOf(reflectData.GetVarNameOf(postfix));
        MDY_ASSERT_FORCE(reflect::ToUniformVariableType(reflectType) == type);

        uniformStructVarList.mMemberValues.emplace_back(postfix, size, type, locId);
      }
    }
    else
    {
      this->mUniformVariableList.emplace_back(specifier, size, type, locId);
    }
  }

#ifdef false
  // Output activated attirbute variable information on console and file in debug_mode.
  for (const auto& variable : this->mPlainUniformVariableLists)
  {
    DyPushLogDebugDebug(
      "{} | Shader uniform variable information | Name : {} | Slotsize : {} | Type : {} | Location : {}",
      this->mShaderName,
      variable.mVariableName, variable.mVariableSlotSize,
      DyGetDebugStringOfUniformVariableType(variable.mVariableType).data(), variable.mVariableLocation);
  }
#endif
}

void FResourceShader::pStoreUniformBufferObjectProperties() noexcept
{
  const TU32 activatedUboCount = XGLWrapper::QueryShaderProgramIV(this->mShaderProgramId, GL_ACTIVE_UNIFORM_BLOCKS);
  this->mUniformBufferObjectList.reserve(activatedUboCount);

  for (TU32 i = 0; i < activatedUboCount; ++i)
  { // If process was failed, just do next thing.
    std::optional<std::string> optResult = XGLWrapper::GetShaderProgramUniformBlockInfo(this->mShaderProgramId, i);
    if (optResult.has_value() == false) { continue; }

    this->mUniformBufferObjectList.emplace_back(optResult.value());
  }

#if defined(_DEBUG) == true
  // Output activated attirbute variable information on console and file in debug_mode.
  for (const auto& variable : this->mUniformBufferObjectList)
  {
    DyPushLogDebugDebug(
      "{} | Shader UBO information | Buffer name : {}", 
      this->mSpecifierName, variable.mUboSpecifierName);
  }
#endif
}

FResourceShader::~FResourceShader()
{
  { MDY_GRAPHIC_SET_CRITICALSECITON();
    XGLWrapper::DeleteShaderProgram(this->mShaderProgramId);
  }
  SProfilingHelper::DecreaseOnBindShaderCount(1);
}

TU32 FResourceShader::GetShaderProgramId() const noexcept
{
  return this->mShaderProgramId;
}

void FResourceShader::UseShader() const noexcept
{
  MDY_ASSERT_MSG(this->mShaderProgramId > 0, "Shader program must be valid.");
  XGLWrapper::UseShaderProgram(this->mShaderProgramId);
}

void FResourceShader::DisuseShader() const noexcept
{
  XGLWrapper::DisuseShaderProgram();
}

const std::vector<DUniformVariableInformation>& 
FResourceShader::GetUniformVariableList() const noexcept
{
  return this->mUniformVariableList;
}

const FResourceShader::TUniformStructListMap& 
FResourceShader::GetUniformStructListMap() const noexcept
{
  return this->mUniformStructVarListMap;
}

const FResourceShader::TUniformStructItemMap& 
FResourceShader::GetUniformStructItemMap() const noexcept
{
  return this->mUniformStructVarItemMap;
}

bool FResourceShader::IsSupportingInstancing() const noexcept
{
  return this->mIsSupportingInstancing;
}

} /// ::dy namespace