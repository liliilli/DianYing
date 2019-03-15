#ifndef GUARD_DY_COMPONENT_UI_CDYACTORSCRIPTBASE_H
#define GUARD_DY_COMPONENT_UI_CDYACTORSCRIPTBASE_H
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

#include <Dy/Component/Interface/IScriptable.h>

//!
//! Forward declaration
//!

namespace dy
{
class FActor;
} /// ::dy namespace

//!
//! Implementation
//!

namespace dy
{

/// @class CBaseActorScript
/// @brief Actor script base type. this type should not be used independently. \n
/// Instead of this, just use `CActorScript`.
class CBaseActorScript : public IScriptable
{
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

public:
  CBaseActorScript(FActor& ioRefActor) : mPtrBoundActor(&ioRefActor) { }

  /// @brief Get mutable reference of actor.
  MDY_NODISCARD FActor& GetActorReference() noexcept;

protected:
  /// Script name for specification and searching.
  std::string mScriptName             = MDY_INITIALIZE_EMPTYSTR;
  /// Flag for checking binded script instance.
  bool        mIsScriptInstanceBinded = false;

private:
  FActor*   mPtrBoundActor          = MDY_INITIALIZE_NULL;
};

} /// ::dy namespace

#endif /// GUARD_DY_COMPONENT_UI_CDYACTORSCRIPTBASE_H