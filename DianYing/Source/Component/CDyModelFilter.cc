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
#include <Dy/Management/Internal/Render/FDyModelHandlerManager.h>

namespace dy
{

CDyModelFilter::CDyModelFilter(FDyActor& actorReference) : ADyGeneralBaseComponent(actorReference) { }

EDySuccess CDyModelFilter::Initialize(const PDyModelFilterComponentMetaInfo& metaInfo)
{
  // Bind model first.
  this->mModelSpecifier = metaInfo.mDetails.mModelSpecifierName;
  this->mBinderModel.TryRequireResource(this->mModelSpecifier);

  // Get model handle manager, and ask for whether model handler is already bound.
  auto& handleManager = FDyModelHandlerManager::GetInstance();
  if (handleManager.IsBoundModelExist(this->mModelSpecifier) == false)
  {
    // If not, let manager create handler and bound it,
    // and let actor bind transform to handler and register specifier as key.
    MDY_CALL_ASSERT_SUCCESS(handleManager.TryCreateHandler(this->mModelSpecifier));
  }
  handleManager.BindToHandler(this->mModelSpecifier, *this->GetBindedActor(), *this);

  this->Activate();
  return DY_SUCCESS;
}

void CDyModelFilter::Release()
{
  if (MDY_CHECK_ISNOTNULL(this->mModelRendererReferencePtr))
  { // Unbind CDyModelFilter from binded CDyModelRenderer.
    this->mModelRendererReferencePtr->UnbindModelFilterReference();
  }

  // GC
  auto& handleManager = FDyModelHandlerManager::GetInstance();
  if (handleManager.IsBoundModelExist(this->mModelSpecifier) == true)
  {
    handleManager.UnbindToHandler(this->mModelSpecifier, *this->GetBindedActor(), *this);
    // Check 
    if (handleManager.IsActorInfoNeedToBeGc(this->mModelSpecifier, *this->GetBindedActor()) == true)
    {
      MDY_CALL_ASSERT_SUCCESS(handleManager.TryRemoveBoundActor(this->mModelSpecifier, *this->GetBindedActor()));
      if (handleManager.IsBoundModelNeedToGc(this->mModelSpecifier) == true)
      {
        MDY_CALL_ASSERT_SUCCESS(handleManager.TryRemoveBoundModel(this->mModelSpecifier));
      }
    }
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

  MDY_ASSERT_MSG(MDY_CHECK_ISNULL(this->mModelRendererReferencePtr), "CDyModelFilter::mModelRendererReferencePtr must be null when unbinding.");
  rendererRef.BindModelFilterReference(*this);
}

void CDyModelFilter::TryDeactivateInstance()
{
  // Check final activation flag and unbind instance from CDyModelRenderer.
  MDY_ASSERT_MSG(MDY_CHECK_ISNOTNULL(this->mModelRendererReferencePtr), "CDyModelFilter::mModelRendererReferencePtr must not be null when unbinding.");
  this->mModelRendererReferencePtr->UnbindModelFilterReference();
}

const TDyResourceBinderModel& CDyModelFilter::GetModelReference() const noexcept
{
  return this->mBinderModel;
}

TDyResourceBinderModel& CDyModelFilter::GetModelReference() noexcept
{
  return this->mBinderModel;
}

const std::string& CDyModelFilter::GetModelSpecifier() const noexcept
{
  return this->mModelSpecifier;
}

void CDyModelFilter::fBindModelRendererReference(CDyModelRenderer& validReference)
{
  MDY_ASSERT_MSG(MDY_CHECK_ISNULL(this->mModelRendererReferencePtr), "CDyModelFilter::mModelRendererReferencePtr must be null when unbinding.");
  this->mModelRendererReferencePtr = &validReference;
}

void CDyModelFilter::fUnbindModelRendererReference()
{
  MDY_ASSERT_MSG(MDY_CHECK_ISNOTNULL(this->mModelRendererReferencePtr), "CDyModelFilter::mModelRendererReferencePtr must not be null when unbinding.");
  this->mModelRendererReferencePtr = MDY_INITIALIZE_NULL;
}

} /// ::dy namespace
