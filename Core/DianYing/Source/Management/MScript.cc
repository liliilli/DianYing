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

#include <Dy/Core/GEngine.h>
#include <Dy/Component/Internal/Widget/CWidgetScriptCpp.h>
#include <Dy/Component/Internal/Script/FWidgetScriptState.h>
#include <Dy/Component/Internal/Script/FGlobalScriptState.h>
#include <Dy/Element/FActor.h>
#include <Dy/Management/Helper/XLuaBindingEntry.h>

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
  lua.new_usertype<dy::AWorldObject>("AWorldObject",
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
void DyInitilaizeFActor(_MIO_ sol::state& lua)
{
#ifdef false
  /// Binding lua
  lua.new_usertype<dy::FActor>("FActor",
      /// Base
      sol::base_classes, sol::bases<AWorldObject>(),
      /// Cosntructors
      "new", sol::constructors<FActor(void)>(),
      ///
      "Activate",     &FActor::Activate,
      "Deactivate",   &FActor::Deactivate,
      "IsActivated",  &FActor::IsActivated,
      "GetActorName", &FActor::GetActorName
  );
#endif
}

} /// ::dy namespace

//!
//! Implementation
//!


namespace dy
{

struct MScript::Impl final
{
  DY_DECLARE_PIMPL_CTOR(MScript);
  ~Impl();

  /// @brief Try remove widget script from dy system.
  /// But, removed widget script does not actually removed instantly, \n
  /// moved gc list and removed actually on next frame prior to update.
  EDySuccess TryForwardWidgetScriptToGCList(const FWidgetScriptState* iPtrWidgetScriptState);
  /// @brief Try move inserted widget script to main container.
  void TryMoveInsertWidgetScriptToMainContainer();
  
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
  /// @brief Clear widget script gc list `mGCedWidgetScriptList` anyway.
  void ClearWidgetScriptGCList() { this->mGCedWidgetScriptList.clear(); }
  /// @brief remove emptied script list.
  void RemoveEmptyOnWidgetScriptList();

  /// @brief Update actor script.
  void UpdateActorScript(TF32 iDt);
  /// @brief Update actor script if only script present type is type.
  void UpdateActorScript(TF32 dt, EScriptState type);

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
  DY_INITIALIZE_PIMPL();
  return EDySuccess::DY_SUCCESS;
}

EDySuccess MScript::pfRelease()
{
  DY_RESET_PIMPL();
  return EDySuccess::DY_SUCCESS;
}

sol::state& MScript::GetLuaInstance() noexcept 
{ 
  return DY_PIMPL->mLua;
}

FWidgetScriptState* MScript::CreateWidgetScript(
  const std::string& iScriptSpecifier, 
  FWidget& iRefWidget, 
  bool iIsAwakened)
{
  const auto& instanceInfo = MIOMeta::GetInstance().GetScriptMetaInformation(iScriptSpecifier);

  MDY_ASSERT_FORCE(instanceInfo.mScriptType != EDyScriptType::NoneError);
  MDY_ASSERT_FORCE(iIsAwakened == true);

  auto component = std::make_unique<FWidgetScriptState>(iRefWidget, instanceInfo);
  DY_PIMPL->mInsertWidgetScriptList.emplace_back(std::move(component));

  return DY_PIMPL->mInsertWidgetScriptList.back().get();
}

void MScript::CreateGlobalScriptInstances() { DY_PIMPL->CreateGlobalScriptInstances(); }
void MScript::RemoveGlobalScriptInstances() { DY_PIMPL->RemoveGlobalScriptInstances(); }
void MScript::CallonStartGlobalScriptList() { DY_PIMPL->CallonStartGlobalScriptList(); }
void MScript::CallonEndGlobalScriptList()   { DY_PIMPL->CallonEndGlobalScriptList(); }

FActorScriptState* MScript::CreateActorScript(
  const std::string& iScriptSpecifier, 
  FActor& iRefActor, 
  bool iIsAwakened)
{
  const auto& instanceInfo = MIOMeta::GetInstance().GetScriptMetaInformation(iScriptSpecifier);
  MDY_ASSERT_MSG(instanceInfo.mScriptType != EDyScriptType::NoneError, "");
  MDY_ASSERT_MSG(iIsAwakened == true, "Unexpected error occurred.");

  auto component = std::make_unique<FActorScriptState>(iRefActor, instanceInfo);
  DY_PIMPL->mInsertActorScriptList.emplace_back(std::move(component));

  return DY_PIMPL->mInsertActorScriptList.back().get();
}

EDySuccess MScript::TryForwardWidgetScriptToGCList(const FWidgetScriptState* iPtrWidgetScriptState)
{
  return DY_PIMPL->TryForwardWidgetScriptToGCList(iPtrWidgetScriptState);
}

EDySuccess MScript::TryForwardActorScriptToGCList(const FActorScriptState* iPtrActorScriptStatus)
{
  return DY_PIMPL->TryForwardActorScriptToGCList(iPtrActorScriptStatus);
}

void MScript::TryMoveInsertWidgetScriptToMainContainer()
{
  DY_PIMPL->TryMoveInsertWidgetScriptToMainContainer();
}

void MScript::TryMoveInsertActorScriptToMainContainer()
{
  DY_PIMPL->TryMoveInsertActorScriptToMainContainer();
}

void MScript::UpdateWidgetScript(TF32 dt) { DY_PIMPL->UpdateWidgetScript(dt); }
void MScript::UpdateWidgetScript(TF32 dt, EScriptState type) { DY_PIMPL->UpdateWidgetScript(dt, type); }

bool MScript::IsGcedWidgetScriptExist() const noexcept 
{ 
  return DY_PIMPL->mGCedWidgetScriptList.empty() == false; 
}

void MScript::CallDestroyFuncWidgetScriptGCList() 
{ 
  for (auto& ptrsmtScript : DY_PIMPL->mGCedWidgetScriptList)
  {
    if (ptrsmtScript == nullptr) { continue; }
    ptrsmtScript->MDY_PRIVATE(CallDestroyFunctionAnyway)();

    // If engine must be stopped and end application, return instantly.
    if (TEMP_CAST(gEngine)->MDY_PRIVATE(IsGameEndCalled)() == true) { return; }
  }
}

void MScript::ClearWidgetScriptGCList() 
{ 
  DY_PIMPL->mGCedWidgetScriptList.clear();
}

void MScript::RemoveEmptyOnWidgetScriptList() { return DY_PIMPL->RemoveEmptyOnWidgetScriptList(); }
void MScript::UpdateActorScript(TF32 dt) { DY_PIMPL->UpdateActorScript(dt); }
void MScript::UpdateActorScript(TF32 dt, EScriptState type) { DY_PIMPL->UpdateActorScript(dt, type); }

bool MScript::IsGcedActorScriptExist() const noexcept 
{ 
  return DY_PIMPL->mGCedActorScriptList.empty() == false; 
}

void MScript::CallDestroyFuncActorScriptGCList() 
{ 
  for (auto& ptrsmtScript : DY_PIMPL->mGCedActorScriptList)
  {
    // Call `Destory` function of Actor script.
    if (ptrsmtScript == nullptr) { continue; }
    ptrsmtScript->MDY_PRIVATE(CallDestroyFunctionAnyway)();

    // If engine must be stopped and end application, return instantly.
    if (TEMP_CAST(gEngine)->MDY_PRIVATE(IsGameEndCalled)() == true) 
    { 
      return; 
    }
  }
}

void MScript::ClearActorScriptGCList() 
{ 
  DY_PIMPL->mGCedActorScriptList.clear(); 
}

DY_DEFINE_PIMPL(MScript);
DY_DEFINE_DEFAULT_DESTRUCTOR(MScript);

} /// ::dy namespace