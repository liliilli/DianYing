#ifndef GUARD_DY_COMPONENT_INTERNAL_CDYCPPSCRIPT_H
#define GUARD_DY_COMPONENT_INTERNAL_CDYCPPSCRIPT_H
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

#include <Dy/Component/Abstract/ADyActorCppScript.h>
#include <Dy/Component/Internal/Actor/CDyActorScriptBase.h>

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
/// @class CDyActorScriptCpp
/// @brief Only for cpp internal script component.
///
class CDyActorScriptCpp final : public CDyActorScriptBase
{
public:
  CDyActorScriptCpp(_MIN_ FDyActor& actorReference, _MIN_ const PDyScriptInstanceMetaInfo& iDesc);
  virtual ~CDyActorScriptCpp() = default;

  /// @brief  Get `most derived` `low-level` script instance pointer.
  MDY_NODISCARD ADyActorCppScript* MDY_PRIVATE(GetScriptInstance)() const noexcept; 

private:
  void Initiate()       override final;
  void Start()          override final;
  void Update(_MIN_ float dt) override final;
  void OnEnabled()      override final;
  void OnDisabled()     override final;
  void Destroy()        override final;

  /// Script instance.
  std::unique_ptr<ADyActorCppScript> mScriptInstance = nullptr;
};

} /// ::dy namespace

#endif /// GUARD_DY_COMPONENT_INTERNAL_CDYCPPSCRIPT_H