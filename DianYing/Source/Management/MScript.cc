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
#include <Dy/Management/MScript.h>

#define SOL_CHECK_ARGUMENT 1
#include <sol2/sol.hpp>

#include <Dy/Core/DyEngine.h>
#include <Dy/Component/Internal/Widget/CWidgetScriptCpp.h>
#include <Dy/Component/Internal/Script/FWidgetScriptState.h>
#include <Dy/Component/Internal/Script/FGlobalScriptState.h>
#include <Dy/Element/Actor.h>
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
/// @brief  Initialize MLog functionality to lua to use it.
/// @param  lua Lua binding instance
///
void DyInitializeMLog(_MIO_ sol::state& lua)
{
  /// Binding lua
  lua.new_enum<MLog::ELevel>("EDyLogLevel",
    {
      { "Trace",        MLog::ELevel::Trace },
      { "Debug",        MLog::ELevel::Debug },
      { "Information",  MLog::ELevel::Information },
      { "Warning",      MLog::ELevel::Warning },
      { "Error",        MLog::ELevel::Error },
      { "Critical",     MLog::ELevel::Critical }
    }
  );

  /// When binding lua, destructor must be public and can be seen from outside.
  lua.new_usertype<dy::MLog>("MLog",
      "new",            sol::no_constructor,
      "GetInstance",    &MLog::GetInstance,
      //"PushLog",        &MLog::PushLog,
      "SetVisibleLevel",&MLog::SetVisibleLevel
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
#ifdef false
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
#endif
}

} /// ::dy namespace

//!
//! Implementation
//!


namespace dy
{

class MScript::Impl final
{
public:
  Impl();
  ~Impl();

  /// @brief Get reference of lua instance.
  /// @return lua instance l-value reference.
  sol::state& GetLuaInstance() noexcept;

  /// @brief Create widget script. \n
  /// @TODO IMPLEMENT LUA VERSION. 
  /// @param iScriptSpecifier
  /// @param iRefWidget
  /// @param iIsAwakened
  FWidgetScriptState* 
  CreateWidgetScript(const std::string& iScriptSpecifier, FDyUiWidget& iRefWidget, bool iIsAwakened);

  /// @brief Try remove widget script from dy system.
  /// But, removed widget script does not actually removed instantly, \n
  /// moved gc list and removed actually on next frame prior to update.
  EDySuccess TryForwardWidgetScriptToGCList(const FWidgetScriptState* iPtrWidgetScriptState);
  /// @brief Try move inserted widget script to main container.
  void TryMoveInsertWidgetScriptToMainContainer();
  
  /// @brief Create widget script. \n
  /// @TODO IMPLEMENT LUA VERSION. 
  /// @param iScriptSpecifier
  /// @param iRefActor
  /// @param iIsAwakened
  FActorScriptState* 
  CreateActorScript(const std::string& iScriptSpecifier, FDyActor& iRefActor, bool iIsAwakened);
  /// @brief Try remove actor script from dy system.
  /// But, removed actor script does not actually removed instantly, \n
  /// moved gc list and removed actually on next frame prior to update.
  EDySuccess TryForwardActorScriptToGCList(const FActorScriptState* iPtrActorScriptStatus);
  /// @brief Try move inserted actor script to main container.
  void TryMoveInsertActorScriptToMainContainer();

  /// @brief Update widget script.
  void UpdateWidgetScript(TF32 dt);
  /// @brief Update widget script if only script present type is type.
  void UpdateWidgetScript(TF32 dt, EScriptState type);
  /// @brief Check widget script that must be gced is exist on list.
  bool IsGcedWidgetScriptExist() const noexcept { return this->mGCedWidgetScriptList.empty() == false; }
  /// @brief Call `destroy` GCed widget script 
  void CallDestroyFuncWidgetScriptGCList();
  /// @brief Clear widget script gc list `mGCedWidgetScriptList` anyway.
  void ClearWidgetScriptGCList() { this->mGCedWidgetScriptList.clear(); }
  /// @brief remove emptied script list.
  void RemoveEmptyOnWidgetScriptList();

  /// @brief Update actor script.
  void UpdateActorScript(TF32 iDt);
  /// @brief Update actor script if only script present type is type.
  void UpdateActorScript(TF32 dt, EScriptState type);
  /// @brief Check there are gced -candidate actor script instances.
  bool IsGcedActorScriptExist() const noexcept { return this->mGCedActorScriptList.empty() == false; }
  /// @brief Call `destroy` actor script 
  void CallDestroyFuncActorScriptGcList();
  /// @brief Clear actor script gc list `mGCedActorScriptList` anyway.
  void ClearActorScriptGCList() { this->mGCedActorScriptList.clear(); }

  /// @brief Create global script instance list.
  /// This function must be called once per application runtime.
  void CreateGlobalScriptInstances();
  /// @brief Remove global script instance list.
  /// This function must be called once per application runtime.
  void RemoveGlobalScriptInstances() { this->mGlobalScriptContainer.clear(); }
  /// @brief Call `onStart` function to all global script.
  void CallonStartGlobalScriptList() 
  {
    for (const auto& [scriptName, ptrsmtScript] : this->mGlobalScriptContainer)
    {
      ptrsmtScript->CallStart();
    }
  }
  /// @brief Call `onEnd` function to all global script.
  void CallonEndGlobalScriptList()
  {
    for (const auto&[scriptName, ptrsmtScript] : this->mGlobalScriptContainer)
    {
      ptrsmtScript->CallEnd();
    }
  }

private:
  sol::state mLua;

  using TDyGlobalScriptList = std::unordered_map<std::string, std::unique_ptr<FGlobalScriptState>>;
  TDyGlobalScriptList mGlobalScriptContainer;

  using TDyWidgetScriptList = std::vector<std::unique_ptr<FWidgetScriptState>>;
  TDyWidgetScriptList mInsertWidgetScriptList = {};
  TDyWidgetScriptList mWidgetScriptList       = {};
  TDyWidgetScriptList mGCedWidgetScriptList   = {};

  /// Activated CDyScript component list.
  /// this list must not be invalidated when iterating list, but except for unenrolling.
  using TDyActorScriptList = std::vector<std::unique_ptr<FActorScriptState>>;
  TDyActorScriptList  mInsertActorScriptList  = {};
  TDyActorScriptList  mActorScriptList        = {};
  TDyActorScriptList  mGCedActorScriptList    = {};
};

} /// ::dy namespace
#include <Dy/Management/Inline/MScriptImpl.inl>

//!
//! Proxy
//!

namespace dy
{

EDySuccess MScript::pfInitialize()
{
  this->mPimpl = new (std::nothrow) Impl();
  return DY_SUCCESS;
}

EDySuccess MScript::pfRelease()
{
  delete this->mPimpl;
  return DY_SUCCESS;
}

sol::state& MScript::GetLuaInstance() noexcept { return this->mPimpl->GetLuaInstance(); }

FWidgetScriptState* MScript::CreateWidgetScript(
    const std::string& iScriptSpecifier, 
    FDyUiWidget& iRefWidget, 
    bool iIsAwakened)
{
  return this->mPimpl->CreateWidgetScript(iScriptSpecifier, iRefWidget, iIsAwakened);
}

void MScript::CreateGlobalScriptInstances() { this->mPimpl->CreateGlobalScriptInstances(); }
void MScript::RemoveGlobalScriptInstances() { this->mPimpl->RemoveGlobalScriptInstances(); }
void MScript::CallonStartGlobalScriptList() { this->mPimpl->CallonStartGlobalScriptList(); }
void MScript::CallonEndGlobalScriptList()   { this->mPimpl->CallonEndGlobalScriptList(); }

FActorScriptState* MScript::CreateActorScript(
    const std::string& iScriptSpecifier, 
    FDyActor& iRefActor, 
    bool iIsAwakened)
{
  return this->mPimpl->CreateActorScript(iScriptSpecifier, iRefActor, iIsAwakened);
}

EDySuccess MScript::TryForwardWidgetScriptToGCList(const FWidgetScriptState* iPtrWidgetScriptState)
{
  return this->mPimpl->TryForwardWidgetScriptToGCList(iPtrWidgetScriptState);
}

EDySuccess MScript::TryForwardActorScriptToGCList(const FActorScriptState* iPtrActorScriptStatus)
{
  return this->mPimpl->TryForwardActorScriptToGCList(iPtrActorScriptStatus);
}

void MScript::TryMoveInsertWidgetScriptToMainContainer()
{
  this->mPimpl->TryMoveInsertWidgetScriptToMainContainer();
}

void MScript::TryMoveInsertActorScriptToMainContainer()
{
  this->mPimpl->TryMoveInsertActorScriptToMainContainer();
}

void MScript::UpdateWidgetScript(TF32 dt) { this->mPimpl->UpdateWidgetScript(dt); }
void MScript::UpdateWidgetScript(TF32 dt, EScriptState type) { this->mPimpl->UpdateWidgetScript(dt, type); }
bool MScript::IsGcedWidgetScriptExist() const noexcept { return this->mPimpl->IsGcedWidgetScriptExist(); }
void MScript::CallDestroyFuncWidgetScriptGCList() { this->mPimpl->CallDestroyFuncWidgetScriptGCList(); }
void MScript::ClearWidgetScriptGCList() { this->mPimpl->ClearWidgetScriptGCList(); }
void MScript::RemoveEmptyOnWidgetScriptList() { return this->mPimpl->RemoveEmptyOnWidgetScriptList(); }
void MScript::UpdateActorScript(TF32 dt) { this->mPimpl->UpdateActorScript(dt); }
void MScript::UpdateActorScript(TF32 dt, EScriptState type) { this->mPimpl->UpdateActorScript(dt, type); }
bool MScript::IsGcedActorScriptExist() const noexcept { return this->mPimpl->IsGcedActorScriptExist(); }
void MScript::CallDestroyFuncActorScriptGCList() { this->mPimpl->CallDestroyFuncActorScriptGcList(); }
void MScript::ClearActorScriptGCList() { this->mPimpl->ClearActorScriptGCList(); }

} /// ::dy namespace