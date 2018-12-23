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

#include <Dy/Component/Internal/Actor/CDyActorScriptLua.h>
#include <Dy/Management/IO/MetaInfoManager.h>
#include <Dy/Management/ScriptManager.h>
#include <Dy/Element/Actor.h>

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

CDyActorScriptLua::CDyActorScriptLua(_MIN_ FDyActor& actorReference, _MIN_ const PDyScriptInstanceMetaInfo& iDesc) : 
    CDyActorScriptBase{actorReference}
{
#ifdef false
  this->mScriptName = iDesc.mSpecifierName;

  // (1) Get script meta information.
  auto& metaInfoManager = MDyMetaInfo::GetInstance();
  MDY_ASSERT(metaInfoManager.IsScriptMetaInformationExist(this->mScriptName) == true, MSVSTR(sErrorScriptNotFound));
  const auto& validScriptMetaInfo = metaInfoManager.GetScriptMetaInformation(this->mScriptName);

  // (2) Bind script, but need to check integrity test also.
  auto& scriptManager   = MDyScript::GetInstance();
  auto& luaInstance     = scriptManager.GetLuaInstance();

  MDY_NOTUSED auto _    = luaInstance.safe_script_file(validScriptMetaInfo.mFilePath);
  this->mScriptInstance = luaInstance[this->mScriptName];
  // @TODO RESOLVE THIS (ERROR & EXCEPTION FROM INSIDE)
  this->mScriptInstance["__pDyInitializeWith"](this->mScriptInstance, *this->GetBindedActor());
  this->mIsScriptInstanceBinded = true;
#endif

  //if (metaInfo.mInitiallyActivated == true) { this->Activate(); }
}

void CDyActorScriptLua::Initiate()
{
  MDY_ASSERT(this->mIsScriptInstanceBinded == true, "Unexpected error occurred.");
  this->mScriptInstance[MSVSTR(sFunction_Initiate)](this->mScriptInstance);
}

void CDyActorScriptLua::Start()
{
  MDY_ASSERT(this->mIsScriptInstanceBinded == true, "Unexpected error occurred.");
  this->mScriptInstance[MSVSTR(sFunction_Start)](this->mScriptInstance);
}

void CDyActorScriptLua::Update(float dt)
{
  MDY_ASSERT(this->mIsScriptInstanceBinded == true, "Unexpected error occurred.");
  this->mScriptInstance[MSVSTR(sFunction_Update)](this->mScriptInstance);

#ifdef false
  auto* obj = this->GetBindedActor();
  auto transform = obj->GetTransform();
  transform->AddWorldEulerAngle(EDyAxis3D::Z, 0.16f);
#endif
}

void CDyActorScriptLua::OnEnabled()
{
  MDY_ASSERT(this->mIsScriptInstanceBinded == true, "Unexpected error occurred.");
  this->mScriptInstance[MSVSTR(sFunction_OnEnabled)](this->mScriptInstance);
}

void CDyActorScriptLua::OnDisabled()
{
  MDY_ASSERT(this->mIsScriptInstanceBinded == true, "Unexpected error occurred.");
  this->mScriptInstance[MSVSTR(sFunction_OnDisabled)](this->mScriptInstance);
}

void CDyActorScriptLua::Destroy()
{
  MDY_ASSERT(this->mIsScriptInstanceBinded == true, "Unexpected error occurred.");
  this->mScriptInstance[MSVSTR(sFunction_Destroy)](this->mScriptInstance);
}

} /// ::dy namespace