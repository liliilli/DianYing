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
#include <Dy/Management/IO/MIOResource.h>
#include <Dy/Core/Resource/Resource/FDyAttachmentResource.h>

namespace dy
{

EDySuccess MIOResource::pfInitialize()
{
  return DY_SUCCESS;
}

EDySuccess MIOResource::pfRelease()
{
  return DY_SUCCESS;
}

void MIOResource::InsertResult(_MIN_ EResourceType type, _MIN_ void* ptrrawInstance)
{
  switch (type)
  {
  case EResourceType::GLShader:
  {
    auto ptr = std::unique_ptr<FDyShaderResource>(static_cast<FDyShaderResource*>(ptrrawInstance));
    this->__mShaderContainer.MoveInsert(ptr->GetSpecifierName(), std::move(ptr));
  } break;
  case EResourceType::Texture:
  {
    auto ptr = std::unique_ptr<FDyTextureResource>(static_cast<FDyTextureResource*>(ptrrawInstance));
    this->__mTextureContainer.MoveInsert(ptr->GetSpecifierName(), std::move(ptr));
  } break;
  case EResourceType::Mesh:
  {
    auto ptr = std::unique_ptr<FDyMeshResource>(static_cast<FDyMeshResource*>(ptrrawInstance));
    this->__mMeshContainer.MoveInsert(ptr->GetSpecifierName(), std::move(ptr));
  } break;
  case EResourceType::Model:
  {
    auto ptr = std::unique_ptr<FDyModelResource>(static_cast<FDyModelResource*>(ptrrawInstance));
    this->__mModelContainer.MoveInsert(ptr->GetSpecifierName(), std::move(ptr));
  } break;
  case EResourceType::Material:
  {
    auto ptr = std::unique_ptr<FDyMaterialResource>(static_cast<FDyMaterialResource*>(ptrrawInstance));
    this->__mMaterialContainer.MoveInsert(ptr->GetSpecifierName(), std::move(ptr));
  } break;
  case EResourceType::GLAttachment:
  {
    auto ptr = std::unique_ptr<FDyAttachmentResource>(static_cast<FDyAttachmentResource*>(ptrrawInstance));
    this->__mAttachmentContainer.MoveInsert(ptr->GetSpecifierName(), std::move(ptr));
  } break;
  case EResourceType::GLFrameBuffer:
  {
    auto ptr = std::unique_ptr<FDyFrameBufferResource>(static_cast<FDyFrameBufferResource*>(ptrrawInstance));
    this->__mFrameBufferContainer.MoveInsert(ptr->GetSpecifierName(), std::move(ptr));
  } break;
  default: MDY_UNEXPECTED_BRANCH(); break;
  }
}

EDySuccess MIOResource::MDY_PRIVATE(TryRemove)(_MIN_ const std::string& iSpecifier, _MIN_ EResourceType iType)
{
  switch (iType)
  {
  case EResourceType::GLShader:     { return this->__mShaderContainer.Remove(iSpecifier); } 
  case EResourceType::Texture:      { return this->__mTextureContainer.Remove(iSpecifier); }
  case EResourceType::Mesh:         { return this->__mMeshContainer.Remove(iSpecifier); } 
  case EResourceType::Model:        { return this->__mModelContainer.Remove(iSpecifier); } 
  case EResourceType::Material:     { return this->__mMaterialContainer.Remove(iSpecifier); } 
  case EResourceType::GLAttachment: { return this->__mAttachmentContainer.Remove(iSpecifier); } 
  case EResourceType::GLFrameBuffer:{ return this->__mFrameBufferContainer.Remove(iSpecifier); } 
  default: MDY_UNEXPECTED_BRANCH_BUT_RETURN(DY_FAILURE);
  }
}

} /// ::dy namespace