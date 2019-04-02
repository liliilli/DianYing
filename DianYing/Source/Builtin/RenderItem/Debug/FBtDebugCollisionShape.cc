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

#include <Dy/Builtin/RenderItem/Debug/FBtDebugCollisionShape.h>
#include <Dy/Management/MWorld.h>
#include <Dy/Core/Resource/Resource/AResourceFrameBufferBase.h>
#include <Dy/Core/Resource/Resource/FResourceShader.h>
#include <Dy/Core/Rendering/Type/EDrawType.h>
#include <Dy/Core/Rendering/Wrapper/XGLWrapper.h>
#include <Dy/Core/Resource/Resource/FResourceMesh.h>
#include <Dy/Management/Rendering/MRendering.h>
#include <Dy/Component/Internal/Physics/CBasePhysicsCollider.h>
#include <Dy/Management/MSetting.h>

namespace dy
{

void FBtRenderDebugCollisionShape::__ConstructionHelper
  ::ConstructBuffer(PDyRenderItemInstanceMetaInfo& oMeta)
{
  oMeta.mUuid = DUuid("56c0d498-4fad-4f46-bdd4-ddcdfbad234a", true);
}

EDySuccess FBtRenderDebugCollisionShape::OnPreRenderCheckCondition()
{
  const auto& setting = MSetting::GetInstance();
  if (setting.IsRenderPhysicsCollisionShape() == false)
  {
    return DY_FAILURE;
  }

  // Second.
  return this->AreResourcesValid() ? DY_SUCCESS : DY_FAILURE;
}

bool FBtRenderDebugCollisionShape::AreResourcesValid()
{
  return this->mBinderShader.IsResourceExist() == true 
      && this->mBinderFrameBuffer.IsResourceExist() == true;;
}

void FBtRenderDebugCollisionShape::OnFailedCheckCondition()
{
  if (this->AreResourcesValid() == false) { return; }

  this->OnSetupRenderingSetting();
  this->mBinderFrameBuffer->UnbindFrameBuffer();
}

void FBtRenderDebugCollisionShape::OnSetupRenderingSetting()
{
  // Bind
  // We need not update camera. Because already updated.
  this->mBinderFrameBuffer->BindFrameBuffer();
  glClearColor(0, 0, 0, 0);
  glClear(GL_COLOR_BUFFER_BIT);
}

void FBtRenderDebugCollisionShape::OnRender()
{
  auto& list = MRendering::GetInstance().GetColliderMeshQueueList();
  for (auto&[ptrCollider, transformMatrix] : list)
  {
    this->RenderObject(*ptrCollider, transformMatrix);
  }
}

void FBtRenderDebugCollisionShape::RenderObject(
  CBasePhysicsCollider& iRefCollider,
  const DMat4& iTransformMatrix)
{
  //!
  //! Debug rendering.
  //! https://docs.nvidia.com/gameworks/content/gameworkslibrary/physx/guide/Manual/DebugVisualization.html#debugvisualization
  //!
  // Check update collider mesh information.
  if (iRefCollider.IsNeedToUpdateColliderMesh() == true)
  {
    iRefCollider.UpdateColliderMesh();
  }

  // Update uniform.
  this->mBinderShader->TryUpdateUniform<EUniformVariableType::Matrix4>("uTransform", iTransformMatrix);
  this->mBinderShader->TryUpdateUniform<EUniformVariableType::Integer>("uColorIndex", 0);
  this->mBinderShader->UseShader();
  this->mBinderShader->TryUpdateUniformList();

  // Binding Mesh information
  switch (iRefCollider.GetColliderType())
  {
  case EDyColliderType::Sphere:
  { this->mBinderSphere->BindVertexArray();
    glBindBuffer    (GL_ARRAY_BUFFER, this->mBinderSphere->GetVertexBufferId());
    glBufferSubData (GL_ARRAY_BUFFER, 0, sizeof(DVec3) * 100, iRefCollider.GetColliderMesh()[0].Data());
    glBindBuffer    (GL_ARRAY_BUFFER, 0);

    // Render.
    XGLWrapper::Draw(EDrawType::LineLoop, false, 100);
  } break;
  case EDyColliderType::Capsule: 
  { this->mBinderCapsule->BindVertexArray();
    glBindBuffer    (GL_ARRAY_BUFFER, this->mBinderCapsule->GetVertexBufferId());
    glBufferSubData (GL_ARRAY_BUFFER, 0, sizeof(DVec3) * 100, iRefCollider.GetColliderMesh()[0].Data());
    glBindBuffer    (GL_ARRAY_BUFFER, 0);

    // Render.
    XGLWrapper::Draw(EDrawType::LineLoop, true, 101);
  } break;
  case EDyColliderType::Box: 
  { this->mBinderBox->BindVertexArray(); 
    glBindBuffer    (GL_ARRAY_BUFFER, this->mBinderBox->GetVertexBufferId());
    glBufferSubData (GL_ARRAY_BUFFER, 0, sizeof(DVec3) * 20, iRefCollider.GetColliderMesh()[0].Data());
    glBindBuffer    (GL_ARRAY_BUFFER, 0);

    // Render.
    XGLWrapper::Draw(EDrawType::LineStrip, false, 20);
  } break;
  default: MDY_UNEXPECTED_BRANCH(); break;
  }
}

void FBtRenderDebugCollisionShape::OnReleaseRenderingSetting()
{
  // Unbind
  this->mBinderFrameBuffer->UnbindFrameBuffer();
}

void FBtRenderDebugCollisionShape::OnPostRender()
{
  /* Do nothing */
}

} /// ::dy namespace
