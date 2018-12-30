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

FDyActor::FDyActor(_MIN_ const PDyObjectMetaInfo& objectMetaDesc)
{
  // (1) Set properties.
  bool isTransformCreated = false;
  this->pSetObjectName(objectMetaDesc.mSpecifierName);

  auto metaComponentInfo = objectMetaDesc.mMetaComponentInfo;
  if (objectMetaDesc.mProperties.mIsUsingPrefab == true)
  { // Insert prefab information.
    MDY_ASSERT(objectMetaDesc.mProperties.mPrefabSpecifierName.empty() == false, "Unexpected error occurred.");
    const auto& prefab = MDyMetaInfo::GetInstance().GetPrefabMetaInformation(objectMetaDesc.mProperties.mPrefabSpecifierName);
    metaComponentInfo.insert(metaComponentInfo.end(), MDY_BIND_BEGIN_END(prefab.mMetaComponentInfo));
  }

  // (2) Create components
  for (const auto& [type, componentInfo] : metaComponentInfo)
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

  // (3) Make children actors.
  for (const auto& objectInformation : objectMetaDesc.mChildrenList)
  { // Create object, FDyActor
    switch (objectInformation->mObjectType)
    {
    case EDyMetaObjectType::Actor:
    { // General object type. Make FDyActor instance.
      auto instancePtr = std::make_unique<FDyActor>(*objectInformation);
      // Check activation flags and execute sub-routines of each components.
      instancePtr->pUpdateActivateFlagFromParent();
      if (objectInformation->mProperties.mInitialActivated == true) 
      { 
        instancePtr->Activate(); 
      }

      auto [it, result] = this->mChildActorMap.try_emplace(instancePtr->GetActorName(), std::move(instancePtr));
      MDY_ASSERT(result == true, "Unexpected error occured in inserting FDyActor to object map.");

      // Set child's parent as this.
      auto& [specifier, ptrsmtChild] = *it;
      ptrsmtChild->SetParent(*this);
    } break;
    case EDyMetaObjectType::SceneScriptor:  MDY_NOT_IMPLEMENTED_ASSERT(); break;
    case EDyMetaObjectType::Object:         MDY_NOT_IMPLEMENTED_ASSERT(); break;
    default: MDY_UNEXPECTED_BRANCH(); break;
    }
  }

  // Create CDyEmptyTransform when not having CDyTransform.
  MDY_ASSERT(isTransformCreated == true, "CDyTransform component must be created to all FDyActor.");
}

FDyActor::~FDyActor()
{
  this->mComponentList.clear();
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
  if (MDY_CHECK_ISNULL(this->mPtrParentActor))
  {
    this->mActivationFlag.UpdateParent(true);
  }
  else
  {
    this->mActivationFlag.UpdateParent(this->mPtrParentActor->IsActivated());
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

void FDyActor::SetParent(_MIN_ FDyActor& validParentRawPtr) noexcept
{
  this->mPtrParentActor = &validParentRawPtr;
}

void FDyActor::SetParentRelocateTransform(NotNull<FDyActor*> validParentRawPtr) noexcept
{
  MDY_NOT_IMPLEMENTED_ASSERT();
  this->SetParent(*validParentRawPtr);
}

void FDyActor::SetParentAsRoot() noexcept
{
  MDY_NOT_IMPLEMENTED_ASSERT();
  this->mPtrParentActor = nullptr;
}

void FDyActor::SetParentToRootRelocateTransform() noexcept
{
  MDY_NOT_IMPLEMENTED_ASSERT();
  this->SetParentAsRoot();
}

std::unique_ptr<CDyActorScript> 
FDyActor::MDY_PRIVATE_SPECIFIER(MakeScriptComponent)(_MIN_ const PDyScriptComponentMetaInfo& info)
{
  auto& metaManager = MDyMetaInfo::GetInstance();
  MDY_ASSERT(metaManager.IsScriptMetaInformationExist(info.mDetails.mSpecifierName) == true, "");

  const auto& instanceInfo = metaManager.GetScriptMetaInformation(info.mDetails.mSpecifierName);
  MDY_ASSERT(instanceInfo.mScriptType != EDyScriptType::NoneError, "");

  return std::make_unique<CDyActorScript>(*this, info.mDetails.mSpecifierName);
}

bool FDyActor::IsHaveParent() const noexcept
{
  return MDY_CHECK_ISNOTNULL(this->mPtrParentActor);
}

FDyActor* FDyActor::GetParent() const noexcept
{
  if (MDY_CHECK_ISNULL(this->mPtrParentActor)) { return nullptr; }
  else                                         { return this->mPtrParentActor; }
}

bool FDyActor::IsHavingChildrenObject() const noexcept
{
  return std::any_of(MDY_BIND_BEGIN_END(this->mChildActorMap), [](const auto& iPair) 
  {
    return MDY_CHECK_ISNOTEMPTY(iPair.second);
  });
}

FDyActor::TActorMap& FDyActor::GetChildrenContainer() noexcept
{
  return this->mChildActorMap;
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
