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
#include <Dy/Component/CDyModelAnimator.h>
#include <Dy/Management/Internal/Render/FDyModelHandlerManager.h>
#include <Dy/Element/Actor.h>
#include <Dy/Component/CDyModelFilter.h>
#include "Dy/Management/WorldManager.h"

namespace dy
{

CDyModelAnimator::CDyModelAnimator(_MIN_ FDyActor& actorReference) : ADyGeneralBaseComponent{actorReference}
{ }

EDySuccess CDyModelAnimator::Initialize(_MIN_ const PDyModelAnimatorComponentMetaInfo& descriptor)
{
  // Initialize skeleton & (temporary) animation scrap.
  this->mBinderAnimationScrap.TryRequireResource(descriptor.mDetails.mTempAnimationScrap);
  this->mBinderSkeleton.TryRequireResource(descriptor.mDetails.mSkeletonSpecifier);

  // Component activation check.
  if (descriptor.mInitiallyActivated == true) { this->Activate(); }
  return DY_SUCCESS;
}

void CDyModelAnimator::Release()
{
  if (this->IsComponentActivated() == true) { this->Deactivate(); }
}

void CDyModelAnimator::Update(_MIN_ float dt)
{ }

std::string CDyModelAnimator::ToString()
{
  MDY_NOT_IMPLEMENTED_ASSERT();
  return "";
}

void CDyModelAnimator::TryActivateInstance()
{
  // Get model filter component.
  auto opRenderer = this->GetBindedActor()->GetGeneralComponent<CDyModelFilter>();
  if (opRenderer.has_value() == false) { return; }

  CDyModelFilter& refCompFilter = *opRenderer.value();
  if (refCompFilter.IsComponentActivated() == false) { return; }

  // Get model handle manager, and ask for whether model handler is already bound.
  auto& handleManager = FDyModelHandlerManager::GetInstance();
  const auto& modelSpecifier = refCompFilter.GetModelSpecifier();
  if (handleManager.IsBoundModelExist(modelSpecifier) == false)
  {
    // If not, let manager create handler and bound it,
    // and let actor bind animator to handler and register specifier as key.
    MDY_CALL_ASSERT_SUCCESS(handleManager.TryCreateHandler(modelSpecifier));
  }
  handleManager.BindToHandler(modelSpecifier, *this->GetBindedActor(), *this);

  // Bind to activated animator component list of manager.
  auto& worldManager = MDyWorld::GetInstance();
  worldManager.__BindActiveModelAnimator(*this);
}

void CDyModelAnimator::TryDeactivateInstance()
{
  // Unbind from activated animator component list of manager.
  auto& worldManager = MDyWorld::GetInstance();
  worldManager.__UnbindActiveModelAnimator(*this);

  // Get model filter component.
  auto opRenderer = this->GetBindedActor()->GetGeneralComponent<CDyModelFilter>();
  if (opRenderer.has_value() == false) { return; }

  CDyModelFilter& refCompFilter = *opRenderer.value();
  if (refCompFilter.IsComponentActivated() == false) { return; }

  // GC renderer ptr to given handler.
  auto& handleManager       = FDyModelHandlerManager::GetInstance();
  const auto& modelSpecifier = refCompFilter.GetModelSpecifier();

  if (handleManager.IsBoundModelExist(modelSpecifier) == true)
  {
    // Unbind this component from handler instance.
    handleManager.UnbindToHandler(modelSpecifier, *this->GetBindedActor(), *this);

    // Check Actor instance need to be removed. 
    if (handleManager.IsActorInfoNeedToBeGc(modelSpecifier, *this->GetBindedActor()) == true)
    {
      MDY_CALL_ASSERT_SUCCESS(handleManager.TryRemoveBoundActor(modelSpecifier, *this->GetBindedActor()));

      // Because handler have been broken immutability, so check Bound model instance need to be gced also.
      if (handleManager.IsBoundModelNeedToGc(modelSpecifier) == true)
      {
        MDY_CALL_ASSERT_SUCCESS(handleManager.TryRemoveBoundModel(modelSpecifier));
      }
    }
  }
}

} /// ::dy namespace