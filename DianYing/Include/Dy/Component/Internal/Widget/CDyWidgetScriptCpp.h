#ifndef GUARD_DY_COMPONENT_UI_CDYWIDGETSCRIPTCPP_H
#define GUARD_DY_COMPONENT_UI_CDYWIDGETSCRIPTCPP_H
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

#include <Dy/Component/Internal/Widget/CDyWidgetScriptBase.h>
#include <Dy/Component/Abstract/ADyWidgetCppScript.h>
#include <Dy/Meta/Information/ComponentMetaInformation.h>

namespace dy
{

///
/// @class CDyWidgetScriptCpp
/// @brief Only for cpp internal widget script component.
///
class CDyWidgetScriptCpp final : public CDyWidgetScriptBase 
{
public:
  CDyWidgetScriptCpp(_MIN_ FDyUiWidget& widgetReference, _MIN_ const PDyScriptInstanceMetaInfo& descriptor); 
  virtual ~CDyWidgetScriptCpp() = default;

  /// @brief  Get `most derived` `low-level` script instance pointer.
  MDY_NODISCARD ADyWidgetCppScript* MDY_PRIVATE_SPECIFIER(GetScriptInstance)() const noexcept; 

private:
  void Initiate()       override final;
  void Start()          override final;
  void Update(float dt) override final;
  void OnEnabled()      override final;
  void OnDisabled()     override final;
  void Destroy()        override final;

  /// Script instance.
  std::unique_ptr<ADyWidgetCppScript> mScriptInstance = nullptr;
};

} /// ::dy namespace

#endif /// GUARD_DY_COMPONENT_UI_CDYWIDGETSCRIPTCPP_H