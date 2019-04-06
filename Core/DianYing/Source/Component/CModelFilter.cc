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
#include <Dy/Component/CModelFilter.h>
#include <Dy/Element/FActor.h>
#include <Dy/Component/CModelRenderer.h>
#include <Dy/Management/Internal/Render/FDyModelHandlerManager.h>

namespace dy
{

CModelFilter::CModelFilter(FActor& actorReference) : AGeneralBaseComponent(actorReference) { }

EDySuccess CModelFilter::Initialize(const PDyModelFilterComponentMetaInfo& metaInfo)
{
  // Bind model first.
  this->mModelSpecifier = metaInfo.mDetails.mModelSpecifierName;
  if (this->mModelSpecifier.empty() == false)
  {
    this->mBinderModel.TryRequireResource(this->mModelSpecifier);
  }

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

void CModelFilter::Release()
{
  if (MDY_CHECK_ISNOTNULL(this->mModelRendererReferencePtr))
  { // Unbind CModelFilter from binded CModelRenderer.
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

std::string CModelFilter::ToString()
{
  MDY_NOT_IMPLEMENTED_ASSERT();
  return MDY_INITIALIZE_EMPTYSTR;
}

void CModelFilter::TryActivateInstance()
{
  auto opRenderer = this->GetBindedActor()->GetGeneralComponent<CModelRenderer>();
  if (opRenderer.has_value() == false) { return; }

  CModelRenderer& rendererRef = *opRenderer.value();
  if (rendererRef.IsComponentActivated() == false) { return; }

  MDY_ASSERT_MSG(MDY_CHECK_ISNULL(this->mModelRendererReferencePtr), "CModelFilter::mModelRendererReferencePtr must be null when unbinding.");
  rendererRef.BindModelFilterReference(*this);
}

void CModelFilter::TryDeactivateInstance()
{
  // Check final activation flag and unbind instance from CModelRenderer.
  MDY_ASSERT_MSG(MDY_CHECK_ISNOTNULL(this->mModelRendererReferencePtr), "CModelFilter::mModelRendererReferencePtr must not be null when unbinding.");
  this->mModelRendererReferencePtr->UnbindModelFilterReference();
}

const TDyResourceBinderModel& CModelFilter::GetModelReference() const noexcept
{
  return this->mBinderModel;
}

TDyResourceBinderModel& CModelFilter::GetModelReference() noexcept
{
  return this->mBinderModel;
}

const std::string& CModelFilter::GetModelSpecifier() const noexcept
{
  return this->mModelSpecifier;
}

void CModelFilter::fBindModelRendererReference(CModelRenderer& validReference)
{
  MDY_ASSERT_MSG(MDY_CHECK_ISNULL(this->mModelRendererReferencePtr), "CModelFilter::mModelRendererReferencePtr must be null when unbinding.");
  this->mModelRendererReferencePtr = &validReference;
}

void CModelFilter::fUnbindModelRendererReference()
{
  MDY_ASSERT_MSG(MDY_CHECK_ISNOTNULL(this->mModelRendererReferencePtr), "CModelFilter::mModelRendererReferencePtr must not be null when unbinding.");
  this->mModelRendererReferencePtr = MDY_INITIALIZE_NULL;
}

} /// ::dy namespace
