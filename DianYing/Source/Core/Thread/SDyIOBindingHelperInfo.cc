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
#include <Dy/Core/Thread/SDyIOBindingHelper.h>
#include <Dy/Core/Thread/IO/EDyIOTask.h>
#include <Dy/Core/Thread/SDyIOConnectionHelper.h>
#include <Dy/Core/DyEngine.h>

namespace dy
{

MDY_NODISCARD EDySuccess
SDyIOBindingHelper::MDY_PRIVATE(pTryRequireInformation)
(_MIN_ const std::string& iSpecifier, _MIN_ EResourceType iType, _MIN_ const __FDyBinderBase* iPtrBinder)
{
  MDY_ASSERT_MSG(MDY_CHECK_ISNOTNULL(gEngine), "gEngine must not be null.");
  auto& ioThread = *gEngine->pfGetIOThread();

  if (ioThread.pIsReferenceInstanceExist(iSpecifier, iType, EDyResourceStyle::Information) == false)
  { // If binding is failed, it specifies that RI has not been created (Related task queue was not created neither).
    // So we need populate task queue for resource as temporary.
    SDyIOConnectionHelper::PopulateResource(iSpecifier, iType, EDyResourceStyle::Information, EDyScope::Temporal);
  }

  MDY_CALL_ASSERT_SUCCESS(ioThread.TryBindBinderToInformationRI(iSpecifier, iType, iPtrBinder));
  if (ioThread.pIsReferenceInstanceBound(iSpecifier, iType, EDyResourceStyle::Information) == false)
  {
    return DY_FAILURE;
  }

  return DY_SUCCESS;
}

MDY_NODISCARD std::optional<const __TDyRscInfo_T<EResourceType::GLShader>*>
SDyIOBindingHelper::MDY_PRIVATE(pTryRequireInformation_GLShader)
(_MIN_ const std::string& iSpecifier, _MIN_ const __FDyBinderBase* iPtrBinder)
{
  if (const auto checkFlag = MDY_PRIVATE(pTryRequireInformation)(iSpecifier, EResourceType::GLShader, iPtrBinder);
      checkFlag == DY_FAILURE) { return std::nullopt; }

  // If RI has been bound, try get pointer of resource but if nullptr, just return null or valid pointer as optional.
  const auto* ptr = MIORescInfo::GetInstance().GetPtrInformation<EResourceType::GLShader>(iSpecifier);
  MDY_ASSERT_MSG(MDY_CHECK_ISNOTNULL(ptr), "Ptr must be valid.");
  return ptr;
}

MDY_NODISCARD std::optional<const __TDyRscInfo_T<EResourceType::Texture>*>
SDyIOBindingHelper::MDY_PRIVATE(pTryRequireInformation_Texture)
(_MIN_ const std::string& iSpecifier, _MIN_ const __FDyBinderBase* iPtrBinder)
{
  if (const auto checkFlag = MDY_PRIVATE(pTryRequireInformation)(iSpecifier, EResourceType::Texture, iPtrBinder);
      checkFlag == DY_FAILURE) { return std::nullopt; }

  // If RI has been bound, try get pointer of resource but if nullptr, just return null or valid pointer as optional.
  const auto* ptr = MIORescInfo::GetInstance().GetPtrInformation<EResourceType::Texture>(iSpecifier);
  MDY_ASSERT_MSG(MDY_CHECK_ISNOTNULL(ptr), "Ptr must be valid.");
  return ptr;
}

MDY_NODISCARD std::optional<const __TDyRscInfo_T<EResourceType::GLAttachment>*>
SDyIOBindingHelper::MDY_PRIVATE(pTryRequireInformation_Attachment)
(_MIN_ const std::string& iSpecifier, _MIN_ const __FDyBinderBase* iPtrBinder)
{
  if (const auto checkFlag = MDY_PRIVATE(pTryRequireInformation)(iSpecifier, EResourceType::GLAttachment, iPtrBinder);
      checkFlag == DY_FAILURE) { return std::nullopt; }

  // If RI has been bound, try get pointer of resource but if nullptr, just return null or valid pointer as optional.
  const auto* ptr = MIORescInfo::GetInstance().GetPtrInformation<EResourceType::GLAttachment>(iSpecifier);
  MDY_ASSERT_MSG(MDY_CHECK_ISNOTNULL(ptr), "Ptr must be valid.");
  return ptr;
}

MDY_NODISCARD std::optional<const __TDyRscInfo_T<EResourceType::GLFrameBuffer>*>
SDyIOBindingHelper::MDY_PRIVATE(pTryRequireInformation_GLFrameBuffer)
(_MIN_ const std::string& iSpecifier, _MIN_ const __FDyBinderBase* iPtrBinder)
{
  if (const auto checkFlag = MDY_PRIVATE(pTryRequireInformation)(iSpecifier, EResourceType::GLFrameBuffer, iPtrBinder);
      checkFlag == DY_FAILURE) { return std::nullopt; }

  // If RI has been bound, try get pointer of resource but if nullptr, just return null or valid pointer as optional.
  const auto* ptr = MIORescInfo::GetInstance().GetPtrInformation<EResourceType::GLFrameBuffer>(iSpecifier);
  MDY_ASSERT_MSG(MDY_CHECK_ISNOTNULL(ptr), "Ptr must be valid.");
  return ptr;
}

MDY_NODISCARD std::optional<const __TDyRscInfo_T<EResourceType::Material>*>
SDyIOBindingHelper::MDY_PRIVATE(pTryRequireInformation_Material)
(_MIN_ const std::string& iSpecifier, _MIN_ const __FDyBinderBase* iPtrBinder)
{
  if (const auto checkFlag = MDY_PRIVATE(pTryRequireInformation)(iSpecifier, EResourceType::Material, iPtrBinder);
      checkFlag == DY_FAILURE) { return std::nullopt; }

  // If RI has been bound, try get pointer of resource but if nullptr, just return null or valid pointer as optional.
  const auto* ptr = MIORescInfo::GetInstance().GetPtrInformation<EResourceType::Material>(iSpecifier);
  MDY_ASSERT_MSG(MDY_CHECK_ISNOTNULL(ptr), "Ptr must be valid.");
  return ptr;
}

MDY_NODISCARD std::optional<const __TDyRscInfo_T<EResourceType::Mesh>*>
SDyIOBindingHelper::MDY_PRIVATE(pTryRequireInformation_Mesh)
(_MIN_ const std::string& iSpecifier, _MIN_ const __FDyBinderBase* iPtrBinder)
{
  if (const auto checkFlag = MDY_PRIVATE(pTryRequireInformation)(iSpecifier, EResourceType::Mesh, iPtrBinder);
      checkFlag == DY_FAILURE) { return std::nullopt; }

  // If RI has been bound, try get pointer of resource but if nullptr, just return null or valid pointer as optional.
  const auto* ptr = MIORescInfo::GetInstance().GetPtrInformation<EResourceType::Mesh>(iSpecifier);
  MDY_ASSERT_MSG(MDY_CHECK_ISNOTNULL(ptr), "Ptr must be valid.");
  return ptr;
}

MDY_NODISCARD std::optional<const __TDyRscInfo_T<EResourceType::Model>*>
SDyIOBindingHelper::MDY_PRIVATE(pTryRequireInformation_Model)
(_MIN_ const std::string& iSpecifier, _MIN_ const __FDyBinderBase* iPtrBinder)
{
  if (const auto checkFlag = MDY_PRIVATE(pTryRequireInformation)(iSpecifier, EResourceType::Model, iPtrBinder);
      checkFlag == DY_FAILURE) { return std::nullopt; }

  // If RI has been bound, try get pointer of resource but if nullptr, just return null or valid pointer as optional.
  const auto* ptr = MIORescInfo::GetInstance().GetPtrInformation<EResourceType::Model>(iSpecifier);
  MDY_ASSERT_MSG(MDY_CHECK_ISNOTNULL(ptr), "Ptr must be valid.");
  return ptr;
}

MDY_NODISCARD std::optional<const __TDyRscInfo_T<EResourceType::Skeleton>*>
SDyIOBindingHelper::MDY_PRIVATE(pTryRequireInformation_Skeleton)
(_MIN_ const std::string& iSpecifier, _MIN_ const __FDyBinderBase* iPtrBinder)
{
  if (const auto checkFlag = MDY_PRIVATE(pTryRequireInformation)(iSpecifier, EResourceType::Skeleton, iPtrBinder);
      checkFlag == DY_FAILURE) { return std::nullopt; }

  // If RI has been bound, try get pointer of resource but if nullptr, just return null or valid pointer as optional.
  const auto* ptr = MIORescInfo::GetInstance().GetPtrInformation<EResourceType::Skeleton>(iSpecifier);
  MDY_ASSERT_MSG(MDY_CHECK_ISNOTNULL(ptr), "Ptr must be valid.");
  return ptr;
}

std::optional<const __TDyRscInfo_T<EResourceType::AnimationScrap>*> 
SDyIOBindingHelper::MDY_PRIVATE(pTryRequireInformation_AnimationScrap)
(_MIN_ const std::string& iSpecifier, _MIN_ const __FDyBinderBase* iPtrBinder)
{
  if (const auto checkFlag = MDY_PRIVATE(pTryRequireInformation)(iSpecifier, EResourceType::AnimationScrap, iPtrBinder);
      checkFlag == DY_FAILURE) { return std::nullopt; }

  // If RI has been bound, try get pointer of resource but if nullptr, just return null or valid pointer as optional.
  const auto* ptr = MIORescInfo::GetInstance().GetPtrInformation<EResourceType::AnimationScrap>(iSpecifier);
  MDY_ASSERT_MSG(MDY_CHECK_ISNOTNULL(ptr), "Ptr must be valid.");
  return ptr;
}

std::optional<const __TDyRscInfo_T<EResourceType::Sound>*> 
SDyIOBindingHelper::MDY_PRIVATE(pTryRequireInformation_Sound)
(_MIN_ const std::string& iSpecifier, _MIN_ const __FDyBinderBase* iPtrBinder)
{
  if (const auto checkFlag = MDY_PRIVATE(pTryRequireInformation)(iSpecifier, EResourceType::Sound, iPtrBinder);
      checkFlag == DY_FAILURE) { return std::nullopt; }

  // If RI has been bound, try get pointer of resource but if nullptr, just return null or valid pointer as optional.
  const auto* ptr = MIORescInfo::GetInstance().GetPtrInformation<EResourceType::Sound>(iSpecifier);
  MDY_ASSERT_MSG(MDY_CHECK_ISNOTNULL(ptr), "Ptr must be valid.");
  return ptr;
}

EDySuccess SDyIOBindingHelper::MDY_PRIVATE(pTryDetachInformation)
(_MIN_ const std::string& iSpecifier, _MIN_ EResourceType iType, _MIN_ const __FDyBinderBase* iPtrBinder)
{
  MDY_ASSERT_MSG(iType != EResourceType::NoneError, "iType must be valid resource type.");
  MDY_ASSERT_MSG(MDY_CHECK_ISNOTNULL(gEngine), "gEngine must not be null.");
  auto& ioThread = *gEngine->pfGetIOThread();

  // If binding is failed, it specifies that RI has not been created (Related task queue was not created neither).
  // So we need populate task queue for resource as temporary.
  if (ioThread.pIsReferenceInstanceExist(iSpecifier, iType, EDyResourceStyle::Information) == false)
  { 
    MDY_UNEXPECTED_BRANCH_BUT_RETURN(DY_FAILURE);
  }

  MDY_CALL_ASSERT_SUCCESS(ioThread.TryDetachBinderFromInformationRI(iSpecifier, iType, iPtrBinder));
  return DY_SUCCESS;
}

} /// ::dy namespace