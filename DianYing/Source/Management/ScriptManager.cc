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
#include <Dy/Management/ScriptManager.h>
#include <Dy/Core/DyEngine.h>
#include <Dy/Component/Internal/Widget/CDyWidgetScriptCpp.h>
#include <Dy/Element/Actor.h>
#include <Dy/Management/LoggingManager.h>
#include <Dy/Management/Helper/LuaBindingEntry.h>

//!
//! Local function & forward declaration
//!

namespace dy
{

///
MDY_SET_IMMUTABLE_STRING(sCDyScriptFrame,
R"dy(
LDyScript = {
  new = function(self, v)
      o = v or {}
      setmetatable(o, self)
      self.__index = self
      return o
    end

  , __pDyInitializeWith = function(self, actorInstance)
      self.mActor = actorInstance
    end

  , Initiate = function(self) end
  , Start = function(self) end
  , Update = function(self) end
  , Destroy = function(self) end
  , OnEnabled = function(self) end
  , OnDisabled = function(self) end
}
)dy");

///
/// @brief  Initialize MDyLog functionality to lua to use it.
/// @param  lua Lua binding instance
///
void DyInitializeMDyLog(_MIO_ sol::state& lua)
{
  /// Binding lua
  lua.new_enum<dy::EDyLogLevel>("EDyLogLevel",
    {
      { "Trace",        EDyLogLevel::Trace },
      { "Debug",        EDyLogLevel::Debug },
      { "Information",  EDyLogLevel::Information },
      { "Warning",      EDyLogLevel::Warning },
      { "Error",        EDyLogLevel::Error },
      { "Critical",     EDyLogLevel::Critical }
    }
  );

  /// When binding lua, destructor must be public and can be seen from outside.
  lua.new_usertype<dy::MDyLog>("MDyLog",
      "new",            sol::no_constructor,
      "GetInstance",    &MDyLog::GetInstance,
      "PushLog",        &MDyLog::PushLog<>,
      "SetVisibleLevel",&MDyLog::SetVisibleLevel
  );
}

///
/// @brief
/// @param  lua
///
void DyInitilaizeFDyObject(_MIO_ sol::state& lua)
{
  /// Binding lua
  lua.new_usertype<dy::FDyObject>("FDyObject",
      "new", sol::constructors<>()
  );
}

///
/// @brief
/// @param
///
void DyInitializeHelperTypes(_MIO_ sol::state& lua)
{

}

///
/// @brief
/// @param  lua
///
void DyInitializeCDyComponents(_MIO_ sol::state& lua)
{

}

///
/// @brief
/// @param  lua
///
void DyInitilaizeFDyActor(_MIO_ sol::state& lua)
{
  /// Binding lua
  lua.new_usertype<dy::FDyActor>("FDyActor",
      /// Base
      sol::base_classes, sol::bases<FDyObject>(),
      /// Cosntructors
      "new", sol::constructors<FDyActor(void)>(),
      ///
      "Activate",     &FDyActor::Activate,
      "Deactivate",   &FDyActor::Deactivate,
      "IsActivated",  &FDyActor::IsActivated,
      "GetActorName", &FDyActor::GetActorName
  );
}

} /// ::dy namespace

//!
//! Implementation
//!

namespace dy
{

EDySuccess MDyScript::pfInitialize()
{
  this->mLua.open_libraries(sol::lib::base, sol::lib::string, sol::lib::math);
  /// Manager binding
  DyBindLuaEntry(this->mLua);
  DyInitializeMDyLog(mLua);
  DyInitilaizeFDyObject(mLua);
  DyInitilaizeFDyActor(mLua);
  this->mLua.safe_script(sCDyScriptFrame);
  return DY_SUCCESS;
}

EDySuccess MDyScript::pfRelease()
{
  // sol::state is RAII, so does release automatically.
  this->mInsertActorScriptList.clear();
  this->mInsertWidgetScriptList.clear();
  this->mGCedActorScriptList.clear();
  this->mGCedWidgetScriptList.clear();
  return DY_SUCCESS;
}

sol::state& MDyScript::GetLuaInstance() noexcept
{
  return this->mLua;
}

FDyWidgetScriptState* MDyScript::CreateWidgetScript(
    _MIN_ const std::string& iScriptSpecifier, 
    _MIN_ FDyUiWidget& iRefWidget, 
    _MIN_ bool iIsAwakened)
{
  const auto& instanceInfo = MDyMetaInfo::GetInstance().GetScriptMetaInformation(iScriptSpecifier);
  MDY_ASSERT(instanceInfo.mScriptType != EDyScriptType::NoneError, "");
  MDY_ASSERT(iIsAwakened == true, "Unexpected error occurred.");

  auto component = std::make_unique<FDyWidgetScriptState>(iRefWidget, instanceInfo);
  this->mInsertWidgetScriptList.emplace_back(std::move(component));
  return this->mInsertWidgetScriptList.back().get();
}

FDyActorScriptState* MDyScript::CreateActorScript(
    _MIN_ const std::string& iScriptSpecifier, 
    _MIN_ FDyActor& iRefActor, 
    _MIN_ bool iIsAwakened)
{
  const auto& instanceInfo = MDyMetaInfo::GetInstance().GetScriptMetaInformation(iScriptSpecifier);
  MDY_ASSERT(instanceInfo.mScriptType != EDyScriptType::NoneError, "");
  MDY_ASSERT(iIsAwakened == true, "Unexpected error occurred.");

  auto component = std::make_unique<FDyActorScriptState>(iRefActor, instanceInfo);
  this->mInsertActorScriptList.emplace_back(std::move(component));
  return this->mInsertActorScriptList.back().get();
}

EDySuccess MDyScript::TryForwardWidgetScriptToGCList(_MIN_ const FDyWidgetScriptState* iPtrWidgetScriptState)
{
  for (auto& ptrsmtScript : this->mInsertWidgetScriptList)
  {
    if (MDY_CHECK_ISEMPTY(ptrsmtScript)) { continue; }
    if (ptrsmtScript.get() == iPtrWidgetScriptState) 
    { // If exist, move script to gc list.
      this->mGCedWidgetScriptList.emplace_back(std::move(ptrsmtScript)); 
      return DY_SUCCESS;
    }
  }

  for (auto& ptrsmtScript : this->mWidgetScriptList)
  {
    if (MDY_CHECK_ISEMPTY(ptrsmtScript)) { continue; }
    if (ptrsmtScript.get() == iPtrWidgetScriptState) 
    { // If exist, move script to gc list.
      this->mGCedWidgetScriptList.emplace_back(std::move(ptrsmtScript)); 
      return DY_SUCCESS;
    }
  }

  return DY_FAILURE;
}

EDySuccess MDyScript::TryForwardActorScriptToGCList(_MIN_ const FDyActorScriptState* iPtrActorScriptStatus)
{
  for (auto& ptrsmtScript : this->mInsertActorScriptList)
  {
    if (MDY_CHECK_ISEMPTY(ptrsmtScript)) { continue; }
    if (ptrsmtScript.get() == iPtrActorScriptStatus) 
    { // If exist, move script to gc list.
      this->mGCedActorScriptList.emplace_back(std::move(ptrsmtScript)); 
      return DY_SUCCESS;
    }
  }

  for (auto& ptrsmtScript : this->mActorScriptList)
  {
    if (MDY_CHECK_ISEMPTY(ptrsmtScript)) { continue; }
    if (ptrsmtScript.get() == iPtrActorScriptStatus) 
    { // If exist, move script to gc list.
      this->mGCedActorScriptList.emplace_back(std::move(ptrsmtScript)); 
      return DY_SUCCESS;
    }
  }

  return DY_FAILURE;
}

void MDyScript::TryMoveInsertWidgetScriptToMainContainer()
{
  if (this->mInsertWidgetScriptList.empty() == true) { return; }

  for (auto& insertWidgetScript : this->mInsertWidgetScriptList)
  { // `insertWidgetScript` is always not empty.
    this->mWidgetScriptList.emplace_back(std::move(insertWidgetScript));
  }
  this->mInsertWidgetScriptList.clear();
}

void MDyScript::TryMoveInsertActorScriptToMainContainer()
{
  if (this->mInsertActorScriptList.empty() == true) { return; }

  for (auto& actorScript : this->mInsertActorScriptList)
  { // `actorScript` is always not empty.
    this->mActorScriptList.emplace_back(std::move(actorScript));
  }
  this->mInsertActorScriptList.clear();
}

void MDyScript::UpdateWidgetScript(_MIN_ TF32 dt)
{
  for (auto& ptrsmtScript : this->mWidgetScriptList)
  {
    if (MDY_CHECK_ISEMPTY(ptrsmtScript)) { continue; }
    ptrsmtScript->CallScriptFunction(dt);
    // If engine must be stopped and end application, return instantly.
    if (gEngine->MDY_PRIVATE_SPECIFIER(IsGameEndCalled)() == true) { return; }
  }
}

void MDyScript::UpdateWidgetScript(_MIN_ TF32 dt, _MIN_ EDyScriptState type)
{
  for (auto& ptrsmtScript : this->mInsertWidgetScriptList)
  {
    if (MDY_CHECK_ISEMPTY(ptrsmtScript)) { continue; }
    if (ptrsmtScript->GetScriptStatus() == type) { ptrsmtScript->CallScriptFunction(dt); }
    // If engine must be stopped and end application, return instantly.
    if (gEngine->MDY_PRIVATE_SPECIFIER(IsGameEndCalled)() == true) { return; }
  }

  for (auto& ptrsmtScript : this->mWidgetScriptList)
  {
    if (MDY_CHECK_ISEMPTY(ptrsmtScript)) { continue; }
    if (ptrsmtScript->GetScriptStatus() == type) { ptrsmtScript->CallScriptFunction(dt); }
    // If engine must be stopped and end application, return instantly.
    if (gEngine->MDY_PRIVATE_SPECIFIER(IsGameEndCalled)() == true) { return; }
  }
}

bool MDyScript::IsGcedWidgetScriptExist() const noexcept
{
  return this->mGCedWidgetScriptList.empty() == false;
}

void MDyScript::CallDestroyFuncWidgetScriptGCList()
{
  for (auto& ptrsmtScript : this->mGCedWidgetScriptList)
  {
    if (MDY_CHECK_ISEMPTY(ptrsmtScript)) { continue; }
    ptrsmtScript->MDY_PRIVATE_SPECIFIER(CallDestroyFunctionAnyway)();
    // If engine must be stopped and end application, return instantly.
    if (gEngine->MDY_PRIVATE_SPECIFIER(IsGameEndCalled)() == true) { return; }
  }
}

void MDyScript::ClearWidgetScriptGCList()
{
  this->mGCedWidgetScriptList.clear();
}

void MDyScript::RemoveEmptyOnWidgetScriptList()
{
  this->mInsertWidgetScriptList.erase(std::remove(MDY_BIND_BEGIN_END(this->mInsertWidgetScriptList), nullptr), this->mInsertWidgetScriptList.end());
  this->mWidgetScriptList.erase(std::remove(MDY_BIND_BEGIN_END(this->mWidgetScriptList), nullptr), this->mWidgetScriptList.end());
}

void MDyScript::UpdateActorScript(TF32 dt)
{
  for (auto& ptrsmtScript : this->mActorScriptList)
  {
    if (MDY_CHECK_ISEMPTY(ptrsmtScript)) { continue; }
    ptrsmtScript->CallScriptFunction(dt);
    // If engine must be stopped and end application, return instantly.
    if (gEngine->MDY_PRIVATE_SPECIFIER(IsGameEndCalled)() == true) { return; }
  }
}

void MDyScript::UpdateActorScript(TF32 dt, EDyScriptState type)
{
  for (auto& ptrsmtScript : this->mInsertActorScriptList)
  {
    if (MDY_CHECK_ISEMPTY(ptrsmtScript)) { continue; }
    if (ptrsmtScript->GetScriptStatus() == type) { ptrsmtScript->CallScriptFunction(dt); }
    // If engine must be stopped and end application, return instantly.
    if (gEngine->MDY_PRIVATE_SPECIFIER(IsGameEndCalled)() == true) { return; }
  }

  for (auto& ptrsmtScript : this->mActorScriptList)
  {
    if (MDY_CHECK_ISEMPTY(ptrsmtScript)) { continue; }
    if (ptrsmtScript->GetScriptStatus() == type) { ptrsmtScript->CallScriptFunction(dt); }
    // If engine must be stopped and end application, return instantly.
    if (gEngine->MDY_PRIVATE_SPECIFIER(IsGameEndCalled)() == true) { return; }
  }
}

void MDyScript::ClearActorScriptGCList()
{
  this->mGCedActorScriptList.clear();
}

} /// ::dy namespace