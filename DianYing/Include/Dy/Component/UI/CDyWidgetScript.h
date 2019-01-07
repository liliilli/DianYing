#ifndef GUARD_DY_COMPONENT_UI_CDYWIDGETSCRIPT_H
#define GUARD_DY_COMPONENT_UI_CDYWIDGETSCRIPT_H
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

//!
//! Forward declaration
//!

namespace dy
{
class FDyUiWidget;
class FDyWidgetScriptState;
} /// ::dy namespace

//!
//! Implementation
//!

namespace dy
{

///
/// @class CDyWidgetScript
/// @brief Widget script component for binding internal script instance.
///
class CDyWidgetScript final
{
public:
  CDyWidgetScript(_MIN_ const std::string& iScriptSpecifier, _MINOUT_ FDyUiWidget& iWidget);
  ~CDyWidgetScript();

private:
  FDyUiWidget*          mPtrWidget        = MDY_INITIALIZE_NULL;
  FDyWidgetScriptState* mPtrWidgetStatus  = MDY_INITIALIZE_NULL;
};

} /// ::dy namespace

#endif /// GUARD_DY_COMPONENT_UI_CDYWIDGETSCRIPTBASE_H