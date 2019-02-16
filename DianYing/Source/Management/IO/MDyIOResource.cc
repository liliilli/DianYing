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
#include <Dy/Management/IO/MDyIOResource.h>
#include <Dy/Core/Resource/Resource/FDyAttachmentResource.h>

namespace dy
{

EDySuccess MDyIOResource::pfInitialize()
{
  return DY_SUCCESS;
}

EDySuccess MDyIOResource::pfRelease()
{
  return DY_SUCCESS;
}

void MDyIOResource::InsertResult(_MIN_ EDyResourceType type, _MIN_ void* ptrrawInstance)
{
  switch (type)
  {
  case EDyResourceType::GLShader:
  {
    auto ptr = std::unique_ptr<FDyShaderResource>(static_cast<FDyShaderResource*>(ptrrawInstance));
    this->__mShaderContainer.MoveInsert(ptr->GetSpecifierName(), std::move(ptr));
  } break;
  case EDyResourceType::Texture:
  {
    auto ptr = std::unique_ptr<FDyTextureResource>(static_cast<FDyTextureResource*>(ptrrawInstance));
    this->__mTextureContainer.MoveInsert(ptr->GetSpecifierName(), std::move(ptr));
  } break;
  case EDyResourceType::Mesh:
  {
    auto ptr = std::unique_ptr<FDyMeshResource>(static_cast<FDyMeshResource*>(ptrrawInstance));
    this->__mMeshContainer.MoveInsert(ptr->GetSpecifierName(), std::move(ptr));
  } break;
  case EDyResourceType::Model:
  {
    auto ptr = std::unique_ptr<FDyModelResource>(static_cast<FDyModelResource*>(ptrrawInstance));
    this->__mModelContainer.MoveInsert(ptr->GetSpecifierName(), std::move(ptr));
  } break;
  case EDyResourceType::Material:
  {
    auto ptr = std::unique_ptr<FDyMaterialResource>(static_cast<FDyMaterialResource*>(ptrrawInstance));
    this->__mMaterialContainer.MoveInsert(ptr->GetSpecifierName(), std::move(ptr));
  } break;
  case EDyResourceType::GLAttachment:
  {
    auto ptr = std::unique_ptr<FDyAttachmentResource>(static_cast<FDyAttachmentResource*>(ptrrawInstance));
    this->__mAttachmentContainer.MoveInsert(ptr->GetSpecifierName(), std::move(ptr));
  } break;
  case EDyResourceType::GLFrameBuffer:
  {
    auto ptr = std::unique_ptr<FDyFrameBufferResource>(static_cast<FDyFrameBufferResource*>(ptrrawInstance));
    this->__mFrameBufferContainer.MoveInsert(ptr->GetSpecifierName(), std::move(ptr));
  } break;
  default: MDY_UNEXPECTED_BRANCH(); break;
  }
}

EDySuccess MDyIOResource::MDY_PRIVATE(TryRemove)(_MIN_ const std::string& iSpecifier, _MIN_ EDyResourceType iType)
{
  switch (iType)
  {
  case EDyResourceType::GLShader:     { return this->__mShaderContainer.Remove(iSpecifier); } 
  case EDyResourceType::Texture:      { return this->__mTextureContainer.Remove(iSpecifier); }
  case EDyResourceType::Mesh:         { return this->__mMeshContainer.Remove(iSpecifier); } 
  case EDyResourceType::Model:        { return this->__mModelContainer.Remove(iSpecifier); } 
  case EDyResourceType::Material:     { return this->__mMaterialContainer.Remove(iSpecifier); } 
  case EDyResourceType::GLAttachment: { return this->__mAttachmentContainer.Remove(iSpecifier); } 
  case EDyResourceType::GLFrameBuffer:{ return this->__mFrameBufferContainer.Remove(iSpecifier); } 
  default: MDY_UNEXPECTED_BRANCH_BUT_RETURN(DY_FAILURE);
  }
}

} /// ::dy namespace