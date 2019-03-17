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
#include <Dy/Core/Thread/SDyIOConnectionHelper.h>
#include <Dy/Core/GDyEngine.h>
#include <Dy/Helper/Library/HelperString.h>
#include <Dy/Meta/Information/MetaInfoMaterial.h>

namespace dy
{

void SDyIOConnectionHelper::PopulateResource(
    _MIN_ const std::string& specifier,
    _MIN_ EResourceType resourceType,
    _MIN_ EDyResourceStyle resourceStyle,
    _MIN_ EResourceScope scope)
{
  MDY_ASSERT_MSG(MDY_CHECK_ISNOTNULL(gEngine), "gEngine must not be null.");
  auto& ioThread = *gEngine->pfGetIOThread();
  
  // Check resourceType is `Skeleton` and `AnimationScrap` which can not be created as `Resource` but only `Information`.
  if (resourceType == EResourceType::Skeleton
  ||  resourceType == EResourceType::AnimationScrap)
  {
    if (resourceStyle == EDyResourceStyle::Resource) { resourceStyle = EDyResourceStyle::Information; }
  }

  MDY_CALL_ASSERT_SUCCESS(ioThread.outTryEnqueueTask(specifier, resourceType, resourceStyle, scope));
}

void SDyIOConnectionHelper::TryStop()
{
  MDY_ASSERT_MSG(MDY_CHECK_ISNOTNULL(gEngine), "gEngine must not be null.");
  auto& ioThread = *gEngine->pfGetIOThread();
  ioThread.outTryStop();
}

void SDyIOConnectionHelper::TryGC(_MIN_ EResourceScope iScope, _MIN_ EDyResourceStyle iStyle)
{
  MDY_ASSERT_MSG(MDY_CHECK_ISNOTNULL(gEngine), "gEngine must not be null.");
  auto& ioThread = *gEngine->pfGetIOThread();
  ioThread.outTryForwardCandidateRIToGCList(iScope, iStyle);
}

void SDyIOConnectionHelper::PopulateResourceList(
    _MIN_ const std::vector<DDyResourceName>& specifierList, 
    _MIN_ const EResourceScope iScope,
    _MIN_ std::function<void(void)> callback)
{
  for (const auto& [type, specifier] : specifierList)
  {
    PopulateResource(specifier, type, EDyResourceStyle::Resource, iScope);
  }

  MDY_ASSERT_MSG(MDY_CHECK_ISNOTNULL(gEngine), "gEngine must not be null.");
  auto& ioThread = *gEngine->pfGetIOThread();
  ioThread.BindSleepCallbackFunction(callback);
}

void SDyIOConnectionHelper::PopulateResourceList(
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
      { PopulateResource(specifier, type, EDyResourceStyle::Information, iScope); }
      else // Other type can be populated with Resource & Information.
      { PopulateResource(specifier, type, EDyResourceStyle::Resource, iScope); }
    }
  }

  MDY_ASSERT_MSG(MDY_CHECK_ISNOTNULL(gEngine), "gEngine must not be null.");
  auto& ioThread = *gEngine->pfGetIOThread();
  ioThread.BindSleepCallbackFunction(callback);
}

void SDyIOConnectionHelper::PopulateResourceList(
    _MIN_ const TDDyResourceNameSet& iSpecifierSet, 
    _MIN_ const EResourceScope iScope,
    _MIN_ std::function<void()> iCallback)
{
  for (const auto& [type, specifier] : iSpecifierSet)
  {
    PopulateResource(specifier, type, EDyResourceStyle::Resource, iScope);
  }

  MDY_ASSERT_MSG(MDY_CHECK_ISNOTNULL(gEngine), "gEngine must not be null.");
  auto& ioThread = *gEngine->pfGetIOThread();
  ioThread.BindSleepCallbackFunction(iCallback);
}

void SDyIOConnectionHelper::PopulateInstantMaterialResource(
    _MIN_ PDyMaterialInstanceMetaInfo& desc,
    _MIN_ TResourceBinder<EResourceType::Material>& refMat, 
    _MIN_ bool(*callback)())
{
  static TU32 instantMatId = 0;

  // Specify `scope`.
  EResourceScope scope = EResourceScope::Temporal;
  if (callback != nullptr) { scope = EResourceScope::UserDefined; }

  // Set instant specifier name.
  MDY_ASSERT_MSG(MDY_CHECK_ISNOTNULL(gEngine), "gEngine must not be null.");
  auto& ioThread = *gEngine->pfGetIOThread();
  desc.mSpecifierName = fmt::format("instant{}{:04}", CreateRandomString(5), instantMatId++);
  
  // Require population instant mateiral resource.
  MDY_CALL_ASSERT_SUCCESS(ioThread.InstantPopulateMaterialResource(desc, refMat, scope, callback));
  refMat.TryRequireResource(desc.mSpecifierName);
}

bool SDyIOConnectionHelper::IsReferenceInstanceExist(
    _MIN_ const std::string& iSpecifier, 
    _MIN_ EResourceType iType, 
    _MIN_ EDyResourceStyle iStyle)
{
  MDY_ASSERT_MSG(MDY_CHECK_ISNOTNULL(gEngine), "gEngine must not be null.");
  auto& ioThread = *gEngine->pfGetIOThread();
  return ioThread.pIsReferenceInstanceExist(iSpecifier, iType, iStyle);
}

void SDyIOConnectionHelper::InsertResult(_MIN_ const DDyIOWorkerResult& result) noexcept
{
  MDY_ASSERT_MSG(MDY_CHECK_ISNOTNULL(gEngine), "gEngine must not be null.");
  auto& ioThread = *gEngine->pfGetIOThread();
  ioThread.outInsertResult(result);
}

void SDyIOConnectionHelper::InsertGcCandidate(std::unique_ptr<DDyIOReferenceInstance>&& iRefRI)
{
  MDY_ASSERT_MSG(MDY_CHECK_ISNOTNULL(gEngine), "gEngine must not be null.");
  auto& ioThread = *gEngine->pfGetIOThread();
  ioThread.outInsertGcCandidate(iRefRI);
}

bool SDyIOConnectionHelper::CheckIOResultInCondition() noexcept
{
  MDY_ASSERT_MSG(MDY_CHECK_ISNOTNULL(gEngine), "gEngine must not be null.");
  auto& ioThread = *gEngine->pfGetIOThread();
  return ioThread.outCheckIOResultInCondition();
}

bool SDyIOConnectionHelper::IsMainTaskListIsEmpty() noexcept
{
  MDY_ASSERT_MSG(MDY_CHECK_ISNOTNULL(gEngine), "gEngine must not be null.");
  auto& ioThread = *gEngine->pfGetIOThread();
  return ioThread.isoutIsMainTaskListIsEmpty();
}

void SDyIOConnectionHelper::ForceProcessDeferredMainTaskList()
{
  MDY_ASSERT_MSG(MDY_CHECK_ISNOTNULL(gEngine), "gEngine must not be null.");
  auto& ioThread = *gEngine->pfGetIOThread();
  ioThread.outMainForceProcessDeferredMainTaskList();
}

void SDyIOConnectionHelper::ForceProcessIOInsertPhase()
{
  MDY_ASSERT_MSG(MDY_CHECK_ISNOTNULL(gEngine), "gEngine must not be null.");
  auto& ioThread = *gEngine->pfGetIOThread();
  ioThread.outForceProcessIOInsertPhase();
}

bool SDyIOConnectionHelper::IsIOThreadSleep()
{
  MDY_ASSERT_MSG(MDY_CHECK_ISNOTNULL(gEngine), "gEngine must not be null.");
  auto& ioThread = *gEngine->pfGetIOThread();
  return ioThread.outIsIOThreadSlept();
}

EDySuccess SDyIOConnectionHelper::TryCallSleptCallbackFunction()
{
  MDY_ASSERT_MSG(MDY_CHECK_ISNOTNULL(gEngine), "gEngine must not be null.");
  auto& ioThread = *gEngine->pfGetIOThread();
  return ioThread.outTryCallSleptCallbackFunction();
}

} /// ::dy namespace