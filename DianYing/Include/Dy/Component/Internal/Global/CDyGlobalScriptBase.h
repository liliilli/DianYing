#ifndef GUARD_DY_COMPONENT_INTERNAL_GLOBAL_CDyGlobalScriptBase_H
#define GUARD_DY_COMPONENT_INTERNAL_GLOBAL_CDyGlobalScriptBase_H
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

//!
//! Forward declaration
//!

namespace dy
{

class FDyUiWidget;

} /// ::dy namespace

//!
//! Implementation
//!

namespace dy
{

///
/// @class CDyGlobalScriptBase
/// @brief Widget script base type. this type should not be used independently. \n
/// Instead of this, just use `CDyWidgetScript`.
///
class CDyGlobalScriptBase
{
public:
  virtual ~CDyGlobalScriptBase() = default;
  virtual void OnStart() = 0;
  virtual void OnEnd() = 0;

  /// Script name for specification and searching.
  std::string mScriptName = "";
};

} /// ::dy namespace

#endif /// GUARD_DY_COMPONENT_INTERNAL_GLOBAL_CDyGlobalScriptBase_H