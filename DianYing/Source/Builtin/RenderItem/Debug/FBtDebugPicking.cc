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

#include <Dy/Builtin/RenderItem/Debug/FBtDebugPicking.h>
#include <Dy/Management/MWorld.h>
#include <Dy/Core/Resource/Resource/AResourceFrameBufferBase.h>
#include <Dy/Core/Resource/Resource/FResourceShader.h>
#include <Dy/Core/Rendering/Type/EDrawType.h>
#include <Dy/Core/Rendering/Wrapper/XGLWrapper.h>
#include <Dy/Core/Resource/Resource/FResourceMesh.h>
#include <Dy/Management/Rendering/MRendering.h>
#include <Dy/Component/Internal/Physics/CBasePhysicsCollider.h>
#include <Dy/Management/MSetting.h>
#include <Dy/Management/MInput.h>

namespace dy
{

void FBtRenderDebugPicking::__ConstructionHelper
  ::ConstructBuffer(PDyRenderItemInstanceMetaInfo& oMeta)
{
  oMeta.mUuid = DUuid("c1349dca-0b5b-4b73-b66f-4d648ceee638", true);
}

EDySuccess FBtRenderDebugPicking::OnPreRenderCheckCondition()
{
  if (auto& refInput = MInput::GetInstance();
      refInput.IsActorPicked() == false)
  {
    return DY_FAILURE;
  }
  else
  {
    //auto& refActor = *(*refInput.__GetPPtrPickingTarget());
  }

  // Second.
  return this->AreResourcesValid() ? DY_SUCCESS : DY_FAILURE;
}

bool FBtRenderDebugPicking::AreResourcesValid()
{
  return this->mBinderShader.IsResourceExist() == true 
      && this->mBinderFrameBuffer.IsResourceExist() == true;
}

void FBtRenderDebugPicking::OnSetupRenderingSetting()
{
  // Bind
  // We need not update camera. Because already updated.
  this->mBinderFrameBuffer->BindFrameBuffer();
}

void FBtRenderDebugPicking::OnRender()
{

}

void FBtRenderDebugPicking::RenderObject(
  CBasePhysicsCollider& iRefCollider,
  const DMat4& iTransformMatrix)
{
  // Update uniform.
  this->mBinderShader->TryUpdateUniform<EUniformVariableType::Matrix4>("uModelTransform", DMat4{});
  this->mBinderShader->TryUpdateUniform<EUniformVariableType::Integer>("uColorIndex", 2); // Magenta
  this->mBinderShader->UseShader();
  this->mBinderShader->TryUpdateUniformList();

  // Render.
  //XGLWrapper::Draw(EDrawType::LineStrip, true, mBinderAABB->GetIndicesCounts());
  this->mBinderShader->DisuseShader();
}

void FBtRenderDebugPicking::OnReleaseRenderingSetting()
{
  // Unbind
  this->mBinderFrameBuffer->UnbindFrameBuffer();
}

} /// ::dy namespace
