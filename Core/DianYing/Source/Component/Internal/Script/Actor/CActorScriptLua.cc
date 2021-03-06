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

#include <Dy/Component/Internal/Actor/CActorScriptLua.h>
#include <Dy/Management/IO/MIOMeta.h>
#include <Dy/Management/MScript.h>
#include <Dy/Element/FActor.h>

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

namespace dy
{

CActorScriptLua::CActorScriptLua(FActor& actorReference, const PDyScriptInstanceMetaInfo& iDesc) 
  : CBaseActorScript{actorReference}
{
  MDY_ASSERT_MSG(iDesc.mScriptType == EDyScriptType::Lua,   "Script type is not matched to CActorScriptLua.");
  MDY_ASSERT_MSG(iDesc.mScriptMode == EDyScriptMode::Actor, "Given script must be actor type.");

    // (2) Bind script, but need to check integrity test also.
  auto& scriptManager   = MScript::GetInstance();
  auto& luaInstance     = scriptManager.GetLuaInstance();

  MDY_NOTUSED auto _    = luaInstance.safe_script_file(iDesc.mFilePath);
  this->mScriptInstance = luaInstance[this->mScriptName];

  // @TODO RESOLVE THIS (ERROR & EXCEPTION FROM INSIDE)
  this->mScriptInstance["__pDyInitializeWith"](this->mScriptInstance, this->GetActorReference());

  this->mScriptName = iDesc.mSpecifierName;
  this->mIsScriptInstanceBinded = true;
  //if (metaInfo.mInitiallyActivated == true) { this->Activate(); }
}

void CActorScriptLua::Initiate()
{
  MDY_ASSERT_MSG(this->mIsScriptInstanceBinded == true, "Unexpected error occurred.");
  this->mScriptInstance[(sFunction_Initiate)](this->mScriptInstance);
}

void CActorScriptLua::Start()
{
  MDY_ASSERT_MSG(this->mIsScriptInstanceBinded == true, "Unexpected error occurred.");
  this->mScriptInstance[(sFunction_Start)](this->mScriptInstance);
}

void CActorScriptLua::Update(float dt)
{
  MDY_ASSERT_MSG(this->mIsScriptInstanceBinded == true, "Unexpected error occurred.");
  this->mScriptInstance[(sFunction_Update)](this->mScriptInstance);

#ifdef false
  auto* obj = this->GetBindedActor();
  auto transform = obj->GetTransform();
  transform->AddWorldEulerAngle(EAxis::Z, 0.16f);
#endif
}

void CActorScriptLua::OnEnabled()
{
  MDY_ASSERT_MSG(this->mIsScriptInstanceBinded == true, "Unexpected error occurred.");
  this->mScriptInstance[(sFunction_OnEnabled)](this->mScriptInstance);
}

void CActorScriptLua::OnDisabled()
{
  MDY_ASSERT_MSG(this->mIsScriptInstanceBinded == true, "Unexpected error occurred.");
  this->mScriptInstance[(sFunction_OnDisabled)](this->mScriptInstance);
}

void CActorScriptLua::Destroy()
{
  MDY_ASSERT_MSG(this->mIsScriptInstanceBinded == true, "Unexpected error occurred.");
  this->mScriptInstance[(sFunction_Destroy)](this->mScriptInstance);
}

} /// ::dy namespace