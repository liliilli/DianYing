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
#include <Dy/Component/CDyModelRenderer.h>

#include <Dy/Component/CDyModelFilter.h>
#include <Dy/Element/Actor.h>
#include <Dy/Management/Rendering/RenderingManager.h>
#include <Dy/Management/WorldManager.h>

#include <Dy/Core/Resource/Type/TDyResourceBinder.h>
#include <Dy/Core/Resource/Resource/FDyModelResource.h>
#include <Dy/Management/Helper/SDyProfilingHelper.h>
#include <Dy/Management/Internal/Render/FDyModelHandlerManager.h>

namespace dy
{

CDyModelRenderer::CDyModelRenderer(FDyActor& actorReference) : ADyGeneralBaseComponent(actorReference)
{ }

EDySuccess CDyModelRenderer::Initialize(const PDyModelRendererComponentMetaInfo& descriptor)
{
  this->mIsEnabledCreateShadow = descriptor.mDetails.mIsEnabledCreateShadow;
  if (descriptor.mInitiallyActivated == true) 
  { 
    this->Activate(); 
  }

  return DY_SUCCESS;
}

void CDyModelRenderer::Release()
{
  if (MDY_CHECK_ISNOTNULL(this->mPtrModelFilterComponent))
  { // Unbind CDyModelFilter from binded CDyModelRenderer.
    this->mPtrModelFilterComponent->fUnbindModelRendererReference();
    this->mPtrModelFilterComponent = MDY_INITIALIZE_NULL;
  }

  this->mPtrModelFilterComponent = MDY_INITIALIZE_NULL;
}

std::optional<TI32> CDyModelRenderer::GetModelSubmeshCount() const noexcept
{
  // If CDyModelFilter is not binded to CDyModelRenderer, just return no value.
  if (MDY_CHECK_ISNULL(this->mPtrModelFilterComponent)) { return std::nullopt; }
  return static_cast<TI32>(this->mPtrModelFilterComponent->GetModelReference()->GetMeshResourceList().size());
}

const FDyMeshResource& CDyModelRenderer::GetSubmeshResourcePtr(_MIN_ TU32 index) const noexcept
{
  const auto& submeshResourceList = this->mPtrModelFilterComponent->GetModelReference()->GetMeshResourceList();
  return *submeshResourceList[index]->Get();
}

const FDyMaterialResource& CDyModelRenderer::GetMaterialResourcePtr(_MIN_ TU32 index) const noexcept
{
  //MDY_ASSERT(index < this->mBinderMaterialListSize, "Index for CDyModelRenderer::mMaterialResourcePtr is out of bound.");
  const auto& materialResourceList = this->mPtrModelFilterComponent->GetModelReference()->GetMaterialResourceList();
  return *materialResourceList[index]->Get();
}

TDyLResourceBinderModel* CDyModelRenderer::GetModelResourceBinder() noexcept
{
  if (MDY_CHECK_ISNULL(this->mPtrModelFilterComponent)) { return nullptr; }
  return &this->mPtrModelFilterComponent->GetModelReference();
}

const TDyLResourceBinderModel* CDyModelRenderer::GetModelResourceBinder() const noexcept
{
  if (MDY_CHECK_ISNULL(this->mPtrModelFilterComponent)) { return nullptr; }
  return &this->mPtrModelFilterComponent->GetModelReference();
}

std::string CDyModelRenderer::ToString()
{
  MDY_NOT_IMPLEMENTED_ASSERT();
  return MDY_INITIALIZE_EMPTYSTR;
}

void CDyModelRenderer::TryActivateInstance()
{
  // Customized body ∨
  auto opRenderer = this->GetBindedActor()->GetGeneralComponent<CDyModelFilter>();
  if (opRenderer.has_value() == false) { return; }

  CDyModelFilter& filterRef = *opRenderer.value();
  if (filterRef.IsComponentActivated() == false) { return; }

  MDY_ASSERT(MDY_CHECK_ISNULL(this->mPtrModelFilterComponent), "CDyModelFilter::mModelRendererReferencePtr must be null when unbinding.");
  this->BindModelFilterReference(filterRef);

  // Check and rebind script instance to MDyWorld.
  const auto activatedIndex     = MDyWorld::GetInstance().pfEnrollActiveModelRenderer(*this);
  this->mActivatedUpdateListId  = activatedIndex;
}

void CDyModelRenderer::TryDeactivateInstance()
{
  MDY_ASSERT(MDY_CHECK_ISNOTNULL(this->mPtrModelFilterComponent), "CDyModelFilter::mModelRendererReferencePtr must not be null when unbinding.");
  this->UnbindModelFilterReference();

  MDyWorld::GetInstance().pfUnenrollActiveModelRenderer(this->mActivatedUpdateListId);
  this->mActivatedUpdateListId = MDY_INITIALIZE_DEFINT;
}

void CDyModelRenderer::BindModelFilterReference(CDyModelFilter& validReference)
{
  MDY_ASSERT(MDY_CHECK_ISNULL(this->mPtrModelFilterComponent), "CDyModelRenderer::mPtrModelFilterComponent must be a nullptr when binding.");
  this->mPtrModelFilterComponent = &validReference;
  this->mPtrModelFilterComponent->fBindModelRendererReference(*this);

  // Get model handle manager, and ask for whether model handler is already bound.
  auto& handleManager       = FDyModelHandlerManager::GetInstance();
  const auto modelSpecifier = this->mPtrModelFilterComponent->GetModelSpecifier();
  if (handleManager.IsBoundModelExist(modelSpecifier) == false)
  {
    // If not, let manager create handler and bound it,
    // and let actor bind transform to handler and register specifier as key.
    MDY_CALL_ASSERT_SUCCESS(handleManager.TryCreateHandler(modelSpecifier));
  }
  handleManager.BindToHandler(modelSpecifier, *this->GetBindedActor(), *this);
}

void CDyModelRenderer::UnbindModelFilterReference()
{
  MDY_ASSERT(MDY_CHECK_ISNOTNULL(this->mPtrModelFilterComponent), "CDyModelRenderer::mPtrModelFilterComponent must not be a nullptr when binding.");
  this->mPtrModelFilterComponent->fUnbindModelRendererReference();

  // GC renderer ptr to given handler.
  auto& handleManager       = FDyModelHandlerManager::GetInstance();
  const auto modelSpecifier = this->mPtrModelFilterComponent->GetModelSpecifier();

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

  // Null.
  this->mPtrModelFilterComponent = MDY_INITIALIZE_NULL;
}

} /// ::dy namespace