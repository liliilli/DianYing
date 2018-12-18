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
SDyIOBindingHelper::MDY_PRIVATE_FUNC_SPECIFIER(pTryRequireInformation)
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
SDyIOBindingHelper::MDY_PRIVATE_FUNC_SPECIFIER(pTryRequireInformation_GLShader)
(_MIN_ const std::string& iSpecifier, _MIN_ const __FDyBinderBase* iPtrBinder)
{
  if (const auto checkFlag = MDY_PRIVATE_FUNC_SPECIFIER(pTryRequireInformation)(iSpecifier, EDyResourceType::GLShader, iPtrBinder);
      checkFlag == DY_FAILURE) { return std::nullopt; }

  // If RI has been bound, try get pointer of resource but if nullptr, just return null or valid pointer as optional.
  const auto& ioData = MDyIOData::GetInstance();
  const auto* ptr = ioData.GetPtrInformation<EDyResourceType::GLShader>(iSpecifier);
  MDY_ASSERT(MDY_CHECK_ISNOTNULL(ptr), "Ptr must be valid.");
  return ptr;
}

MDY_NODISCARD std::optional<const __TDyRscInfo_T<EDyResourceType::Texture>*>
SDyIOBindingHelper::MDY_PRIVATE_FUNC_SPECIFIER(pTryRequireInformation_Texture)
(_MIN_ const std::string& iSpecifier, _MIN_ const __FDyBinderBase* iPtrBinder)
{
  if (const auto checkFlag = MDY_PRIVATE_FUNC_SPECIFIER(pTryRequireInformation)(iSpecifier, EDyResourceType::Texture, iPtrBinder);
      checkFlag == DY_FAILURE) { return std::nullopt; }

  // If RI has been bound, try get pointer of resource but if nullptr, just return null or valid pointer as optional.
  const auto& ioData = MDyIOData::GetInstance();
  const auto* ptr = ioData.GetPtrInformation<EDyResourceType::Texture>(iSpecifier);
  MDY_ASSERT(MDY_CHECK_ISNOTNULL(ptr), "Ptr must be valid.");
  return ptr;
}

MDY_NODISCARD std::optional<const __TDyRscInfo_T<EDyResourceType::GLAttachment>*>
SDyIOBindingHelper::MDY_PRIVATE_FUNC_SPECIFIER(pTryRequireInformation_Attachment)
(_MIN_ const std::string& iSpecifier, _MIN_ const __FDyBinderBase* iPtrBinder)
{
  if (const auto checkFlag = MDY_PRIVATE_FUNC_SPECIFIER(pTryRequireInformation)(iSpecifier, EDyResourceType::GLAttachment, iPtrBinder);
      checkFlag == DY_FAILURE) { return std::nullopt; }

  // If RI has been bound, try get pointer of resource but if nullptr, just return null or valid pointer as optional.
  const auto& ioData = MDyIOData::GetInstance();
  const auto* ptr = ioData.GetPtrInformation<EDyResourceType::GLAttachment>(iSpecifier);
  MDY_ASSERT(MDY_CHECK_ISNOTNULL(ptr), "Ptr must be valid.");
  return ptr;
}

} /// ::dy namespace