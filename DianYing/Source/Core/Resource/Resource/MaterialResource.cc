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
#include <Dy/Core/Resource/Resource/MaterialResource.h>

#include <Dy/Core/Resource/Resource/ShaderResource.h>
#include <Dy/Management/IO/IODataManager.h>
#include <Dy/Management/IO/IOResourceManager.h>

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
  if (this->mShaderResource.mValidShaderPointer)
  {
    this->mShaderResource.mValidShaderPointer->__pfSetMaterialResourceLink(this);
  }
  for (auto& pairTexture : this->mTextureResources)
  {
    pairTexture.mValidTexturePointer->__pfSetMaterialResourceLink(DyMakeNotNull(this));
  }

  return DY_SUCCESS;
}

CDyMaterialResource::~CDyMaterialResource()
{
  // Unbind previous and next level.
  if (this->__mLinkedMaterialInformationPtr)
  {
    this->__mLinkedMaterialInformationPtr->__pfResetMaterialResourceLink();
  }

  // Unbind with all valid shader and texture resources.
  if (this->mShaderResource.mValidShaderPointer)
  {
    this->mShaderResource.mValidShaderPointer->__pfResetMaterialResourceLinking(DyMakeNotNull(this));
  }
  for (auto& pairTexture : this->mTextureResources)
  {
    if (!pairTexture.mValidTexturePointer) continue;
    pairTexture.mValidTexturePointer->__pfResetMaterialResourceLink(DyMakeNotNull(this));
  }
}

NotNull<CDyShaderResource*> CDyMaterialResource::GetShaderResource() noexcept
{
  return DyMakeNotNull(this->mShaderResource.mValidShaderPointer);
}

void CDyMaterialResource::__pfResetTextureResourcePtr(NotNull<CDyTextureResource*> ptr) noexcept
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