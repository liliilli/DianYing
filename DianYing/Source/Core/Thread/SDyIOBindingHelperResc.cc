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
#include <Dy/Core/Thread/SDyIOBindingHelper.h>
#include <Dy/Core/Thread/IO/EDyIOTask.h>
#include <Dy/Core/DyEngine.h>

namespace dy
{

MDY_NODISCARD EDySuccess
SDyIOBindingHelper::MDY_PRIVATE_FUNC_SPECIFIER(pTryRequireResource)
(_MIN_ const std::string& iSpecifier, _MIN_ EDyResourceType iType, _MIN_ const __FDyBinderBase* iPtrBinder)
{
  MDY_ASSERT(MDY_CHECK_ISNOTNULL(gEngine), "gEngine must not be null.");
  auto& ioThread = *gEngine->pfGetIOThread();

  if (ioThread.pIsReferenceInstanceExist(iSpecifier, iType, EDyResourceStyle::Resource) == false)
  { // If binding is failed, it specifies that RI has not been created (Related task queue was not created neither).
    // So we need populate task queue for resource as temporary.
    MDY_UNEXPECTED_BRANCH_BUT_RETURN(DY_FAILURE);
  }

  MDY_CALL_ASSERT_SUCCESS(ioThread.TryBindBinderToResourceRI(iSpecifier, iType, iPtrBinder));
  if (ioThread.pIsReferenceInstanceBound(iSpecifier, iType, EDyResourceStyle::Resource) == false)
  {
    MDY_UNEXPECTED_BRANCH_BUT_RETURN(DY_FAILURE);
  }

  return DY_SUCCESS;
}

std::optional<const __TResourceType<EDyResourceType::GLShader>::type*>
SDyIOBindingHelper::MDY_PRIVATE_FUNC_SPECIFIER(pTryRequireResource_GLShader)
(_MIN_ const std::string& iSpecifier, _MIN_ const __FDyBinderBase* iPtrBinder)
{
  const auto checkFlag = __pTryRequireResource(iSpecifier, EDyResourceType::GLShader, iPtrBinder);
  if (checkFlag == DY_FAILURE) { return std::nullopt; }

  // If RI has been bound, try get pointer of resource but if nullptr, just return null or valid pointer as optional.
  const auto& ioResourceManager = MDyIOResource::GetInstance();
  const auto* ptr = ioResourceManager.GetPtrInformation<EDyResourceType::GLShader>(iSpecifier);
  MDY_ASSERT(MDY_CHECK_ISNOTNULL(ptr), "Ptr must be valid.");
  return ptr;
}

MDY_NODISCARD std::optional<const __TResourceType_T<EDyResourceType::Mesh>*>
SDyIOBindingHelper::MDY_PRIVATE_FUNC_SPECIFIER(pTryRequireResource_Mesh)
(_MIN_ const std::string& iSpecifier, _MIN_ const __FDyBinderBase* iPtrBinder)
{
  const auto checkFlag = __pTryRequireResource(iSpecifier, EDyResourceType::Mesh, iPtrBinder);
  if (checkFlag == DY_FAILURE) { return std::nullopt; }

  // If RI has been bound, try get pointer of resource but if nullptr, just return null or valid pointer as optional.
  const auto& ioResourceManager = MDyIOResource::GetInstance();
  const auto* ptr = ioResourceManager.GetPtrInformation<EDyResourceType::Mesh>(iSpecifier);
  MDY_ASSERT(MDY_CHECK_ISNOTNULL(ptr), "Ptr must be valid.");
  return ptr;
}

std::optional<const __TResourceType_T<EDyResourceType::Model>*>
SDyIOBindingHelper::MDY_PRIVATE_FUNC_SPECIFIER(pTryRequireResource_Model)
(_MIN_ const std::string& iSpecifier, _MIN_ const __FDyBinderBase* iPtrBinder)
{
  const auto checkFlag = __pTryRequireResource(iSpecifier, EDyResourceType::Model, iPtrBinder);
  if (checkFlag == DY_FAILURE) { return std::nullopt; }

  // If RI has been bound, try get pointer of resource but if nullptr, just return null or valid pointer as optional.
  const auto& ioResourceManager = MDyIOResource::GetInstance();
  const auto* ptr = ioResourceManager.GetPtrInformation<EDyResourceType::Model>(iSpecifier);
  MDY_ASSERT(MDY_CHECK_ISNOTNULL(ptr), "Ptr must be valid.");
  return ptr;
}

std::optional<const __TResourceType_T<EDyResourceType::Texture>*>
SDyIOBindingHelper::MDY_PRIVATE_FUNC_SPECIFIER(pTryRequireResource_Texture)
(_MIN_ const std::string& iSpecifier, _MIN_ const __FDyBinderBase* iPtrBinder)
{
  const auto checkFlag = __pTryRequireResource(iSpecifier, EDyResourceType::Texture, iPtrBinder);
  if (checkFlag == DY_FAILURE) { return std::nullopt; }

  // If RI has been bound, try get pointer of resource but if nullptr, just return null or valid pointer as optional.
  const auto& ioResourceManager = MDyIOResource::GetInstance();
  const auto* ptr = ioResourceManager.GetPtrInformation<EDyResourceType::Texture>(iSpecifier);
  MDY_ASSERT(MDY_CHECK_ISNOTNULL(ptr), "Ptr must be valid.");
  return ptr;
}

std::optional<const __TResourceType_T<EDyResourceType::Material>*>
SDyIOBindingHelper::MDY_PRIVATE_FUNC_SPECIFIER(pTryRequireResource_Material)
(_MIN_ const std::string& iSpecifier, _MIN_ const __FDyBinderBase* iPtrBinder)
{
  const auto checkFlag = __pTryRequireResource(iSpecifier, EDyResourceType::Material, iPtrBinder);
  if (checkFlag == DY_FAILURE) { return std::nullopt; }

  // If RI has been bound, try get pointer of resource but if nullptr, just return null or valid pointer as optional.
  const auto& ioResourceManager = MDyIOResource::GetInstance();
  const auto* ptr = ioResourceManager.GetPtrInformation<EDyResourceType::Material>(iSpecifier);
  MDY_ASSERT(MDY_CHECK_ISNOTNULL(ptr), "Ptr must be valid.");
  return ptr;
}

std::optional<const __TResourceType_T<EDyResourceType::GLAttachment>*> 
SDyIOBindingHelper::MDY_PRIVATE_FUNC_SPECIFIER(pTryRequireResource_Attachment)
(const std::string& iSpecifier, const __FDyBinderBase* iPtrBinder)
{
  const auto checkFlag = __pTryRequireResource(iSpecifier, EDyResourceType::GLAttachment, iPtrBinder);
  if (checkFlag == DY_FAILURE) { return std::nullopt; }

  // If RI has been bound, try get pointer of resource but if nullptr, just return null or valid pointer as optional.
  const auto& ioResourceManager = MDyIOResource::GetInstance();
  const auto* ptr = ioResourceManager.GetPtrInformation<EDyResourceType::GLAttachment>(iSpecifier);
  MDY_ASSERT(MDY_CHECK_ISNOTNULL(ptr), "Ptr must be valid.");
  return ptr;;
}

std::optional<const __TResourceType_T<EDyResourceType::GLFrameBuffer>*> 
SDyIOBindingHelper::MDY_PRIVATE_FUNC_SPECIFIER(pTryRequireResource_FrameBuffer)
(const std::string& iSpecifier, const __FDyBinderBase* iPtrBinder)
{
  const auto checkFlag = __pTryRequireResource(iSpecifier, EDyResourceType::GLFrameBuffer, iPtrBinder);
  if (checkFlag == DY_FAILURE) { return std::nullopt; }

  // If RI has been bound, try get pointer of resource but if nullptr, just return null or valid pointer as optional.
  const auto& ioResourceManager = MDyIOResource::GetInstance();
  const auto* ptr = ioResourceManager.GetPtrInformation<EDyResourceType::GLFrameBuffer>(iSpecifier);
  MDY_ASSERT(MDY_CHECK_ISNOTNULL(ptr), "Ptr must be valid.");
  return ptr;
}

} /// ::dy namespace