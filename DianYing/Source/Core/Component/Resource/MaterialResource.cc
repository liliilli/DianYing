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

EDySuccess CDyMaterialResource::pfInitializeMaterialResource(const PDyMaterialResourceDescriptor& materialInformation)
{
  // Forward descriptor information to member.
  this->mBlendMode        = materialInformation.mBlendMode;
  this->mMaterialName     = materialInformation.mMaterialName;
  this->mShaderResource   = materialInformation.mShaderTuple;
  this->mTextureResources = materialInformation.mTextureTuples;

  // Bind this to each valid resource.
  if (this->mShaderResource.mShaderPointer)
  {
    this->mShaderResource.mShaderPointer->__pfLinkMaterialResource(this);
  }
  for (auto& pairTexture : this->mTextureResources)
  {
    pairTexture.mTexturePointer->__pfLinkMaterialResourcePtr(this);
  }

  return DY_SUCCESS;
}

CDyMaterialResource::~CDyMaterialResource()
{
  // Unbind previous and next level.
  if (this->__mLinkedMaterialInformationPtr)
  {
    this->__mLinkedMaterialInformationPtr->__pfSetMaterialResourceLink(nullptr);
  }

  // Unbind with all valid shader and texture resources.
  if (this->mShaderResource.mShaderPointer)
  {
    this->mShaderResource.mShaderPointer->__pfResetMaterialResourceLinking(this);
  }
  for (auto& pairTexture : this->mTextureResources)
  {
    if (!pairTexture.mTexturePointer) continue;
    pairTexture.mTexturePointer->__pfResetMaterialResourcePtr(this);
  }
}

CDyShaderResource* CDyMaterialResource::GetShaderResource() noexcept
{
  return this->mShaderResource.mShaderPointer;
}

void CDyMaterialResource::__pfResetTextureResourcePtr(CDyTextureResource* ptr) noexcept
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