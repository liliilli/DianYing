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

namespace dy
{

///
/// @class CDyScript
/// @brief Script component type class.
///
class CDyScript final : public ADyBaseComponent, public IDyScriptable
{
public:
  CDyScript(FDyActor& actorReference);

  ///
  /// @brief
  ///
  void Activate() noexcept override final;

  ///
  /// @brief
  ///
  void Deactivate() noexcept override final;

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
  ///
  void Initiate() override final;

  ///
  /// @brief
  ///
  void Start() override final;

  ///
  /// @brief
  ///
  void Update(float dt) override final;

  ///
  /// @brief
  ///
  void OnEnabled() override final;

  ///
  /// @brief
  ///
  void OnDisabled() override final;

  ///
  /// @brief
  ///
  void Destroy() override final;

  ///
  /// @brief
  ///
  MDY_NODISCARD std::string ToString() override final;

private:

  MDY_SET_TYPEMATCH_FUNCTION(::dy::ADyBaseComponent, CDyScript);
  MDY_SET_CRC32_HASH_WITH_TYPE(CDyScript);
};

} /// ::dy namespace

#endif /// GUARD_DY_COMPONENT_CDYSCRIPT_H