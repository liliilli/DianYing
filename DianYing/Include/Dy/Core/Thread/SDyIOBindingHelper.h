#ifndef GUARD_DY_CORE_THREAD_SDYIOBINDINGHELPER_H
#define GUARD_DY_CORE_THREAD_SDYIOBINDINGHELPER_H
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

#include <Dy/Meta/Type/EDyResourceType.h>
#include <Dy/Core/Resource/Type/TemplateRescInfoType.h>
#include <Dy/Core/Resource/Type/FDyBinderBase.h>

namespace dy
{

///
/// @class SDyIOBindingHelper
/// @brief Binding helper class for information & resource type from outside world.
///
class SDyIOBindingHelper final
{
private:
  /// @brief Use this require resource to another dimension.
  template <EDyResourceType TType>
  static MDY_NODISCARD std::optional<const __TResourceType_T<TType>*>
  TryRequireResource(_MIN_ const std::string& iSpecifier, _MIN_ const __FDyBinderBase* iPtrBinder)
  {
    if constexpr (TType == EDyResourceType::GLShader)     
    { return MDY_PRIVATE_FUNC_SPECIFIER(pTryRequireResource_GLShader)(iSpecifier, iPtrBinder); }
    else if constexpr (TType == EDyResourceType::Model)   
    { return MDY_PRIVATE_FUNC_SPECIFIER(pTryRequireResource_Model)(iSpecifier, iPtrBinder); }
    else if constexpr (TType == EDyResourceType::Texture) 
    { return MDY_PRIVATE_FUNC_SPECIFIER(pTryRequireResource_Texture)(iSpecifier, iPtrBinder); }
    else if constexpr (TType == EDyResourceType::Material)
    { return MDY_PRIVATE_FUNC_SPECIFIER(pTryRequireResource_Material)(iSpecifier, iPtrBinder); }
    else if constexpr (TType == EDyResourceType::GLAttachment)  
    { return MDY_PRIVATE_FUNC_SPECIFIER(pTryRequireResource_Attachment)(iSpecifier, iPtrBinder); }
    else if constexpr (TType == EDyResourceType::GLFrameBuffer) 
    { return MDY_PRIVATE_FUNC_SPECIFIER(pTryRequireResource_FrameBuffer)(iSpecifier, iPtrBinder); }
    else 
    { MDY_UNEXPECTED_BRANCH_BUT_RETURN(std::nullopt); }
  }

  static MDY_NODISCARD EDySuccess
  MDY_PRIVATE_FUNC_SPECIFIER(pTryRequireResource)
  (_MIN_ const std::string& iSpecifier, _MIN_ EDyResourceType iType, _MIN_ const __FDyBinderBase* iPtrBinder);

  static MDY_NODISCARD std::optional<const __TResourceType_T<EDyResourceType::GLShader>*>
  MDY_PRIVATE_FUNC_SPECIFIER(pTryRequireResource_GLShader)
  (_MIN_ const std::string& iSpecifier, _MIN_ const __FDyBinderBase* iPtrBinder);

  static MDY_NODISCARD std::optional<const __TResourceType_T<EDyResourceType::Model>*>
  MDY_PRIVATE_FUNC_SPECIFIER(pTryRequireResource_Model)
  (_MIN_ const std::string& iSpecifier, _MIN_ const __FDyBinderBase* iPtrBinder);

  static MDY_NODISCARD std::optional<const __TResourceType_T<EDyResourceType::Texture>*>
  MDY_PRIVATE_FUNC_SPECIFIER(pTryRequireResource_Texture)
  (_MIN_ const std::string& iSpecifier, _MIN_ const __FDyBinderBase* iPtrBinder);

  static MDY_NODISCARD std::optional<const __TResourceType_T<EDyResourceType::Material>*>
  MDY_PRIVATE_FUNC_SPECIFIER(pTryRequireResource_Material)
  (_MIN_ const std::string& iSpecifier, _MIN_ const __FDyBinderBase* iPtrBinder);

  static MDY_NODISCARD std::optional<const __TResourceType_T<EDyResourceType::GLAttachment>*>
  MDY_PRIVATE_FUNC_SPECIFIER(pTryRequireResource_Attachment)
  (_MIN_ const std::string& iSpecifier, _MIN_ const __FDyBinderBase* iPtrBinder);

  static MDY_NODISCARD std::optional<const __TResourceType_T<EDyResourceType::GLFrameBuffer>*>
  MDY_PRIVATE_FUNC_SPECIFIER(pTryRequireResource_FrameBuffer)
  (_MIN_ const std::string& iSpecifier, _MIN_ const __FDyBinderBase* iPtrBinder);

  /// @brief Use this require resource to another dimension.
  template <EDyResourceType TType>
  static MDY_NODISCARD std::optional<const __TDyRscInfo_T<TType>*>
  TryRequireInformation(_MIN_ const std::string& iSpecifier, _MIN_ const __FDyBinderBase* iPtrBinder)
  {
    if constexpr (TType == EDyResourceType::GLShader)     
    { return MDY_PRIVATE_FUNC_SPECIFIER(pTryRequireInformation_GLShader)(iSpecifier, iPtrBinder); }
    else if constexpr (TType == EDyResourceType::GLAttachment)     
    { return MDY_PRIVATE_FUNC_SPECIFIER(pTryRequireInformation_Attachment)(iSpecifier, iPtrBinder); }
    else if constexpr (TType == EDyResourceType::Texture)     
    { return MDY_PRIVATE_FUNC_SPECIFIER(pTryRequireInformation_Texture)(iSpecifier, iPtrBinder); }
    else if constexpr (TType == EDyResourceType::Material)     
    { return MDY_PRIVATE_FUNC_SPECIFIER(pTryRequireInformation_Material)(iSpecifier, iPtrBinder); }
    else 
    { MDY_UNEXPECTED_BRANCH_BUT_RETURN(std::nullopt); }
  }
  
  static MDY_NODISCARD EDySuccess
  MDY_PRIVATE_FUNC_SPECIFIER(pTryRequireInformation)
  (_MIN_ const std::string& iSpecifier, _MIN_ EDyResourceType iType, _MIN_ const __FDyBinderBase* iPtrBinder);
  
  static MDY_NODISCARD std::optional<const __TDyRscInfo_T<EDyResourceType::GLShader>*>
  MDY_PRIVATE_FUNC_SPECIFIER(pTryRequireInformation_GLShader)
  (_MIN_ const std::string& iSpecifier, _MIN_ const __FDyBinderBase* iPtrBinder);
  
  static MDY_NODISCARD std::optional<const __TDyRscInfo_T<EDyResourceType::Texture>*>
  MDY_PRIVATE_FUNC_SPECIFIER(pTryRequireInformation_Texture)
  (_MIN_ const std::string& iSpecifier, _MIN_ const __FDyBinderBase* iPtrBinder);
    
  static MDY_NODISCARD std::optional<const __TDyRscInfo_T<EDyResourceType::GLAttachment>*>
  MDY_PRIVATE_FUNC_SPECIFIER(pTryRequireInformation_Attachment)
  (_MIN_ const std::string& iSpecifier, _MIN_ const __FDyBinderBase* iPtrBinder);

  static MDY_NODISCARD std::optional<const __TDyRscInfo_T<EDyResourceType::Material>*>
  MDY_PRIVATE_FUNC_SPECIFIER(pTryRequireInformation_Material)
  (_MIN_ const std::string& iSpecifier, _MIN_ const __FDyBinderBase* iPtrBinder);

  template <EDyResourceType TType>
  friend struct __TDyResourceBinderBase;
  template <EDyResourceType TType>
  friend struct __TDyInformationBinderBase;
};

} /// ::dy namespace

#endif /// GUARD_DY_CORE_THREAD_SDYIOBINDINGHELPER_H