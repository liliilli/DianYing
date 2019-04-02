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
#include <Dy/Core/Thread/SIOConnectionHelper.h>
#include <Dy/Core/GEngine.h>
#include <Dy/Helper/Library/HelperString.h>
#include <Dy/Meta/Information/MetaInfoMaterial.h>

namespace dy
{

void SIOConnectionHelper::PopulateResource(
    _MIN_ const std::string& specifier,
    _MIN_ EResourceType resourceType,
    _MIN_ EResourceStyle resourceStyle,
    _MIN_ EResourceScope scope)
{
  MDY_ASSERT_MSG(MDY_CHECK_ISNOTNULL(gEngine), "gEngine must not be null.");
  auto& ioThread = *gEngine->pfGetIOThread();
  
  // Check resourceType is `Skeleton` and `AnimationScrap` which can not be created as `Resource` but only `Information`.
  if (resourceType == EResourceType::Skeleton
  ||  resourceType == EResourceType::AnimationScrap)
  {
    if (resourceStyle == EResourceStyle::Resource) { resourceStyle = EResourceStyle::Information; }
  }

  MDY_CALL_ASSERT_SUCCESS(ioThread.outTryEnqueueTask(specifier, resourceType, resourceStyle, scope));
}

void SIOConnectionHelper::TryStop()
{
  MDY_ASSERT_MSG(gEngine != nullptr, "gEngine must not be null.");
  auto& ioThread = *gEngine->pfGetIOThread();
  ioThread.SyncTryStop();
}

void SIOConnectionHelper::TryGC(EResourceScope iScope, EResourceStyle iStyle)
{
  MDY_ASSERT_MSG(gEngine != nullptr, "gEngine must not be null.");
  auto& ioThread = *gEngine->pfGetIOThread();
  ioThread.outTryForwardCandidateRIToGCList(iScope, iStyle);
}

void SIOConnectionHelper::PopulateResourceList(
    _MIN_ const std::vector<DDyResourceName>& specifierList, 
    _MIN_ const EResourceScope iScope,
    _MIN_ std::function<void(void)> callback)
{
  for (const auto& [type, specifier] : specifierList)
  {
    PopulateResource(specifier, type, EResourceStyle::Resource, iScope);
  }

  MDY_ASSERT_MSG(MDY_CHECK_ISNOTNULL(gEngine), "gEngine must not be null.");
  auto& ioThread = *gEngine->pfGetIOThread();
  ioThread.BindSleepCallbackFunction(callback);
}

void SIOConnectionHelper::PopulateResourceList(
    _MIN_ const std::vector<std::vector<DDyResourceName>>& specifierList, 
    _MIN_ const EResourceScope iScope,
    _MIN_ std::function<void(void)> callback)
{
  for (const auto& list : specifierList)
  {
    for (const auto& [type, specifier] : list)
    {
      // If `type` is AnimationScrap or model skeleton, this must be populated only as `Style::Information`.
      if (type == EResourceType::AnimationScrap
      ||  type == EResourceType::Skeleton
      ||  type == EResourceType::Sound)
      { PopulateResource(specifier, type, EResourceStyle::Information, iScope); }
      else // Other type can be populated with Resource & Information.
      { PopulateResource(specifier, type, EResourceStyle::Resource, iScope); }
    }
  }

  MDY_ASSERT_MSG(MDY_CHECK_ISNOTNULL(gEngine), "gEngine must not be null.");
  auto& ioThread = *gEngine->pfGetIOThread();
  ioThread.BindSleepCallbackFunction(callback);
}

void SIOConnectionHelper::PopulateResourceList(
    _MIN_ const TDDyResourceNameSet& iSpecifierSet, 
    _MIN_ const EResourceScope iScope,
    _MIN_ std::function<void()> iCallback)
{
  for (const auto& [type, specifier] : iSpecifierSet)
  {
    PopulateResource(specifier, type, EResourceStyle::Resource, iScope);
  }

  MDY_ASSERT_MSG(MDY_CHECK_ISNOTNULL(gEngine), "gEngine must not be null.");
  auto& ioThread = *gEngine->pfGetIOThread();
  ioThread.BindSleepCallbackFunction(iCallback);
}

void SIOConnectionHelper::PopulateInstantMaterialResource(
    _MIN_ PDyMaterialInstanceMetaInfo& desc,
    _MIN_ TResourceBinder<EResourceType::Material>& refMat, 
    _MIN_ bool(*callback)())
{
  static TU32 instantMatId = 0;

  // Specify `scope`.
  EResourceScope scope = EResourceScope::Temporal;
  if (callback != nullptr) { scope = EResourceScope::UserDefined; }

  // Set instant specifier name.
  MDY_ASSERT_MSG(gEngine != nullptr, "gEngine must not be null.");
  auto& ioThread = *gEngine->pfGetIOThread();
  desc.mSpecifierName = fmt::format("instant{}{:04}", CreateRandomString(5), instantMatId++);
  
  // Require population instant mateiral resource.
  MDY_CALL_ASSERT_SUCCESS(ioThread.InstantPopulateMaterialResource(desc, refMat, scope, callback));
  refMat.TryRequireResource(desc.mSpecifierName);
}

bool SIOConnectionHelper::IsReferenceInstanceExist(
  const std::string& iSpecifier, 
  EResourceType iType, EResourceStyle iStyle)
{
  MDY_ASSERT_MSG(gEngine != nullptr, "gEngine must not be null.");
  auto& ioThread = *gEngine->pfGetIOThread();
  return ioThread.pIsReferenceInstanceExist(iSpecifier, iType, iStyle);
}

bool SIOConnectionHelper::IsGcCandidateExist() noexcept
{
  MDY_ASSERT_MSG(gEngine != nullptr, "gEngine must not be null.");
  auto& ioThread = *gEngine->pfGetIOThread();
  return ioThread.IsGcCandidateExist();
}

void SIOConnectionHelper::TryGC()
{
  MDY_ASSERT_MSG(gEngine != nullptr, "gEngine must not be null.");
  auto& ioThread = *gEngine->pfGetIOThread();
  return ioThread.TryGC();
}

void SIOConnectionHelper::InsertWorkerResult(const DRescIOWorkerResult& result) noexcept
{
  MDY_ASSERT_MSG(gEngine != nullptr, "gEngine must not be null.");
  auto& ioThread = *gEngine->pfGetIOThread();
  ioThread.SyncInsertResult(result);
}

void SIOConnectionHelper::InsertGcCandidate(std::unique_ptr<DIOReferenceInstance>&& iRefRI)
{
  MDY_ASSERT_MSG(gEngine != nullptr, "gEngine must not be null.");
  auto& ioThread = *gEngine->pfGetIOThread();
  ioThread.outInsertGcCandidate(iRefRI);
}

bool SIOConnectionHelper::IsWorkerResultExist() noexcept
{
  MDY_ASSERT_MSG(gEngine != nullptr, "gEngine must not be null.");
  auto& ioThread = *gEngine->pfGetIOThread();
  return ioThread.SyncIsWorkerResultExist();
}

bool SIOConnectionHelper::IsMainTaskListEmpty() noexcept
{
  MDY_ASSERT_MSG(gEngine != nullptr, "gEngine must not be null.");
  auto& ioThread = *gEngine->pfGetIOThread();
  return ioThread.isoutIsMainTaskListIsEmpty();
}

void SIOConnectionHelper::ForceProcessDeferredMainTaskList()
{
  MDY_ASSERT_MSG(MDY_CHECK_ISNOTNULL(gEngine), "gEngine must not be null.");
  auto& ioThread = *gEngine->pfGetIOThread();
  ioThread.outMainForceProcessDeferredMainTaskList();
}

void SIOConnectionHelper::ForceProcessIOInsertPhase()
{
  MDY_ASSERT_MSG(MDY_CHECK_ISNOTNULL(gEngine), "gEngine must not be null.");
  auto& ioThread = *gEngine->pfGetIOThread();
  ioThread.outForceProcessIOInsertPhase();
}

bool SIOConnectionHelper::IsIOThreadSleep()
{
  MDY_ASSERT_MSG(MDY_CHECK_ISNOTNULL(gEngine), "gEngine must not be null.");
  auto& ioThread = *gEngine->pfGetIOThread();
  return ioThread.outIsIOThreadSlept();
}

EDySuccess SIOConnectionHelper::TryCallSleptCallbackFunction()
{
  MDY_ASSERT_MSG(MDY_CHECK_ISNOTNULL(gEngine), "gEngine must not be null.");
  auto& ioThread = *gEngine->pfGetIOThread();
  return ioThread.outTryCallSleptCallbackFunction();
}

} /// ::dy namespace