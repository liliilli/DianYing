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
#include <Dy/Management/ScriptManager.h>

//!
//! Implementation
//!

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

std::string CDyScript::ToString()
{
  return "CDyScript::ToString NOT IMPLEMENTED YET!";
}

} /// ::dy namespace