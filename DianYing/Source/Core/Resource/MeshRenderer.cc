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
#include <Dy/Core/Resource/MeshRenderer.h>

#include <Dy/Core/Resource/Internal/EtcType.h>
#include <Dy/Core/Resource/Resource_Deprecated/ModelResource_Deprecated.h>

#include <Dy/Management/IO/IOResourceManager.h>
#include <Dy/Management/WorldManager.h>
#include <Dy/Core/Resource/Object/Camera.h>
#include <Dy/Management/LoggingManager.h>
#include <Dy/Management/Rendering/RenderingManager.h>

namespace dy
{

EDySuccess CDyMeshRenderer::pfInitialize(const PDyRendererConsturctionDescriptor& desc)
{
  auto& resourceManager = MDyIOResource::GetInstance();

  // Bind model. If not exists, make model resource using information but not have it, return fail.
  if (const auto modelResourcePtr = resourceManager.GetModelResource(desc.mModelName); !modelResourcePtr)
  {
    const auto res = resourceManager.CreateModelResource_Deprecated(desc.mModelName);
    if (res == DY_FAILURE) return DY_FAILURE;

    this->mModelReferencePtr = resourceManager.GetModelResource(desc.mModelName);
  }
  else { this->mModelReferencePtr = modelResourcePtr; }

  // Bind material. If not exists, make material resource using information, but return fail.
  for (const auto& materialName : desc.mMaterialNames)
  {
    if (auto* materialResourcePtr = resourceManager.GetMaterialResource(materialName); !materialResourcePtr)
    {
      const auto res = resourceManager.CreateMaterialResource_Deprecated(materialName);
      if (res == DY_FAILURE) { return DY_FAILURE; }

      this->mMaterialResourcePtr.emplace_back(resourceManager.GetMaterialResource(materialName));
    }
    else { this->mMaterialResourcePtr.emplace_back(materialResourcePtr); }
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

void CDyMeshRenderer::Update(float dt)
{
  if (this->mModelReferencePtr == nullptr || !this->mModelReferencePtr->IsEnabledModelAnimated()) return;

  static float runningTime = 0.f;
  runningTime += dt;

  this->mModelReferencePtr->UpdateBoneAnimationTransformList(runningTime);
}

void CDyMeshRenderer::CallDraw()
{
  //auto& renderingManager = MDyRendering::GetInstance();
  //renderingManager.PushDrawCallTask(*this);
}

} /// ::dy namespace