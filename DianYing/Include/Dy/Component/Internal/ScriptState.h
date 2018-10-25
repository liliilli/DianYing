#ifndef GUARD_DY_INTERNAL_SCRIPTSTATE_H
#define GUARD_DY_INTERNAL_SCRIPTSTATE_H
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

#include <Dy/Component/Interface/IDyInitializeHelper.h>

//!
//! Forward declaration
//!

namespace dy
{
class CDyScript;
} /// ::dy namespace

//!
//! Implementation
//!

namespace dy
{

///
/// @enum EDyScriptState
/// @brief
///
enum class EDyScriptState
{
  NoneError,
  CalledNothing,
  CalledInitiate,
  CalledUpdate,
};

///
/// @class PDyScriptStateDescriptor
/// @brief
///
struct PDyScriptStateDescriptor final
{
  /// Valid script instance pointer to bind.
  CDyScript* mScriptPtr = MDY_INITIALIZE_NULL;
};

///
/// @class FDyScriptState
/// @brief
///
class FDyScriptState final : public IDyInitializeHelper<PDyScriptStateDescriptor>
{
public:
  ///
  EDySuccess Initialize(_MIN_ const PDyScriptStateDescriptor& descriptor) override;

  ///
  void Release() override;

  ///
  /// @brief
  ///
  void CallScriptFunction(_MIN_ const float dt) noexcept;

private:
  ///
  CDyScript* mScriptPtr = MDY_INITIALIZE_NULL;
  ///
  EDyScriptState mState = EDyScriptState::NoneError;
};

} /// ::dy namespace

#endif /// GUARD_DY_INTERNAL_SCRIPTSTATE_H