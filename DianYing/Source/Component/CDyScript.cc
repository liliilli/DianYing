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
#include <Dy/Component/CDyScript.h>
#include <Dy/Management/WorldManager.h>

namespace dy
{

CDyScript::CDyScript(FDyActor& actorReference) : ADyBaseComponent(actorReference) { }

void CDyScript::Activate() noexcept
{
  ADyBaseComponent::Activate();

  // Check and rebind script instance to MDyWorld.
  if (this->mActivateFlag.IsOutputValueChanged() == true && this->mActivateFlag.GetOutput() == true)
  {
    const auto activatedIndex     = MDyWorld::GetInstance().pfEnrollActiveScript(DyMakeNotNull(this));
    this->mActivatedUpdateListId  = activatedIndex;
  }
}

void CDyScript::Deactivate() noexcept
{
  ADyBaseComponent::Deactivate();

  // Check and unbind script instance to MDyWorld.
  if (this->mActivateFlag.IsOutputValueChanged() == true && this->mActivateFlag.GetOutput() == false)
  {
    MDyWorld::GetInstance().pfUnenrollActiveScript(this->mActivatedUpdateListId);
    this->mActivatedUpdateListId = MDY_INITIALIZE_DEFINT;
  }
}

void CDyScript::pPropagateParentActorActivation(const DDy3StateBool& actorBool) noexcept
{
  ADyBaseComponent::pPropagateParentActorActivation(actorBool);

  if (this->mActivateFlag.IsOutputValueChanged() == true)
  {
    auto& worldManager = MDyWorld::GetInstance();

    if (this->mActivateFlag.GetOutput() == true)
    { // Check and rebind script instance to MDyWorld.
      const auto activatedIndex     = worldManager.pfEnrollActiveScript(DyMakeNotNull(this));
      this->mActivatedUpdateListId  = activatedIndex;
    }
    else
    { // Check and unbind script instance to MDyWorld.
      worldManager.pfUnenrollActiveScript(this->mActivatedUpdateListId);
      this->mActivatedUpdateListId = MDY_INITIALIZE_DEFINT;
    }
  }
}

void CDyScript::CallScriptFunction(_MIN_ const float dt) noexcept
{
  this->mScriptState.CallScriptFunction(dt);
}

void CDyScript::Initiate()
{
  MDY_LOG_INFO("{0}::{0}::Initiate()", this->GetBindedActor()->GetActorName());
}

void CDyScript::Start()
{
  MDY_LOG_INFO("{0}::{0}::Start()", this->GetBindedActor()->GetActorName());
}

void CDyScript::Update(float dt)
{
  auto* obj       = this->GetBindedActor();
  auto transform  = obj->GetTransform();
  transform->AddWorldEulerAngle(EDyAxis3D::Z, 0.16f);
}

void CDyScript::OnEnabled()
{

}

void CDyScript::OnDisabled()
{

}

void CDyScript::Destroy()
{

}

std::string CDyScript::ToString()
{
  return "CDyScript::ToString NOT IMPLEMENTED YET!";
}

EDySuccess CDyScript::Initialize(const DDyScriptMetaInformation& metaInfo)
{
  // @TODO ASSERT THAT SCRIPT COMPONENT IS ACTIVATED EVEN WHEN FIRST TIME.
  this->mScriptName             = metaInfo.mScriptName;
  this->mScriptPath             = metaInfo.mScriptPath;
  if (metaInfo.mInitiallyActivated) { this->Activate(); }

  // Initialize script state instance.
  PDyScriptStateDescriptor desc;
  desc.mScriptPtr = this;
  this->mScriptState.Initialize(desc);

  return DY_SUCCESS;
}

void CDyScript::Release()
{
  this->mScriptState.Release();
  this->Deactivate();
}

} /// ::dy namespace