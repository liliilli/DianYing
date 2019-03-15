#ifndef GUARD_DY_CORE_THREAD_SDYIOBINDINGHELPER_H
#define GUARD_DY_CORE_THREAD_SDYIOBINDINGHELPER_H
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

#include <Dy/Meta/Type/EResourceType.h>
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
  template <EResourceType TType>
  MDY_NODISCARD static std::optional<const __TResourceType_T<TType>*>
  TryRequireResource(_MIN_ const std::string& iSpecifier, _MIN_ const __FDyBinderBase* iPtrBinder)
  {
    if constexpr (TType == EResourceType::GLShader)     
    { return MDY_PRIVATE(pTryRequireResource_GLShader)(iSpecifier, iPtrBinder); }
    else if constexpr (TType == EResourceType::Mesh)   
    { return MDY_PRIVATE(pTryRequireResource_Mesh)(iSpecifier, iPtrBinder); }
    else if constexpr (TType == EResourceType::Model)   
    { return MDY_PRIVATE(pTryRequireResource_Model)(iSpecifier, iPtrBinder); }
    else if constexpr (TType == EResourceType::Texture) 
    { return MDY_PRIVATE(pTryRequireResource_Texture)(iSpecifier, iPtrBinder); }
    else if constexpr (TType == EResourceType::Material)
    { return MDY_PRIVATE(pTryRequireResource_Material)(iSpecifier, iPtrBinder); }
    else if constexpr (TType == EResourceType::GLAttachment)  
    { return MDY_PRIVATE(pTryRequireResource_Attachment)(iSpecifier, iPtrBinder); }
    else if constexpr (TType == EResourceType::GLFrameBuffer) 
    { return MDY_PRIVATE(pTryRequireResource_FrameBuffer)(iSpecifier, iPtrBinder); }
    else 
    { MDY_UNEXPECTED_BRANCH_BUT_RETURN(std::nullopt); }
  }

  MDY_NODISCARD static EDySuccess
  MDY_PRIVATE(pTryRequireResource)
  (_MIN_ const std::string& iSpecifier, _MIN_ EResourceType iType, _MIN_ const __FDyBinderBase* iPtrBinder);

  MDY_NODISCARD static std::optional<const __TResourceType_T<EResourceType::GLShader>*>
  MDY_PRIVATE(pTryRequireResource_GLShader)
  (_MIN_ const std::string& iSpecifier, _MIN_ const __FDyBinderBase* iPtrBinder);

  MDY_NODISCARD static std::optional<const __TResourceType_T<EResourceType::Mesh>*>
  MDY_PRIVATE(pTryRequireResource_Mesh)
  (_MIN_ const std::string& iSpecifier, _MIN_ const __FDyBinderBase* iPtrBinder);

  MDY_NODISCARD static std::optional<const __TResourceType_T<EResourceType::Model>*>
  MDY_PRIVATE(pTryRequireResource_Model)
  (_MIN_ const std::string& iSpecifier, _MIN_ const __FDyBinderBase* iPtrBinder);

  MDY_NODISCARD static std::optional<const __TResourceType_T<EResourceType::Texture>*>
  MDY_PRIVATE(pTryRequireResource_Texture)
  (_MIN_ const std::string& iSpecifier, _MIN_ const __FDyBinderBase* iPtrBinder);

  MDY_NODISCARD static std::optional<const __TResourceType_T<EResourceType::Material>*>
  MDY_PRIVATE(pTryRequireResource_Material)
  (_MIN_ const std::string& iSpecifier, _MIN_ const __FDyBinderBase* iPtrBinder);

  MDY_NODISCARD static std::optional<const __TResourceType_T<EResourceType::GLAttachment>*>
  MDY_PRIVATE(pTryRequireResource_Attachment)
  (_MIN_ const std::string& iSpecifier, _MIN_ const __FDyBinderBase* iPtrBinder);

  MDY_NODISCARD static std::optional<const __TResourceType_T<EResourceType::GLFrameBuffer>*>
  MDY_PRIVATE(pTryRequireResource_FrameBuffer)
  (_MIN_ const std::string& iSpecifier, _MIN_ const __FDyBinderBase* iPtrBinder);

  /// @brief Use this require resource to another dimension.
  template <EResourceType TType>
  MDY_NODISCARD static EDySuccess
  TryDetachResource(_MIN_ const std::string& iSpecifier, _MIN_ const __FDyBinderBase* iPtrBinder)
  {
    return MDY_PRIVATE(pTryDetachResource)(iSpecifier, TType, iPtrBinder);
  }

  MDY_NODISCARD static EDySuccess
  MDY_PRIVATE(pTryDetachResource)
  (_MIN_ const std::string& iSpecifier, _MIN_ EResourceType iType, _MIN_ const __FDyBinderBase* iPtrBinder);

  /// @brief Use this require resource to another dimension.
  template <EResourceType TType>
  MDY_NODISCARD static std::optional<const __TDyRscInfo_T<TType>*>
  TryRequireInformation(_MIN_ const std::string& iSpecifier, _MIN_ const __FDyBinderBase* iPtrBinder)
  {
    if constexpr (TType == EResourceType::GLShader)     
    { return MDY_PRIVATE(pTryRequireInformation_GLShader)(iSpecifier, iPtrBinder); }
    else if constexpr (TType == EResourceType::GLAttachment)     
    { return MDY_PRIVATE(pTryRequireInformation_Attachment)(iSpecifier, iPtrBinder); }
    else if constexpr (TType == EResourceType::GLFrameBuffer)     
    { return MDY_PRIVATE(pTryRequireInformation_GLFrameBuffer)(iSpecifier, iPtrBinder); }
    else if constexpr (TType == EResourceType::Texture)     
    { return MDY_PRIVATE(pTryRequireInformation_Texture)(iSpecifier, iPtrBinder); }
    else if constexpr (TType == EResourceType::Material)     
    { return MDY_PRIVATE(pTryRequireInformation_Material)(iSpecifier, iPtrBinder); }
    else if constexpr (TType == EResourceType::Mesh)
    { return MDY_PRIVATE(pTryRequireInformation_Mesh)(iSpecifier, iPtrBinder); }
    else if constexpr (TType == EResourceType::Model)     
    { return MDY_PRIVATE(pTryRequireInformation_Model)(iSpecifier, iPtrBinder); }
    else if constexpr (TType == EResourceType::Skeleton)
    { return MDY_PRIVATE(pTryRequireInformation_Skeleton)(iSpecifier, iPtrBinder); }
    else if constexpr (TType == EResourceType::AnimationScrap)
    { return MDY_PRIVATE(pTryRequireInformation_AnimationScrap)(iSpecifier, iPtrBinder); }
    else if constexpr (TType == EResourceType::Sound)
    { return MDY_PRIVATE(pTryRequireInformation_Sound)(iSpecifier, iPtrBinder); }
    else 
    { MDY_UNEXPECTED_BRANCH_BUT_RETURN(std::nullopt); }
  }
  
  MDY_NODISCARD static EDySuccess
  MDY_PRIVATE(pTryRequireInformation)
  (_MIN_ const std::string& iSpecifier, _MIN_ EResourceType iType, _MIN_ const __FDyBinderBase* iPtrBinder);
  
  MDY_NODISCARD static std::optional<const __TDyRscInfo_T<EResourceType::GLShader>*>
  MDY_PRIVATE(pTryRequireInformation_GLShader)
  (_MIN_ const std::string& iSpecifier, _MIN_ const __FDyBinderBase* iPtrBinder);

  MDY_NODISCARD static std::optional<const __TDyRscInfo_T<EResourceType::GLFrameBuffer>*>
  MDY_PRIVATE(pTryRequireInformation_GLFrameBuffer)
  (_MIN_ const std::string& iSpecifier, _MIN_ const __FDyBinderBase* iPtrBinder);
  
  MDY_NODISCARD static std::optional<const __TDyRscInfo_T<EResourceType::Texture>*>
  MDY_PRIVATE(pTryRequireInformation_Texture)
  (_MIN_ const std::string& iSpecifier, _MIN_ const __FDyBinderBase* iPtrBinder);
    
  MDY_NODISCARD static std::optional<const __TDyRscInfo_T<EResourceType::GLAttachment>*>
  MDY_PRIVATE(pTryRequireInformation_Attachment)
  (_MIN_ const std::string& iSpecifier, _MIN_ const __FDyBinderBase* iPtrBinder);

  MDY_NODISCARD static std::optional<const __TDyRscInfo_T<EResourceType::Material>*>
  MDY_PRIVATE(pTryRequireInformation_Material)
  (_MIN_ const std::string& iSpecifier, _MIN_ const __FDyBinderBase* iPtrBinder);

  MDY_NODISCARD static std::optional<const __TDyRscInfo_T<EResourceType::Mesh>*>
  MDY_PRIVATE(pTryRequireInformation_Mesh)
  (_MIN_ const std::string& iSpecifier, _MIN_ const __FDyBinderBase* iPtrBinder);

  MDY_NODISCARD static std::optional<const __TDyRscInfo_T<EResourceType::Model>*>
  MDY_PRIVATE(pTryRequireInformation_Model)
  (_MIN_ const std::string& iSpecifier, _MIN_ const __FDyBinderBase* iPtrBinder);
  
  MDY_NODISCARD static std::optional<const __TDyRscInfo_T<EResourceType::Skeleton>*>
  MDY_PRIVATE(pTryRequireInformation_Skeleton)
  (_MIN_ const std::string& iSpecifier, _MIN_ const __FDyBinderBase* iPtrBinder);
   
  MDY_NODISCARD static std::optional<const __TDyRscInfo_T<EResourceType::AnimationScrap>*>
  MDY_PRIVATE(pTryRequireInformation_AnimationScrap)
  (_MIN_ const std::string& iSpecifier, _MIN_ const __FDyBinderBase* iPtrBinder);
    
  MDY_NODISCARD static std::optional<const __TDyRscInfo_T<EResourceType::Sound>*>
  MDY_PRIVATE(pTryRequireInformation_Sound)
  (_MIN_ const std::string& iSpecifier, _MIN_ const __FDyBinderBase* iPtrBinder);
 
  /// @brief Use this require resource to another dimension.
  template <EResourceType TType>
  MDY_NODISCARD static EDySuccess
  TryDetachInformation(_MIN_ const std::string& iSpecifier, _MIN_ const __FDyBinderBase* iPtrBinder)
  {
    return MDY_PRIVATE(pTryDetachInformation)(iSpecifier, TType, iPtrBinder);
  }

  MDY_NODISCARD static EDySuccess
  MDY_PRIVATE(pTryDetachInformation)
  (_MIN_ const std::string& iSpecifier, _MIN_ EResourceType iType, _MIN_ const __FDyBinderBase* iPtrBinder);

  template <EResourceType TType>
  friend struct __TDyResourceBinderBase;
  template <EResourceType TType>
  friend struct __TDyInformationBinderBase;
};

} /// ::dy namespace

#endif /// GUARD_DY_CORE_THREAD_SDYIOBINDINGHELPER_H