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

#include <Dy/Element/Abstract/ADyBaseComponent.h>
#include <Dy/Element/Interface/IDyScriptable.h>

//!
//! Forward declaration
//!

namespace dy
{

struct DDyScriptMetaInformation;

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
class CDyScript final : public ADyBaseComponent, public IDyScriptable
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
  /// @brief
  /// @TODO SCRIPT THIS
  ///
  void Initiate() override final;

  ///
  /// @brief
  /// @TODO SCRIPT THIS
  ///
  void Start() override final;

  ///
  /// @brief
  /// @TODO SCRIPT THIS
  ///
  void Update(float dt) override final;

  ///
  /// @brief
  /// @TODO SCRIPT THIS
  ///
  void OnEnabled() override final;

  ///
  /// @brief
  /// @TODO SCRIPT THIS
  ///
  void OnDisabled() override final;

  ///
  /// @brief
  /// @TODO SCRIPT THIS
  ///
  void Destroy() override final;

  ///
  /// @brief
  /// @TODO SCRIPT THIS
  ///
  MDY_NODISCARD std::string ToString() override final;

  ///
  /// @brief
  /// @param  metaInfo
  /// @return
  /// @TODO SCRIPT THIS
  ///
  MDY_NODISCARD EDySuccess Initialize(_MIN_ const DDyScriptMetaInformation& metaInfo);

  ///
  /// @brief
  /// @TODO SCRIPT THIS
  ///
  void Release();

  ///
  /// @brief  Return verification name of this script component instance.
  /// @return Script vertification name must not be empty.
  ///
  MDY_NODISCARD FORCEINLINE const std::string& GetScriptVerificationName() const noexcept
  {
    PHITOS_ASSERT(this->mScriptName.empty() == false, "Script name must not be empty.");
    return this->mScriptName;
  }

private:
  /// Script name for specification and searching.
  MDY_TRANSIENT std::string mScriptName = MDY_INITILAIZE_EMPTYSTR;
  /// Script path to execute lua script file.
  MDY_TRANSIENT std::string mScriptPath = MDY_INITILAIZE_EMPTYSTR;

  MDY_SET_TYPEMATCH_FUNCTION(::dy::ADyBaseComponent, CDyScript);
  MDY_SET_CRC32_HASH_WITH_TYPE(CDyScript);
};

} /// ::dy namespace

#endif /// GUARD_DY_COMPONENT_CDYSCRIPT_H