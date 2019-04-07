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
#include <Dy/Core/Resource/Resource/FResourceMaterial.h>
#include <Dy/Builtin/Constant/GeneralValue.h>
#include <Dy/Core/Resource/Information/FInformationMaterial.h>
#include <Dy/Core/Resource/Information/FInformationShader.h>
#include <Dy/Core/Resource/Information/FInformationTexture.h>
#include <Dy/Core/Resource/Resource/AResourceTextureBase.h>
#include <Dy/Core/Resource/Resource/FResourceShader.h>
#include <Dy/Core/Rendering/Wrapper/XGLWrapper.h>
#include <Dy/Helper/System/Idioms.h>
#include <Dy/Meta/Information/MetaInfoMaterial.h>
#include <Dy/Management/IO/MIOMeta.h>

namespace dy
{

FResourceMaterial::FResourceMaterial(const FInformationMaterial& iInformation) 
  : mSpecifierName{iInformation.GetSpecifierName()},
    mBlendMode{iInformation.GetBlendMode()},
    mBinderShader{iInformation.GetPtrShaderInformation()->GetSpecifierName()}
{
  this->mBinderMaterial.TryRequireResource(mSpecifierName);
  const auto& ptrTextureInfoList = iInformation.GetPtrTextureInformationList();
  for (const auto& ptrTextureInfo : ptrTextureInfoList)
  { // Bind texture resource of this material.
    MDY_ASSERT_MSG(MDY_CHECK_ISNOTEMPTY(ptrTextureInfo), "Unexpected error occurred.");
    SafeUniquePtrEmplaceBack(this->mBinderTextureList, (*ptrTextureInfo)->GetSpecifierName());
  }

  // If default uniform value is exist, apply them to shader.
  MDY_ASSERT(this->mBinderShader.IsResourceExist() == true);
  auto& managerMetaInfo = MIOMeta::GetInstance();
  const auto& metaItem = managerMetaInfo.GetMaterialMetaInformation(
    TryRemovePostfix(mSpecifierName, kInstancingPostfix)
  );
  if (metaItem.mUniformValues.empty() == false)
  {
    for (const auto& [uniformName, smtValueInstance] : metaItem.mUniformValues)
    {
      using EUniform = EUniformVariableType;
      switch (smtValueInstance->mType)
      {
      case EUniform::Matrix4: 
      { const auto& item = static_cast<TUniformValue<EUniform::Matrix4>&>(*smtValueInstance);
        this->mBinderShader->TryUpdateUniform<EUniform::Matrix4>(uniformName, item.mValue);
      } break;
      case EUniform::Matrix3:
      { const auto& item = static_cast<TUniformValue<EUniform::Matrix3>&>(*smtValueInstance);
        this->mBinderShader->TryUpdateUniform<EUniform::Matrix3>(uniformName, item.mValue);
      } break;
      case EUniform::Matrix2: break;
      case EUniform::Vector4:
      { const auto& item = static_cast<TUniformValue<EUniform::Vector4>&>(*smtValueInstance);
        this->mBinderShader->TryUpdateUniform<EUniform::Vector4>(uniformName, item.mValue);
      } break;
      case EUniform::Vector3:
      { const auto& item = static_cast<TUniformValue<EUniform::Vector3>&>(*smtValueInstance);
        this->mBinderShader->TryUpdateUniform<EUniform::Vector3>(uniformName, item.mValue);
      } break;
      case EUniform::Vector2: 
      { const auto& item = static_cast<TUniformValue<EUniform::Vector2>&>(*smtValueInstance);
        this->mBinderShader->TryUpdateUniform<EUniform::Vector2>(uniformName, item.mValue);
      } break;
      case EUniform::IVec4: break;
      case EUniform::IVec3: break;
      case EUniform::IVec2: break;
      case EUniform::Integer:
      { const auto& item = static_cast<TUniformValue<EUniform::Integer>&>(*smtValueInstance);
        this->mBinderShader->TryUpdateUniform<EUniform::Integer>(uniformName, item.mValue);
      } break;
      case EUniform::Unsigned: 
      { const auto& item = static_cast<TUniformValue<EUniform::Unsigned>&>(*smtValueInstance);
        this->mBinderShader->TryUpdateUniform<EUniform::Unsigned>(uniformName, item.mValue);
      } break;
      case EUniform::Float: 
      { const auto& item = static_cast<TUniformValue<EUniform::Float>&>(*smtValueInstance);
        this->mBinderShader->TryUpdateUniform<EUniform::Float>(uniformName, item.mValue);
      } break;
      case EUniform::Bool:
      { const auto& item = static_cast<TUniformValue<EUniform::Bool>&>(*smtValueInstance);
        this->mBinderShader->TryUpdateUniform<EUniform::Bool>(uniformName, item.mValue);
      } break;
      case EUniform::Matrix4Array:
      { const auto& item = static_cast<TUniformValue<EUniform::Matrix4Array>&>(*smtValueInstance);
        this->mBinderShader->TryUpdateUniform<EUniform::Matrix4Array>(uniformName, item.mValue);
      } break;
      case EUniform::Vector3Array:
      { const auto& item = static_cast<TUniformValue<EUniform::Vector3Array>&>(*smtValueInstance);
        this->mBinderShader->TryUpdateUniform<EUniform::Vector3Array>(uniformName, item.mValue);
      } break;
      default: MDY_UNEXPECTED_BRANCH(); break;
      }
    }
  }

  this->mIsInstant = false;
}

FResourceMaterial::FResourceMaterial(const PDyMaterialInstanceMetaInfo& iInstanceInfo) 
  : mSpecifierName{iInstanceInfo.mSpecifierName},
    mBlendMode{iInstanceInfo.mBlendMode},
    mBinderShader{iInstanceInfo.mShaderSpecifier}
{
  for (const auto& bindingTextureItem : iInstanceInfo.mTextureNames)
  { // Bind texture resource of this material.
    if (bindingTextureItem.mTextureSpecifier.empty() == true) { continue; }
    SafeUniquePtrEmplaceBack(this->mBinderTextureList, bindingTextureItem.mTextureSpecifier);
  }
  this->mIsInstant = true;
}

EDySuccess FResourceMaterial::TryUpdateTextureList() noexcept
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
    return EDySuccess::DY_FAILURE;
  }

  const auto& textureResources = this->GetBindedTextureResourcePtrList();
  const auto  textureResourceListSize = static_cast<TI32>(textureResources.size());
  for (TI32 j = 0; j < textureResourceListSize; ++j)
  {
    const auto& textureBinder = (*textureResources[j]);
    XGLWrapper::BindTexture(j, textureBinder->GetTextureType(), textureBinder->GetTextureId());
  }
  return EDySuccess::DY_SUCCESS;
}

EDySuccess FResourceMaterial::TryDetachTextureListFromShader() noexcept
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
    return EDySuccess::DY_FAILURE;
  }

  const auto& textureResources = this->GetBindedTextureResourcePtrList();
  const auto  textureResourceListSize = static_cast<TI32>(textureResources.size());
  for (TI32 j = 0; j < textureResourceListSize; ++j)
  {
    const auto& textureBinder = (*textureResources[j]);
    XGLWrapper::UnbindTexture(j, textureBinder->GetTextureType());
  }
  return EDySuccess::DY_SUCCESS;
}

} /// ::dy namespace
