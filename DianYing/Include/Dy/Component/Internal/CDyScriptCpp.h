#ifndef GUARD_DY_COMPONENT_INTERNAL_CDYCPPSCRIPT_H
#define GUARD_DY_COMPONENT_INTERNAL_CDYCPPSCRIPT_H
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

#include <Dy/Component/CDyScript.h>
#include <Dy/Component/Interface/IDyInitializeHelper.h>
#include <Dy/Component/Interface/IDyCppScript.h>

namespace dy
{

///
/// @class CDyScriptCpp
/// @brief Only for cpp internal script component.
///
class CDyScriptCpp final : public CDyScript, public IDyInitializeHelper<PDyScriptComponentMetaInfo>
{
  MDY_SET_TYPEMATCH_FUNCTION(::dy::CDyScript, CDyScriptCpp);
  MDY_SET_CRC32_HASH_WITH_TYPE(CDyScriptCpp);

  CDyScriptCpp(FDyActor& actorReference) : CDyScript(actorReference) {};
  virtual ~CDyScriptCpp() = default;

  EDySuccess Initialize(const PDyScriptComponentMetaInfo& descriptor) override final;
  void Release() override final;

  void pScriptRelease() override final
  {
    return this->Release();
  }

private:
  void Initiate()       override final;
  void Start()          override final;
  void Update(float dt) override final;
  void OnEnabled()      override final;
  void OnDisabled()     override final;
  void Destroy()        override final;

  /// Script instance.
  std::unique_ptr<IDyCppScript> mScriptInstance = nullptr;
};

} /// ::dy namespace

#endif /// GUARD_DY_COMPONENT_INTERNAL_CDYCPPSCRIPT_H