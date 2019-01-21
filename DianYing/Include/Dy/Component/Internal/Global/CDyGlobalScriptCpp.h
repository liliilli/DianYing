#ifndef GUARD_DY_COMPONENT_UI_CDYWIDGETSCRIPTCPP_H
#define GUARD_DY_COMPONENT_UI_CDYWIDGETSCRIPTCPP_H
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

#include <Dy/Component/Internal/Global/CDyGlobalScriptBase.h>
#include <Dy/Component/Abstract/ADyGlobalCppScript.h>
#include <Dy/Meta/Information/ComponentMetaInformation.h>

namespace dy
{

///
/// @class CDyGlobalScriptCpp
/// @brief Only for cpp internal global (game application) script component.
///
class CDyGlobalScriptCpp final : public CDyGlobalScriptBase
{
public:
  CDyGlobalScriptCpp(_MIN_ const PDyScriptInstanceMetaInfo& descriptor); 
  virtual ~CDyGlobalScriptCpp() = default;

  /// @brief  Get `most derived` `low-level` script instance pointer.
  MDY_NODISCARD ADyGlobalCppScript* MDY_PRIVATE_SPECIFIER(GetScriptInstance)() const noexcept; 

private:
  void OnStart() override final;
  void OnEnd()   override final;

  /// Script instance.
  std::unique_ptr<ADyGlobalCppScript> mScriptInstance = nullptr;
};

} /// ::dy namespace

#endif /// GUARD_DY_COMPONENT_UI_CDYWIDGETSCRIPTCPP_H