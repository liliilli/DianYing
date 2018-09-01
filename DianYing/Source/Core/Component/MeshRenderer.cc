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
#include <Dy/Core/Component/MeshRenderer.h>

#include <Dy/Core/Component/Internal/EtcType.h>
#include <Dy/Core/Component/Resource/ModelResource.h>

#include <Dy/Management/HeapResourceManager.h>

namespace dy
{

EDySuccess CDyMeshRenderer::pfInitialize(const PDyRendererConsturctionDescriptor& desc)
{
  auto& resourceManager = dy::MDyResource::GetInstance();

  // Bind model. If not exists, make model resource using information but not have it, return fail.
  auto modelResourcePtr = resourceManager.GetModelResource(desc.mModelName);
  if (!modelResourcePtr)
  {
    const auto res = resourceManager.CreateModelResource(desc.mModelName);
    if (res == DY_FAILURE)
    {
      return DY_FAILURE;
    }
    modelResourcePtr = resourceManager.GetModelResource(desc.mModelName);
  }
  this->mModelReferencePtr = modelResourcePtr;

  // Bind material. If not exists, make material resource using information, but return fail.
  for (const auto& materialName : desc.mMaterialNames)
  {
    auto materialResourcePtr = resourceManager.GetMaterialResource(materialName);
    if (!materialResourcePtr)
    {
      const auto res = resourceManager.CreateMaterialResource(materialName);
      if (res == DY_FAILURE)
      {
        return DY_FAILURE;
      }
      materialResourcePtr = resourceManager.GetMaterialResource(materialName);
    }
    this->mMaterialResourcePtr.emplace_back(materialResourcePtr);
  }

  return DY_SUCCESS;
}

void CDyMeshRenderer::Render()
{
  auto* reference = this->mModelReferencePtr;
  int32_t tempI = 0;
  for (const auto& submeshResource : reference->GetSubmeshResources())
  {
    // Activate shader of one material.
    const auto shaderResource = this->mMaterialResourcePtr[tempI]->GetShaderResource();
    shaderResource->UseShader();

    // Bind submesh VAO id.
    glBindVertexArray(submeshResource->GetVertexArrayId());

    // Bind textures of one material.
    {
      const auto& textureResources        = this->mMaterialResourcePtr[tempI]->GetTextureResources();
      const auto  textureResourceListSize = static_cast<int32_t>(textureResources.size());
      for (int32_t i = 0; i < textureResourceListSize; ++i)
      {
        const auto texturePointer = textureResources[i].mTexturePointer;
        glActiveTexture(GL_TEXTURE0 + i);
        switch (texturePointer->GetTextureType())
        {
        case EDyTextureStyleType::D1:
          glBindTexture(GL_TEXTURE_1D, texturePointer->GetTextureId());
          break;
        case EDyTextureStyleType::D2:
          glBindTexture(GL_TEXTURE_2D, texturePointer->GetTextureId());
          break;
        default: assert(false); break;
        }
      }
    }

    // Call function call drawing array or element. (not support instancing yet)
    if (submeshResource->IsEnabledIndices())
    {
      glDrawElements(GL_TRIANGLES, submeshResource->GetIndicesCounts(), GL_UNSIGNED_INT, nullptr);
    }
    else
    {
      glDrawArrays(GL_TRIANGLES, 0, submeshResource->GetVertexCounts());
    }

    // Unbind, unset, deactivate settings for this submesh and material.
    shaderResource->UnbindShader();
    shaderResource->UnuseShader();
  }
}

} /// ::dy namespace