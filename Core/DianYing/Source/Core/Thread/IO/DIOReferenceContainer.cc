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
#include <Dy/Core/Thread/IO/DIOReferenceContainer.h>
#include <Dy/Core/Resource/Type/IBinderBase.h>
#include <Dy/Core/Thread/SIOConnectionHelper.h>
#include <Dy/Helper/Library/HelperContainer.h>
#include <Dy/Management/MLog.h>

namespace dy
{

bool DIOReferenceContainer::IsReferenceInstanceExist(
  const std::string& specifier, EResourceType type) const noexcept
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

bool DIOReferenceContainer::IsReferenceInstanceBound(
  const std::string& specifier, EResourceType type) const noexcept
{
  switch (type)
  {
  case EResourceType::GLShader: return this->mMapGLShaderReference.at(specifier)->mIsResourceValid;
  case EResourceType::Texture:  return this->mMapTextureReference.at(specifier)->mIsResourceValid;
  case EResourceType::Mesh:     return this->mMapMeshReference.at(specifier)->mIsResourceValid;
  case EResourceType::Model:    return this->mMapModelReference.at(specifier)->mIsResourceValid;
  case EResourceType::Skeleton:       return this->mMapModelSkeletonReference.at(specifier)->mIsResourceValid;
  case EResourceType::AnimationScrap: return this->mMapModelAnimScrapReference.at(specifier)->mIsResourceValid;
  case EResourceType::Material:       return this->mMapMaterialReference.at(specifier)->mIsResourceValid;
  case EResourceType::GLAttachment:   return this->mMapAttachmentReference.at(specifier)->mIsResourceValid;
  case EResourceType::GLFrameBuffer:  return this->mMapFrameBufferReference.at(specifier)->mIsResourceValid;
  case EResourceType::Sound:          return this->mMapSoundReference.at(specifier)->mIsResourceValid;
  default: MDY_UNEXPECTED_BRANCH_BUT_RETURN(false);
  }
}

bool DIOReferenceContainer::TryEnlargeResourceScope(
  EResourceScope scope, const std::string& specifier, EResourceType type)
{
  // Find given resource type instance. Given specifier and type must be valid on list.
  std::unique_ptr<DIOReferenceInstance>* ptrInstance = nullptr;
  switch (type)
  {
  case EResourceType::GLShader: { ptrInstance = &this->mMapGLShaderReference[specifier]; } break;
  case EResourceType::Texture:  { ptrInstance = &this->mMapTextureReference[specifier]; }  break;
  case EResourceType::Mesh:     { ptrInstance = &this->mMapMeshReference[specifier]; } break;
  case EResourceType::Model:    { ptrInstance = &this->mMapModelReference[specifier]; }    break;
  case EResourceType::Material: { ptrInstance = &this->mMapMaterialReference[specifier]; } break;
  case EResourceType::GLAttachment:   { ptrInstance = &this->mMapAttachmentReference[specifier]; } break;
  case EResourceType::GLFrameBuffer:  { ptrInstance = &this->mMapFrameBufferReference[specifier]; } break;
  case EResourceType::Skeleton:       { ptrInstance = &this->mMapModelSkeletonReference[specifier]; } break;
  case EResourceType::AnimationScrap: { ptrInstance = &this->mMapModelAnimScrapReference[specifier]; } break;
  case EResourceType::Sound:          { ptrInstance = &this->mMapSoundReference[specifier]; } break; 
  default: MDY_UNEXPECTED_BRANCH(); throw;
  }

  // Compare scope and if previous scope is not larger than given input scope, update it.
  if (scope > (*ptrInstance)->mScope) 
  { 
    (*ptrInstance)->mScope = scope; 
  }
  return true;
}

EDySuccess DIOReferenceContainer::TryBindBinderToResourceRI(
  const std::string& iSpecifier,
  EResourceType iType,
  IBinderBase& iPtrBinder)
{
  // Check RI is exist, if not found just return failure.
  if (this->IsReferenceInstanceExist(iSpecifier, iType) == false) 
  { 
    return EDySuccess::DY_FAILURE; 
  }

  // Attach binder to RI container.
  switch (iType)
  {
  case EResourceType::Model:          this->mMapModelReference[iSpecifier]->AttachBinder(iPtrBinder);    break;
  case EResourceType::GLShader:       this->mMapGLShaderReference[iSpecifier]->AttachBinder(iPtrBinder); break;
  case EResourceType::Texture:        this->mMapTextureReference[iSpecifier]->AttachBinder(iPtrBinder);  break;
  case EResourceType::Mesh:           this->mMapMeshReference[iSpecifier]->AttachBinder(iPtrBinder);     break;
  case EResourceType::Material:       this->mMapMaterialReference[iSpecifier]->AttachBinder(iPtrBinder); break;
  case EResourceType::GLAttachment:   this->mMapAttachmentReference[iSpecifier]->AttachBinder(iPtrBinder);       break;
  case EResourceType::GLFrameBuffer:  this->mMapFrameBufferReference[iSpecifier]->AttachBinder(iPtrBinder);      break;
  case EResourceType::Skeleton:       this->mMapModelSkeletonReference[iSpecifier]->AttachBinder(iPtrBinder);    break;
  case EResourceType::AnimationScrap: this->mMapModelAnimScrapReference[iSpecifier]->AttachBinder(iPtrBinder);   break;
  case EResourceType::Sound:          this->mMapSoundReference[iSpecifier]->AttachBinder(iPtrBinder);   break;
  default: MDY_UNEXPECTED_BRANCH(); throw;
  }
  return EDySuccess::DY_SUCCESS;
}

EDySuccess DIOReferenceContainer::TryDetachBinderFromResourceRI(
  const std::string& iSpecifier, 
  EResourceType iType, 
  IBinderBase& iPtrBinder)
{
  // Check RI is exist, if not found just return failure.
  if (this->IsReferenceInstanceExist(iSpecifier, iType) == false) { return EDySuccess::DY_FAILURE; }

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
  default: MDY_UNEXPECTED_BRANCH(); throw;
  }

  // Detach binder from given instance.
  (*ptrRiMap)[iSpecifier]->DetachBinder(iPtrBinder);

  // If target RI need to be GCed (so should be moved into GCList), move it.
  if ((*ptrRiMap)[iSpecifier]->HaveToBeGCed() == true)
  {
    DyPushLogDebugDebug("Moved Reference Instance to GClist. {}", iSpecifier);
    SIOConnectionHelper::InsertGcCandidate(std::move((*ptrRiMap)[iSpecifier]));
    ptrRiMap->erase(iSpecifier);
  }

  return EDySuccess::DY_SUCCESS;
}

std::vector<std::unique_ptr<DIOReferenceInstance>>
DIOReferenceContainer::GetForwardCandidateRIAsList(EResourceScope iScope)
{
  std::vector<std::unique_ptr<DIOReferenceInstance>> result;
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

void DIOReferenceContainer::ForwardCandidateRIFromList(
  EResourceScope iScope,
  TStringHashMap<std::unique_ptr<DIOReferenceInstance>>& ioContainer, 
  std::vector<std::unique_ptr<DIOReferenceInstance>>& oResult)
{
  for (auto it = ioContainer.begin(); it != ioContainer.end();)
  {
    auto& [specifier, instance] = *it;
    if (instance->mScope != iScope) 
    { 
      ++it; continue; 
    }

    if (instance->mIsResourceValid == true && instance->IsBeingBound() == false)
    {
      oResult.emplace_back(std::move(instance));
      it = ioContainer.erase(it);
    }
    else { ++it; }
  }
}

EDySuccess DIOReferenceContainer::CreateReferenceInstance(
  const std::string& specifier,
  EResourceType type, EResourceStyle style, EResourceScope scope)
{
  TRefInstanceMap* ptrRIHashMap;

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
  default: MDY_UNEXPECTED_BRANCH(); throw;
  }

  auto instanceRI = std::make_unique<DIOReferenceInstance>(specifier, style, type, scope);
  auto [it, isSuccessful] = ptrRIHashMap->try_emplace(specifier, std::move(instanceRI));
  MDY_ASSERT_MSG(isSuccessful == true, "RI Container creation must be successful.");

  return EDySuccess::DY_SUCCESS;
}

EDySuccess DIOReferenceContainer::MoveReferenceInstance(std::unique_ptr<DIOReferenceInstance>&& ioRi)
{
  TRefInstanceMap* ptrRIHashMap;
  switch (ioRi->mResourceType)
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
  default: MDY_UNEXPECTED_BRANCH(); throw;
  }

  auto [it, isSuccessful] = ptrRIHashMap->try_emplace(ioRi->mSpecifierName, std::move(ioRi));
  MDY_ASSERT_MSG(isSuccessful == true, "RI Container creation must be successful.");
  return EDySuccess::DY_SUCCESS;
}

EDySuccess DIOReferenceContainer::TryUpdateValidity(
  EResourceType type, const std::string& specifier, bool isValid, void* iPtrInstance)
{
  // Get pointer of hash-map.
  TRefInstanceMap* ptrMap;
  switch (type)
  {
  case EResourceType::Mesh:           ptrMap = &this->mMapMeshReference;     break;
  case EResourceType::Model:          ptrMap = &this->mMapModelReference;    break;
  case EResourceType::GLShader:       ptrMap = &this->mMapGLShaderReference; break;
  case EResourceType::Texture:        ptrMap = &this->mMapTextureReference;  break;
  case EResourceType::Material:       ptrMap = &this->mMapMaterialReference; break;
  case EResourceType::GLAttachment:   ptrMap = &this->mMapAttachmentReference;       break;
  case EResourceType::GLFrameBuffer:  ptrMap = &this->mMapFrameBufferReference;      break;
  case EResourceType::Skeleton:       ptrMap = &this->mMapModelSkeletonReference;    break;
  case EResourceType::AnimationScrap: ptrMap = &this->mMapModelAnimScrapReference;   break;
  case EResourceType::Sound:          ptrMap = &this->mMapSoundReference;   break;
  default: MDY_UNEXPECTED_BRANCH(); throw;
  }

  // Get `not-valid` Reference Instance instance, try to update validity to true to use this from outside world.
  auto& instance = (*ptrMap)[specifier];
  if (isValid != instance->mIsResourceValid) 
  { 
    if (isValid == true) 
    { // If resource is valid so must forward instance pointer to binder...
      instance->SetValid(iPtrInstance); 
    }
    else
    { // If resource is not valid, so must detach instance pointer from binders...
      instance->SetNotValid();
    }
    return EDySuccess::DY_SUCCESS; 
  }

  return EDySuccess::DY_FAILURE; 
}

} /// ::dy namespace