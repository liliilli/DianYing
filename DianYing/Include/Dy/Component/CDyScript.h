#ifndef GUARD_DY_COMPONENT_CDYSCRIPT_H
#define GUARD_DY_COMPONENT_CDYSCRIPT_H
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

#include <functional>
#include <sol2/sol.hpp>
#include <Dy/Element/Abstract/ADyBaseComponent.h>
#include <Dy/Element/Interface/IDyScriptable.h>
#include <Dy/Component/Internal/ScriptState.h>

//!
//! Forward declaration
//!

namespace dy
{

struct PDyScriptComponentMetaInfo;

} /// ::unnamed namespace

//!
//! Implementation
//!

namespace dy
{

///
/// @class CDyScript
/// @brief Script component type class.
/// @TODO SCRIPT THIS
///
class CDyScript : public ADyBaseComponent, public IDyScriptable
{
public:
  CDyScript(FDyActor& actorReference);

  ///
  /// @brief  Activate CDyScript instance.
  ///
  void Activate() noexcept override final;

  ///
  /// @brief  Deactivate CDyScript instance.
  ///
  void Deactivate() noexcept override final;

  ///
  /// @brief  Update parent activation flag from FDyActor.
  /// @param  actorBool 3-state boolean type from FDyActor.
  ///
  void pPropagateParentActorActivation(const DDy3StateBool& actorBool) noexcept override final;

  //! Script calling sequence.
  //! 1) WHEN SCRIPT IS DEACTIVATED (any way) => Do nothing!
  //!
  //! 2) WHEN SCRIPT IS ACTIVATED (any way)
  //!
  //! ~ Time sequence ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  //! Immediately       Update                                    After Update
  //! ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  //!
  //!       CollisionCallback() >--.   .------<<<------.
  //!                              |   |               |
  //! Initiate() --+--> Start() >--+---+--> Update() --'--.--+--> Destroy()
  //!              |                                      |  |
  //!              ^                Wait.                 |  ^
  //!              |                 ||                   |  |
  //!              '-- OnEnabled() <-++-- OnDisabled() <--'  ^
  //!                                ||                      |
  //!                                '+------->>>------------'
  //!

  ///
  /// @brief Call script function following status of CDyScript::mScriptState
  ///
  void CallScriptFunction(_MIN_ const float dt) noexcept;

  ///
  /// @brief
  /// @TODO SCRIPT THIS
  ///
  MDY_NODISCARD std::string ToString() override final;

  ///
  /// @brief  Return verification name of this script component instance.
  /// @return Script vertification name must not be empty.
  ///
  MDY_NODISCARD FORCEINLINE const std::string& GetScriptVerificationName() const noexcept
  {
    MDY_ASSERT(this->mScriptName.empty() == false, "Script name must not be empty.");
    return this->mScriptName;
  }

  ///
  /// @brief Release derived resources of `CDyScript`.
  /// This function shadowing derived Release function intentionally.
  ///
  void Release()
  {
    return this->pScriptRelease();
  }

protected:
  ///
  /// @brief
  ///
  virtual void pScriptRelease() = 0;

protected:
  /// Script name for specification and searching.
  MDY_TRANSIENT std::string           mScriptName   = MDY_INITIALIZE_EMPTYSTR;
  /// Script state for calling arbitary function.
  FDyScriptState                      mScriptState;
  /// Flag for checking binded script instance.
  bool                                mIsScriptInstanceBinded = false;

  MDY_SET_TYPEMATCH_FUNCTION(::dy::ADyBaseComponent, CDyScript);
  MDY_SET_CRC32_HASH_WITH_TYPE(CDyScript);

  friend void FDyScriptState::CallScriptFunction(_MIN_ const float dt) noexcept;
};

} /// ::dy namespace

#endif /// GUARD_DY_COMPONENT_CDYSCRIPT_H