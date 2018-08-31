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

#include <Dy/Core/Component/Texture.h>
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

void CDyMaterialResource::TemporalRender()
{
  if (!this->mShaderResource.mShaderPointer)
    return;

  this->mShaderResource.mShaderPointer->UseShader();
  this->mShaderResource.mShaderPointer->BindShader();

  const auto textureResourceListSize = static_cast<int32_t>(this->mTextureResources.size());
  for (int32_t i = 0; i < textureResourceListSize; ++i)
  {
    const auto* textureResource = this->mTextureResources[i].mTexturePointer;
    if (!textureResource) continue;

    glActiveTexture(GL_TEXTURE0 + i);
    switch (textureResource->GetTextureType())
    {
    case EDyTextureStyleType::D1:
      glBindTexture(GL_TEXTURE_1D, textureResource->GetTextureId());
      break;
    case EDyTextureStyleType::D2:
      glBindTexture(GL_TEXTURE_2D, textureResource->GetTextureId());
      break;
    default: assert(false); break;
    }
  }

  glDrawArrays(GL_TRIANGLES, 0, 3);
  this->mShaderResource.mShaderPointer->UnbindShader();
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