#ifndef GUARD_DY_COMPONENT_UI_CDYACTORSCRIPT_H
#define GUARD_DY_COMPONENT_UI_CDYACTORSCRIPT_H
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

#include <Dy/Element/Abstract/ABaseComponent.h>
#include <Dy/Component/Internal/Actor/FActorScriptState.h>

//!
//! Forward declaration
//!

namespace dy
{
} /// ::dy namespace

//!
//! Implementation
//!

namespace dy
{

/// @class CActorScript
/// @brief actor script component for binding internal script instance.
class CActorScript final : public ABaseComponent
{
  MDY_SET_TYPEMATCH_FUNCTION(::dy::ABaseComponent, CActorScript);
  MDY_SET_CRC32_HASH_WITH_TYPE(CActorScript);
public:
  CActorScript(FActor& iActor, const std::string& iScriptSpecifier);
  ~CActorScript();

  /// @brief
  MDY_NODISCARD std::string ToString() override final;

private:
  /// @brief This function must be implemented. 
  /// This function will be called when `Activate()` or `SetupFlagAsParent` called, 
  /// and final state boolean value is true from false.
  void TryActivateInstance() override final;
  
  /// @brief This function must be implemented.
  /// This function will be called when `Deactivate()` or `SetupFlagAsParent(false)` called,
  /// and final state boolean value is false from true.
  void TryDeactivateInstance() override final;

  FActorScriptState* mPtrScriptStatus  = MDY_INITIALIZE_NULL;
};

} /// ::dy namespace

#endif /// GUARD_DY_COMPONENT_UI_CDYACTORSCRIPT_H
