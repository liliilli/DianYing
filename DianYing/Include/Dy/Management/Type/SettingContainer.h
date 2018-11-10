#ifndef GUARD_DY_MANAGEMENT_TYPE_SETTINGCONTAINER_H
#define GUARD_DY_MANAGEMENT_TYPE_SETTINGCONTAINER_H
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

#include <Dy/Helper/GlobalType.h>
#include <nlohmann/json_fwd.hpp>
#include <Dy/Helper/Type/VectorInt2.h>

namespace dy
{

///
/// @struct DDySettingDescription
/// @brief  Setting description able to be serialized.
///
struct DDySettingDescription final
{
  /// Project name of this project.
  std::string mProjectName    = MDY_INITILAIZE_EMPTYSTR;
  /// Window name of this project. if empty, just use MDySetting::mProjectName as window name with version.
  std::string mWindowName     = MDY_INITILAIZE_EMPTYSTR;
  /// [High. Mid. Low]
  TI32 mVersionHigh           = MDY_INITIALIZE_DEFINT;
  TI32 mVersionMid            = MDY_INITIALIZE_DEFINT;
  TI32 mVersionLow            = MDY_INITIALIZE_DEFINT;
  ///
  std::string mCompanyName    = MDY_INITILAIZE_EMPTYSTR;
  std::string mHomepage       = MDY_INITILAIZE_EMPTYSTR;
  std::string mSupportContact = MDY_INITILAIZE_EMPTYSTR;
};

void to_json  (_MINOUT_ nlohmann::json& j,    _MIN_ const DDySettingDescription& p);
void from_json(_MIN_ const nlohmann::json& j, _MOUT_ DDySettingDescription& p);

///
/// @struct DDySettingGameplay
/// @brief  Setting gameplay that able to be serialized.
///
struct DDySettingGameplay final
{
  struct DDyShadow final
  { /// Global default shadow map size.
    DDyVectorInt2 mShadowGlobalDefaultMap = {};
  };

  /// Initial scene name. (or uuid?)
  std::string   mInitialSceneSpecifier           = MDY_INITILAIZE_EMPTYSTR;
  /// Initial game resolution scene.
  DDyVectorInt2 mInitialResolution      = {};
  /// Shadow default map resolution or properties.
  DDyShadow     mShadow                 = {};
};

void to_json  (_MINOUT_ nlohmann::json& j,    _MIN_ const DDySettingGameplay& p);
void from_json(_MIN_ const nlohmann::json& j, _MOUT_ DDySettingGameplay& p);

void to_json  (_MINOUT_ nlohmann::json& j,    _MIN_ const DDySettingGameplay::DDyShadow& p);
void from_json(_MIN_ const nlohmann::json& j, _MOUT_ DDySettingGameplay::DDyShadow& p);

} /// ::dy namespace

#endif /// GUARD_DY_MANAGEMENT_TYPE_SETTINGCONTAINER_H