#ifndef GAURD_DY_COMPONENT_INTERNAL_SCRIPT_FActorSCRIPTSTATUS_H
#define GAURD_DY_COMPONENT_INTERNAL_SCRIPT_FActorSCRIPTSTATUS_H
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

#include <Dy/Meta/Information/ScriptMetaInformation.h>
#include <Dy/Component/Internal/EScriptState.h>
#include <Dy/Component/Internal/Actor/CBaseActorScript.h>

//!
//! Forward declaration
//!

namespace dy
{
class   FActor;
struct  PDyScriptInstanceMetaInfo;
} /// ::dy namespace

//!
//! Implementation
//!

namespace dy
{

/// @class FActorScriptState
/// @brief Status binding type for actor script easing calling functions.
class FActorScriptState final
{
public:
  FActorScriptState(FActor& iRefActor, const PDyScriptInstanceMetaInfo& descriptor); 

  /// @brief Call actor script function properly.
  void CallScriptFunction(TF32 dt) noexcept;

  /// @brief Call destroy function.
  void MDY_PRIVATE(CallDestroyFunctionAnyway)() noexcept;

  /// @brief Get script type (Cpp, Lua).
  MDY_NODISCARD EDyScriptType GetScriptType() const noexcept;

  /// @brief Get script present status.
  MDY_NODISCARD EScriptState GetScriptStatus() const noexcept;

  /// @brief Get internal widget script instance (level 2)
  MDY_NODISCARD CBaseActorScript* MDY_PRIVATE(GetPtrInternalActorScript)() const noexcept;

private:
  /// @brief Widget script pointer instance.
  std::unique_ptr<CBaseActorScript> mScriptInstance = nullptr;
  /// @brief Status of this actor script.
  EScriptState mStatus  = EScriptState::NoneError;
  /// @brief Internal script type of this status instance.
  EDyScriptType  mType    = EDyScriptType::NoneError;
};

} /// ::dy namespace

#endif /// GAURD_DY_COMPONENT_INTERNAL_SCRIPT_FActorSCRIPTSTATUS_H
