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
#include <Dy/Core/Rendering/Pipeline/DebugAABBRenderer.h>
#include <Dy/Core/Resource/Resource/FDyFrameBufferResource.h>
#include <Dy/Core/Resource/Resource/FDyShaderResource.h>
#include <Dy/Core/Resource/Resource/FDyMeshResource.h>
#include <Dy/Core/Rendering/Wrapper/FDyGLWrapper.h>
#include <Dy/Component/CDyPhysicsCollider.h>

namespace dy
{
  
bool FDyDebugAABBRenderer::IsReady() const noexcept
{
  return this->mBinderShader.IsResourceExist() == true 
      && this->mBinderFrameBuffer.IsResourceExist() == true
      && this->mBinderAABB.IsResourceExist() == true;
}

EDySuccess FDyDebugAABBRenderer::TryPushRenderingSetting()
{
  if (this->IsReady() == false) { return DY_FAILURE; }
 
  // Bind
  // We need not update camera. Because already updated.
  this->mBinderFrameBuffer->BindFrameBuffer();
  return DY_SUCCESS;
}

EDySuccess FDyDebugAABBRenderer::TryPopRenderingSetting()
{
  if (this->IsReady() == false) { return DY_FAILURE; }

  // Unbind
  this->mBinderFrameBuffer->UnbindFrameBuffer();
  return DY_SUCCESS;
}

void FDyDebugAABBRenderer::Clear()
{
  if (this->IsReady() == false) { return; }

  this->mBinderFrameBuffer->BindFrameBuffer();
  glClearColor(0, 0, 0, 0);
  glClear(GL_COLOR_BUFFER_BIT);
  this->mBinderFrameBuffer->UnbindFrameBuffer();
}

void FDyDebugAABBRenderer::RenderScreen(_MIN_ CDyPhysicsCollider& iRefCollider, _MIN_ const DDyMatrix4x4& iTransformMatrix)
{
  // Update uniform.
  this->mBinderShader->TryUpdateUniform<EDyUniformVariableType::Matrix4>("uTransform", iTransformMatrix);
  this->mBinderShader->TryUpdateUniform<EDyUniformVariableType::Integer>("uColorIndex", 1); // Magenta
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
  std::vector<DDyVector3> aabbVbo;
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
  glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(DDyVector3) * 8, &aabbVbo[0].X);
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  // Render.
  FDyGLWrapper::Draw(EDyDrawType::LineStrip, true, mBinderAABB->GetIndicesCounts());
}

} /// ::dy namespace