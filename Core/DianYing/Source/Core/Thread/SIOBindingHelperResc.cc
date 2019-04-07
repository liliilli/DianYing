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
#include <Dy/Core/Thread/SIOBindingHelper.h>
#include <Dy/Core/Thread/IO/EIOTask.h>
#include <Dy/Core/GEngine.h>

namespace dy
{

MDY_NODISCARD EDySuccess
SIOBindingHelper::pTryRequireResource
(const std::string& iSpecifier, EResourceType iType, IBinderBase& iPtrBinder)
{
  MDY_ASSERT_MSG(MDY_CHECK_ISNOTNULL(TEMP_CAST(gEngine)), "TEMP_CAST(gEngine) must not be null.");
  auto& ioThread = *TEMP_CAST(gEngine)->pfGetIOThread();

  if (ioThread.pIsReferenceInstanceExist(iSpecifier, iType, EResourceStyle::Resource) == false)
  { // If binding is failed, it specifies that RI has not been created (Related task queue was not created neither).
    // So we need populate task queue for resource as temporary.
    ioThread.outTryEnqueueTask(iSpecifier, iType, EResourceStyle::Resource, EResourceScope::Temporal);
  }

  MDY_CALL_ASSERT_SUCCESS(ioThread.TryBindBinderToResourceRI(iSpecifier, iType, iPtrBinder));
  if (ioThread.pIsReferenceInstanceBound(iSpecifier, iType, EResourceStyle::Resource) == false)
  {
    return EDySuccess::DY_FAILURE;
  }

  return EDySuccess::DY_SUCCESS;
}

std::optional<const __TResourceType<EResourceType::GLShader>::type*>
SIOBindingHelper::MDY_PRIVATE(pTryRequireResource_GLShader)
(const std::string& iSpecifier, IBinderBase& iPtrBinder)
{
  const auto checkFlag = pTryRequireResource(iSpecifier, EResourceType::GLShader, iPtrBinder);
  if (checkFlag == EDySuccess::DY_FAILURE) { return std::nullopt; }

  // If RI has been bound, try get pointer of resource but if nullptr, just return null or valid pointer as optional.
  const auto& ioResourceManager = MIOResource::GetInstance();
  const auto* ptr = ioResourceManager.GetPtrInformation<EResourceType::GLShader>(iSpecifier);
  MDY_ASSERT_MSG(ptr != nullptr, "Ptr must be valid.");
  return ptr;
}

std::optional<const __TResourceType_T<EResourceType::Mesh>*>
SIOBindingHelper::MDY_PRIVATE(pTryRequireResource_Mesh)
(const std::string& iSpecifier, IBinderBase& iPtrBinder)
{
  const auto checkFlag = pTryRequireResource(iSpecifier, EResourceType::Mesh, iPtrBinder);
  if (checkFlag == EDySuccess::DY_FAILURE) { return std::nullopt; }

  // If RI has been bound, try get pointer of resource but if nullptr, just return null or valid pointer as optional.
  const auto& ioResourceManager = MIOResource::GetInstance();
  const auto* ptr = ioResourceManager.GetPtrInformation<EResourceType::Mesh>(iSpecifier);
  MDY_ASSERT_MSG(ptr != nullptr, "Ptr must be valid.");
  return ptr;
}

std::optional<const __TResourceType_T<EResourceType::Model>*>
SIOBindingHelper::MDY_PRIVATE(pTryRequireResource_Model)
(const std::string& iSpecifier, IBinderBase& iPtrBinder)
{
  const auto checkFlag = pTryRequireResource(iSpecifier, EResourceType::Model, iPtrBinder);
  if (checkFlag == EDySuccess::DY_FAILURE) { return std::nullopt; }

  // If RI has been bound, try get pointer of resource but if nullptr, just return null or valid pointer as optional.
  const auto& ioResourceManager = MIOResource::GetInstance();
  const auto* ptr = ioResourceManager.GetPtrInformation<EResourceType::Model>(iSpecifier);
  MDY_ASSERT_MSG(ptr != nullptr, "Ptr must be valid.");
  return ptr;
}

std::optional<const __TResourceType_T<EResourceType::Texture>*>
SIOBindingHelper::MDY_PRIVATE(pTryRequireResource_Texture)
(const std::string& iSpecifier, IBinderBase& iPtrBinder)
{
  const auto checkFlag = pTryRequireResource(iSpecifier, EResourceType::Texture, iPtrBinder);
  if (checkFlag == EDySuccess::DY_FAILURE) { return std::nullopt; }

  // If RI has been bound, try get pointer of resource but if nullptr, just return null or valid pointer as optional.
  const auto& ioResourceManager = MIOResource::GetInstance();
  const auto* ptr = ioResourceManager.GetPtrInformation<EResourceType::Texture>(iSpecifier);
  MDY_ASSERT_MSG(ptr != nullptr, "Ptr must be valid.");
  return ptr;
}

std::optional<const __TResourceType_T<EResourceType::Material>*>
SIOBindingHelper::MDY_PRIVATE(pTryRequireResource_Material)
(const std::string& iSpecifier, IBinderBase& iPtrBinder)
{
  const auto checkFlag = pTryRequireResource(iSpecifier, EResourceType::Material, iPtrBinder);
  if (checkFlag == EDySuccess::DY_FAILURE) { return std::nullopt; }

  // If RI has been bound, try get pointer of resource but if nullptr, just return null or valid pointer as optional.
  const auto& ioResourceManager = MIOResource::GetInstance();
  const auto* ptr = ioResourceManager.GetPtrInformation<EResourceType::Material>(iSpecifier);
  MDY_ASSERT_MSG(ptr != nullptr, "Ptr must be valid.");
  return ptr;
}

std::optional<const __TResourceType_T<EResourceType::GLAttachment>*> 
SIOBindingHelper::MDY_PRIVATE(pTryRequireResource_Attachment)
(const std::string& iSpecifier, IBinderBase& iPtrBinder)
{
  const auto checkFlag = pTryRequireResource(iSpecifier, EResourceType::GLAttachment, iPtrBinder);
  if (checkFlag == EDySuccess::DY_FAILURE) { return std::nullopt; }

  // If RI has been bound, try get pointer of resource but if nullptr, just return null or valid pointer as optional.
  const auto& ioResourceManager = MIOResource::GetInstance();
  const auto* ptr = ioResourceManager.GetPtrInformation<EResourceType::GLAttachment>(iSpecifier);
  MDY_ASSERT_MSG(ptr != nullptr, "Ptr must be valid.");
  return ptr;;
}

std::optional<const __TResourceType_T<EResourceType::GLFrameBuffer>*> 
SIOBindingHelper::MDY_PRIVATE(pTryRequireResource_FrameBuffer)
(const std::string& iSpecifier, IBinderBase& iPtrBinder)
{
  const auto checkFlag = pTryRequireResource(iSpecifier, EResourceType::GLFrameBuffer, iPtrBinder);
  if (checkFlag == EDySuccess::DY_FAILURE) { return std::nullopt; }

  // If RI has been bound, try get pointer of resource but if nullptr, just return null or valid pointer as optional.
  const auto& ioResourceManager = MIOResource::GetInstance();
  const auto* ptr = ioResourceManager.GetPtrInformation<EResourceType::GLFrameBuffer>(iSpecifier);
  MDY_ASSERT_MSG(ptr != nullptr, "Ptr must be valid.");
  return ptr;
}

EDySuccess SIOBindingHelper::MDY_PRIVATE(pTryDetachResource)
(const std::string& iSpecifier, EResourceType iType, IBinderBase& iPtrBinder)
{
  MDY_ASSERT_MSG(iType != EResourceType::NoneError, "iType must be valid resource type.");
  MDY_ASSERT_MSG(TEMP_CAST(gEngine) != nullptr, "TEMP_CAST(gEngine) must not be null.");
  auto& ioThread = *TEMP_CAST(gEngine)->pfGetIOThread();

  // If binding is failed, it specifies that RI has not been created (Related task queue was not created neither).
  // So we need populate task queue for resource as temporary.
  if (ioThread.pIsReferenceInstanceExist(iSpecifier, iType, EResourceStyle::Resource) == false)
  { 
    MDY_UNEXPECTED_BRANCH_BUT_RETURN(EDySuccess::DY_FAILURE);
  }

  MDY_CALL_ASSERT_SUCCESS(ioThread.TryDetachBinderFromResourceRI(iSpecifier, iType, iPtrBinder));
  return EDySuccess::DY_SUCCESS;
}

} /// ::dy namespace