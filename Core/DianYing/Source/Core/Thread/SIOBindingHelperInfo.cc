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
#include <Dy/Core/Thread/SIOConnectionHelper.h>
#include <Dy/Core/GEngine.h>

namespace dy
{

MDY_NODISCARD EDySuccess
SIOBindingHelper::MDY_PRIVATE(pTryRequireInformation)
(const std::string& iSpecifier, EResourceType iType, IBinderBase& iPtrBinder)
{
  MDY_ASSERT_MSG(MDY_CHECK_ISNOTNULL(TEMP_CAST(gEngine)), "TEMP_CAST(gEngine) must not be null.");
  auto& ioThread = *TEMP_CAST(gEngine)->pfGetIOThread();

  if (ioThread.pIsReferenceInstanceExist(iSpecifier, iType, EResourceStyle::Information) == false)
  { // If binding is failed, it specifies that RI has not been created (Related task queue was not created neither).
    // So we need populate task queue for resource as temporary.
    SIOConnectionHelper::PopulateResource(iSpecifier, iType, EResourceStyle::Information, EResourceScope::Temporal);
  }

  MDY_CALL_ASSERT_SUCCESS(ioThread.TryBindBinderToInformationRI(iSpecifier, iType, iPtrBinder));
  if (ioThread.pIsReferenceInstanceBound(iSpecifier, iType, EResourceStyle::Information) == false)
  {
    return EDySuccess::DY_FAILURE;
  }

  return EDySuccess::DY_SUCCESS;
}

MDY_NODISCARD std::optional<const __TDyRscInfo_T<EResourceType::GLShader>*>
SIOBindingHelper::MDY_PRIVATE(pTryRequireInformation_GLShader)
(const std::string& iSpecifier, IBinderBase& iPtrBinder)
{
  if (const auto checkFlag = MDY_PRIVATE(pTryRequireInformation)(iSpecifier, EResourceType::GLShader, iPtrBinder);
      checkFlag == EDySuccess::DY_FAILURE) { return std::nullopt; }

  // If RI has been bound, try get pointer of resource but if nullptr, just return null or valid pointer as optional.
  const auto* ptr = MIORescInfo::GetInstance().GetPtrInformation<EResourceType::GLShader>(iSpecifier);
  MDY_ASSERT_MSG(MDY_CHECK_ISNOTNULL(ptr), "Ptr must be valid.");
  return ptr;
}

MDY_NODISCARD std::optional<const __TDyRscInfo_T<EResourceType::Texture>*>
SIOBindingHelper::MDY_PRIVATE(pTryRequireInformation_Texture)
(const std::string& iSpecifier, IBinderBase& iPtrBinder)
{
  if (const auto checkFlag = MDY_PRIVATE(pTryRequireInformation)(iSpecifier, EResourceType::Texture, iPtrBinder);
      checkFlag == EDySuccess::DY_FAILURE) { return std::nullopt; }

  // If RI has been bound, try get pointer of resource but if nullptr, just return null or valid pointer as optional.
  const auto* ptr = MIORescInfo::GetInstance().GetPtrInformation<EResourceType::Texture>(iSpecifier);
  MDY_ASSERT_MSG(MDY_CHECK_ISNOTNULL(ptr), "Ptr must be valid.");
  return ptr;
}

MDY_NODISCARD std::optional<const __TDyRscInfo_T<EResourceType::GLAttachment>*>
SIOBindingHelper::MDY_PRIVATE(pTryRequireInformation_Attachment)
(const std::string& iSpecifier, IBinderBase& iPtrBinder)
{
  if (const auto checkFlag = MDY_PRIVATE(pTryRequireInformation)(iSpecifier, EResourceType::GLAttachment, iPtrBinder);
      checkFlag == EDySuccess::DY_FAILURE) { return std::nullopt; }

  // If RI has been bound, try get pointer of resource but if nullptr, just return null or valid pointer as optional.
  const auto* ptr = MIORescInfo::GetInstance().GetPtrInformation<EResourceType::GLAttachment>(iSpecifier);
  MDY_ASSERT_MSG(MDY_CHECK_ISNOTNULL(ptr), "Ptr must be valid.");
  return ptr;
}

MDY_NODISCARD std::optional<const __TDyRscInfo_T<EResourceType::GLFrameBuffer>*>
SIOBindingHelper::MDY_PRIVATE(pTryRequireInformation_GLFrameBuffer)
(const std::string& iSpecifier, IBinderBase& iPtrBinder)
{
  if (const auto checkFlag = MDY_PRIVATE(pTryRequireInformation)(iSpecifier, EResourceType::GLFrameBuffer, iPtrBinder);
      checkFlag == EDySuccess::DY_FAILURE) { return std::nullopt; }

  // If RI has been bound, try get pointer of resource but if nullptr, just return null or valid pointer as optional.
  const auto* ptr = MIORescInfo::GetInstance().GetPtrInformation<EResourceType::GLFrameBuffer>(iSpecifier);
  MDY_ASSERT_MSG(MDY_CHECK_ISNOTNULL(ptr), "Ptr must be valid.");
  return ptr;
}

MDY_NODISCARD std::optional<const __TDyRscInfo_T<EResourceType::Material>*>
SIOBindingHelper::MDY_PRIVATE(pTryRequireInformation_Material)
(const std::string& iSpecifier, IBinderBase& iPtrBinder)
{
  if (const auto checkFlag = MDY_PRIVATE(pTryRequireInformation)(iSpecifier, EResourceType::Material, iPtrBinder);
      checkFlag == EDySuccess::DY_FAILURE) { return std::nullopt; }

  // If RI has been bound, try get pointer of resource but if nullptr, just return null or valid pointer as optional.
  const auto* ptr = MIORescInfo::GetInstance().GetPtrInformation<EResourceType::Material>(iSpecifier);
  MDY_ASSERT_MSG(MDY_CHECK_ISNOTNULL(ptr), "Ptr must be valid.");
  return ptr;
}

MDY_NODISCARD std::optional<const __TDyRscInfo_T<EResourceType::Mesh>*>
SIOBindingHelper::MDY_PRIVATE(pTryRequireInformation_Mesh)
(const std::string& iSpecifier, IBinderBase& iPtrBinder)
{
  if (const auto checkFlag = MDY_PRIVATE(pTryRequireInformation)(iSpecifier, EResourceType::Mesh, iPtrBinder);
      checkFlag == EDySuccess::DY_FAILURE) { return std::nullopt; }

  // If RI has been bound, try get pointer of resource but if nullptr, just return null or valid pointer as optional.
  const auto* ptr = MIORescInfo::GetInstance().GetPtrInformation<EResourceType::Mesh>(iSpecifier);
  MDY_ASSERT_MSG(MDY_CHECK_ISNOTNULL(ptr), "Ptr must be valid.");
  return ptr;
}

MDY_NODISCARD std::optional<const __TDyRscInfo_T<EResourceType::Model>*>
SIOBindingHelper::MDY_PRIVATE(pTryRequireInformation_Model)
(const std::string& iSpecifier, IBinderBase& iPtrBinder)
{
  if (const auto checkFlag = MDY_PRIVATE(pTryRequireInformation)(iSpecifier, EResourceType::Model, iPtrBinder);
      checkFlag == EDySuccess::DY_FAILURE) { return std::nullopt; }

  // If RI has been bound, try get pointer of resource but if nullptr, just return null or valid pointer as optional.
  const auto* ptr = MIORescInfo::GetInstance().GetPtrInformation<EResourceType::Model>(iSpecifier);
  MDY_ASSERT_MSG(MDY_CHECK_ISNOTNULL(ptr), "Ptr must be valid.");
  return ptr;
}

MDY_NODISCARD std::optional<const __TDyRscInfo_T<EResourceType::Skeleton>*>
SIOBindingHelper::MDY_PRIVATE(pTryRequireInformation_Skeleton)
(const std::string& iSpecifier, IBinderBase& iPtrBinder)
{
  if (const auto checkFlag = MDY_PRIVATE(pTryRequireInformation)(iSpecifier, EResourceType::Skeleton, iPtrBinder);
      checkFlag == EDySuccess::DY_FAILURE) { return std::nullopt; }

  // If RI has been bound, try get pointer of resource but if nullptr, just return null or valid pointer as optional.
  const auto* ptr = MIORescInfo::GetInstance().GetPtrInformation<EResourceType::Skeleton>(iSpecifier);
  MDY_ASSERT_MSG(MDY_CHECK_ISNOTNULL(ptr), "Ptr must be valid.");
  return ptr;
}

std::optional<const __TDyRscInfo_T<EResourceType::AnimationScrap>*> 
SIOBindingHelper::MDY_PRIVATE(pTryRequireInformation_AnimationScrap)
(const std::string& iSpecifier, IBinderBase& iPtrBinder)
{
  if (const auto checkFlag = MDY_PRIVATE(pTryRequireInformation)(iSpecifier, EResourceType::AnimationScrap, iPtrBinder);
      checkFlag == EDySuccess::DY_FAILURE) { return std::nullopt; }

  // If RI has been bound, try get pointer of resource but if nullptr, just return null or valid pointer as optional.
  const auto* ptr = MIORescInfo::GetInstance().GetPtrInformation<EResourceType::AnimationScrap>(iSpecifier);
  MDY_ASSERT_MSG(MDY_CHECK_ISNOTNULL(ptr), "Ptr must be valid.");
  return ptr;
}

std::optional<const __TDyRscInfo_T<EResourceType::Sound>*> 
SIOBindingHelper::MDY_PRIVATE(pTryRequireInformation_Sound)
(const std::string& iSpecifier, IBinderBase& iPtrBinder)
{
  if (const auto checkFlag = MDY_PRIVATE(pTryRequireInformation)(iSpecifier, EResourceType::Sound, iPtrBinder);
      checkFlag == EDySuccess::DY_FAILURE) { return std::nullopt; }

  // If RI has been bound, try get pointer of resource but if nullptr, just return null or valid pointer as optional.
  const auto* ptr = MIORescInfo::GetInstance().GetPtrInformation<EResourceType::Sound>(iSpecifier);
  MDY_ASSERT_MSG(MDY_CHECK_ISNOTNULL(ptr), "Ptr must be valid.");
  return ptr;
}

EDySuccess SIOBindingHelper::MDY_PRIVATE(pTryDetachInformation)
(const std::string& iSpecifier, EResourceType iType, IBinderBase& iPtrBinder)
{
  MDY_ASSERT_MSG(iType != EResourceType::NoneError, "iType must be valid resource type.");
  MDY_ASSERT_MSG(MDY_CHECK_ISNOTNULL(TEMP_CAST(gEngine)), "TEMP_CAST(gEngine) must not be null.");
  auto& ioThread = *TEMP_CAST(gEngine)->pfGetIOThread();

  // If binding is failed, it specifies that RI has not been created (Related task queue was not created neither).
  // So we need populate task queue for resource as temporary.
  if (ioThread.pIsReferenceInstanceExist(iSpecifier, iType, EResourceStyle::Information) == false)
  { 
    MDY_UNEXPECTED_BRANCH_BUT_RETURN(EDySuccess::DY_FAILURE);
  }

  MDY_CALL_ASSERT_SUCCESS(ioThread.TryDetachBinderFromInformationRI(iSpecifier, iType, iPtrBinder));
  return EDySuccess::DY_SUCCESS;
}

} /// ::dy namespace