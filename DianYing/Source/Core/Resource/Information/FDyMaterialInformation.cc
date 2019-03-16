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
#include <Dy/Core/Resource/Information/FDyMaterialInformation.h>
#include <Dy/Builtin/Constant/GeneralValue.h>
#include <Dy/Helper/System/Idioms.h>
#include <Dy/Meta/Information/MetaInfoMaterial.h>

namespace dy
{

FDyMaterialInformation::FDyMaterialInformation(const PDyMaterialInstanceMetaInfo& metaInfo, bool iIsInstanced) 
  : mSpecifierName{metaInfo.mSpecifierName},
    mBlendMode{metaInfo.mBlendMode},
    mIsInstanced{iIsInstanced}
{
  // Bind informations RI.
  if (this->IsInstanced() == false)
  {
    this->mBinderShaderInfo.TryRequireResource(metaInfo.mShaderSpecifier);
    MDY_ASSERT_MSG(this->mBinderShaderInfo.IsResourceExist() == true, "Unexpected error occurred.");
  }
  else
  {
    // Must do
    this->mSpecifierName += kInstancingPostfix;

    // If supports instacing, shader specifier should have postfix name, "__inst".
    this->mBinderShaderInfo.TryRequireResource(metaInfo.mShaderSpecifier + kInstancingPostfix);
    MDY_ASSERT_MSG(this->mBinderShaderInfo.IsResourceExist() == true, "Unexpected error occurred.");
  }

  for (const auto& bindingTextureItem : metaInfo.mTextureNames)
  {
    if (bindingTextureItem.mTextureSpecifier.empty() == true) { break; }
    SafeUniquePtrEmplaceBack(this->mBinderTextureInfoList, bindingTextureItem.mTextureSpecifier);
  }
}

const std::string& FDyMaterialInformation::GetSpecifierName() const noexcept
{
  return this->mSpecifierName;
}

EMaterialBlendMode FDyMaterialInformation::GetBlendMode() const noexcept
{
  return this->mBlendMode;
}

const FDyMaterialInformation::TPtrTextureInfoList& 
FDyMaterialInformation::GetPtrTextureInformationList() const noexcept
{
  return this->mBinderTextureInfoList;
}

bool FDyMaterialInformation::IsInstanced() const noexcept
{
  return this->mIsInstanced;
}

const TDyInformationBinderShader& FDyMaterialInformation::GetPtrShaderInformation() const noexcept
{
  return this->mBinderShaderInfo;
}

} /// ::dy namespace