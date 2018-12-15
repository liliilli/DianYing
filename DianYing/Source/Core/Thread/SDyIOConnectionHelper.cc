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
#include <Dy/Core/Thread/SDyIOConnectionHelper.h>
#include <Dy/Core/DyEngine.h>

namespace dy
{

void SDyIOConnectionHelper::PopulateResource(
    _MIN_ const std::string& specifier,
    _MIN_ EDyResourceType resourceType,
    _MIN_ EDyResourceStyle resourceStyle,
    _MIN_ EDyScope scope)
{
  MDY_ASSERT(MDY_CHECK_ISNOTNULL(gEngine), "gEngine must not be null.");
  auto& ioThread = *gEngine->pfGetIOThread();
  MDY_CALL_ASSERT_SUCCESS(ioThread.outTryEnqueueTask(specifier, resourceType, resourceStyle, scope));
}

void SDyIOConnectionHelper::TryGC()
{
  MDY_NOT_IMPLEMENTED_ASSERT();
}

void SDyIOConnectionHelper::TryStop()
{
  MDY_ASSERT(MDY_CHECK_ISNOTNULL(gEngine), "gEngine must not be null.");
  auto& ioThread = *gEngine->pfGetIOThread();
  ioThread.outTryStop();
}

void SDyIOConnectionHelper::PopulateResourceList(_MIN_ const std::vector<DDyResourceName>& specifierList, _MIN_ bool isWaited)
{
  MDY_ASSERT(isWaited == true, "non-wait version not supported.");
  for (const auto& [type, specifier] : specifierList)
  {
    PopulateResource(specifier, type, EDyResourceStyle::Resource, EDyScope::Global);
  }
}

void SDyIOConnectionHelper::InsertResult(_MIN_ const DDyIOWorkerResult& result) noexcept
{
  MDY_ASSERT(MDY_CHECK_ISNOTNULL(gEngine), "gEngine must not be null.");
  auto& ioThread = *gEngine->pfGetIOThread();
  ioThread.outInsertResult(result);
}

bool SDyIOConnectionHelper::CheckIOResultInCondition() noexcept
{
  MDY_ASSERT(MDY_CHECK_ISNOTNULL(gEngine), "gEngine must not be null.");
  auto& ioThread = *gEngine->pfGetIOThread();
  return ioThread.outCheckIOResultInCondition();
}

bool SDyIOConnectionHelper::IsMainTaskListIsEmpty() noexcept
{
  MDY_ASSERT(MDY_CHECK_ISNOTNULL(gEngine), "gEngine must not be null.");
  auto& ioThread = *gEngine->pfGetIOThread();
  return ioThread.isoutIsMainTaskListIsEmpty();
}

void SDyIOConnectionHelper::ForceProcessDeferredMainTaskList()
{
  MDY_ASSERT(MDY_CHECK_ISNOTNULL(gEngine), "gEngine must not be null.");
  auto& ioThread = *gEngine->pfGetIOThread();
  ioThread.outMainForceProcessDeferredMainTaskList();
}

void SDyIOConnectionHelper::ForceProcessIOInsertPhase()
{
  MDY_ASSERT(MDY_CHECK_ISNOTNULL(gEngine), "gEngine must not be null.");
  auto& ioThread = *gEngine->pfGetIOThread();
  ioThread.outForceProcessIOInsertPhase();
}

bool SDyIOConnectionHelper::IsIOThreadSleep()
{
  MDY_ASSERT(MDY_CHECK_ISNOTNULL(gEngine), "gEngine must not be null.");
  auto& ioThread = *gEngine->pfGetIOThread();
  return ioThread.outIsIOThreadSlept();
}

MDY_NODISCARD EDySuccess
SDyIOConnectionHelper::pTryRequireResource(_MIN_ const std::string& iSpecifier, _MIN_ EDyResourceType iType, _MIN_ const __FDyBinderBase* iPtrBinder)
{
  MDY_ASSERT(MDY_CHECK_ISNOTNULL(gEngine), "gEngine must not be null.");
  auto& ioThread = *gEngine->pfGetIOThread();

  if (ioThread.pIsReferenceInstanceExist(iSpecifier, iType, EDyResourceStyle::Resource) == false)
  { // If binding is failed, it specifies that RI has not been created (Related task queue was not created neither).
    // So we need populate task queue for resource as temporary.
    MDY_NOT_IMPLEMENTED_ASSERT();
    return DY_FAILURE;
  }

  MDY_CALL_ASSERT_SUCCESS(ioThread.TryBindBinderToResourceRI(iSpecifier, iType, iPtrBinder));
  if (ioThread.pIsReferenceInstanceBound(iSpecifier, iType, EDyResourceStyle::Resource) == false)
  {
    return DY_FAILURE;
  }

  return DY_SUCCESS;
}

std::optional<const __TResourceType<EDyResourceType::GLShader>::type*>
SDyIOConnectionHelper::pTryRequireResource_GLShader(_MIN_ const std::string& iSpecifier, _MIN_ const __FDyBinderBase* iPtrBinder)
{
  const auto checkFlag = pTryRequireResource(iSpecifier, EDyResourceType::GLShader, iPtrBinder);
  if (checkFlag == DY_FAILURE) { return std::nullopt; }

  // If RI has been bound, try get pointer of resource but if nullptr, just return null or valid pointer as optional.
  const auto& ioResourceManager = MDyIOResource::GetInstance();
  const auto* ptr = ioResourceManager.GetPtrInformation<EDyResourceType::GLShader>(iSpecifier);
  MDY_ASSERT(MDY_CHECK_ISNOTNULL(ptr), "Ptr must be valid.");
  return ptr;
}

std::optional<const __TResourceType_T<EDyResourceType::Model>*>
SDyIOConnectionHelper::pTryRequireResource_Model(_MIN_ const std::string& iSpecifier, _MIN_ const __FDyBinderBase* iPtrBinder)
{
  const auto checkFlag = pTryRequireResource(iSpecifier, EDyResourceType::Model, iPtrBinder);
  if (checkFlag == DY_FAILURE) { return std::nullopt; }

  // If RI has been bound, try get pointer of resource but if nullptr, just return null or valid pointer as optional.
  const auto& ioResourceManager = MDyIOResource::GetInstance();
  const auto* ptr = ioResourceManager.GetPtrInformation<EDyResourceType::Model>(iSpecifier);
  MDY_ASSERT(MDY_CHECK_ISNOTNULL(ptr), "Ptr must be valid.");
  return ptr;
}

std::optional<const __TResourceType_T<EDyResourceType::Texture>*>
SDyIOConnectionHelper::pTryRequireResource_Texture(_MIN_ const std::string& iSpecifier, _MIN_ const __FDyBinderBase* iPtrBinder)
{
  const auto checkFlag = pTryRequireResource(iSpecifier, EDyResourceType::Texture, iPtrBinder);
  if (checkFlag == DY_FAILURE) { return std::nullopt; }

  // If RI has been bound, try get pointer of resource but if nullptr, just return null or valid pointer as optional.
  const auto& ioResourceManager = MDyIOResource::GetInstance();
  const auto* ptr = ioResourceManager.GetPtrInformation<EDyResourceType::Texture>(iSpecifier);
  MDY_ASSERT(MDY_CHECK_ISNOTNULL(ptr), "Ptr must be valid.");
  return ptr;
}

std::optional<const __TResourceType_T<EDyResourceType::Material>*>
SDyIOConnectionHelper::pTryRequireResource_Material(_MIN_ const std::string& iSpecifier, _MIN_ const __FDyBinderBase* iPtrBinder)
{
  const auto checkFlag = pTryRequireResource(iSpecifier, EDyResourceType::Material, iPtrBinder);
  if (checkFlag == DY_FAILURE) { return std::nullopt; }

  // If RI has been bound, try get pointer of resource but if nullptr, just return null or valid pointer as optional.
  const auto& ioResourceManager = MDyIOResource::GetInstance();
  const auto* ptr = ioResourceManager.GetPtrInformation<EDyResourceType::Material>(iSpecifier);
  MDY_ASSERT(MDY_CHECK_ISNOTNULL(ptr), "Ptr must be valid.");
  return ptr;
}

} /// ::dy namespace