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
#include <Dy/Component/CDyModelFilter.h>
#include <Dy/Element/Actor.h>
#include <Dy/Component/CDyModelRenderer.h>

namespace dy
{

CDyModelFilter::CDyModelFilter(FDyActor& actorReference) : ADyGeneralBaseComponent(actorReference) { }

EDySuccess CDyModelFilter::Initialize(const PDyModelFilterComponentMetaInfo& metaInfo)
{
  // Bind model first.
  this->mModelSpecifier = metaInfo.mDetails.mModelSpecifierName;
  this->mBinderModel.TryRequireResource(this->mModelSpecifier);

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

std::string CDyModelFilter::ToString()
{
  MDY_NOT_IMPLEMENTED_ASSERT();
  return MDY_INITIALIZE_EMPTYSTR;
}

void CDyModelFilter::TryActivateInstance()
{
  auto opRenderer = this->GetBindedActor()->GetGeneralComponent<CDyModelRenderer>();
  if (opRenderer.has_value() == false) { return; }

  CDyModelRenderer& rendererRef = *opRenderer.value();
  if (rendererRef.IsComponentActivated() == false) { return; }

  MDY_ASSERT(MDY_CHECK_ISNULL(this->mModelRendererReferencePtr), "CDyModelFilter::mModelRendererReferencePtr must be null when unbinding.");
  rendererRef.BindModelFilterReference(*this);
}

void CDyModelFilter::TryDeactivateInstance()
{
  // Check final activation flag and unbind instance from CDyModelRenderer.
  MDY_ASSERT(MDY_CHECK_ISNOTNULL(this->mModelRendererReferencePtr), "CDyModelFilter::mModelRendererReferencePtr must not be null when unbinding.");
  this->mModelRendererReferencePtr->UnbindModelFilterReference();
}

const TDyLResourceBinderModel& CDyModelFilter::GetModelReference() const noexcept
{
  return this->mBinderModel;
}

TDyLResourceBinderModel& CDyModelFilter::GetModelReference() noexcept
{
  return this->mBinderModel;
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
