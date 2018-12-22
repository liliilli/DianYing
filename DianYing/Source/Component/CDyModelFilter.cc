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
#include <Dy/Component/CDyModelFilter.h>
#include <Dy/Element/Actor.h>
#include <Dy/Component/CDyModelRenderer.h>

namespace dy
{

CDyModelFilter::CDyModelFilter(FDyActor& actorReference) : ADyGeneralBaseComponent(actorReference) { }

EDySuccess CDyModelFilter::Initialize(const PDyModelFilterComponentMetaInfo& metaInfo)
{
  const auto& modelSpecfier = metaInfo.mDetails.mModelSpecifierName;
  mBinderModel.TryRequireResource(modelSpecfier);

  this->Activate();
  return DY_SUCCESS;
}

void CDyModelFilter::Release()
{
  if (MDY_CHECK_ISNOTNULL(this->mModelRendererReferencePtr))
  { // Unbind CDyModelFilter from binded CDyModelRenderer.
    this->mModelRendererReferencePtr->UnbindModelFilterReference();
  }
}

void CDyModelFilter::Activate() noexcept
{
  ADyBaseComponent::Activate();

  // Customized body ∨
  MDY_NOTUSED auto _ = this->pTryBindingToModelRendererComponent();
}

void CDyModelFilter::Deactivate() noexcept
{
  ADyBaseComponent::Deactivate();

  // Customized body ∨

  MDY_NOTUSED auto _ = this->pTryUnbindingToModelRendererComponent();
}

void CDyModelFilter::pPropagateParentActorActivation(const DDy3StateBool& actorBool) noexcept
{
  ADyBaseComponent::pPropagateParentActorActivation(actorBool);

  // Customized body ∨

  { MDY_NOTUSED auto _ = this->pTryBindingToModelRendererComponent(); }
  { MDY_NOTUSED auto _ = this->pTryUnbindingToModelRendererComponent(); }
}

std::string CDyModelFilter::ToString()
{
  MDY_NOT_IMPLEMENTED_ASSERT();
  return MDY_INITIALIZE_EMPTYSTR;
}

EDySuccess CDyModelFilter::pTryBindingToModelRendererComponent()
{
  // Check final activation flag and rebind instance to CDyModelRenderer.

  if (this->mActivateFlag.IsOutputValueChanged() == false)  { return DY_FAILURE; }
  if (this->mActivateFlag.GetOutput() == false)             { return DY_FAILURE; }

  auto opRenderer = this->GetBindedActor()->GetGeneralComponent<CDyModelRenderer>();
  if (opRenderer.has_value() == false)                      { return DY_FAILURE; }

  CDyModelRenderer& rendererRef = *opRenderer.value();
  if (rendererRef.IsComponentActivated() == false)          { return DY_FAILURE; }

  MDY_ASSERT(MDY_CHECK_ISNULL(this->mModelRendererReferencePtr), "CDyModelFilter::mModelRendererReferencePtr must be null when unbinding.");
  rendererRef.BindModelFilterReference(*this);

  return DY_SUCCESS;
}

EDySuccess CDyModelFilter::pTryUnbindingToModelRendererComponent()
{
  if (this->mActivateFlag.IsOutputValueChanged() == false)  { return DY_FAILURE; }
  if (this->mActivateFlag.GetOutput() == true)             { return DY_FAILURE; }

  // Check final activation flag and unbind instance from CDyModelRenderer.
  MDY_ASSERT(MDY_CHECK_ISNOTNULL(this->mModelRendererReferencePtr), "CDyModelFilter::mModelRendererReferencePtr must not be null when unbinding.");
  this->mModelRendererReferencePtr->UnbindModelFilterReference();

  return DY_SUCCESS;
}

void CDyModelFilter::fBindModelRendererReference(CDyModelRenderer& validReference)
{
  MDY_ASSERT(MDY_CHECK_ISNULL(this->mModelRendererReferencePtr), "CDyModelFilter::mModelRendererReferencePtr must be null when unbinding.");
  this->mModelRendererReferencePtr = &validReference;
}

void CDyModelFilter::fUnbindModelRendererReference()
{
  MDY_ASSERT(MDY_CHECK_ISNOTNULL(this->mModelRendererReferencePtr), "CDyModelFilter::mModelRendererReferencePtr must not be null when unbinding.");
  this->mModelRendererReferencePtr = MDY_INITIALIZE_NULL;
}

} /// ::dy namespace
