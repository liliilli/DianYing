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
#include <Dy/Core/DyEngine.h>

namespace dy
{

MDY_NODISCARD EDySuccess
SDyIOBindingHelper::MDY_PRIVATE_SPECIFIER(pTryRequireInformation)
(_MIN_ const std::string& iSpecifier, _MIN_ EDyResourceType iType, _MIN_ const __FDyBinderBase* iPtrBinder)
{
  MDY_ASSERT(MDY_CHECK_ISNOTNULL(gEngine), "gEngine must not be null.");
  auto& ioThread = *gEngine->pfGetIOThread();

  if (ioThread.pIsReferenceInstanceExist(iSpecifier, iType, EDyResourceStyle::Information) == false)
  { // If binding is failed, it specifies that RI has not been created (Related task queue was not created neither).
    // So we need populate task queue for resource as temporary.
    MDY_UNEXPECTED_BRANCH_BUT_RETURN(DY_FAILURE);
  }

  MDY_CALL_ASSERT_SUCCESS(ioThread.TryBindBinderToInformationRI(iSpecifier, iType, iPtrBinder));
  if (ioThread.pIsReferenceInstanceBound(iSpecifier, iType, EDyResourceStyle::Information) == false)
  {
    MDY_UNEXPECTED_BRANCH_BUT_RETURN(DY_FAILURE);
  }

  return DY_SUCCESS;
}

MDY_NODISCARD std::optional<const __TDyRscInfo_T<EDyResourceType::GLShader>*>
SDyIOBindingHelper::MDY_PRIVATE_SPECIFIER(pTryRequireInformation_GLShader)
(_MIN_ const std::string& iSpecifier, _MIN_ const __FDyBinderBase* iPtrBinder)
{
  if (const auto checkFlag = MDY_PRIVATE_SPECIFIER(pTryRequireInformation)(iSpecifier, EDyResourceType::GLShader, iPtrBinder);
      checkFlag == DY_FAILURE) { return std::nullopt; }

  // If RI has been bound, try get pointer of resource but if nullptr, just return null or valid pointer as optional.
  const auto* ptr = MDyIOData::GetInstance().GetPtrInformation<EDyResourceType::GLShader>(iSpecifier);
  MDY_ASSERT(MDY_CHECK_ISNOTNULL(ptr), "Ptr must be valid.");
  return ptr;
}

MDY_NODISCARD std::optional<const __TDyRscInfo_T<EDyResourceType::Texture>*>
SDyIOBindingHelper::MDY_PRIVATE_SPECIFIER(pTryRequireInformation_Texture)
(_MIN_ const std::string& iSpecifier, _MIN_ const __FDyBinderBase* iPtrBinder)
{
  if (const auto checkFlag = MDY_PRIVATE_SPECIFIER(pTryRequireInformation)(iSpecifier, EDyResourceType::Texture, iPtrBinder);
      checkFlag == DY_FAILURE) { return std::nullopt; }

  // If RI has been bound, try get pointer of resource but if nullptr, just return null or valid pointer as optional.
  const auto* ptr = MDyIOData::GetInstance().GetPtrInformation<EDyResourceType::Texture>(iSpecifier);
  MDY_ASSERT(MDY_CHECK_ISNOTNULL(ptr), "Ptr must be valid.");
  return ptr;
}

MDY_NODISCARD std::optional<const __TDyRscInfo_T<EDyResourceType::GLAttachment>*>
SDyIOBindingHelper::MDY_PRIVATE_SPECIFIER(pTryRequireInformation_Attachment)
(_MIN_ const std::string& iSpecifier, _MIN_ const __FDyBinderBase* iPtrBinder)
{
  if (const auto checkFlag = MDY_PRIVATE_SPECIFIER(pTryRequireInformation)(iSpecifier, EDyResourceType::GLAttachment, iPtrBinder);
      checkFlag == DY_FAILURE) { return std::nullopt; }

  // If RI has been bound, try get pointer of resource but if nullptr, just return null or valid pointer as optional.
  const auto* ptr = MDyIOData::GetInstance().GetPtrInformation<EDyResourceType::GLAttachment>(iSpecifier);
  MDY_ASSERT(MDY_CHECK_ISNOTNULL(ptr), "Ptr must be valid.");
  return ptr;
}

MDY_NODISCARD std::optional<const __TDyRscInfo_T<EDyResourceType::GLFrameBuffer>*>
SDyIOBindingHelper::MDY_PRIVATE_SPECIFIER(pTryRequireInformation_GLFrameBuffer)
(_MIN_ const std::string& iSpecifier, _MIN_ const __FDyBinderBase* iPtrBinder)
{
  if (const auto checkFlag = MDY_PRIVATE_SPECIFIER(pTryRequireInformation)(iSpecifier, EDyResourceType::GLFrameBuffer, iPtrBinder);
      checkFlag == DY_FAILURE) { return std::nullopt; }

  // If RI has been bound, try get pointer of resource but if nullptr, just return null or valid pointer as optional.
  const auto* ptr = MDyIOData::GetInstance().GetPtrInformation<EDyResourceType::GLFrameBuffer>(iSpecifier);
  MDY_ASSERT(MDY_CHECK_ISNOTNULL(ptr), "Ptr must be valid.");
  return ptr;
}

MDY_NODISCARD std::optional<const __TDyRscInfo_T<EDyResourceType::Material>*>
SDyIOBindingHelper::MDY_PRIVATE_SPECIFIER(pTryRequireInformation_Material)
(_MIN_ const std::string& iSpecifier, _MIN_ const __FDyBinderBase* iPtrBinder)
{
  if (const auto checkFlag = MDY_PRIVATE_SPECIFIER(pTryRequireInformation)(iSpecifier, EDyResourceType::Material, iPtrBinder);
      checkFlag == DY_FAILURE) { return std::nullopt; }

  // If RI has been bound, try get pointer of resource but if nullptr, just return null or valid pointer as optional.
  const auto* ptr = MDyIOData::GetInstance().GetPtrInformation<EDyResourceType::Material>(iSpecifier);
  MDY_ASSERT(MDY_CHECK_ISNOTNULL(ptr), "Ptr must be valid.");
  return ptr;
}

MDY_NODISCARD std::optional<const __TDyRscInfo_T<EDyResourceType::Mesh>*>
SDyIOBindingHelper::MDY_PRIVATE_SPECIFIER(pTryRequireInformation_Mesh)
(_MIN_ const std::string& iSpecifier, _MIN_ const __FDyBinderBase* iPtrBinder)
{
  if (const auto checkFlag = MDY_PRIVATE_SPECIFIER(pTryRequireInformation)(iSpecifier, EDyResourceType::Mesh, iPtrBinder);
      checkFlag == DY_FAILURE) { return std::nullopt; }

  // If RI has been bound, try get pointer of resource but if nullptr, just return null or valid pointer as optional.
  const auto* ptr = MDyIOData::GetInstance().GetPtrInformation<EDyResourceType::Mesh>(iSpecifier);
  MDY_ASSERT(MDY_CHECK_ISNOTNULL(ptr), "Ptr must be valid.");
  return ptr;
}

MDY_NODISCARD std::optional<const __TDyRscInfo_T<EDyResourceType::Model>*>
SDyIOBindingHelper::MDY_PRIVATE_SPECIFIER(pTryRequireInformation_Model)
(_MIN_ const std::string& iSpecifier, _MIN_ const __FDyBinderBase* iPtrBinder)
{
  if (const auto checkFlag = MDY_PRIVATE_SPECIFIER(pTryRequireInformation)(iSpecifier, EDyResourceType::Model, iPtrBinder);
      checkFlag == DY_FAILURE) { return std::nullopt; }

  // If RI has been bound, try get pointer of resource but if nullptr, just return null or valid pointer as optional.
  const auto* ptr = MDyIOData::GetInstance().GetPtrInformation<EDyResourceType::Model>(iSpecifier);
  MDY_ASSERT(MDY_CHECK_ISNOTNULL(ptr), "Ptr must be valid.");
  return ptr;
}

EDySuccess SDyIOBindingHelper::MDY_PRIVATE_SPECIFIER(pTryDetachInformation)
(_MIN_ const std::string& iSpecifier, _MIN_ EDyResourceType iType, _MIN_ const __FDyBinderBase* iPtrBinder)
{
  MDY_ASSERT(iType != EDyResourceType::NoneError, "iType must be valid resource type.");
  MDY_ASSERT(MDY_CHECK_ISNOTNULL(gEngine), "gEngine must not be null.");
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