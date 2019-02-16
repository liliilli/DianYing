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
  MDY_NODISCARD static std::optional<const __TResourceType_T<TType>*>
  TryRequireResource(_MIN_ const std::string& iSpecifier, _MIN_ const __FDyBinderBase* iPtrBinder)
  {
    if constexpr (TType == EDyResourceType::GLShader)     
    { return MDY_PRIVATE(pTryRequireResource_GLShader)(iSpecifier, iPtrBinder); }
    else if constexpr (TType == EDyResourceType::Mesh)   
    { return MDY_PRIVATE(pTryRequireResource_Mesh)(iSpecifier, iPtrBinder); }
    else if constexpr (TType == EDyResourceType::Model)   
    { return MDY_PRIVATE(pTryRequireResource_Model)(iSpecifier, iPtrBinder); }
    else if constexpr (TType == EDyResourceType::Texture) 
    { return MDY_PRIVATE(pTryRequireResource_Texture)(iSpecifier, iPtrBinder); }
    else if constexpr (TType == EDyResourceType::Material)
    { return MDY_PRIVATE(pTryRequireResource_Material)(iSpecifier, iPtrBinder); }
    else if constexpr (TType == EDyResourceType::GLAttachment)  
    { return MDY_PRIVATE(pTryRequireResource_Attachment)(iSpecifier, iPtrBinder); }
    else if constexpr (TType == EDyResourceType::GLFrameBuffer) 
    { return MDY_PRIVATE(pTryRequireResource_FrameBuffer)(iSpecifier, iPtrBinder); }
    else 
    { MDY_UNEXPECTED_BRANCH_BUT_RETURN(std::nullopt); }
  }

  MDY_NODISCARD static EDySuccess
  MDY_PRIVATE(pTryRequireResource)
  (_MIN_ const std::string& iSpecifier, _MIN_ EDyResourceType iType, _MIN_ const __FDyBinderBase* iPtrBinder);

  MDY_NODISCARD static std::optional<const __TResourceType_T<EDyResourceType::GLShader>*>
  MDY_PRIVATE(pTryRequireResource_GLShader)
  (_MIN_ const std::string& iSpecifier, _MIN_ const __FDyBinderBase* iPtrBinder);

  MDY_NODISCARD static std::optional<const __TResourceType_T<EDyResourceType::Mesh>*>
  MDY_PRIVATE(pTryRequireResource_Mesh)
  (_MIN_ const std::string& iSpecifier, _MIN_ const __FDyBinderBase* iPtrBinder);

  MDY_NODISCARD static std::optional<const __TResourceType_T<EDyResourceType::Model>*>
  MDY_PRIVATE(pTryRequireResource_Model)
  (_MIN_ const std::string& iSpecifier, _MIN_ const __FDyBinderBase* iPtrBinder);

  MDY_NODISCARD static std::optional<const __TResourceType_T<EDyResourceType::Texture>*>
  MDY_PRIVATE(pTryRequireResource_Texture)
  (_MIN_ const std::string& iSpecifier, _MIN_ const __FDyBinderBase* iPtrBinder);

  MDY_NODISCARD static std::optional<const __TResourceType_T<EDyResourceType::Material>*>
  MDY_PRIVATE(pTryRequireResource_Material)
  (_MIN_ const std::string& iSpecifier, _MIN_ const __FDyBinderBase* iPtrBinder);

  MDY_NODISCARD static std::optional<const __TResourceType_T<EDyResourceType::GLAttachment>*>
  MDY_PRIVATE(pTryRequireResource_Attachment)
  (_MIN_ const std::string& iSpecifier, _MIN_ const __FDyBinderBase* iPtrBinder);

  MDY_NODISCARD static std::optional<const __TResourceType_T<EDyResourceType::GLFrameBuffer>*>
  MDY_PRIVATE(pTryRequireResource_FrameBuffer)
  (_MIN_ const std::string& iSpecifier, _MIN_ const __FDyBinderBase* iPtrBinder);

  /// @brief Use this require resource to another dimension.
  template <EDyResourceType TType>
  MDY_NODISCARD static EDySuccess
  TryDetachResource(_MIN_ const std::string& iSpecifier, _MIN_ const __FDyBinderBase* iPtrBinder)
  {
    return MDY_PRIVATE(pTryDetachResource)(iSpecifier, TType, iPtrBinder);
  }

  MDY_NODISCARD static EDySuccess
  MDY_PRIVATE(pTryDetachResource)
  (_MIN_ const std::string& iSpecifier, _MIN_ EDyResourceType iType, _MIN_ const __FDyBinderBase* iPtrBinder);

  /// @brief Use this require resource to another dimension.
  template <EDyResourceType TType>
  MDY_NODISCARD static std::optional<const __TDyRscInfo_T<TType>*>
  TryRequireInformation(_MIN_ const std::string& iSpecifier, _MIN_ const __FDyBinderBase* iPtrBinder)
  {
    if constexpr (TType == EDyResourceType::GLShader)     
    { return MDY_PRIVATE(pTryRequireInformation_GLShader)(iSpecifier, iPtrBinder); }
    else if constexpr (TType == EDyResourceType::GLAttachment)     
    { return MDY_PRIVATE(pTryRequireInformation_Attachment)(iSpecifier, iPtrBinder); }
    else if constexpr (TType == EDyResourceType::GLFrameBuffer)     
    { return MDY_PRIVATE(pTryRequireInformation_GLFrameBuffer)(iSpecifier, iPtrBinder); }
    else if constexpr (TType == EDyResourceType::Texture)     
    { return MDY_PRIVATE(pTryRequireInformation_Texture)(iSpecifier, iPtrBinder); }
    else if constexpr (TType == EDyResourceType::Material)     
    { return MDY_PRIVATE(pTryRequireInformation_Material)(iSpecifier, iPtrBinder); }
    else if constexpr (TType == EDyResourceType::Mesh)
    { return MDY_PRIVATE(pTryRequireInformation_Mesh)(iSpecifier, iPtrBinder); }
    else if constexpr (TType == EDyResourceType::Model)     
    { return MDY_PRIVATE(pTryRequireInformation_Model)(iSpecifier, iPtrBinder); }
    else if constexpr (TType == EDyResourceType::Skeleton)
    { return MDY_PRIVATE(pTryRequireInformation_Skeleton)(iSpecifier, iPtrBinder); }
    else if constexpr (TType == EDyResourceType::AnimationScrap)
    { return MDY_PRIVATE(pTryRequireInformation_AnimationScrap)(iSpecifier, iPtrBinder); }
    else if constexpr (TType == EDyResourceType::Sound)
    { return MDY_PRIVATE(pTryRequireInformation_Sound)(iSpecifier, iPtrBinder); }
    else 
    { MDY_UNEXPECTED_BRANCH_BUT_RETURN(std::nullopt); }
  }
  
  MDY_NODISCARD static EDySuccess
  MDY_PRIVATE(pTryRequireInformation)
  (_MIN_ const std::string& iSpecifier, _MIN_ EDyResourceType iType, _MIN_ const __FDyBinderBase* iPtrBinder);
  
  MDY_NODISCARD static std::optional<const __TDyRscInfo_T<EDyResourceType::GLShader>*>
  MDY_PRIVATE(pTryRequireInformation_GLShader)
  (_MIN_ const std::string& iSpecifier, _MIN_ const __FDyBinderBase* iPtrBinder);

  MDY_NODISCARD static std::optional<const __TDyRscInfo_T<EDyResourceType::GLFrameBuffer>*>
  MDY_PRIVATE(pTryRequireInformation_GLFrameBuffer)
  (_MIN_ const std::string& iSpecifier, _MIN_ const __FDyBinderBase* iPtrBinder);
  
  MDY_NODISCARD static std::optional<const __TDyRscInfo_T<EDyResourceType::Texture>*>
  MDY_PRIVATE(pTryRequireInformation_Texture)
  (_MIN_ const std::string& iSpecifier, _MIN_ const __FDyBinderBase* iPtrBinder);
    
  MDY_NODISCARD static std::optional<const __TDyRscInfo_T<EDyResourceType::GLAttachment>*>
  MDY_PRIVATE(pTryRequireInformation_Attachment)
  (_MIN_ const std::string& iSpecifier, _MIN_ const __FDyBinderBase* iPtrBinder);

  MDY_NODISCARD static std::optional<const __TDyRscInfo_T<EDyResourceType::Material>*>
  MDY_PRIVATE(pTryRequireInformation_Material)
  (_MIN_ const std::string& iSpecifier, _MIN_ const __FDyBinderBase* iPtrBinder);

  MDY_NODISCARD static std::optional<const __TDyRscInfo_T<EDyResourceType::Mesh>*>
  MDY_PRIVATE(pTryRequireInformation_Mesh)
  (_MIN_ const std::string& iSpecifier, _MIN_ const __FDyBinderBase* iPtrBinder);

  MDY_NODISCARD static std::optional<const __TDyRscInfo_T<EDyResourceType::Model>*>
  MDY_PRIVATE(pTryRequireInformation_Model)
  (_MIN_ const std::string& iSpecifier, _MIN_ const __FDyBinderBase* iPtrBinder);
  
  MDY_NODISCARD static std::optional<const __TDyRscInfo_T<EDyResourceType::Skeleton>*>
  MDY_PRIVATE(pTryRequireInformation_Skeleton)
  (_MIN_ const std::string& iSpecifier, _MIN_ const __FDyBinderBase* iPtrBinder);
   
  MDY_NODISCARD static std::optional<const __TDyRscInfo_T<EDyResourceType::AnimationScrap>*>
  MDY_PRIVATE(pTryRequireInformation_AnimationScrap)
  (_MIN_ const std::string& iSpecifier, _MIN_ const __FDyBinderBase* iPtrBinder);
    
  MDY_NODISCARD static std::optional<const __TDyRscInfo_T<EDyResourceType::Sound>*>
  MDY_PRIVATE(pTryRequireInformation_Sound)
  (_MIN_ const std::string& iSpecifier, _MIN_ const __FDyBinderBase* iPtrBinder);
 
  /// @brief Use this require resource to another dimension.
  template <EDyResourceType TType>
  MDY_NODISCARD static EDySuccess
  TryDetachInformation(_MIN_ const std::string& iSpecifier, _MIN_ const __FDyBinderBase* iPtrBinder)
  {
    return MDY_PRIVATE(pTryDetachInformation)(iSpecifier, TType, iPtrBinder);
  }

  MDY_NODISCARD static EDySuccess
  MDY_PRIVATE(pTryDetachInformation)
  (_MIN_ const std::string& iSpecifier, _MIN_ EDyResourceType iType, _MIN_ const __FDyBinderBase* iPtrBinder);

  template <EDyResourceType TType>
  friend struct __TDyResourceBinderBase;
  template <EDyResourceType TType>
  friend struct __TDyInformationBinderBase;
};

} /// ::dy namespace

#endif /// GUARD_DY_CORE_THREAD_SDYIOBINDINGHELPER_H