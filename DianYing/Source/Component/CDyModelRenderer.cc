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
#include <Dy/Component/CDyModelRenderer.h>

#include <Dy/Component/CDyModelFilter.h>
#include <Dy/Element/Actor.h>
#include <Dy/Management/IO/IOResourceManager.h>
#include <Dy/Management/Rendering/RenderingManager.h>
#include <Dy/Management/WorldManager.h>

namespace dy
{

CDyModelRenderer::CDyModelRenderer(FDyActor& actorReference) : ADyGeneralBaseComponent(actorReference)
{ }

EDySuccess CDyModelRenderer::Initialize(const PDyModelRendererComponentMetaInfo& descriptor)
{
  this->mIsEnabledCreateShadow = descriptor.mDetails.mIsEnabledCreateShadow;

  // Bind material. If not exists, make material resource using information, but return fail.
  auto& resourceManager = MDyIOResource::GetInstance();
  for (const auto& materialName : descriptor.mDetails.mMaterialName)
  {
    if (auto* materialResourcePtr = resourceManager.GetMaterialResource(materialName); !materialResourcePtr)
    {
      const auto res = resourceManager.CreateMaterialResource(materialName);
      if (res == DY_FAILURE) { return DY_FAILURE; }

      this->mMaterialResourcePtr.emplace_back(resourceManager.GetMaterialResource(materialName));
    }
    else
    {
      this->mMaterialResourcePtr.emplace_back(materialResourcePtr);
    }
  }

  if (descriptor.mInitiallyActivated) { this->Activate(); }

  return DY_SUCCESS;
}

void CDyModelRenderer::Release()
{
  if (MDY_CHECK_ISNOTNULL(this->mModelFilter))
  { // Unbind CDyModelFilter from binded CDyModelRenderer.
    this->mModelFilter->fUnbindModelRendererReference();
    this->mModelFilter = MDY_INITIALIZE_NULL;
  }

  this->mModelFilter = MDY_INITIALIZE_NULL;
}

void CDyModelRenderer::RequestDrawCall() noexcept
{
  MDyRendering::GetInstance().PushDrawCallTask(*this);
}

void CDyModelRenderer::Activate() noexcept
{
  ADyBaseComponent::Activate();

  // Customized body ∨
  MDY_NOTUSED auto _ = this->pTryBindingToModelFilterComponent();

  // Check and rebind script instance to MDyWorld.
  if (this->mActivateFlag.IsOutputValueChanged() == true && this->mActivateFlag.GetOutput() == true)
  {
    const auto activatedIndex     = MDyWorld::GetInstance().pfEnrollActiveModelRenderer(*this);
    this->mActivatedUpdateListId  = activatedIndex;
  }
}

void CDyModelRenderer::Deactivate() noexcept
{
  ADyBaseComponent::Deactivate();

  // Customized body ∨
  MDY_NOTUSED auto _ = this->pTryUnbindingToModelFilterComponent();

  // Check and unbind script instance to MDyWorld.
  if (this->mActivateFlag.IsOutputValueChanged() == true && this->mActivateFlag.GetOutput() == false)
  {
    MDyWorld::GetInstance().pfUnenrollActiveModelRenderer(this->mActivatedUpdateListId);
    this->mActivatedUpdateListId = MDY_INITIALIZE_DEFINT;
  }
}

std::optional<TI32> CDyModelRenderer::GetModelSubmeshCount() const noexcept
{
  if (MDY_CHECK_ISNULL(this->mModelFilter))
  { // If CDyModelFilter is not binded to CDyModelRenderer, just return no value.
    return std::nullopt;
  }

  return static_cast<TI32>(this->mModelFilter->GetModelReference()->GetSubmeshResources().size());
}

const CDyMaterialResource& CDyModelRenderer::GetMaterialResourcePtr(_MIN_ const TI32 index) const noexcept
{
  MDY_ASSERT(index < this->mMaterialResourcePtr.size(), "Index for CDyModelRenderer::mMaterialResourcePtr is out of bound.");
  return *this->mMaterialResourcePtr[index];
}

const CDySubmeshResource& CDyModelRenderer::GetSubmeshResourcePtr(const TI32 index) const noexcept
{
  const auto& submeshResourceList = this->mModelFilter->GetModelReference()->GetSubmeshResources();

  return *submeshResourceList[index].get();
}

void CDyModelRenderer::pPropagateParentActorActivation(const DDy3StateBool& actorBool) noexcept
{
  ADyBaseComponent::pPropagateParentActorActivation(actorBool);

  // Customized body ∨
  { MDY_NOTUSED auto _ = this->pTryBindingToModelFilterComponent(); }
  { MDY_NOTUSED auto _ = this->pTryUnbindingToModelFilterComponent(); }

  // Check and rebind script instance to MDyWorld.
  if (this->mActivateFlag.IsOutputValueChanged() == true)
  {
    auto& worldManager = MDyWorld::GetInstance();

    if (this->mActivateFlag.GetOutput() == true)
    { // Check and rebind script instance to MDyWorld.
      const auto activatedIndex     = worldManager.pfEnrollActiveModelRenderer(*this);
      this->mActivatedUpdateListId  = activatedIndex;
    }
    else
    { // Check and unbind script instance to MDyWorld.
      worldManager.pfUnenrollActiveModelRenderer(this->mActivatedUpdateListId);
      this->mActivatedUpdateListId = MDY_INITIALIZE_DEFINT;
    }
  }
}

std::string CDyModelRenderer::ToString()
{
  MDY_NOT_IMPLEMENTED_ASSERT();
  return MDY_INITIALIZE_EMPTYSTR;
}

EDySuccess CDyModelRenderer::pTryBindingToModelFilterComponent()
{
  // Check final activation flag and rebind instance to CDyModelRenderer.

  if (this->mActivateFlag.IsOutputValueChanged() == false)  { return DY_FAILURE; }
  if (this->mActivateFlag.GetOutput() == false)             { return DY_FAILURE; }

  auto opRenderer = this->GetBindedActor()->GetGeneralComponent<CDyModelFilter>();
  if (opRenderer.has_value() == false)                      { return DY_FAILURE; }

  CDyModelFilter& filterRef = *opRenderer.value();
  if (filterRef.IsComponentActivated() == false)            { return DY_FAILURE; }

  MDY_ASSERT(MDY_CHECK_ISNULL(this->mModelFilter), "CDyModelFilter::mModelRendererReferencePtr must be null when unbinding.");
  this->BindModelFilterReference(filterRef);

  return DY_SUCCESS;
}

EDySuccess CDyModelRenderer::pTryUnbindingToModelFilterComponent()
{
  if (this->mActivateFlag.IsOutputValueChanged() == false)  { return DY_FAILURE; }
  if (this->mActivateFlag.GetOutput() == true)             { return DY_FAILURE; }

  // Check final activation flag and unbind instance from CDyModelRenderer.
  MDY_ASSERT(MDY_CHECK_ISNOTNULL(this->mModelFilter), "CDyModelFilter::mModelRendererReferencePtr must not be null when unbinding.");
  this->UnbindModelFilterReference();

  return DY_SUCCESS;
}

void CDyModelRenderer::BindModelFilterReference(CDyModelFilter& validReference)
{
  MDY_ASSERT(MDY_CHECK_ISNULL(this->mModelFilter), "CDyModelRenderer::mModelFilter must be a nullptr when binding.");
  this->mModelFilter = &validReference;
  this->mModelFilter->fBindModelRendererReference(*this);
}

void CDyModelRenderer::UnbindModelFilterReference()
{
  MDY_ASSERT(MDY_CHECK_ISNOTNULL(this->mModelFilter), "CDyModelRenderer::mModelFilter must not be a nullptr when binding.");
  this->mModelFilter->fUnbindModelRendererReference();
  this->mModelFilter = MDY_INITIALIZE_NULL;
}

} /// ::dy namespace