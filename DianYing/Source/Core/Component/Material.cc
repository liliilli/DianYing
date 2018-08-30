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
#include <Dy/Core/Component/Material.h>
#include <Dy/Core/Component/Shader.h>
#include <Dy/Core/Component/Texture.h>

namespace dy
{

EDySuccess CDyMaterialComponent::pInitializeMaterial(const PDyMaterialConstructionDescriptor& materialConstructionDescriptor)
{
  this->mBlendMode = materialConstructionDescriptor.mBlendMode;

  if (!materialConstructionDescriptor.mShaderComponentPtr)
  {
    return DY_FAILURE;
  }
  this->mShaderResourcePtr = materialConstructionDescriptor.mShaderComponentPtr;

  if (materialConstructionDescriptor.mTextureComponents.size() > 16)
  {
    return DY_FAILURE;
  }
  this->mTextureResourcePtrs = materialConstructionDescriptor.mTextureComponents;

  return DY_SUCCESS;
}

void CDyMaterialComponent::TemporalRender()
{
  mShaderResourcePtr->UseShader();
  mShaderResourcePtr->BindShader();

  const auto textureResourceListSize = static_cast<int32_t>(this->mTextureResourcePtrs.size());
  for (int32_t i = 0; i < textureResourceListSize; ++i)
  {
    const auto* textureResource = this->mTextureResourcePtrs[i];

    glActiveTexture(GL_TEXTURE0 + i);
    glBindTexture(textureResource->GetTextureType(), textureResource->GetTextureId());
  }

  glDrawArrays(GL_TRIANGLES, 0, 3);

  mShaderResourcePtr->UnbindShader();
}

} /// ::dy namespace
