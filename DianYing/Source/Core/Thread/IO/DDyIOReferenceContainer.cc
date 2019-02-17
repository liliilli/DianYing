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
#include <Dy/Core/Resource/Type/FDyBinderBase.h>
#include <Dy/Core/Thread/SDyIOConnectionHelper.h>
#include <Dy/Helper/ContainerHelper.h>
#include <Dy/Management/LoggingManager.h>

namespace dy
{

bool DDyIOReferenceContainer::IsReferenceInstanceExist(_MIN_ const std::string& specifier, _MIN_ EDyResourceType type) const noexcept
{
  switch (type)
  {
  case EDyResourceType::GLShader: return DyIsMapContains(this->mMapGLShaderReference, specifier);
  case EDyResourceType::Texture:  return DyIsMapContains(this->mMapTextureReference, specifier);
  case EDyResourceType::Mesh:     return DyIsMapContains(this->mMapMeshReference, specifier);
  case EDyResourceType::Model:    return DyIsMapContains(this->mMapModelReference, specifier);
  case EDyResourceType::Skeleton:       return DyIsMapContains(this->mMapModelSkeletonReference, specifier);
  case EDyResourceType::AnimationScrap: return DyIsMapContains(this->mMapModelAnimScrapReference, specifier);
  case EDyResourceType::Material:       return DyIsMapContains(this->mMapMaterialReference, specifier);
  case EDyResourceType::GLAttachment:   return DyIsMapContains(this->mMapAttachmentReference, specifier);
  case EDyResourceType::GLFrameBuffer:  return DyIsMapContains(this->mMapFrameBufferReference, specifier);
  case EDyResourceType::Sound:          return DyIsMapContains(this->mMapSoundReference, specifier);
  default: MDY_UNEXPECTED_BRANCH_BUT_RETURN(false);
  }
}

bool DDyIOReferenceContainer::IsReferenceInstanceBound(_MIN_ const std::string& specifier, _MIN_ EDyResourceType type) const noexcept
{
  switch (type)
  {
  case EDyResourceType::GLShader: return this->mMapGLShaderReference.at(specifier).mIsResourceValid;
  case EDyResourceType::Texture:  return this->mMapTextureReference.at(specifier).mIsResourceValid;
  case EDyResourceType::Mesh:     return this->mMapMeshReference.at(specifier).mIsResourceValid;
  case EDyResourceType::Model:    return this->mMapModelReference.at(specifier).mIsResourceValid;
  case EDyResourceType::Skeleton:       return this->mMapModelSkeletonReference.at(specifier).mIsResourceValid;
  case EDyResourceType::AnimationScrap: return this->mMapModelAnimScrapReference.at(specifier).mIsResourceValid;
  case EDyResourceType::Material:       return this->mMapMaterialReference.at(specifier).mIsResourceValid;
  case EDyResourceType::GLAttachment:   return this->mMapAttachmentReference.at(specifier).mIsResourceValid;
  case EDyResourceType::GLFrameBuffer:  return this->mMapFrameBufferReference.at(specifier).mIsResourceValid;
  case EDyResourceType::Sound:          return this->mMapSoundReference.at(specifier).mIsResourceValid;
  default: MDY_UNEXPECTED_BRANCH_BUT_RETURN(false);
  }
}

bool DDyIOReferenceContainer::TryEnlargeResourceScope(_MIN_ EDyScope scope, _MIN_ const std::string& specifier, _MIN_ EDyResourceType type)
{
  // Find given resource type instance. Given specifier and type must be valid on list.
  DDyIOReferenceInstance* instance;
  switch (type)
  {
  case EDyResourceType::GLShader: { instance = &this->mMapGLShaderReference[specifier]; } break;
  case EDyResourceType::Texture:  { instance = &this->mMapTextureReference[specifier]; }  break;
  case EDyResourceType::Mesh:     { instance = &this->mMapMeshReference[specifier]; } break;
  case EDyResourceType::Model:    { instance = &this->mMapModelReference[specifier]; }    break;
  case EDyResourceType::Material: { instance = &this->mMapMaterialReference[specifier]; } break;
  case EDyResourceType::GLAttachment:   { instance = &this->mMapAttachmentReference[specifier]; } break;
  case EDyResourceType::GLFrameBuffer:  { instance = &this->mMapFrameBufferReference[specifier]; } break;
  case EDyResourceType::Skeleton:       { instance = &this->mMapModelSkeletonReference[specifier]; } break;
  case EDyResourceType::AnimationScrap: { instance = &this->mMapModelAnimScrapReference[specifier]; } break;
  case EDyResourceType::Sound:          { instance = &this->mMapSoundReference[specifier]; } break; 
  default: MDY_UNEXPECTED_BRANCH_BUT_RETURN(false);
  }

  // Compare scope and if previous scope is not larger than given input scope, update it.
  if (scope > instance->mScope) { instance->mScope = scope; }
  return true;
}

EDySuccess DDyIOReferenceContainer::TryBindBinderToResourceRI(
    _MIN_ const std::string& iSpecifier,
    _MIN_ EDyResourceType iType,
    _MIN_ const __FDyBinderBase* iPtrBinder)
{
  // Check RI is exist, if not found just return failure.
  if (this->IsReferenceInstanceExist(iSpecifier, iType) == false) { return DY_FAILURE; }

  // Attach binder to RI container.
  switch (iType)
  {
  case EDyResourceType::Model:    this->mMapModelReference[iSpecifier].AttachBinder(iPtrBinder);    break;
  case EDyResourceType::GLShader: this->mMapGLShaderReference[iSpecifier].AttachBinder(iPtrBinder); break;
  case EDyResourceType::Texture:  this->mMapTextureReference[iSpecifier].AttachBinder(iPtrBinder);  break;
  case EDyResourceType::Mesh:     this->mMapMeshReference[iSpecifier].AttachBinder(iPtrBinder);     break;
  case EDyResourceType::Material: this->mMapMaterialReference[iSpecifier].AttachBinder(iPtrBinder); break;
  case EDyResourceType::GLAttachment:   this->mMapAttachmentReference[iSpecifier].AttachBinder(iPtrBinder);       break;
  case EDyResourceType::GLFrameBuffer:  this->mMapFrameBufferReference[iSpecifier].AttachBinder(iPtrBinder);      break;
  case EDyResourceType::Skeleton:       this->mMapModelSkeletonReference[iSpecifier].AttachBinder(iPtrBinder);    break;
  case EDyResourceType::AnimationScrap: this->mMapModelAnimScrapReference[iSpecifier].AttachBinder(iPtrBinder);   break;
  case EDyResourceType::Sound:          this->mMapSoundReference[iSpecifier].AttachBinder(iPtrBinder);   break;
  default: MDY_UNEXPECTED_BRANCH_BUT_RETURN(DY_FAILURE);
  }
  return DY_SUCCESS;
}

EDySuccess DDyIOReferenceContainer::TryDetachBinderFromResourceRI(
    _MIN_ const std::string& iSpecifier, 
    _MIN_ EDyResourceType iType, 
    _MIN_ const __FDyBinderBase* iPtrBinder)
{
  // Check RI is exist, if not found just return failure.
  if (this->IsReferenceInstanceExist(iSpecifier, iType) == false) { return DY_FAILURE; }

  // Get Reference instance map from `iType`.
  decltype(mMapModelReference)* ptrRiMap;
  switch (iType)
  {
  case EDyResourceType::Model:    ptrRiMap = &this->mMapModelReference;     break;
  case EDyResourceType::GLShader: ptrRiMap = &this->mMapGLShaderReference;  break;
  case EDyResourceType::Texture:  ptrRiMap = &this->mMapTextureReference;   break;
  case EDyResourceType::Mesh:     ptrRiMap = &this->mMapMeshReference;      break;
  case EDyResourceType::Material: ptrRiMap = &this->mMapMaterialReference;  break;
  case EDyResourceType::GLAttachment:   ptrRiMap = &this->mMapAttachmentReference;  break;
  case EDyResourceType::GLFrameBuffer:  ptrRiMap = &this->mMapFrameBufferReference; break;
  case EDyResourceType::Skeleton:       ptrRiMap = &this->mMapModelSkeletonReference; break;
  case EDyResourceType::AnimationScrap: ptrRiMap = &this->mMapModelAnimScrapReference; break;
  case EDyResourceType::Sound:          ptrRiMap = &this->mMapSoundReference; break;
  default: MDY_UNEXPECTED_BRANCH_BUT_RETURN(DY_FAILURE);
  }

  // Detach binder from given instance.
  (*ptrRiMap)[iSpecifier].DetachBinder(iPtrBinder);

  // If target RI need to be GCed (so should be moved into GCList), move it.
  if ((*ptrRiMap)[iSpecifier].IsNeedToBeGced() == true)
  {
    MDY_LOG_DEBUG_D("Moved Reference Instance to GClist. {}", iSpecifier);
    SDyIOConnectionHelper::InsertGcCandidate((*ptrRiMap)[iSpecifier]);
    ptrRiMap->erase(iSpecifier);
  }

  return DY_SUCCESS;
}

std::vector<DDyIOReferenceInstance> 
DDyIOReferenceContainer::GetForwardCandidateRIAsList(_MIN_ EDyScope iScope)
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
    _MIN_ EDyScope iScope,
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
    _MIN_ EDyResourceType type, _MIN_ EDyResourceStyle style, _MIN_ EDyScope scope)
{
  TStringHashMap<DDyIOReferenceInstance>* ptrRIHashMap;

  switch (type)
  {
  case EDyResourceType::GLShader: { ptrRIHashMap = &this->mMapGLShaderReference; } break;
  case EDyResourceType::Texture:  { ptrRIHashMap = &this->mMapTextureReference; } break;
  case EDyResourceType::Mesh:     { ptrRIHashMap = &this->mMapMeshReference; } break;
  case EDyResourceType::Model:    { ptrRIHashMap = &this->mMapModelReference; } break;
  case EDyResourceType::Material: { ptrRIHashMap = &this->mMapMaterialReference; } break;
  case EDyResourceType::GLAttachment:   { ptrRIHashMap = &this->mMapAttachmentReference; } break;
  case EDyResourceType::GLFrameBuffer:  { ptrRIHashMap = &this->mMapFrameBufferReference; } break;
  case EDyResourceType::Skeleton:       { ptrRIHashMap = &this->mMapModelSkeletonReference; } break;
  case EDyResourceType::AnimationScrap: { ptrRIHashMap = &this->mMapModelAnimScrapReference; } break;
  case EDyResourceType::Sound:          { ptrRIHashMap = &this->mMapSoundReference; } break;
  default: MDY_UNEXPECTED_BRANCH_BUT_RETURN(DY_FAILURE);
  }

  auto [it, isSuccessful] = ptrRIHashMap->try_emplace(specifier, specifier, style, type, scope);
  MDY_ASSERT(isSuccessful == true, "RI Container creation must be successful.");
  return DY_SUCCESS;
}

EDySuccess DDyIOReferenceContainer::MoveReferenceInstance(_MINOUT_ DDyIOReferenceInstance&& iRi)
{
  TStringHashMap<DDyIOReferenceInstance>* ptrRIHashMap = nullptr;
  switch (iRi.mResourceType)
  {
  case EDyResourceType::GLShader: { ptrRIHashMap = &this->mMapGLShaderReference; } break;
  case EDyResourceType::Texture:  { ptrRIHashMap = &this->mMapTextureReference; } break;
  case EDyResourceType::Mesh:     { ptrRIHashMap = &this->mMapMeshReference; } break;
  case EDyResourceType::Model:    { ptrRIHashMap = &this->mMapModelReference; } break;
  case EDyResourceType::Material: { ptrRIHashMap = &this->mMapMaterialReference; } break;
  case EDyResourceType::GLAttachment:   { ptrRIHashMap = &this->mMapAttachmentReference; } break;
  case EDyResourceType::GLFrameBuffer:  { ptrRIHashMap = &this->mMapFrameBufferReference; } break;
  case EDyResourceType::Skeleton:       { ptrRIHashMap = &this->mMapModelSkeletonReference; } break;
  case EDyResourceType::AnimationScrap: { ptrRIHashMap = &this->mMapModelAnimScrapReference; } break;
  case EDyResourceType::Sound:          { ptrRIHashMap = &this->mMapSoundReference; } break;
  default: MDY_UNEXPECTED_BRANCH_BUT_RETURN(DY_FAILURE);
  }

  auto [it, isSuccessful] = ptrRIHashMap->try_emplace(iRi.mSpecifierName, std::move(iRi));
  MDY_ASSERT(isSuccessful == true, "RI Container creation must be successful.");
  return DY_SUCCESS;
}

EDySuccess DDyIOReferenceContainer::TryUpdateValidity(
    _MIN_ EDyResourceType type, 
    _MIN_ const std::string& specifier, 
    _MIN_ bool isValid,
    _MIN_ void* iPtrInstance)
{
  // Get pointer of hash-map.
  TStringHashMap<DDyIOReferenceInstance>* map;
  switch (type)
  {
  case EDyResourceType::Mesh:     map = &this->mMapMeshReference;     break;
  case EDyResourceType::Model:    map = &this->mMapModelReference;    break;
  case EDyResourceType::GLShader: map = &this->mMapGLShaderReference; break;
  case EDyResourceType::Texture:  map = &this->mMapTextureReference;  break;
  case EDyResourceType::Material: map = &this->mMapMaterialReference; break;
  case EDyResourceType::GLAttachment:   map = &this->mMapAttachmentReference;       break;
  case EDyResourceType::GLFrameBuffer:  map = &this->mMapFrameBufferReference;      break;
  case EDyResourceType::Skeleton:       map = &this->mMapModelSkeletonReference;    break;
  case EDyResourceType::AnimationScrap: map = &this->mMapModelAnimScrapReference;   break;
  case EDyResourceType::Sound:          map = &this->mMapSoundReference;   break;
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
        const_cast<__FDyBinderBase*>(ptrBinderBase)->TryUpdateResourcePtr(instance.mPtrInstance);
      }
    }
    else
    {
      instance.SetNotValid();
      // If resource is not valid, so must detach instance pointer from binders...
      for (const auto& ptrBinderBase : instance.mPtrBoundBinderList)
      {
        if (MDY_CHECK_ISNULL(ptrBinderBase)) { continue; }
        const_cast<__FDyBinderBase*>(ptrBinderBase)->TryDetachResourcePtr();
      }
    }
    return DY_SUCCESS; 
  }
  else { return DY_FAILURE; }
}

} /// ::dy namespace