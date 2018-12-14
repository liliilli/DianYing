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
#include <Dy/Management/IO/MDyIOData.h>
#include "Dy/Core/Resource/Information/FDyModelInformation.h"

namespace dy
{

EDySuccess MDyIOData::pfInitialize()
{
  return DY_SUCCESS;
}

EDySuccess MDyIOData::pfRelease()
{
  return DY_SUCCESS;
}

void MDyIOData::InsertResult(_MIN_ EDyResourceType type, _MIN_ void* ptrrawInstance)
{
  switch (type)
  {
  case EDyResourceType::GLShader:
  {
    auto ptr = std::unique_ptr<FDyShaderInformation>(static_cast<FDyShaderInformation*>(ptrrawInstance));
    this->__mShaderContainer.MoveInsert(ptr->GetSpecifierName(), std::move(ptr));
  } break;
  case EDyResourceType::Texture:
  {
    auto ptr = std::unique_ptr<FDyTextureInformation>(static_cast<FDyTextureInformation*>(ptrrawInstance));
    this->__mTextureContainer.MoveInsert(ptr->GetSpecifierName(), std::move(ptr));
  } break;
  case EDyResourceType::Model:
  {
    auto ptr = std::unique_ptr<FDyModelInformation>(static_cast<FDyModelInformation*>(ptrrawInstance));
    this->__mModelContainer.MoveInsert(ptr->GetSpecifierName(), std::move(ptr));
  } break;
  case EDyResourceType::Material:
  {
    auto ptr = std::unique_ptr<FDyMaterialInformation>(static_cast<FDyMaterialInformation*>(ptrrawInstance));
    this->__mMaterialContainer.MoveInsert(ptr->GetSpecifierName(), std::move(ptr));
  } break;
  default: MDY_UNEXPECTED_BRANCH(); break;
  }
}

} /// ::dy namespace