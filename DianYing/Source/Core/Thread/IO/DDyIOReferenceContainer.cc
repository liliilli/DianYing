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
#include <Dy/Core/Thread/IO/DDyIOReferenceContainer.h>
#include <Dy/Core/Resource/Type/IBinderBase.h>
#include <Dy/Core/Thread/SDyIOConnectionHelper.h>
#include <Dy/Helper/Library/HelperContainer.h>
#include <Dy/Management/MLog.h>

namespace dy
{

bool DDyIOReferenceContainer::IsReferenceInstanceExist(_MIN_ const std::string& specifier, _MIN_ EResourceType type) const noexcept
{
  switch (type)
  {
  case EResourceType::GLShader: return Contains(this->mMapGLShaderReference, specifier);
  case EResourceType::Texture:  return Contains(this->mMapTextureReference, specifier);
  case EResourceType::Mesh:     return Contains(this->mMapMeshReference, specifier);
  case EResourceType::Model:    return Contains(this->mMapModelReference, specifier);
  case EResourceType::Skeleton:       return Contains(this->mMapModelSkeletonReference, specifier);
  case EResourceType::AnimationScrap: return Contains(this->mMapModelAnimScrapReference, specifier);
  case EResourceType::Material:       return Contains(this->mMapMaterialReference, specifier);
  case EResourceType::GLAttachment:   return Contains(this->mMapAttachmentReference, specifier);
  case EResourceType::GLFrameBuffer:  return Contains(this->mMapFrameBufferReference, specifier);
  case EResourceType::Sound:          return Contains(this->mMapSoundReference, specifier);
  default: MDY_UNEXPECTED_BRANCH_BUT_RETURN(false);
  }
}

bool DDyIOReferenceContainer::IsReferenceInstanceBound(_MIN_ const std::string& specifier, _MIN_ EResourceType type) const noexcept
{
  switch (type)
  {
  case EResourceType::GLShader: return this->mMapGLShaderReference.at(specifier).mIsResourceValid;
  case EResourceType::Texture:  return this->mMapTextureReference.at(specifier).mIsResourceValid;
  case EResourceType::Mesh:     return this->mMapMeshReference.at(specifier).mIsResourceValid;
  case EResourceType::Model:    return this->mMapModelReference.at(specifier).mIsResourceValid;
  case EResourceType::Skeleton:       return this->mMapModelSkeletonReference.at(specifier).mIsResourceValid;
  case EResourceType::AnimationScrap: return this->mMapModelAnimScrapReference.at(specifier).mIsResourceValid;
  case EResourceType::Material:       return this->mMapMaterialReference.at(specifier).mIsResourceValid;
  case EResourceType::GLAttachment:   return this->mMapAttachmentReference.at(specifier).mIsResourceValid;
  case EResourceType::GLFrameBuffer:  return this->mMapFrameBufferReference.at(specifier).mIsResourceValid;
  case EResourceType::Sound:          return this->mMapSoundReference.at(specifier).mIsResourceValid;
  default: MDY_UNEXPECTED_BRANCH_BUT_RETURN(false);
  }
}

bool DDyIOReferenceContainer::TryEnlargeResourceScope(_MIN_ EResourceScope scope, _MIN_ const std::string& specifier, _MIN_ EResourceType type)
{
  // Find given resource type instance. Given specifier and type must be valid on list.
  DDyIOReferenceInstance* instance;
  switch (type)
  {
  case EResourceType::GLShader: { instance = &this->mMapGLShaderReference[specifier]; } break;
  case EResourceType::Texture:  { instance = &this->mMapTextureReference[specifier]; }  break;
  case EResourceType::Mesh:     { instance = &this->mMapMeshReference[specifier]; } break;
  case EResourceType::Model:    { instance = &this->mMapModelReference[specifier]; }    break;
  case EResourceType::Material: { instance = &this->mMapMaterialReference[specifier]; } break;
  case EResourceType::GLAttachment:   { instance = &this->mMapAttachmentReference[specifier]; } break;
  case EResourceType::GLFrameBuffer:  { instance = &this->mMapFrameBufferReference[specifier]; } break;
  case EResourceType::Skeleton:       { instance = &this->mMapModelSkeletonReference[specifier]; } break;
  case EResourceType::AnimationScrap: { instance = &this->mMapModelAnimScrapReference[specifier]; } break;
  case EResourceType::Sound:          { instance = &this->mMapSoundReference[specifier]; } break; 
  default: MDY_UNEXPECTED_BRANCH_BUT_RETURN(false);
  }

  // Compare scope and if previous scope is not larger than given input scope, update it.
  if (scope > instance->mScope) { instance->mScope = scope; }
  return true;
}

EDySuccess DDyIOReferenceContainer::TryBindBinderToResourceRI(
    _MIN_ const std::string& iSpecifier,
    _MIN_ EResourceType iType,
    _MIN_ const __IBinderBase* iPtrBinder)
{
  // Check RI is exist, if not found just return failure.
  if (this->IsReferenceInstanceExist(iSpecifier, iType) == false) { return DY_FAILURE; }

  // Attach binder to RI container.
  switch (iType)
  {
  case EResourceType::Model:    this->mMapModelReference[iSpecifier].AttachBinder(iPtrBinder);    break;
  case EResourceType::GLShader: this->mMapGLShaderReference[iSpecifier].AttachBinder(iPtrBinder); break;
  case EResourceType::Texture:  this->mMapTextureReference[iSpecifier].AttachBinder(iPtrBinder);  break;
  case EResourceType::Mesh:     this->mMapMeshReference[iSpecifier].AttachBinder(iPtrBinder);     break;
  case EResourceType::Material: this->mMapMaterialReference[iSpecifier].AttachBinder(iPtrBinder); break;
  case EResourceType::GLAttachment:   this->mMapAttachmentReference[iSpecifier].AttachBinder(iPtrBinder);       break;
  case EResourceType::GLFrameBuffer:  this->mMapFrameBufferReference[iSpecifier].AttachBinder(iPtrBinder);      break;
  case EResourceType::Skeleton:       this->mMapModelSkeletonReference[iSpecifier].AttachBinder(iPtrBinder);    break;
  case EResourceType::AnimationScrap: this->mMapModelAnimScrapReference[iSpecifier].AttachBinder(iPtrBinder);   break;
  case EResourceType::Sound:          this->mMapSoundReference[iSpecifier].AttachBinder(iPtrBinder);   break;
  default: MDY_UNEXPECTED_BRANCH_BUT_RETURN(DY_FAILURE);
  }
  return DY_SUCCESS;
}

EDySuccess DDyIOReferenceContainer::TryDetachBinderFromResourceRI(
    _MIN_ const std::string& iSpecifier, 
    _MIN_ EResourceType iType, 
    _MIN_ const __IBinderBase* iPtrBinder)
{
  // Check RI is exist, if not found just return failure.
  if (this->IsReferenceInstanceExist(iSpecifier, iType) == false) { return DY_FAILURE; }

  // Get Reference instance map from `iType`.
  decltype(mMapModelReference)* ptrRiMap;
  switch (iType)
  {
  case EResourceType::Model:    ptrRiMap = &this->mMapModelReference;     break;
  case EResourceType::GLShader: ptrRiMap = &this->mMapGLShaderReference;  break;
  case EResourceType::Texture:  ptrRiMap = &this->mMapTextureReference;   break;
  case EResourceType::Mesh:     ptrRiMap = &this->mMapMeshReference;      break;
  case EResourceType::Material: ptrRiMap = &this->mMapMaterialReference;  break;
  case EResourceType::GLAttachment:   ptrRiMap = &this->mMapAttachmentReference;  break;
  case EResourceType::GLFrameBuffer:  ptrRiMap = &this->mMapFrameBufferReference; break;
  case EResourceType::Skeleton:       ptrRiMap = &this->mMapModelSkeletonReference; break;
  case EResourceType::AnimationScrap: ptrRiMap = &this->mMapModelAnimScrapReference; break;
  case EResourceType::Sound:          ptrRiMap = &this->mMapSoundReference; break;
  default: MDY_UNEXPECTED_BRANCH_BUT_RETURN(DY_FAILURE);
  }

  // Detach binder from given instance.
  (*ptrRiMap)[iSpecifier].DetachBinder(iPtrBinder);

  // If target RI need to be GCed (so should be moved into GCList), move it.
  if ((*ptrRiMap)[iSpecifier].IsNeedToBeGced() == true)
  {
    DyPushLogDebugDebug("Moved Reference Instance to GClist. {}", iSpecifier);
    SDyIOConnectionHelper::InsertGcCandidate((*ptrRiMap)[iSpecifier]);
    ptrRiMap->erase(iSpecifier);
  }

  return DY_SUCCESS;
}

std::vector<DDyIOReferenceInstance> 
DDyIOReferenceContainer::GetForwardCandidateRIAsList(_MIN_ EResourceScope iScope)
{
  std::vector<DDyIOReferenceInstance> result;
  this->ForwardCandidateRIFromList(iScope, this->mMapTextureReference, result);
  this->ForwardCandidateRIFromList(iScope, this->mMapGLShaderReference, result);
  this->ForwardCandidateRIFromList(iScope, this->mMapMeshReference, result);
  this->ForwardCandidateRIFromList(iScope, this->mMapModelReference, result);
  this->ForwardCandidateRIFromList(iScope, this->mMapMaterialReference, result);
  this->ForwardCandidateRIFromList(iScope, this->mMapAttachmentReference, result);
  this->ForwardCandidateRIFromList(iScope, this->mMapFrameBufferReference, result);
  this->ForwardCandidateRIFromList(iScope, this->mMapModelSkeletonReference, result);
  this->ForwardCandidateRIFromList(iScope, this->mMapModelAnimScrapReference, result);
  this->ForwardCandidateRIFromList(iScope, this->mMapSoundReference, result);
  return result;
}

void DDyIOReferenceContainer::ForwardCandidateRIFromList(
    _MIN_ EResourceScope iScope,
    _MINOUT_ TStringHashMap<DDyIOReferenceInstance>& iContainer, 
    _MOUT_ std::vector<DDyIOReferenceInstance>& iResult)
{
  for (auto it = iContainer.begin(); it != iContainer.end();)
  {
    auto& [specifier, instance] = *it;
    if (instance.mScope != iScope) { ++it; continue; }
    if (instance.mIsResourceValid == true && instance.mPtrBoundBinderList.empty() == true)
    {
      iResult.emplace_back(std::move(instance));
      it = iContainer.erase(it);
    }
    else { ++it; }
  }
}

EDySuccess DDyIOReferenceContainer::CreateReferenceInstance(
    _MIN_ const std::string& specifier,
    _MIN_ EResourceType type, _MIN_ EDyResourceStyle style, _MIN_ EResourceScope scope)
{
  TStringHashMap<DDyIOReferenceInstance>* ptrRIHashMap;

  switch (type)
  {
  case EResourceType::GLShader: { ptrRIHashMap = &this->mMapGLShaderReference; } break;
  case EResourceType::Texture:  { ptrRIHashMap = &this->mMapTextureReference; } break;
  case EResourceType::Mesh:     { ptrRIHashMap = &this->mMapMeshReference; } break;
  case EResourceType::Model:    { ptrRIHashMap = &this->mMapModelReference; } break;
  case EResourceType::Material: { ptrRIHashMap = &this->mMapMaterialReference; } break;
  case EResourceType::GLAttachment:   { ptrRIHashMap = &this->mMapAttachmentReference; } break;
  case EResourceType::GLFrameBuffer:  { ptrRIHashMap = &this->mMapFrameBufferReference; } break;
  case EResourceType::Skeleton:       { ptrRIHashMap = &this->mMapModelSkeletonReference; } break;
  case EResourceType::AnimationScrap: { ptrRIHashMap = &this->mMapModelAnimScrapReference; } break;
  case EResourceType::Sound:          { ptrRIHashMap = &this->mMapSoundReference; } break;
  default: MDY_UNEXPECTED_BRANCH_BUT_RETURN(DY_FAILURE);
  }

  auto [it, isSuccessful] = ptrRIHashMap->try_emplace(specifier, specifier, style, type, scope);
  MDY_ASSERT_MSG(isSuccessful == true, "RI Container creation must be successful.");
  return DY_SUCCESS;
}

EDySuccess DDyIOReferenceContainer::MoveReferenceInstance(_MINOUT_ DDyIOReferenceInstance&& iRi)
{
  TStringHashMap<DDyIOReferenceInstance>* ptrRIHashMap = nullptr;
  switch (iRi.mResourceType)
  {
  case EResourceType::GLShader: { ptrRIHashMap = &this->mMapGLShaderReference; } break;
  case EResourceType::Texture:  { ptrRIHashMap = &this->mMapTextureReference; } break;
  case EResourceType::Mesh:     { ptrRIHashMap = &this->mMapMeshReference; } break;
  case EResourceType::Model:    { ptrRIHashMap = &this->mMapModelReference; } break;
  case EResourceType::Material: { ptrRIHashMap = &this->mMapMaterialReference; } break;
  case EResourceType::GLAttachment:   { ptrRIHashMap = &this->mMapAttachmentReference; } break;
  case EResourceType::GLFrameBuffer:  { ptrRIHashMap = &this->mMapFrameBufferReference; } break;
  case EResourceType::Skeleton:       { ptrRIHashMap = &this->mMapModelSkeletonReference; } break;
  case EResourceType::AnimationScrap: { ptrRIHashMap = &this->mMapModelAnimScrapReference; } break;
  case EResourceType::Sound:          { ptrRIHashMap = &this->mMapSoundReference; } break;
  default: MDY_UNEXPECTED_BRANCH_BUT_RETURN(DY_FAILURE);
  }

  auto [it, isSuccessful] = ptrRIHashMap->try_emplace(iRi.mSpecifierName, std::move(iRi));
  MDY_ASSERT_MSG(isSuccessful == true, "RI Container creation must be successful.");
  return DY_SUCCESS;
}

EDySuccess DDyIOReferenceContainer::TryUpdateValidity(
    _MIN_ EResourceType type, 
    _MIN_ const std::string& specifier, 
    _MIN_ bool isValid,
    _MIN_ void* iPtrInstance)
{
  // Get pointer of hash-map.
  TStringHashMap<DDyIOReferenceInstance>* map;
  switch (type)
  {
  case EResourceType::Mesh:     map = &this->mMapMeshReference;     break;
  case EResourceType::Model:    map = &this->mMapModelReference;    break;
  case EResourceType::GLShader: map = &this->mMapGLShaderReference; break;
  case EResourceType::Texture:  map = &this->mMapTextureReference;  break;
  case EResourceType::Material: map = &this->mMapMaterialReference; break;
  case EResourceType::GLAttachment:   map = &this->mMapAttachmentReference;       break;
  case EResourceType::GLFrameBuffer:  map = &this->mMapFrameBufferReference;      break;
  case EResourceType::Skeleton:       map = &this->mMapModelSkeletonReference;    break;
  case EResourceType::AnimationScrap: map = &this->mMapModelAnimScrapReference;   break;
  case EResourceType::Sound:          map = &this->mMapSoundReference;   break;
  default: MDY_UNEXPECTED_BRANCH_BUT_RETURN(DY_FAILURE);
  }

  // Get `not-valid` Reference Instance instance, try to update validity to true to use this from outside world.
  auto& instance = (*map)[specifier];
  if (isValid != instance.mIsResourceValid) 
  { 
    if (isValid == true)
    {
      instance.SetValid(iPtrInstance);
      // If resource is valid so must forward instance pointer to binder...
      for (const auto& ptrBinderBase : instance.mPtrBoundBinderList)
      {
        if (MDY_CHECK_ISNULL(ptrBinderBase)) { continue; }
        const_cast<__IBinderBase*>(ptrBinderBase)->TryUpdateResourcePtr(instance.mPtrInstance);
      }
    }
    else
    {
      instance.SetNotValid();
      // If resource is not valid, so must detach instance pointer from binders...
      for (const auto& ptrBinderBase : instance.mPtrBoundBinderList)
      {
        if (MDY_CHECK_ISNULL(ptrBinderBase)) { continue; }
        const_cast<__IBinderBase*>(ptrBinderBase)->TryDetachResourcePtr();
      }
    }
    return DY_SUCCESS; 
  }
  else { return DY_FAILURE; }
}

} /// ::dy namespace