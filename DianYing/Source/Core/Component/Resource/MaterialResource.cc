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
#include <Dy/Core/Component/Resource/MaterialResource.h>

#include <Dy/Core/Component/Resource/ShaderResource.h>

#include <Dy/Management/DataInformationManager.h>
#include <Dy/Management/HeapResourceManager.h>

namespace dy
{

CDyMaterialResource::~CDyMaterialResource()
{
  // Unbind previous and next level.
  if (__mPrevLevelPtr)
  {
    __mPrevLevelPtr->__pfSetNextLevel(nullptr);
  }

  // Unbind with all valid shader and texture resources.
  if (this->mShaderResource.mShaderPointer)
  {
    this->mShaderResource.mShaderPointer->__pfSetMaterialReset(this);
  }
  for (auto& pairTexture : this->mTextureResources)
  {
    if (!pairTexture.mTexturePointer) continue;
    pairTexture.mTexturePointer->__pfSetMaterialReset(this);
  }
}

CDyShaderResource* CDyMaterialResource::GetShaderResource() noexcept
{
  return this->mShaderResource.mShaderPointer;
}

EDySuccess CDyMaterialResource::pInitializeMaterial(const PDyMaterialResourceDescriptor& materialInformation)
{
  // Forward descriptor information to member.
  this->mBlendMode        = materialInformation.mBlendMode;
  this->mMaterialName     = materialInformation.mMaterialName;
  this->mShaderResource   = materialInformation.mShaderTuple;
  this->mTextureResources = materialInformation.mTextureTuples;

  // Bind this to each valid resource.
  if (this->mShaderResource.mShaderPointer)
  {
    this->mShaderResource.mShaderPointer->__pfSetMaterialBind(this);
  }
  for (auto& pairTexture : this->mTextureResources)
  {
    pairTexture.mTexturePointer->__pfSetMaterialBind(this);
  }

  return DY_SUCCESS;
}

void CDyMaterialResource::__pfResetTexturePtr(CDyTextureResource* ptr) noexcept
{
  for (auto& [textureName, texturePtr] : this->mTextureResources)
  {
    if (texturePtr == ptr)
    {
      textureName = "DY_RELEASED";
      texturePtr  = nullptr;
    }
  }
}

} /// ::dy namespace