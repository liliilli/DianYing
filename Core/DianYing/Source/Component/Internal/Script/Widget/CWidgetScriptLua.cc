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
#include <Dy/Component/Internal/Widget/CWidgetScriptLua.h>

#include <Dy/Management/IO/MIOMeta.h>
#include <Dy/Management/MScript.h>
#include <Dy/Helper/System/Assertion.h>

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

EDySuccess CWidgetScriptLua::Initialize(const PDyScriptComponentMetaInfo& metaInfo)
{
  this->mScriptName = metaInfo.mDetails.mSpecifierName;

  // (1) Get script meta information.
  auto& metaInfoManager = MIOMeta::GetInstance();
  MDY_ASSERT_MSG(metaInfoManager.IsScriptMetaInformationExist(this->mScriptName) == true, (sErrorScriptNotFound));
  const auto& validScriptMetaInfo = metaInfoManager.GetScriptMetaInformation(this->mScriptName);

  // (2) Bind script, but need to check integrity test also.
  auto& scriptManager   = MScript::GetInstance();
  auto& luaInstance     = scriptManager.GetLuaInstance();

  MDY_NOTUSED auto _    = luaInstance.safe_script_file(validScriptMetaInfo.mFilePath);
  this->mScriptInstance = luaInstance[this->mScriptName];
  // @TODO RESOLVE THIS (ERROR & EXCEPTION FROM INSIDE)
  MDY_NOT_IMPLEMENTED_ASSERT();
#ifdef false
  this->mScriptInstance["__pDyInitializeWith"](this->mScriptInstance, *this->GetBindedActor());
  this->mIsScriptInstanceBinded = true;

  if (metaInfo.mInitiallyActivated == true) { this->Activate(); }

  // Initialize script state instance.
  PDyScriptStateDescriptor desc;
  desc.mScriptPtr = this;
  this->mScriptState.Initialize(desc);
#endif

  return EDySuccess::DY_SUCCESS;
}

void CWidgetScriptLua::Initiate()
{
  this->mScriptInstance[(sFunction_Initiate)](this->mScriptInstance);
}

void CWidgetScriptLua::Start()
{
  this->mScriptInstance[(sFunction_Start)](this->mScriptInstance);
}

void CWidgetScriptLua::Update(float dt)
{
  this->mScriptInstance[(sFunction_Update)](this->mScriptInstance);
}

void CWidgetScriptLua::OnEnabled()
{
  this->mScriptInstance[(sFunction_OnEnabled)](this->mScriptInstance);
}

void CWidgetScriptLua::OnDisabled()
{
  this->mScriptInstance[(sFunction_OnDisabled)](this->mScriptInstance);
}

void CWidgetScriptLua::Destroy()
{
  this->mScriptInstance[(sFunction_Destroy)](this->mScriptInstance);
}

} /// ::dy namespace