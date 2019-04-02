#ifndef GUARD_DY_MANAGEMENT_SCRIPTMANAGER_IMPL_INL
#define GUARD_DY_MANAGEMENT_SCRIPTMANAGER_IMPL_INL
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

namespace dy
{

inline MScript::Impl::Impl(MScript& parent) : mImplParent(parent)
{
  this->mLua.open_libraries(sol::lib::base, sol::lib::string, sol::lib::math);
  /// Manager binding
  DyBindLuaEntry(this->mLua);
  DyInitializeMLog(mLua);
  DyInitilaizeFDyObject(mLua);
  DyInitilaizeFActor(mLua);
  this->mLua.safe_script(sCDyScriptFrame);
}

inline MScript::Impl::~Impl()
{
  // sol::state is RAII, so does release automatically.
  this->mInsertActorScriptList.clear();
  this->mInsertWidgetScriptList.clear();
  this->mGCedActorScriptList.clear();
  this->mGCedWidgetScriptList.clear();
}

inline EDySuccess MScript::Impl::TryForwardWidgetScriptToGCList(const FWidgetScriptState* iPtrWidgetScriptState)
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

inline void MScript::Impl::TryMoveInsertWidgetScriptToMainContainer()
{
  if (this->mInsertWidgetScriptList.empty() == true) { return; }

  for (auto& insertWidgetScript : this->mInsertWidgetScriptList)
  { // `insertWidgetScript` is always not empty.
    this->mWidgetScriptList.emplace_back(std::move(insertWidgetScript));
  }
  this->mInsertWidgetScriptList.clear();
}

inline EDySuccess MScript::Impl::TryForwardActorScriptToGCList(const FActorScriptState* iPtrActorScriptStatus)
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

inline void MScript::Impl::TryMoveInsertActorScriptToMainContainer()
{
  if (this->mInsertActorScriptList.empty() == true) { return; }

  for (auto& actorScript : this->mInsertActorScriptList)
  { // `actorScript` is always not empty.
    this->mActorScriptList.emplace_back(std::move(actorScript));
  }
  this->mInsertActorScriptList.clear();
}

inline void MScript::Impl::UpdateWidgetScript(TF32 dt)
{
  for (auto& ptrsmtScript : this->mWidgetScriptList)
  {
    if (MDY_CHECK_ISEMPTY(ptrsmtScript)) { continue; }
    ptrsmtScript->CallScriptFunction(dt);
    // If engine must be stopped and end application, return instantly.
    if (gEngine->MDY_PRIVATE(IsGameEndCalled)() == true) { return; }
  }
}

inline void MScript::Impl::UpdateWidgetScript(TF32 dt, EScriptState type)
{
  for (auto& ptrsmtScript : this->mInsertWidgetScriptList)
  {
    if (MDY_CHECK_ISEMPTY(ptrsmtScript)) { continue; }
    if (ptrsmtScript->GetScriptStatus() == type) { ptrsmtScript->CallScriptFunction(dt); }
    // If engine must be stopped and end application, return instantly.
    if (gEngine->MDY_PRIVATE(IsGameEndCalled)() == true) { return; }
  }

  for (auto& ptrsmtScript : this->mWidgetScriptList)
  {
    if (MDY_CHECK_ISEMPTY(ptrsmtScript)) { continue; }
    if (ptrsmtScript->GetScriptStatus() == type) { ptrsmtScript->CallScriptFunction(dt); }
    // If engine must be stopped and end application, return instantly.
    if (gEngine->MDY_PRIVATE(IsGameEndCalled)() == true) { return; }
  }
}

inline void MScript::Impl::RemoveEmptyOnWidgetScriptList()
{
  EraseRemove(this->mWidgetScriptList, nullptr);
  EraseRemove(this->mInsertWidgetScriptList, nullptr);
  //this->mInsertWidgetScriptList.erase(std::remove(MDY_BIND_BEGIN_END(this->mInsertWidgetScriptList), nullptr), this->mInsertWidgetScriptList.end());
  //this->mWidgetScriptList.erase(std::remove(MDY_BIND_BEGIN_END(this->mWidgetScriptList), nullptr), this->mWidgetScriptList.end());
}

inline void MScript::Impl::UpdateActorScript(TF32 iDt)
{
  for (auto& ptrsmtScript : this->mActorScriptList)
  {
    if (ptrsmtScript == nullptr) { continue; }
    ptrsmtScript->CallScriptFunction(iDt);
    // If engine must be stopped and end application, return instantly.
    if (gEngine->MDY_PRIVATE(IsGameEndCalled)() == true) { return; }
  }
}

inline void MScript::Impl::UpdateActorScript(TF32 dt, EScriptState type)
{
  for (auto& ptrsmtScript : this->mInsertActorScriptList)
  {
    if (ptrsmtScript == nullptr) { continue; }
    if (ptrsmtScript->GetScriptStatus() == type) { ptrsmtScript->CallScriptFunction(dt); }
    // If engine must be stopped and end application, return instantly.
    if (gEngine->MDY_PRIVATE(IsGameEndCalled)() == true) { return; }
  }

  for (auto& ptrsmtScript : this->mActorScriptList)
  {
    if (ptrsmtScript == nullptr) { continue; }
    if (ptrsmtScript->GetScriptStatus() == type) { ptrsmtScript->CallScriptFunction(dt); }
    // If engine must be stopped and end application, return instantly.
    if (gEngine->MDY_PRIVATE(IsGameEndCalled)() == true) { return; }
  }
}

inline void MScript::Impl::CreateGlobalScriptInstances()
{
  const auto& metaManager = MIOMeta::GetInstance();
  const auto& list = metaManager.GetGlobalScriptMetaInfos();
  for (const auto& [scriptName, scriptMeta] : list)
  {
    this->mGlobalScriptContainer.try_emplace(
      scriptName,
      std::make_unique<FGlobalScriptState>(*scriptMeta));
  }
}

}

#endif /// GUARD_DY_MANAGEMENT_SCRIPTMANAGER_IMPL_INL