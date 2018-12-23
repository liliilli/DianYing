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
#include <Dy/Management/WorldManager.h>
#include <Dy/Management/LoggingManager.h>
#include <Dy/Management/IO/MetaInfoManager.h>
#include <Dy/Management/SettingManager.h>
#include <Dy/Core/Thread/SDyIOConnectionHelper.h>
#include <Dy/Core/Resource/Type/EDyScope.h>
#include "Dy/Core/DyEngine.h"
#include "Dy/Management/ScriptManager.h"

namespace dy
{

EDySuccess MDyWorld::pfInitialize()
{
  MDY_LOG_INFO_D("{} | MDyWorld::pfInitialize().", "FunctionCall");
  return DY_SUCCESS;
}

EDySuccess MDyWorld::pfRelease()
{
  MDY_LOG_INFO_D("{} | MDyWorld::pfRelease().", "FunctionCall");
  if (this->mLevel)
  {
    this->mLevel->Release();
    this->mLevel = nullptr;
    this->Update(-1);
  }

  return DY_SUCCESS;
}

void MDyWorld::Update(_MIN_ float dt)
{
  // Garbage collect needless "FDyActor"s
  if (this->mActorGc.empty() == false) { this->mActorGc.clear(); }

  // GC components.
  this->pGcAcitvatedComponents();

  // Level update routine
  if (this->mLevel)
  {
    this->mLevel->Update(dt);
  }
}

void MDyWorld::pGcAcitvatedComponents()
{
  // Remove activated script update list reversely.
  if (this->mErasionScriptCandidateList.empty() == false)
  {
    std::sort(MDY_BIND_BEGIN_END(this->mErasionScriptCandidateList), std::greater<TI32>());
    for (const auto& index : this->mErasionScriptCandidateList)
    { // Remove!
      // @TODO CALL SCRIPT MANAGE TO GC.
      //this->mActivatedScripts.erase(this->mActivatedScripts.begin() + index);
    }
    // Clear!
    this->mErasionScriptCandidateList.clear();
  }

  // Remove activated model renderer (CDyModelRenderer) list reversely to avoiding invalidation index.
  if (this->mErasionModelRenderersCandidateList.empty() == false)
  {
    std::sort(MDY_BIND_BEGIN_END(this->mErasionModelRenderersCandidateList), std::greater<>());
    for (const auto& index : this->mErasionModelRenderersCandidateList)
    { // Remove!
      this->mActivatedModelRenderers.erase(this->mActivatedModelRenderers.begin() + index);
    }
    // Clear!
    this->mErasionModelRenderersCandidateList.clear();
  }

  // Remove activated camera (CDyCamera) list reversely to avoiding invalidation index.
  if (this->mErasionCamerasCandidateList.empty() == false)
  {
    std::sort(MDY_BIND_BEGIN_END(this->mErasionCamerasCandidateList), std::greater<>());
    for (const auto& index : this->mErasionCamerasCandidateList)
    { // Remove!
      this->mActivatedOnRenderingCameras.erase(this->mActivatedOnRenderingCameras.begin() + index);
    }
    // Clear!
    this->mErasionCamerasCandidateList.clear();
  }
}

void MDyWorld::UpdateObjects(_MIN_ float dt)
{
  if (this->mLevel)
  { 
#ifdef false
    // Update(Start, Update, etc...) script carefully.
    for (auto& script : this->mActivatedScripts)
    {
      if (MDY_CHECK_ISNULL(script)) { continue; }
      script->CallScriptFunction(dt);
    }
#endif

    // CDyModelRenderer update
    for (auto& modelRenderer : this->mActivatedModelRenderers)
    {
      if (MDY_CHECK_ISNULL(modelRenderer)) { continue; }
      modelRenderer->Update(dt);
    }

    // CDyCamera update
    for (_MIO_ auto& camera : this->mActivatedOnRenderingCameras)
    {
      if (MDY_CHECK_ISNULL(camera)) { continue; }
      camera->Update(dt);
    }

    // After update, check
  }
}

void MDyWorld::RequestDrawCall(float dt)
{
  // @TODO IMPLEMENT SW OCCLUSION CULLING (HW?)
  for (auto& modelRenderer : this->mActivatedModelRenderers)
  { // Request draw calls (without SW occlusion culling)
    modelRenderer->RequestDrawCall();
  }
}

CDyLegacyCamera* MDyWorld::GetMainCameraPtr() const noexcept
{
  return this->mValidMainCameraPtr;
}

std::optional<CDyCamera*> MDyWorld::GetFocusedCameraValidReference(const TI32 index) const noexcept
{
  MDY_ASSERT(index < this->mActivatedOnRenderingCameras.size(),
                R"dy(Input parameter "index" for "MDyWorld::GetFocusedCameraValidReferenc" must be equal or less than "MDyWorld::mActivatedOnRenderingCameras".)dy");

  auto* camera = this->mActivatedOnRenderingCameras[index];
  if (MDY_CHECK_ISNULL(camera)) { return std::nullopt; }
  else                          { return camera; }
}

EDySuccess MDyWorld::OpenLevel(_MIN_ const std::string& levelName)
{
  if (MDyMetaInfo::GetInstance().GetLevelMetaInformation(levelName) == nullptr)
  {
    MDY_LOG_ERROR("{} | Failed to find and travel next level. Level name is not found. | Level name : {}", levelName);
    return DY_FAILURE;
  }

  this->SetLevelTransition(levelName);
  return DY_SUCCESS;
}

EDySuccess MDyWorld::MDY_PRIVATE_SPECIFIER(OpenFirstLevel)()
{
  this->SetLevelTransition(MDySetting::GetInstance().GetInitialSceneInformationName());

  // Let present level do release sequence
  // Game Status Sequence 12-13.
  this->MDY_PRIVATE_SPECIFIER(RemoveLevel)();
  this->MDY_PRIVATE_SPECIFIER(PopulateNextLevelResources)();
  return DY_SUCCESS;
}

EDySuccess MDyWorld::MDY_PRIVATE_SPECIFIER(RemoveLevel)()
{
  // Let present level do release sequence
  if (MDY_CHECK_ISEMPTY(this->mLevel)) { return DY_FAILURE; }

  this->mLevel->Release(); 
  this->mLevel = nullptr;
  return DY_SUCCESS;
}

EDySuccess MDyWorld::MDY_PRIVATE_SPECIFIER(PopulateNextLevelResources)()
{
  if (this->mNextLevelName.empty() == true) { return DY_FAILURE; }
  if (MDyMetaInfo::GetInstance().IsLevelMetaInformation(this->mNextLevelName) == false) { return DY_FAILURE; }

  // Get level meta information, and construct resource list.
  const auto& levMetaInfo = *MDyMetaInfo::GetInstance().GetLevelMetaInformation(this->mNextLevelName);
  const TDDyResourceNameSet levelResourceSet = levMetaInfo.GetLevelResourceSet();

  // Populate resource and wait until resource populating is done.
  // If done, call `build next level` in outside (MDySync).
  SDyIOConnectionHelper::PopulateResourceList(
      levelResourceSet, 
      EDyScope::Level,
      []() 
  { 
    auto& mWorld = MDyWorld::GetInstance();
    mWorld.MDY_PRIVATE_SPECIFIER(BuildNextLevel)(); 
    mWorld.MDY_PRIVATE_SPECIFIER(TransitionToNextLevel)();
    DyEngine::GetInstance().SetNextGameStatus(EDyGlobalGameStatus::GameRuntime);
  });
  return DY_SUCCESS;
}

void MDyWorld::MDY_PRIVATE_SPECIFIER(BuildNextLevel)()
{
  this->mLevel = std::make_unique<FDyLevel>();
  const auto* levelMetaInfo = MDyMetaInfo::GetInstance().GetLevelMetaInformation(this->mNextLevelName);
  this->mLevel->Initialize(*levelMetaInfo);

  // Game Status Sequence - 9, 10.
  MDyScript::GetInstance().UpdateActorScript(0.0f, EDyScriptState::CalledNothing);
  this->mLevel->MDY_PRIVATE_SPECIFIER(AlignActorsPosition)();
}

EDySuccess MDyWorld::MDY_PRIVATE_SPECIFIER(TransitionToNextLevel)()
{
  this->mPreviousLevelName  = this->mPresentLevelName;
  this->mPresentLevelName   = this->mNextLevelName;
  this->mNextLevelName      = MDY_INITIALIZE_EMPTYSTR;
  this->mIsNeedTransitNextLevel = false;
  return DY_SUCCESS;
}

bool MDyWorld::IsLevelPresentValid() const noexcept
{
  return MDY_CHECK_ISNOTEMPTY(this->mLevel);
}

FDyLevel& MDyWorld::GetValidLevelReference() noexcept
{
  MDY_ASSERT(IsLevelPresentValid() == true, "Level must be valid when retrieving level reference.");
  return *this->mLevel;
}

EDySuccess MDyWorld::TryCreateDebugUi()
{
  return this->mUiInstanceContainer.TryCreateDebugUi();
}

bool MDyWorld::IsDebugUiExist() const noexcept
{
  return this->mUiInstanceContainer.IsDebugUiExist();
}

EDySuccess MDyWorld::TryRemoveDebugUi()
{
  return this->mUiInstanceContainer.TryRemoveDebugUi();
}

void MDyWorld::MDY_PRIVATE_SPECIFIER(TryRenderDebugUi)()
{
  this->mUiInstanceContainer.TryRenderDebugUi();
}

EDySuccess MDyWorld::TryCreateLoadingUi()
{
  return this->mUiInstanceContainer.TryCreateLoadingUi();
}

bool MDyWorld::IsLoadingUiExist() const noexcept
{
  return this->mUiInstanceContainer.IsLoadingUiExist();
}

EDySuccess MDyWorld::TryRemoveLoadingUi()
{
  return this->mUiInstanceContainer.TryRemoveLoadingUi();
}

void MDyWorld::MDY_PRIVATE_SPECIFIER(TryRenderLoadingUi)()
{
  this->mUiInstanceContainer.TryRenderLoadingUi();
}

void MDyWorld::SetLevelTransition(_MIN_ const std::string& iSpecifier)
{
  if (MDyMetaInfo::GetInstance().IsLevelMetaInformation(iSpecifier) == false)
  {
    MDY_LOG_ERROR("Failed to transit next level, `{0}`. `{0}` level is not exist.", iSpecifier);
    return;
  }

  this->mNextLevelName          = iSpecifier;
  this->mIsNeedTransitNextLevel = true;
}

void MDyWorld::pfBindFocusCamera(_MIN_ CDyLegacyCamera& validCameraPtr) noexcept
{
  MDY_ASSERT(MDY_CHECK_ISNOTNULL(&validCameraPtr), "validCameraPtr must be valid, not nullptr.");
  this->mValidMainCameraPtr = &validCameraPtr;
}

void MDyWorld::pfUnbindCameraFocus()
{
  if (this->mValidMainCameraPtr)
  {
    this->mValidMainCameraPtr = nullptr;
    MDY_LOG_INFO_D("{} | MainCamera pointing unbinded.", "MDyWorld::pfUnbindCameraFocus()");
  }
  else
  {
    MDY_LOG_WARNING_D("{} | Valid mainCamera pointer does not point anything.", "MDyWorld::pfUnbindCameraFocus()");
  }
}

void MDyWorld::pfMoveActorToGc(_MIN_ NotNull<FDyActor*> actorRawPtr) noexcept
{
  this->mActorGc.emplace_back(std::unique_ptr<FDyActor>(actorRawPtr));
}

void MDyWorld::pfUnenrollActiveModelRenderer(_MIN_ TI32 index) noexcept
{
  MDY_ASSERT(index < this->mActivatedModelRenderers.size(), "index must be smaller than this->mActivatedModelRenderers.size().");

  this->mActivatedModelRenderers[index] = MDY_INITIALIZE_NULL;
  this->mErasionModelRenderersCandidateList.emplace_back(index);
}

void MDyWorld::pfUnenrollActiveCamera(_MIO_ TI32& index) noexcept
{
  MDY_ASSERT(index < this->mActivatedOnRenderingCameras.size(), "index must be smaller than this->mActivatedOnRenderingCameras.size().");

  this->mActivatedOnRenderingCameras[index] = MDY_INITIALIZE_NULL;
  this->mErasionCamerasCandidateList.emplace_back(index);

  index = MDY_INITIALIZE_DEFINT;
}

#ifdef false
void MDyWorld::pfUnenrollActiveScript(_MIN_ TI32 index) noexcept
{
  MDY_ASSERT(index < this->mActivatedScripts.size(), "index must be smaller than this->mActivatedScripts.size().");

  this->mActivatedScripts[index] = MDY_INITIALIZE_NULL;
  this->mErasionScriptCandidateList.emplace_back(index);
}

TI32 MDyWorld::pfEnrollActiveScript(_MIN_ const NotNull<CDyScript*>& pawnRawPtr) noexcept
{
  this->mActivatedScripts.emplace_back(pawnRawPtr);
  return static_cast<TI32>(this->mActivatedScripts.size()) - 1;
}
#endif

TI32 MDyWorld::pfEnrollActiveModelRenderer(_MIN_ CDyModelRenderer& validComponent) noexcept
{
  this->mActivatedModelRenderers.emplace_back(&validComponent);
  return static_cast<TI32>(this->mActivatedModelRenderers.size()) - 1;
}

TI32 MDyWorld::pfEnrollActiveCamera(_MIN_ CDyCamera& validComponent) noexcept
{
  this->mActivatedOnRenderingCameras.emplace_back(&validComponent);
  return static_cast<TI32>(this->mActivatedOnRenderingCameras.size()) - 1;
}

} /// ::dy namespace