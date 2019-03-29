#include <precompiled.h>
///
/// MIT License
/// Copyright (c) 2018-2019 Jongmin Yun
///
/// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
/// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
/// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
/// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
/// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
/// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
/// SOFTWARE.
///

#include <Dy/Builtin/RenderItem/Debug/FBtDebugCollisionAabb.h>
#include <Dy/Management/MWorld.h>
#include <Dy/Core/Resource/Resource/FDyFrameBufferResource.h>
#include <Dy/Core/Resource/Resource/FDyShaderResource.h>
#include <Dy/Core/Rendering/Type/EDrawType.h>
#include <Dy/Core/Rendering/Wrapper/XGLWrapper.h>
#include <Dy/Core/Resource/Resource/FDyMeshResource.h>
#include <Dy/Management/Rendering/MRendering.h>
#include <Dy/Component/Internal/Physics/CBasePhysicsCollider.h>
#include <Dy/Management/MSetting.h>

namespace dy
{

void FBtRenderDebugCollisionAabb::__ConstructionHelper
  ::ConstructBuffer(PDyRenderItemInstanceMetaInfo& oMeta)
{
  oMeta.mUuid = DUuid("29d8ab34-bc8f-41cf-b9e0-f34c4eabfc14", true);
}

EDySuccess FBtRenderDebugCollisionAabb::OnPreRenderCheckCondition()
{
  const auto& setting = MSetting::GetInstance();
  if (setting.IsRenderPhysicsCollisionAABB() == false)
  {
    return DY_FAILURE;
  }

  // Second.
  return this->AreResourcesValid() ? DY_SUCCESS : DY_FAILURE;
}

bool FBtRenderDebugCollisionAabb::AreResourcesValid()
{
  return this->mBinderShader.IsResourceExist() == true 
      && this->mBinderFrameBuffer.IsResourceExist() == true
      && this->mBinderAABB.IsResourceExist() == true;
}

void FBtRenderDebugCollisionAabb::OnSetupRenderingSetting()
{
  // Bind
  // We need not update camera. Because already updated.
  this->mBinderFrameBuffer->BindFrameBuffer();
}

void FBtRenderDebugCollisionAabb::OnRender()
{
  auto& list = MRendering::GetInstance().GetColliderMeshQueueList();
  for (auto&[ptrCollider, transformMatrix] : list)
  {
    this->RenderObject(*ptrCollider, transformMatrix);
  }
}

void FBtRenderDebugCollisionAabb::RenderObject(
  CBasePhysicsCollider& iRefCollider,
  const DMat4& iTransformMatrix)
{
  // Update uniform.
  this->mBinderShader->TryUpdateUniform<EUniformVariableType::Matrix4>("uTransform", iTransformMatrix);
  this->mBinderShader->TryUpdateUniform<EUniformVariableType::Integer>("uColorIndex", 1); // Magenta
  this->mBinderShader->UseShader();
  this->mBinderShader->TryUpdateUniformList();

  this->mBinderAABB->BindVertexArray();
  // Create buffer.
  /*
   *   1----0    Indice must be
   *  /|    |\ Mx 0 1 2 3 F
   * 2--------3   0 3 6 5 F 0 5 4 1 F 1 4 7 2 F 2 7 6 3 F
   * | 4----5 |   4 5 6 7
   * |/mn    \|
   * 7--------6
   */
  const auto& b = iRefCollider.GetBound();
  std::vector<DVec3> aabbVbo;
  aabbVbo.emplace_back(b.mMax.X, b.mMax.Y, b.mMin.Z);
  aabbVbo.emplace_back(b.mMin.X, b.mMax.Y, b.mMin.Z);
  aabbVbo.emplace_back(b.mMin.X, b.mMax.Y, b.mMax.Z);
  aabbVbo.emplace_back(b.mMax.X, b.mMax.Y, b.mMax.Z);

  aabbVbo.emplace_back(b.mMin.X, b.mMin.Y, b.mMin.Z);
  aabbVbo.emplace_back(b.mMax.X, b.mMin.Y, b.mMin.Z);
  aabbVbo.emplace_back(b.mMax.X, b.mMin.Y, b.mMax.Z);
  aabbVbo.emplace_back(b.mMin.X, b.mMin.Y, b.mMax.Z);

  // Buffer binding.
  glBindBuffer(GL_ARRAY_BUFFER, this->mBinderAABB->GetVertexBufferId());
  glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(DVec3) * 8, &aabbVbo[0].X);
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  // Render.
  XGLWrapper::Draw(EDrawType::LineStrip, true, mBinderAABB->GetIndicesCounts());
}

void FBtRenderDebugCollisionAabb::OnReleaseRenderingSetting()
{
  // Unbind
  this->mBinderFrameBuffer->UnbindFrameBuffer();
}

} /// ::dy namespace
