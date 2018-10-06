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

namespace dy
{

CDyModelRenderer::CDyModelRenderer(FDyActor& actorReference) : ADyGeneralBaseComponent(actorReference)
{ }

EDySuccess CDyModelRenderer::Initialize(const DDyModelRendererMetaInformation& descriptor)
{
  auto* bindedActor = this->GetBindedActor();
  auto  opRenderer  = bindedActor->GetGeneralComponent<CDyModelFilter>();
  if (opRenderer.has_value())
  {
    opRenderer.value()->fBindModelRendererReference(*this);
  }

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

void CDyModelRenderer::Activate() noexcept
{
  ADyBaseComponent::Activate();

  // Customized body ∨
  MDY_CALL_ASSERT_SUCCESS(this->pTryBindingToModelFilterComponent())
}

void CDyModelRenderer::Deactivate() noexcept
{
  ADyBaseComponent::Deactivate();

  // Customized body ∨
  MDY_CALL_ASSERT_SUCCESS(this->pTryUnbindingToModelFilterComponent())
}

void CDyModelRenderer::pPropagateParentActorActivation(const DDy3StateBool& actorBool) noexcept
{
  ADyBaseComponent::pPropagateParentActorActivation(actorBool);

  // Customized body ∨
  MDY_CALL_ASSERT_SUCCESS(this->pTryBindingToModelFilterComponent());
  MDY_CALL_ASSERT_SUCCESS(this->pTryUnbindingToModelFilterComponent())
}

std::string CDyModelRenderer::ToString()
{
  PHITOS_NOT_IMPLEMENTED_ASSERT();
  return MDY_INITILAIZE_EMPTYSTR;
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

  PHITOS_ASSERT(MDY_CHECK_ISNULL(this->mModelFilter), "CDyModelFilter::mModelRendererReferencePtr must be null when unbinding.");
  this->BindModelFilterReference(filterRef);

  return DY_SUCCESS;
}

EDySuccess CDyModelRenderer::pTryUnbindingToModelFilterComponent()
{
  if (this->mActivateFlag.IsOutputValueChanged() == false)  { return DY_FAILURE; }
  if (this->mActivateFlag.GetOutput() == true)             { return DY_FAILURE; }

  // Check final activation flag and unbind instance from CDyModelRenderer.
  PHITOS_ASSERT(MDY_CHECK_ISNOTNULL(this->mModelFilter), "CDyModelFilter::mModelRendererReferencePtr must not be null when unbinding.");
  this->UnbindModelFilterReference();

  return DY_SUCCESS;
}

void CDyModelRenderer::BindModelFilterReference(CDyModelFilter& validReference)
{
  PHITOS_ASSERT(MDY_CHECK_ISNULL(this->mModelFilter), "CDyModelRenderer::mModelFilter must be a nullptr when binding.");
  this->mModelFilter = &validReference;
  this->mModelFilter->fBindModelRendererReference(*this);
}

void CDyModelRenderer::UnbindModelFilterReference()
{
  PHITOS_ASSERT(MDY_CHECK_ISNOTNULL(this->mModelFilter), "CDyModelRenderer::mModelFilter must not be a nullptr when binding.");
  this->mModelFilter->fUnbindModelRendererReference();
  this->mModelFilter = MDY_INITIALIZE_NULL;
}

} /// ::dy namespace