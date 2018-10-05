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
#include <Dy/Management/HeapResourceManager.h>

namespace dy
{

CDyModelFilter::CDyModelFilter(FDyActor& actorReference) : ADyGeneralBaseComponent(actorReference)
{ }

EDySuccess CDyModelFilter::Initialize(const DDyModelMetaInformation& metaInfo)
{
  auto& resourceManager = MDyHeapResource::GetInstance();

  // Bind model.
  if (const auto modelResourcePtr = resourceManager.GetModelResource(metaInfo.mModelName);
      MDY_CHECK_ISNULL(modelResourcePtr))
  { // If not exists, make model resource using information but not have it, return fail.
    const auto res = resourceManager.CreateModelResource(metaInfo.mModelName);
    if (res == DY_FAILURE) { return DY_FAILURE; }

    this->mModelReferencePtr = resourceManager.GetModelResource(metaInfo.mModelName);
  }
  else
  {
    this->mModelReferencePtr = modelResourcePtr;
  }

  return DY_SUCCESS;
}

void CDyModelFilter::Release()
{
  this->mModelReferencePtr = nullptr;
}

} /// ::dy namespace
