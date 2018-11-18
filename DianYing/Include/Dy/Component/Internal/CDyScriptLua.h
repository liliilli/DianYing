#ifndef GUARD_DY_COMPONENT_INTERNAL_CDYSCRIPTLUA_H
#define GUARD_DY_COMPONENT_INTERNAL_CDYSCRIPTLUA_H
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

#include <sol2/sol.hpp>
#include <Dy/Component/CDyScript.h>
#include <Dy/Component/Interface/IDyInitializeHelper.h>

namespace dy
{

///
/// @class CDyScriptLua
/// @brief Only for lua internal script component.
///
class CDyScriptLua final : public CDyScript, public IDyInitializeHelper<PDyScriptComponentMetaInfo>
{
  MDY_SET_TYPEMATCH_FUNCTION(::dy::CDyScript, CDyScriptLua);
  MDY_SET_CRC32_HASH_WITH_TYPE(CDyScriptLua);

  CDyScriptLua(FDyActor& actorReference) : CDyScript(actorReference) {};
  virtual ~CDyScriptLua() = default;

  EDySuccess Initialize(const PDyScriptComponentMetaInfo& descriptor) override final;
  void Release() override final;

  void pScriptRelease() override final
  {
    return this->Release();
  }

private:
  void Initiate() override final;
  void Start() override final;
  void Update(float dt) override final;
  void OnEnabled() override final;
  void OnDisabled() override final;
  void Destroy() override final;

  /// Script path to execute lua script file.
  MDY_TRANSIENT std::string mScriptPath   = MDY_INITIALIZE_EMPTYSTR;
  /// Script instance.
  sol::table                mScriptInstance;
};

} /// ::dy namespace

#endif /// GUARD_DY_COMPONENT_INTERNAL_CDYSCRIPTLUA_H