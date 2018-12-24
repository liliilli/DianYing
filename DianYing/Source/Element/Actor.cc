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
#include <Dy/Element/Actor.h>
#include <Dy/Management/IO/MetaInfoManager.h>
#include <Dy/Component/CDyTransform.h>
#include "Dy/Component/CDyModelFilter.h"
#include "Dy/Component/CDyModelRenderer.h"
#include "Dy/Component/CDyCamera.h"
#include "Dy/Component/CDyDirectionalLight.h"

//!
//! Implementation
//!

namespace dy
{

EDySuccess FDyActor::Initialize(_MIN_ const PDyObjectMetaInfo& objectMetaDesc)
{
  bool isTransformCreated = false;
  this->pSetObjectName(objectMetaDesc.mSpecifierName);

  // Create components
  for (const auto& [type, componentInfo] : objectMetaDesc.mMetaComponentInfo)
  {
    switch (type)
    {
    default: MDY_UNEXPECTED_BRANCH(); break;
    case EDyComponentMetaType::Transform:
    {
      const auto& desc = std::any_cast<const PDyTransformComponentMetaInfo&>(componentInfo);
      MDY_NOTUSED auto transformComponentPtr = this->AddComponent<CDyTransform>(desc);
      isTransformCreated = true;
    } break;
    case EDyComponentMetaType::Script:
    {
      const auto& desc = std::any_cast<const PDyScriptComponentMetaInfo&>(componentInfo);
      MDY_NOTUSED auto scriptComponentPtr = this->AddComponent<CDyActorScript>(desc);
    } break;
    case EDyComponentMetaType::DirectionalLight:
    {
      const auto& desc = std::any_cast<const PDyDirLightComponentMetaInfo&>(componentInfo);
      MDY_NOTUSED auto directionLightComponentPtr = this->AddComponent<CDyDirectionalLight>(desc);
    } break;
    case EDyComponentMetaType::ModelFilter:
    {
      const auto& desc = std::any_cast<const PDyModelFilterComponentMetaInfo&>(componentInfo);
      MDY_NOTUSED auto modelFilterComponentPtr = this->AddComponent<CDyModelFilter>(desc);
    } break;
    case EDyComponentMetaType::ModelRenderer:
    {
      const auto& desc = std::any_cast<const PDyModelRendererComponentMetaInfo&>(componentInfo);
      MDY_NOTUSED auto modelRendererComponentPtr = this->AddComponent<CDyModelRenderer>(desc);
    } break;
    case EDyComponentMetaType::Camera:
    {
      const auto& desc = std::any_cast<const PDyCameraComponentMetaInfo&>(componentInfo);
      MDY_NOTUSED auto cameraComponentPtr = this->AddComponent<CDyCamera>(desc);
    } break;
    }
  }

  // Create CDyEmptyTransform when not having CDyTransform.
  MDY_ASSERT(isTransformCreated == true, "CDyTransform component must be created to all FDyActor.");
  return DY_SUCCESS;
}

EDySuccess FDyActor::Release()
{
  this->mComponentList.clear();
  return DY_SUCCESS;
}

void FDyActor::Activate() noexcept
{
  this->mActivationFlag.UpdateInput(true);
  this->pPropagateActivationFlag();
}

void FDyActor::Deactivate() noexcept
{
  this->mActivationFlag.UpdateInput(true);
  this->pPropagateActivationFlag();
}

void FDyActor::pUpdateActivateFlagFromParent() noexcept
{
  if (MDY_CHECK_ISNULL(this->mParentFDyActorRawPtr))
  {
    this->mActivationFlag.UpdateParent(true);
  }
  else
  {
    this->mActivationFlag.UpdateParent(this->mParentFDyActorRawPtr->IsActivated());
  }

  this->pPropagateActivationFlag();
}

void FDyActor::pPropagateActivationFlag() noexcept
{
  for (auto& unknownComponent : mComponentList)
  {
    if (MDY_CHECK_ISEMPTY(unknownComponent)) { continue; }
    unknownComponent->pPropagateParentActorActivation(this->mActivationFlag);
  }

  for (auto& unknownScript : this->mScriptList)
  {
    if (MDY_CHECK_ISEMPTY(unknownScript)) { continue; }
    unknownScript->pPropagateParentActorActivation(this->mActivationFlag);
  }

  // @TODO PROPAGATE ACTIVATION FLAG TO SUBACTOR ALSO.
}

  void FDyActor::SetParent(NotNull<FDyActor*> validParentRawPtr) noexcept
{
  mParentFDyActorRawPtr = validParentRawPtr;
  MDY_LOG_WARNING("NOT IMPLEMENTED {}", "FDyActor::SetParent");
}

void FDyActor::SetParentRelocateTransform(NotNull<FDyActor*> validParentRawPtr) noexcept
{
  this->SetParent(validParentRawPtr);
  MDY_LOG_WARNING("NOT IMPLEMENTED {}", "FDyActor::SetParentRelocateTransform");
}

void FDyActor::SetParentAsRoot() noexcept
{

  MDY_LOG_WARNING("NOT IMPLEMENTED {}", "FDyActor::SetParentAsRoot");
}

void FDyActor::SetParentToRootRelocateTransform() noexcept
{
  this->SetParentAsRoot();
  MDY_LOG_WARNING("NOT IMPLEMENTED {}", "FDyActor::SetParentToRootRelocateTransform");
}

#ifdef false
CDyActorScript* FDyActor::GetScriptComponent(_MIN_ const std::string& scriptName) noexcept
{
  MDY_ASSERT(scriptName.empty() == false, "scriptName must not be empty at FDyActor::GetScriptComponent()");

  using TInstanceType = decltype(this->mScriptList)::value_type;
  const auto it = std::find_if(MDY_BIND_BEGIN_END(this->mScriptList), [&scriptName](const TInstanceType& instance)
  {
    return instance->GetScriptVerificationName() == scriptName;
  });

  if (it == this->mScriptList.end())  { return std::nullopt; }
  else                                { return it->get(); }
}

EDySuccess FDyActor::RemoveScriptComponent(_MIN_ const std::string& scriptName) noexcept
{
  MDY_ASSERT(scriptName.empty() == false, "scriptName must not be empty at FDyActor::GetScriptComponent()");

  // Find script instance that has scriptName.
  using TInstanceType = decltype(this->mScriptList)::value_type;
  const auto it = std::find_if(MDY_BIND_BEGIN_END(this->mScriptList), [&scriptName](const TInstanceType& instance)
  {
    if (MDY_CHECK_ISEMPTY(instance))  { return false; }
    else                              { return instance->GetScriptVerificationName() == scriptName; }
  });

  if (it == this->mScriptList.end())
  {
    MDY_LOG_WARNING("FDyActor::RemoveComponent | Can not find script to remove. | Name : {}", scriptName);
    return DY_FAILURE;
  }
  else
  {
    // @TODO MOVE DESTROY FUNCTION CALL TO AFTERWARD COMPONENT UPDATE() (BEFORE TRANFORM SYNCHRONIZATION)
    // @TODO FUCTION
    //(*it)->Destroy();
    (*it)->Release();
    this->mScriptList.erase(it);
    return DY_SUCCESS;
  }
}
#endif

void FDyActor::MDY_PRIVATE_SPECIFIER(TryRemoveScriptInstances)() noexcept
{
  return this->mScriptList.clear();
}

NotNull<CDyTransform*> FDyActor::GetTransform() noexcept
{
  return DyMakeNotNull(this->mTransform.get());
}

} /// ::dy namespace
