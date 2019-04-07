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
#include <Dy/Core/Resource/Information/FInformationSound.h>

namespace dy
{

EDySuccess MIORescInfo::pfInitialize()
{
  return EDySuccess::DY_SUCCESS;
}

EDySuccess MIORescInfo::pfRelease()
{
  return EDySuccess::DY_SUCCESS;
}

void MIORescInfo::InsertResult(EResourceType type, void* ptrrawInstance)
{
  switch (type)
  {
  case EResourceType::GLShader:
  {
    auto ptr = std::unique_ptr<FInformationShader>(static_cast<FInformationShader*>(ptrrawInstance));
    this->mShaderContainer.MoveInsert(ptr->GetSpecifierName(), std::move(ptr));
  } break;
  case EResourceType::Texture:
  {
    auto ptr = std::unique_ptr<FInformationTexture>(static_cast<FInformationTexture*>(ptrrawInstance));
    this->mTextureContainer.MoveInsert(ptr->GetSpecifierName(), std::move(ptr));
  } break;
  case EResourceType::Mesh:
  {
    auto ptr = std::unique_ptr<FInformationMesh>(static_cast<FInformationMesh*>(ptrrawInstance));
    this->mMeshContainer.MoveInsert(ptr->GetSpecifierName(), std::move(ptr));
  } break;
  case EResourceType::Model:
  {
    auto ptr = std::unique_ptr<FInformationModel>(static_cast<FInformationModel*>(ptrrawInstance));
    this->mModelContainer.MoveInsert(ptr->GetSpecifierName(), std::move(ptr));
  } break;
  case EResourceType::Skeleton:
  {
    auto ptr = std::unique_ptr<FInformationModelSkeleton>(static_cast<FInformationModelSkeleton*>(ptrrawInstance));
    this->mModelSkeletonContainer.MoveInsert(ptr->GetSpecifierName(), std::move(ptr));
  } break;
  case EResourceType::AnimationScrap:
  {
    auto ptr = std::unique_ptr<FInformationModelAnimScrap>(static_cast<FInformationModelAnimScrap*>(ptrrawInstance));
    this->mModelAnimScrapContainer.MoveInsert(ptr->GetSpecifierName(), std::move(ptr));
  } break;
  case EResourceType::Material:
  {
    auto ptr = std::unique_ptr<FInformationMaterial>(static_cast<FInformationMaterial*>(ptrrawInstance));
    this->mMaterialContainer.MoveInsert(ptr->GetSpecifierName(), std::move(ptr));
  } break;
  case EResourceType::GLAttachment:
  {
    auto ptr = std::unique_ptr<FInformationAttachment>(static_cast<FInformationAttachment*>(ptrrawInstance));
    this->mAttachmentContainer.MoveInsert(ptr->GetSpecifierName(), std::move(ptr));
  } break;
  case EResourceType::GLFrameBuffer:
  {
    auto ptr = std::unique_ptr<FInformationFrameBuffer>(static_cast<FInformationFrameBuffer*>(ptrrawInstance));
    this->mFrameBufferContainer.MoveInsert(ptr->GetSpecifierName(), std::move(ptr));
  } break;
  case EResourceType::Sound:
  {
    auto ptr = std::unique_ptr<FInformationSound>(static_cast<FInformationSound*>(ptrrawInstance));
    this->mSoundContainer.MoveInsert(ptr->GetSpecifierName(), std::move(ptr));
  } break;
  default: MDY_UNEXPECTED_BRANCH(); break;
  }
}

EDySuccess MIORescInfo::MDY_PRIVATE(TryRemove)(const std::string& iSpecifier, EResourceType iType)
{
  switch (iType)
  {
  case EResourceType::GLShader:     { return this->mShaderContainer.Remove(iSpecifier); } 
  case EResourceType::Texture:      { return this->mTextureContainer.Remove(iSpecifier); }
  case EResourceType::Mesh:         { return this->mMeshContainer.Remove(iSpecifier); } 
  case EResourceType::Model:        { return this->mModelContainer.Remove(iSpecifier); } 
  case EResourceType::Material:     { return this->mMaterialContainer.Remove(iSpecifier); } 
  case EResourceType::AnimationScrap: { return this->mModelAnimScrapContainer.Remove(iSpecifier); }
  case EResourceType::Skeleton:     { return this->mModelSkeletonContainer.Remove(iSpecifier); }
  case EResourceType::Sound:        { return this->mSoundContainer.Remove(iSpecifier); }
  case EResourceType::GLAttachment: { return this->mAttachmentContainer.Remove(iSpecifier); } 
  case EResourceType::GLFrameBuffer:{ return this->mFrameBufferContainer.Remove(iSpecifier); } 
  default: MDY_UNEXPECTED_BRANCH_BUT_RETURN(EDySuccess::DY_FAILURE);
  }
}

} /// ::dy namespace