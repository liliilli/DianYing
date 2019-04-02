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
#include <Dy/Core/Resource/Type/XTemplateRescInfoType.h>
#include <Dy/Core/Resource/Type/IBinderBase.h>

namespace dy
{

/// @class SIOBindingHelper
/// @brief Binding helper class for information & resource type from outside world.
class SIOBindingHelper final
{
public:
  /// @brief Use this require resource to another dimension.
  template <EResourceType TType>
  MDY_NODISCARD static std::optional<const __TResourceType_T<TType>*>
  TryRequireResource(const std::string& iSpecifier, IBinderBase& iPtrBinder)
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
  
  /// @brief Use this require resource to another dimension.
  template <EResourceType TType>
  MDY_NODISCARD static EDySuccess
  TryDetachResource(const std::string& iSpecifier, IBinderBase& iPtrBinder)
  {
    return MDY_PRIVATE(pTryDetachResource)(iSpecifier, TType, iPtrBinder);
  }
  
  /// @brief Use this require resource to another dimension.
  template <EResourceType TType>
  MDY_NODISCARD static std::optional<const __TDyRscInfo_T<TType>*>
  TryRequireInformation(const std::string& iSpecifier, IBinderBase& iPtrBinder)
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
   
  /// @brief Use this require resource to another dimension.
  template <EResourceType TType>
  MDY_NODISCARD static EDySuccess
  TryDetachInformation(const std::string& iSpecifier, IBinderBase& iPtrBinder)
  {
    return MDY_PRIVATE(pTryDetachInformation)(iSpecifier, TType, iPtrBinder);
  }

private:
  MDY_NODISCARD static EDySuccess
  pTryRequireResource(const std::string& iSpecifier, EResourceType iType, IBinderBase& iPtrBinder);

  MDY_NODISCARD static std::optional<const __TResourceType_T<EResourceType::GLShader>*>
  MDY_PRIVATE(pTryRequireResource_GLShader)
  (const std::string& iSpecifier, IBinderBase& iPtrBinder);

  MDY_NODISCARD static std::optional<const __TResourceType_T<EResourceType::Mesh>*>
  MDY_PRIVATE(pTryRequireResource_Mesh)
  (const std::string& iSpecifier, IBinderBase& iPtrBinder);

  MDY_NODISCARD static std::optional<const __TResourceType_T<EResourceType::Model>*>
  MDY_PRIVATE(pTryRequireResource_Model)
  (const std::string& iSpecifier, IBinderBase& iPtrBinder);

  MDY_NODISCARD static std::optional<const __TResourceType_T<EResourceType::Texture>*>
  MDY_PRIVATE(pTryRequireResource_Texture)
  (const std::string& iSpecifier, IBinderBase& iPtrBinder);

  MDY_NODISCARD static std::optional<const __TResourceType_T<EResourceType::Material>*>
  MDY_PRIVATE(pTryRequireResource_Material)
  (const std::string& iSpecifier, IBinderBase& iPtrBinder);

  MDY_NODISCARD static std::optional<const __TResourceType_T<EResourceType::GLAttachment>*>
  MDY_PRIVATE(pTryRequireResource_Attachment)
  (const std::string& iSpecifier, IBinderBase& iPtrBinder);

  MDY_NODISCARD static std::optional<const __TResourceType_T<EResourceType::GLFrameBuffer>*>
  MDY_PRIVATE(pTryRequireResource_FrameBuffer)
  (const std::string& iSpecifier, IBinderBase& iPtrBinder);

  MDY_NODISCARD static EDySuccess
  MDY_PRIVATE(pTryDetachResource)(const std::string& iSpecifier, EResourceType iType, IBinderBase& iPtrBinder);

  MDY_NODISCARD static EDySuccess
  MDY_PRIVATE(pTryRequireInformation)
  (const std::string& iSpecifier, EResourceType iType, IBinderBase& iPtrBinder);
  
  MDY_NODISCARD static std::optional<const __TDyRscInfo_T<EResourceType::GLShader>*>
  MDY_PRIVATE(pTryRequireInformation_GLShader)
  (const std::string& iSpecifier, IBinderBase& iPtrBinder);

  MDY_NODISCARD static std::optional<const __TDyRscInfo_T<EResourceType::GLFrameBuffer>*>
  MDY_PRIVATE(pTryRequireInformation_GLFrameBuffer)
  (const std::string& iSpecifier, IBinderBase& iPtrBinder);
  
  MDY_NODISCARD static std::optional<const __TDyRscInfo_T<EResourceType::Texture>*>
  MDY_PRIVATE(pTryRequireInformation_Texture)
  (const std::string& iSpecifier, IBinderBase& iPtrBinder);
    
  MDY_NODISCARD static std::optional<const __TDyRscInfo_T<EResourceType::GLAttachment>*>
  MDY_PRIVATE(pTryRequireInformation_Attachment)
  (const std::string& iSpecifier, IBinderBase& iPtrBinder);

  MDY_NODISCARD static std::optional<const __TDyRscInfo_T<EResourceType::Material>*>
  MDY_PRIVATE(pTryRequireInformation_Material)
  (const std::string& iSpecifier, IBinderBase& iPtrBinder);

  MDY_NODISCARD static std::optional<const __TDyRscInfo_T<EResourceType::Mesh>*>
  MDY_PRIVATE(pTryRequireInformation_Mesh)
  (const std::string& iSpecifier, IBinderBase& iPtrBinder);

  MDY_NODISCARD static std::optional<const __TDyRscInfo_T<EResourceType::Model>*>
  MDY_PRIVATE(pTryRequireInformation_Model)
  (const std::string& iSpecifier, IBinderBase& iPtrBinder);
  
  MDY_NODISCARD static std::optional<const __TDyRscInfo_T<EResourceType::Skeleton>*>
  MDY_PRIVATE(pTryRequireInformation_Skeleton)
  (const std::string& iSpecifier, IBinderBase& iPtrBinder);
   
  MDY_NODISCARD static std::optional<const __TDyRscInfo_T<EResourceType::AnimationScrap>*>
  MDY_PRIVATE(pTryRequireInformation_AnimationScrap)
  (const std::string& iSpecifier, IBinderBase& iPtrBinder);
    
  MDY_NODISCARD static std::optional<const __TDyRscInfo_T<EResourceType::Sound>*>
  MDY_PRIVATE(pTryRequireInformation_Sound)
  (const std::string& iSpecifier, IBinderBase& iPtrBinder);

  MDY_NODISCARD static EDySuccess
  MDY_PRIVATE(pTryDetachInformation)
  (const std::string& iSpecifier, EResourceType iType, IBinderBase& iPtrBinder);
};

} /// ::dy namespace

#endif /// GUARD_DY_CORE_THREAD_SDYIOBINDINGHELPER_H