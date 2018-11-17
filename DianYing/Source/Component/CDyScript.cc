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
//! Forward declaration
//!

namespace dy
{

//!
//! Error
//!

MDY_SET_IMMUTABLE_STRING(sErrorScriptNotFound, "CDyScript::Initialize | Failed to find script meta information. Unexpected error.");

//!
//! Keywords
//!

MDY_SET_IMMUTABLE_STRING(sFunction_Initiate,    "Initiate");
MDY_SET_IMMUTABLE_STRING(sFunction_Start,       "Start");
MDY_SET_IMMUTABLE_STRING(sFunction_Update,      "Update");
MDY_SET_IMMUTABLE_STRING(sFunction_Destroy,     "Destroy");
MDY_SET_IMMUTABLE_STRING(sFunction_OnEnabled,   "OnEnabled");
MDY_SET_IMMUTABLE_STRING(sFunction_OnDisabled,  "OnDisabled");

} /// ::dy namespace

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

void CDyScript::Initiate()
{
  MDY_ASSERT(this->mIsScriptInstanceBinded == true, "Unexpected error occurred.");
  this->mScriptInstance[MSVSTR(sFunction_Initiate)](this->mScriptInstance);
}

void CDyScript::Start()
{
  MDY_ASSERT(this->mIsScriptInstanceBinded == true, "Unexpected error occurred.");
  this->mScriptInstance[MSVSTR(sFunction_Start)](this->mScriptInstance);
}

void CDyScript::Update(float dt)
{
  MDY_ASSERT(this->mIsScriptInstanceBinded == true, "Unexpected error occurred.");
  this->mScriptInstance[MSVSTR(sFunction_Update)](this->mScriptInstance);

#ifdef false
  auto* obj       = this->GetBindedActor();
  auto transform  = obj->GetTransform();
  transform->AddWorldEulerAngle(EDyAxis3D::Z, 0.16f);
#endif
}

void CDyScript::OnEnabled()
{
  MDY_ASSERT(this->mIsScriptInstanceBinded == true, "Unexpected error occurred.");
  this->mScriptInstance[MSVSTR(sFunction_OnEnabled)](this->mScriptInstance);
}

void CDyScript::OnDisabled()
{
  MDY_ASSERT(this->mIsScriptInstanceBinded == true, "Unexpected error occurred.");
  this->mScriptInstance[MSVSTR(sFunction_OnDisabled)](this->mScriptInstance);
}

void CDyScript::Destroy()
{
  MDY_ASSERT(this->mIsScriptInstanceBinded == true, "Unexpected error occurred.");
  this->mScriptInstance[MSVSTR(sFunction_Destroy)](this->mScriptInstance);
}

std::string CDyScript::ToString()
{
  return "CDyScript::ToString NOT IMPLEMENTED YET!";
}

EDySuccess CDyScript::Initialize(const PDyScriptComponentMetaInfo& metaInfo)
{
  this->mScriptName = metaInfo.mDetails.mSpecifierName;

  // Get script meta information.
  auto& metaInfoManager = MDyMetaInfo::GetInstance();
  MDY_ASSERT(metaInfoManager.IsScriptMetaInformationExist(this->mScriptName) == true, MSVSTR(sErrorScriptNotFound));
  const auto& validScriptMetaInfo = metaInfoManager.GetScriptMetaInformation(this->mScriptName);

  // Bind script, but need to check integrity test also.
  auto& scriptManager   = MDyScript::GetInstance();
  auto& luaInstance     = scriptManager.GetLuaInstance();

  MDY_NOTUSED auto _    = luaInstance.safe_script_file(validScriptMetaInfo.mFilePath);
  this->mScriptInstance = luaInstance[this->mScriptName];
  // @TODO RESOLVE THIS (ERROR & EXCEPTION FROM INSIDE)
  this->mScriptInstance["__pDyInitializeWith"](this->mScriptInstance, *this->GetBindedActor());
  this->mIsScriptInstanceBinded = true;

  //
  if (metaInfo.mInitiallyActivated)
  {
    this->Activate();
  }

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