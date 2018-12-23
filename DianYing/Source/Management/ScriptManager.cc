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
#include <Dy/Management/LoggingManager.h>
#include <Dy/Element/Actor.h>
#include <Dy/Management/Helper/LuaBindingEntry.h>
#include <Dy/Component/Internal/Widget/CDyWidgetScriptCpp.h>

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

void MDyScript::TryMoveInsertWidgetScriptToMainContainer()
{
  if (this->mInsertWidgetScriptList.empty() == true) { return; }

  for (auto& insertWidgetScript : this->mInsertWidgetScriptList)
  { // `insertWidgetScript` is always not empty.
    this->mWidgetScriptList.emplace_back(std::move(insertWidgetScript));
  }
  this->mInsertWidgetScriptList.clear();
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

void MDyScript::UpdateWidgetScript(_MIN_ TF32 dt)
{
  for (auto& ptrsmtScript : this->mWidgetScriptList)
  {
    if (MDY_CHECK_ISEMPTY(ptrsmtScript)) { continue; }
    ptrsmtScript->CallScriptFunction(dt);
  }
}

void MDyScript::UpdateWidgetScript(_MIN_ TF32 dt, _MIN_ EDyScriptState type)
{
  for (auto& ptrsmtScript : this->mInsertWidgetScriptList)
  {
    if (MDY_CHECK_ISEMPTY(ptrsmtScript)) { continue; }
    if (ptrsmtScript->GetScriptStatus() == type) { ptrsmtScript->CallScriptFunction(dt); }
  }

  for (auto& ptrsmtScript : this->mWidgetScriptList)
  {
    if (MDY_CHECK_ISEMPTY(ptrsmtScript)) { continue; }
    if (ptrsmtScript->GetScriptStatus() == type) { ptrsmtScript->CallScriptFunction(dt); }
  }
}

} /// ::dy namespace