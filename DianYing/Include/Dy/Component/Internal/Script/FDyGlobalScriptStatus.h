#ifndef GAURD_DY_COMPONENT_INTERNAL_SCRIPT_FDyGlobalScriptStatus_H
#define GAURD_DY_COMPONENT_INTERNAL_SCRIPT_FDyGlobalScriptStatus_H
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

#include <Dy/Component/Internal/ScriptState.h>
#include <Dy/Meta/Information/ScriptMetaInformation.h>
#include <Dy/Component/Internal/Global/CDyGlobalScriptBase.h>

//!
//! Forward declaration
//!

namespace dy
{
struct PDyScriptInstanceMetaInfo;
} /// ::dy namespace

//!
//! Implementation
//!

namespace dy
{

///
/// @class FDyGlobalScriptState
/// @brief Status binding type for global (game application) script easing calling functions.
///
class FDyGlobalScriptState final
{
public:
  FDyGlobalScriptState(_MIN_ const PDyScriptInstanceMetaInfo& descriptor); 

  void CallStart();
  void CallEnd();

  /// @brief Get script type (Cpp, Lua).
  MDY_NODISCARD EDyScriptType GetScriptType() const noexcept;
  /// @brief Get internal widget script instance (level 2)
  MDY_NODISCARD CDyGlobalScriptBase* MDY_PRIVATE_SPECIFIER(GetPtrInternalGlobalScript)() const noexcept;

private:
  /// @brief Widget script pointer instance.
  std::unique_ptr<CDyGlobalScriptBase> mScriptInstance = MDY_INITIALIZE_NULL;
  /// @brief Internal script type of this status instance.
  EDyScriptType  mType    = EDyScriptType::NoneError;
};

} /// ::dy namespace

#endif /// GAURD_DY_COMPONENT_INTERNAL_SCRIPT_FDyGlobalScriptStatus_H
