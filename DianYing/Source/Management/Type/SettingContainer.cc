#include <precompiled.h>
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

/// Header file
#include <Dy/Management/Type/SettingContainer.h>
#include <nlohmann/json.hpp>
#include <nlohmann/adl_serializer.hpp>
#include <Dy/Helper/JsonHelper.h>

//!
//! Forward declaration
//!

namespace
{

//! DDySettingDescription

MDY_SET_IMMUTABLE_STRING(sProjectName,          "ProjectName");
MDY_SET_IMMUTABLE_STRING(sWindowName,           "WindowName");
MDY_SET_IMMUTABLE_STRING(sVersionHigh,          "VersionHigh");
MDY_SET_IMMUTABLE_STRING(sVersionMid,           "VersionMid");
MDY_SET_IMMUTABLE_STRING(sVersionLow,           "VersionLow");
MDY_SET_IMMUTABLE_STRING(sCompanyName,          "CompanyName");
MDY_SET_IMMUTABLE_STRING(sHomepage,             "Homepage");
MDY_SET_IMMUTABLE_STRING(sSupportContact,       "SupportContact");

//! DDySettingGameplay

MDY_SET_IMMUTABLE_STRING(sInitialScene,         "InitialScene");
MDY_SET_IMMUTABLE_STRING(sInitialResolution,    "InitialResolution");

MDY_SET_IMMUTABLE_STRING(sGamePlay_Shadow,                      "Shadow");
MDY_SET_IMMUTABLE_STRING(sGamePlay_Shadow_GlobalDefaultMapSize, "GlobalDefaultMapSize");

} /// ::unnamed namespace

//!
//! Implementation
//!

namespace dy
{

void to_json(_MINOUT_ nlohmann::json& j, _MIN_ const DDySettingDescription& p)
{
  j = nlohmann::json
  {
      {MSVSTR(sProjectName),  p.mProjectName},
      {MSVSTR(sWindowName),   p.mWindowName},

      {MSVSTR(sVersionHigh),  p.mVersionHigh},
      {MSVSTR(sVersionMid),   p.mVersionMid},
      {MSVSTR(sVersionLow),   p.mVersionLow},

      {MSVSTR(sCompanyName),    p.mCompanyName},
      {MSVSTR(sHomepage),       p.mHomepage},
      {MSVSTR(sSupportContact), p.mSupportContact},
  };
}

void from_json(_MIN_ const nlohmann::json& j, _MINOUT_ DDySettingDescription& p)
{
  p.mProjectName  = DyGetValue<std::string>(j, sProjectName);
  p.mWindowName   = DyGetValue<std::string>(j, sWindowName);

  p.mVersionHigh  = DyGetValue<TI32>(j, sVersionHigh);
  p.mVersionMid   = DyGetValue<TI32>(j, sVersionMid);
  p.mVersionLow   = DyGetValue<TI32>(j, sVersionLow);

  p.mCompanyName      = DyGetValue<std::string>(j, sCompanyName);
  p.mHomepage         = DyGetValue<std::string>(j, sHomepage);
  p.mSupportContact   = DyGetValue<std::string>(j, sSupportContact);
}

void to_json(nlohmann::json& j, const DDySettingGameplay& p)
{
  j = nlohmann::json
  {
      {MSVSTR(sInitialScene),       p.mInitialSceneSpecifier},
      {MSVSTR(sInitialResolution),  p.mInitialResolution},
      {MSVSTR(sGamePlay_Shadow),    p.mShadow},
  };
}

void from_json(const nlohmann::json& j, DDySettingGameplay& p)
{
  p.mInitialSceneSpecifier  = DyGetValue<std::string>         (j, sInitialScene);
  p.mInitialResolution      = DyGetValue<DDyVectorInt2>       (j, sInitialResolution);
  p.mShadow                 = DyGetValue<decltype(p.mShadow)> (j, sGamePlay_Shadow);
}

void to_json(nlohmann::json& j, const DDySettingGameplay::DDyShadow& p)
{
  j = nlohmann::json
  {
      {MSVSTR(sGamePlay_Shadow_GlobalDefaultMapSize), p.mShadowGlobalDefaultMap},
  };
}

void from_json(const nlohmann::json& j, DDySettingGameplay::DDyShadow& p)
{
  p.mShadowGlobalDefaultMap = DyGetValue<DDyVectorInt2>(j, sGamePlay_Shadow_GlobalDefaultMapSize);
}

} /// ::dy namespace