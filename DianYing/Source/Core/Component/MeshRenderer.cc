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
#include <Dy/Management/SceneManager.h>
#include <Dy/Core/Component/Object/Camera.h>
#include "Dy/Management/LoggingManager.h"

namespace dy
{

EDySuccess CDyMeshRenderer::pfInitialize(const PDyRendererConsturctionDescriptor& desc)
{
  auto& resourceManager = MDyResource::GetInstance();

  // Bind model. If not exists, make model resource using information but not have it, return fail.
  const auto modelResourcePtr = resourceManager.GetModelResource(desc.mModelName);
  if (!modelResourcePtr)
  {
    const auto res = resourceManager.CreateModelResource(desc.mModelName);
    if (res == DY_FAILURE) return DY_FAILURE;

    this->mModelReferencePtr = resourceManager.GetModelResource(desc.mModelName);
  }
  else
  {
    this->mModelReferencePtr = modelResourcePtr;
  }

  // Bind material. If not exists, make material resource using information, but return fail.
  for (const auto& materialName : desc.mMaterialNames)
  {
    const auto materialResourcePtr = resourceManager.GetMaterialResource(materialName);
    if (!materialResourcePtr)
    {
      const auto res = resourceManager.CreateMaterialResource(materialName);
      if (res == DY_FAILURE) return DY_FAILURE;

      this->mMaterialResourcePtr.emplace_back(resourceManager.GetMaterialResource(materialName));
    }
    else
    {
      this->mMaterialResourcePtr.emplace_back(materialResourcePtr);
    }
  }

  const auto submeshResourceSize = static_cast<int32_t>(this->mModelReferencePtr->GetSubmeshResources().size());
  const auto materialBindingSize = static_cast<int32_t>(this->mMaterialResourcePtr.size());
  if (submeshResourceSize != materialBindingSize)
  {
    MDY_LOG_WARNING("{} | Submesh and material count are not maching each other. | Model name : {} | Submesh size : {} | Material ptr size : {}.",
                    "CDyMeshRenderer::pfInitialize", desc.mModelName, submeshResourceSize, materialBindingSize);
  }

  // Bind submesh and material one by one.
  DDyBindingInformation bindingInfo;
  for (int32_t i = 0; i < submeshResourceSize; ++i)
  {
    bindingInfo.mSubmeshResource = this->mModelReferencePtr->GetSubmeshResources()[i].get();
    if (i >= materialBindingSize)
    {
      bindingInfo.mMaterialResource = nullptr;
    }
    else
    {
      bindingInfo.mMaterialResource = this->mMaterialResourcePtr[i];
    }
    this->mMeshMaterialPtrBindingList.emplace_back(bindingInfo);
  }

  // Output debug.
  for (const auto& bindedMeshMatInfo : this->mMeshMaterialPtrBindingList)
  {
    if (bindedMeshMatInfo.mMaterialResource)
    {
      MDY_LOG_DEBUG_D("Rendering mesh binding info : {} / {}",
                      bindedMeshMatInfo.mSubmeshResource->GetVertexArrayId(),
                      bindedMeshMatInfo.mMaterialResource->GetMaterialName());
    }
  }

  return DY_SUCCESS;
}

void CDyMeshRenderer::Render()
{
  for (const auto& bindedMeshMatInfo : this->mMeshMaterialPtrBindingList)
  {
    // Activate shader of one material.
    const auto shaderResource = bindedMeshMatInfo.mMaterialResource->GetShaderResource();
    if (!shaderResource)
    {
      MDY_LOG_CRITICAL("{} | Shader resource of {} is not binded, Can not render mesh.",
                       "CDyMeshRenderer::Render",
                       bindedMeshMatInfo.mMaterialResource->GetMaterialName());
      continue;
    }
    shaderResource->UseShader();

    // Bind submesh VAO id.
    glBindVertexArray(bindedMeshMatInfo.mSubmeshResource->GetVertexArrayId());

    // @todo temporal
    const auto viewMatrix = glGetUniformLocation(shaderResource->GetShaderProgramId(), "viewMatrix");
    const auto projMatirx = glGetUniformLocation(shaderResource->GetShaderProgramId(), "projectionMatrix");

    auto& sceneManager = MDyScene::GetInstance();
    auto* camera = sceneManager.GetCamera();
    if (camera)
    {
      glUniformMatrix4fv(viewMatrix, 1, GL_FALSE, &camera->GetViewMatrix()[0].X);
      glUniformMatrix4fv(projMatirx, 1, GL_FALSE, &camera->GetProjectionMatrix()[0].X);
    }

    // Bind textures of one material.
    if (bindedMeshMatInfo.mMaterialResource)
    {
      const auto& textureResources        = bindedMeshMatInfo.mMaterialResource->GetTextureResources();
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
    if (bindedMeshMatInfo.mSubmeshResource->IsEnabledIndices())
    {
      glDrawElements(GL_TRIANGLES, bindedMeshMatInfo.mSubmeshResource->GetIndicesCounts(), GL_UNSIGNED_INT, nullptr);
    }
    else
    {
      glDrawArrays(GL_TRIANGLES, 0, bindedMeshMatInfo.mSubmeshResource->GetVertexCounts());
    }

    // Unbind, unset, deactivate settings for this submesh and material.
    glBindVertexArray(0);
    shaderResource->UnuseShader();
  }
}

} /// ::dy namespace