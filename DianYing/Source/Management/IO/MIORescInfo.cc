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
#include <Dy/Management/IO/MIORescInfo.h>
#include <string>
#include "Dy/Core/Resource/Information/FDySoundInformation.h"

namespace dy
{

EDySuccess MIORescInfo::pfInitialize()
{
  return DY_SUCCESS;
}

EDySuccess MIORescInfo::pfRelease()
{
  return DY_SUCCESS;
}

void MIORescInfo::InsertResult(_MIN_ EResourceType type, _MIN_ void* ptrrawInstance)
{
  switch (type)
  {
  case EResourceType::GLShader:
  {
    auto ptr = std::unique_ptr<FDyShaderInformation>(static_cast<FDyShaderInformation*>(ptrrawInstance));
    this->__mShaderContainer.MoveInsert(ptr->GetSpecifierName(), std::move(ptr));
  } break;
  case EResourceType::Texture:
  {
    auto ptr = std::unique_ptr<FDyTextureInformation>(static_cast<FDyTextureInformation*>(ptrrawInstance));
    this->__mTextureContainer.MoveInsert(ptr->GetSpecifierName(), std::move(ptr));
  } break;
  case EResourceType::Mesh:
  {
    auto ptr = std::unique_ptr<FDyMeshInformation>(static_cast<FDyMeshInformation*>(ptrrawInstance));
    this->__mMeshContainer.MoveInsert(ptr->GetSpecifierName(), std::move(ptr));
  } break;
  case EResourceType::Model:
  {
    auto ptr = std::unique_ptr<FDyModelInformation>(static_cast<FDyModelInformation*>(ptrrawInstance));
    this->__mModelContainer.MoveInsert(ptr->GetSpecifierName(), std::move(ptr));
  } break;
  case EResourceType::Skeleton:
  {
    auto ptr = std::unique_ptr<FDyModelSkeletonInformation>(static_cast<FDyModelSkeletonInformation*>(ptrrawInstance));
    this->__mModelSkeletonContainer.MoveInsert(ptr->GetSpecifierName(), std::move(ptr));
  } break;
  case EResourceType::AnimationScrap:
  {
    auto ptr = std::unique_ptr<FDyModelAnimScrapInformation>(static_cast<FDyModelAnimScrapInformation*>(ptrrawInstance));
    this->__mModelAnimScrapContainer.MoveInsert(ptr->GetSpecifierName(), std::move(ptr));
  } break;
  case EResourceType::Material:
  {
    auto ptr = std::unique_ptr<FDyMaterialInformation>(static_cast<FDyMaterialInformation*>(ptrrawInstance));
    this->__mMaterialContainer.MoveInsert(ptr->GetSpecifierName(), std::move(ptr));
  } break;
  case EResourceType::GLAttachment:
  {
    auto ptr = std::unique_ptr<FDyAttachmentInformation>(static_cast<FDyAttachmentInformation*>(ptrrawInstance));
    this->__mAttachmentContainer.MoveInsert(ptr->GetSpecifierName(), std::move(ptr));
  } break;
  case EResourceType::GLFrameBuffer:
  {
    auto ptr = std::unique_ptr<FDyFrameBufferInformation>(static_cast<FDyFrameBufferInformation*>(ptrrawInstance));
    this->__mFrameBufferContainer.MoveInsert(ptr->GetSpecifierName(), std::move(ptr));
  } break;
  case EResourceType::Sound:
  {
    auto ptr = std::unique_ptr<FDySoundInformation>(static_cast<FDySoundInformation*>(ptrrawInstance));
    this->__mSoundContainer.MoveInsert(ptr->GetSpecifierName(), std::move(ptr));
  } break;
  default: MDY_UNEXPECTED_BRANCH(); break;
  }
}

EDySuccess MIORescInfo::MDY_PRIVATE(TryRemove)(_MIN_ const std::string& iSpecifier, _MIN_ EResourceType iType)
{
  switch (iType)
  {
  case EResourceType::GLShader:     { return this->__mShaderContainer.Remove(iSpecifier); } 
  case EResourceType::Texture:      { return this->__mTextureContainer.Remove(iSpecifier); }
  case EResourceType::Mesh:         { return this->__mMeshContainer.Remove(iSpecifier); } 
  case EResourceType::Model:        { return this->__mModelContainer.Remove(iSpecifier); } 
  case EResourceType::Material:     { return this->__mMaterialContainer.Remove(iSpecifier); } 
  case EResourceType::AnimationScrap: { return this->__mModelAnimScrapContainer.Remove(iSpecifier); }
  case EResourceType::Skeleton:     { return this->__mModelSkeletonContainer.Remove(iSpecifier); }
  case EResourceType::Sound:        { return this->__mSoundContainer.Remove(iSpecifier); }
  case EResourceType::GLAttachment: { return this->__mAttachmentContainer.Remove(iSpecifier); } 
  case EResourceType::GLFrameBuffer:{ return this->__mFrameBufferContainer.Remove(iSpecifier); } 
  default: MDY_UNEXPECTED_BRANCH_BUT_RETURN(DY_FAILURE);
  }
}

} /// ::dy namespace