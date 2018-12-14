#include <precompiled.h>
///
/// MIT License
/// Copyright (c) 2018 Jongmin Yun
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
  default: MDY_UNEXPECTED_BRANCH(); break;
  }
}

} /// ::dy namespace