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
#include <Dy/Core/Resource/Resource/FDyMaterialResource.h>
#include <Dy/Core/Resource/Information/FDyMaterialInformation.h>
#include <Dy/Core/Resource/Information/FDyShaderInformation.h>
#include <Dy/Core/Resource/Information/FDyTextureInformation.h>
#include <Dy/Helper/System/Idioms.h>
#include <Dy/Core/Resource/Resource/FDyTextureResource.h>
#include <Dy/Core/Rendering/Wrapper/FDyGLWrapper.h>
#include <Dy/Meta/Information/MetaInfoMaterial.h>

namespace dy
{

FDyMaterialResource::FDyMaterialResource(_MIN_ const FDyMaterialInformation& information) :
    mSpecifierName{information.GetSpecifierName()},
    mBlendMode{information.GetBlendMode()},
    mBinderShader{information.GetPtrShaderInformation()->GetSpecifierName()}
{
  this->mBinderMaterial.TryRequireResource(mSpecifierName);
  const auto& ptrTextureInfoList = information.GetPtrTextureInformationList();
  for (const auto& ptrTextureInfo : ptrTextureInfoList)
  { // Bind texture resource of this material.
    MDY_ASSERT(MDY_CHECK_ISNOTEMPTY(ptrTextureInfo), "Unexpected error occurred.");
    DySafeUniquePtrEmplaceBack(this->mBinderTextureList, (*ptrTextureInfo)->GetSpecifierName());
  }
  this->mIsInstant = false;
}

FDyMaterialResource::FDyMaterialResource(_MIN_ const PDyMaterialInstanceMetaInfo& iInstanceInfo) :
    mSpecifierName{iInstanceInfo.mSpecifierName},
    mBlendMode{iInstanceInfo.mBlendMode},
    mBinderShader{iInstanceInfo.mShaderSpecifier}
{
  for (const auto& textureSpecifier : iInstanceInfo.mTextureNames)
  { // Bind texture resource of this material.
    if (textureSpecifier.empty() == true) { continue; }
    DySafeUniquePtrEmplaceBack(this->mBinderTextureList, textureSpecifier);
  }
  this->mIsInstant = true;
}

EDySuccess FDyMaterialResource::TryUpdateTextureList() noexcept
{
  // Check validation.
  if ((this->mIsInstant == false && this->mBinderMaterial.IsResourceExist() == false)
  ||  this->mBinderShader.IsResourceExist() == false
  ||  std::all_of(MDY_BIND_BEGIN_END(this->mBinderTextureList), 
          [](const decltype(mBinderTextureList)::value_type& item) { 
              return MDY_CHECK_ISNOTEMPTY(item) && item->IsResourceExist() == true; 
          }
      ) == false)
  {
    return DY_FAILURE;
  }

  const auto& textureResources = this->GetBindedTextureResourcePtrList();
  const auto  textureResourceListSize = static_cast<TI32>(textureResources.size());
  for (TI32 j = 0; j < textureResourceListSize; ++j)
  {
    const auto& textureBinder = (*textureResources[j]);
    FDyGLWrapper::BindTexture(j, textureBinder->GetTextureType(), textureBinder->GetTextureId());
  }
  return DY_SUCCESS;
}

EDySuccess FDyMaterialResource::TryDetachTextureListFromShader() noexcept
{
  // Check validation.
  if (this->mBinderMaterial.IsResourceExist() == false
  ||  this->mBinderShader.IsResourceExist() == false
  ||  std::all_of(MDY_BIND_BEGIN_END(this->mBinderTextureList), 
          [](const decltype(mBinderTextureList)::value_type& item) { 
              return MDY_CHECK_ISNOTEMPTY(item) && item->IsResourceExist() == true; 
          }
      ) == false)
  {
    return DY_FAILURE;
  }

  const auto& textureResources = this->GetBindedTextureResourcePtrList();
  const auto  textureResourceListSize = static_cast<TI32>(textureResources.size());
  for (TI32 j = 0; j < textureResourceListSize; ++j)
  {
    const auto& textureBinder = (*textureResources[j]);
    FDyGLWrapper::UnbindTexture(j, textureBinder->GetTextureType());
  }
  return DY_SUCCESS;
}

} /// ::dy namespace
