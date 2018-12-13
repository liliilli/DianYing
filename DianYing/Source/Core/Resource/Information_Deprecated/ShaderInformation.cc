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
#include <Dy/Core/Resource/Information_Deprecated/ShaderInformation_Deprecated.h>
#include <Dy/Core/Resource/Resource_Deprecated/ShaderResource_Deprecated.h>
#include <Dy/Meta/Information/GLShaderMetaInformation.h>
#include <Dy/Management/LoggingManager.h>

namespace
{

MDY_SET_IMMUTABLE_STRING(kShaderInformationTemplate,    "{} | Shader information {} : {}");
MDY_SET_IMMUTABLE_STRING(kShaderFragmentTemplate,       "{} | Shader fragment | Shader file path : {} | Shader fragment type : {}");
MDY_SET_IMMUTABLE_STRING(kShaderInformationBindTo,      "{} | Shader information binds to {}");
MDY_SET_IMMUTABLE_STRING(kDyDataInformation,            "DDyShaderInformation_Deprecated");

///
/// @brief Get immutable string by reading EDyShaderFragmentType type.
///
std::string_view DyGetShaderFragmentTypeStringFrom(dy::EDyShaderFragmentType type) noexcept
{
  switch (type)
  {
  case dy::EDyShaderFragmentType::Vertex:  return "Vertex shader";
  case dy::EDyShaderFragmentType::Hull:    return "Hull shader";
  case dy::EDyShaderFragmentType::Domain:  return "Domain shader";
  case dy::EDyShaderFragmentType::Geometry:return "Geometry shader";
  case dy::EDyShaderFragmentType::Pixel:   return "Pixel shader";
  default: return "Error";
  }
}

} /// ::unnamed namespace

namespace dy
{

DDyShaderInformation_Deprecated::DDyShaderInformation_Deprecated(const PDyShaderConstructionDescriptor& shaderConstructionDescriptor) :
    mShaderInformation{shaderConstructionDescriptor}
{
  // Copy or move information from descriptor.
  MDY_LOG_INFO_D(kShaderInformationTemplate, kDyDataInformation, "name", this->mShaderInformation.mShaderName);

  for (const auto& fragment : this->mShaderInformation.mShaderFragments)
  {
    MDY_LOG_INFO_D(kShaderFragmentTemplate, kDyDataInformation,
                   fragment.mShaderPath_Deprecated, DyGetShaderFragmentTypeStringFrom(fragment.mShaderType).data());
  }
}

DDyShaderInformation_Deprecated::DDyShaderInformation_Deprecated(const PDyGLShaderInstanceMetaInfo& shaderMetaInfo)
{
  PDyShaderConstructionDescriptor info = {};
  info.mShaderName = shaderMetaInfo.mSpecifierName;
  for (int i = 0; i < 6; ++i)
  {
    PDyShaderFragmentInformation shader{};
    shader.mIsEnabledRawLoadShaderCode_Deprecated = shaderMetaInfo.mSourceType == EDyResourceSource::Builtin ? true : false;
    const auto& p = shaderMetaInfo.GetFragment(static_cast<EDyShaderFragmentType>(i));
    if (p.mExternalFilePath.empty() == true && p.mBuiltinBuffer.empty() == true) { continue; }
    shader.mShaderPath_Deprecated    = p.mExternalFilePath;
    shader.mShaderFragmentCode = p.mBuiltinBuffer;
    shader.mShaderType    = static_cast<EDyShaderFragmentType>(i);

    info.mShaderFragments.push_back(shader);
  }

  // Copy or move information from descriptor.
  this->mShaderInformation = info;
  MDY_LOG_INFO_D(kShaderInformationTemplate, kDyDataInformation, "name", this->mShaderInformation.mShaderName);

  for (const auto& fragment : this->mShaderInformation.mShaderFragments)
  {
    MDY_LOG_INFO_D(kShaderFragmentTemplate, kDyDataInformation,
      fragment.mShaderPath_Deprecated, DyGetShaderFragmentTypeStringFrom(fragment.mShaderType).data());
  }
}

DDyShaderInformation_Deprecated::~DDyShaderInformation_Deprecated()
{
  MDY_LOG_INFO_D(kShaderInformationTemplate, "~DDyShaderInformation_Deprecated", "name", this->mShaderInformation.mShaderName);

  if (this->__mLinkedShaderResourcePtr) { this->__mLinkedShaderResourcePtr->__pfResetShaderInformationLink(); }
}

void DDyShaderInformation_Deprecated::__pfSetShaderResourceLink(NotNull<CDyShaderResource_Deprecated*> ptr) const noexcept
{
  MDY_LOG_DEBUG_D(kShaderInformationBindTo, "__pfSetShaderResourceLink", reinterpret_cast<std::ptrdiff_t>(ptr.Get()));
  this->__mLinkedShaderResourcePtr = ptr;
}

} /// ::dy namespace