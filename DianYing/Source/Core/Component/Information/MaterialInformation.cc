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
/// @TODO IMPLEMENT TRANSLUCENT AND CUSTOM BLEND MODE.
///

/// Header file
#include <Dy/Core/Component/Information/MaterialInformation.h>
#include <Dy/Core/Component/Resource/MaterialResource.h>
#include <Dy/Management/LoggingManager.h>

namespace
{

MDY_SET_IMMUTABLE_STRING(kMaterialInformationTemplate, "{} | Material information {} : {}");
MDY_SET_IMMUTABLE_STRING(kMaterialInformation,         "DDyMaterialInformation");
MDY_SET_IMMUTABLE_STRING(kMaterialNotSupport,          "{} | Does Not support now. | {} : {} | {} : {} |");
MDY_SET_IMMUTABLE_STRING(kBlendMode,    "Blend mode");
MDY_SET_IMMUTABLE_STRING(kMaterialName, "Material name");
MDY_SET_IMMUTABLE_STRING(kShaderName,   "Shader name");
MDY_SET_IMMUTABLE_STRING(kOpaque,       "Opaque");
MDY_SET_IMMUTABLE_STRING(kTranslucent,  "Translucent");
MDY_SET_IMMUTABLE_STRING(kCustom,       "Custom");

} /// ::unnamed namespace

namespace dy
{
DDyMaterialInformation::DDyMaterialInformation(const PDyMaterialInstanceMetaInfo& materialConstructionDescriptor) :
    mMaterialInformation{materialConstructionDescriptor}
{
  // Set and output log for this information descriptor properties.
  MDY_LOG_INFO_D(kMaterialInformationTemplate, kMaterialInformation, kMaterialName, this->mMaterialInformation.mSpecifierName);
  MDY_LOG_INFO_D(kMaterialInformationTemplate, kMaterialInformation, kShaderName, this->mMaterialInformation.mShaderSpecifier);

  switch (this->mMaterialInformation.mBlendMode)
  {
  case EDyMaterialBlendMode::Opaque:
    MDY_LOG_INFO_D(kMaterialInformationTemplate, kMaterialInformation, kBlendMode, kOpaque);
    break;
  case EDyMaterialBlendMode::Translucent:
    {
      MDY_LOG_INFO_D(kMaterialInformationTemplate, kMaterialInformation, kBlendMode, kTranslucent);
      MDY_LOG_ERROR(kMaterialNotSupport, kMaterialInformation,
                    kMaterialName, this->mMaterialInformation.mSpecifierName, kBlendMode, kTranslucent);

      this->mMaterialInformation.mBlendMode = EDyMaterialBlendMode::Opaque;
    }
    break;
  case EDyMaterialBlendMode::Custom:
    {
      MDY_LOG_INFO_D(kMaterialInformationTemplate, kMaterialInformation, kBlendMode, kCustom);
      MDY_LOG_ERROR(kMaterialNotSupport, kMaterialInformation,
                    kMaterialName, this->mMaterialInformation.mSpecifierName, kBlendMode, kCustom);

      this->mMaterialInformation.mBlendMode = EDyMaterialBlendMode::Opaque;
    }
    break;
  default: MDY_UNEXPECTED_BRANCH(); break;
  }

  for (const auto& textureName : this->mMaterialInformation.mTextureNames_Deprecated)
  {
    MDY_LOG_INFO_D(kMaterialInformationTemplate, kMaterialInformation, "bind texture name", textureName);
  }
}

DDyMaterialInformation::~DDyMaterialInformation()
{
  MDY_LOG_INFO_D(kMaterialInformationTemplate, kMaterialInformation, kMaterialName, this->mMaterialInformation.mSpecifierName);

  if (__mLinkedMaterialResourcePtr) { __mLinkedMaterialResourcePtr->__pfResetMaterialInformationLink(); }
}

std::string DDyMaterialInformation::ToString()
{
  return this->mMaterialInformation.ToString();
}

int32_t DDyMaterialInformation::__pfEnrollAndGetNextDerivedMaterialIndex(const std::string& name) const noexcept
{
  if (auto it = this->__mPopulatedMaterialIndexMap.find(name); it == this->__mPopulatedMaterialIndexMap.end())
  {
    this->__mPopulatedMaterialIndexMap.emplace(name, 0);
    return 0;
  }
  else return (++it->second);
}

std::unique_ptr<DDyMaterialInformation>
DDyMaterialInformation::__pfPopulateMaterialWith(const PDyMaterialPopulateDescriptor& desc) const noexcept
{
  MDY_LOG_INFO_D("Populate derived material information {} from {}.",
      desc.mMaterialOverrideName,
      this->mMaterialInformation.mSpecifierName
  );

  auto newDesc = this->mMaterialInformation;
  {
    newDesc.mSpecifierName  = desc.mMaterialOverrideName;
    newDesc.mShaderSpecifier    = desc.mOverrideShaderName;
  }

  return std::make_unique<DDyMaterialInformation>(newDesc);
}

} /// ::dy namespace