#ifndef GUARD_DY_COMPONENT_IDYCPPSCRIPT_H
#define GUARD_DY_COMPONENT_IDYCPPSCRIPT_H
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

#include <Dy/Component/Interface/IDyScriptable.h>
#include <Dy/Helper/Pointer.h>

//!
//! Forward declaration
//!

namespace dy
{
class FDyActor;
class CDyActorScriptCpp;
class MDyGameTimer;
class FDyTimerHandle;
} /// ::dy namespace

//!
//! Implementation
//!

namespace dy
{

///
/// @class ADyActorCppScript
/// @brief Cpp script for Actor.
///
MDY_ABSTRACT ADyActorCppScript : public IDyScriptable
{
public:
  /// @brief Get widget reference.
  MDY_NODISCARD FDyActor& GetActorReference();

  /// @brief Get Game-Timer manager.
  MDY_NODISCARD MDyGameTimer& GetGameTimerManager() noexcept;

  void MDY_PRIVATE_SPECIFIER(BindPtrTimerHandle)(_MIN_ FDyTimerHandle& iRefTimerHandler);
  void MDY_PRIVATE_SPECIFIER(DetachPtrTimerHandle)(_MIN_ FDyTimerHandle& iRefTimerHandler);
  void MDY_PRIVATE_SPECIFIER(AbortAllValidTimerHandler)();

private:
  ///
  /// @brief Set reference to outside world.
  /// @param outsideReference Reference of WidgetScriptCpp instance.
  ///
  void pfSetOutsideReference(CDyActorScriptCpp& outsideReference) noexcept
  {
    this->mOutside = &outsideReference;
  }

  CDyActorScriptCpp* mOutside = nullptr;

  std::vector<NotNull<FDyTimerHandle*>> mPtrTimerHandleList = {};
  friend class CDyActorScriptCpp;
};

} /// ::dy namespace

#endif /// GUARD_DY_COMPONENT_IDYCPPSCRIPT_H