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

/// Header file
#include <Dy/Core/Rendering/Pipeline/DebugRenderer.h>
#include <Dy/Core/Resource/Resource/FDyFrameBufferResource.h>
#include <Dy/Core/Resource/Resource/FDyShaderResource.h>
#include <Dy/Core/Resource/Resource/FDyMeshResource.h>
#include <Dy/Component/CDyPhysicsCollider.h>
#include <Dy/Core/Rendering/Wrapper/FDyGLWrapper.h>

namespace dy
{
  
bool FDyDebugRenderer::IsReady() const noexcept
{
  return this->mBinderShader.IsResourceExist() == true 
      && this->mBinderFrameBuffer.IsResourceExist() == true;
}

EDySuccess FDyDebugRenderer::TrySetupRendering()
{
  if (this->IsReady() == false) { return DY_FAILURE; }

  glDisable(GL_DEPTH_TEST);
  
  this->mBinderFrameBuffer->BindFrameBuffer();
  glClearColor(0, 0, 0, 0);
  glClear(GL_COLOR_BUFFER_BIT);

  // We need not update camera. Because already updated.

  return DY_SUCCESS;
}

void FDyDebugRenderer::Clear()
{
  if (this->IsReady() == false) { return; }

  this->mBinderFrameBuffer->BindFrameBuffer();
  glClearColor(0, 0, 0, 0);
  glClear(GL_COLOR_BUFFER_BIT);
  this->mBinderFrameBuffer->UnbindFrameBuffer();
}

void FDyDebugRenderer::RenderScreen(_MIN_ CDyPhysicsCollider& iRefCollider, _MIN_ const DDyMatrix4x4& iTransformMatrix)
{
  // Check update collider mesh information.
  if (iRefCollider.IsNeedToUpdateColliderMesh() == true)
  {
    iRefCollider.UpdateColliderMesh();
  }

  // Update uniform.
  this->mBinderShader->UseShader();
  this->mBinderShader.TryUpdateUniform<EDyUniformVariableType::Matrix4>("uTransform", iTransformMatrix);
  this->mBinderShader.TryUpdateUniform<EDyUniformVariableType::Integer>("uColorIndex", 0);
  this->mBinderShader.TryUpdateUniformList();

  // Binding Mesh information
  switch (iRefCollider.GetColliderType())
  {
  case EDyColliderType::Sphere:
  { this->mBinderSphere->BindVertexArray();
    glBindBuffer    (GL_ARRAY_BUFFER, this->mBinderSphere->GetVertexBufferId());
    glBufferSubData (GL_ARRAY_BUFFER, 0, sizeof(DDyVector3) * 100, &iRefCollider.GetColliderMesh()[0].X);
    glBindBuffer    (GL_ARRAY_BUFFER, 0);

    FDyGLWrapper::Draw(EDyDrawType::LineLoop, false, 100);
  } break;
  case EDyColliderType::Capsule: 
  { MDY_NOT_IMPLEMENTED_ASSERT();
  } break;
  case EDyColliderType::Box: 
  { this->mBinderBox->BindVertexArray(); 
    glBindBuffer    (GL_ARRAY_BUFFER, this->mBinderBox->GetVertexBufferId());
    glBufferSubData (GL_ARRAY_BUFFER, 0, sizeof(DDyVector3) * 20, &iRefCollider.GetColliderMesh()[0].X);
    glBindBuffer    (GL_ARRAY_BUFFER, 0);

    FDyGLWrapper::Draw(EDyDrawType::LineStrip, false, 20);
  } break;
  default: MDY_UNEXPECTED_BRANCH(); break;
  }

  // Render.
  
}

} /// ::dy namespace