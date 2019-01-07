#include <precompiled.h>
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

/// Header file
#include <Dy/Meta/Information/ScriptMetaInformation.h>
#include <Dy/Helper/Library/HelperJson.h>

namespace
{

MDY_SET_IMMUTABLE_STRING(sHeader_Specifier,   "Specifier");
MDY_SET_IMMUTABLE_STRING(sHeader_Type,        "Type");
MDY_SET_IMMUTABLE_STRING(sHeader_Path,        "Path");
MDY_SET_IMMUTABLE_STRING(sHeader_IsCompressed,"IsCompressed");

MDY_SET_IMMUTABLE_STRING(sValue_Cpp, "Cpp");
MDY_SET_IMMUTABLE_STRING(sValue_Lua, "Lua");

MDY_SET_IMMUTABLE_STRING(sValue_Widget, "Widget");
MDY_SET_IMMUTABLE_STRING(sValue_Actor,  "Actor");

} /// ::unnamed namespace

//!
//! Implementation
//!

namespace dy
{

void to_json(_MINOUT_ nlohmann::json& j, _MIN_ const EDyScriptType& p)
{
  MDY_NOT_IMPLEMENTED_ASSERT();
}

void from_json(_MIN_ const nlohmann::json& j, _MOUT_ EDyScriptType& p)
{
  const auto typeString = j.get<std::string>();
  if      (typeString == sValue_Cpp)  { p = EDyScriptType::Cpp; }
  else if (typeString == sValue_Lua)  { p = EDyScriptType::Lua; }
  else                                { MDY_UNEXPECTED_BRANCH(); }
}

void to_json(_MINOUT_ nlohmann::json& j, _MIN_ const EDyScriptMode& p)
{
  MDY_NOT_IMPLEMENTED_ASSERT();
}

void from_json(_MIN_ const nlohmann::json& j, _MINOUT_ EDyScriptMode& p)
{
  const auto typeString = j.get<std::string>();
  if      (typeString == sValue_Widget) { p = EDyScriptMode::Widget; }
  else if (typeString == sValue_Actor)  { p = EDyScriptMode::Actor; }
  else                                  { MDY_UNEXPECTED_BRANCH(); }
}

void to_json(_MINOUT_ nlohmann::json& j, _MIN_ const PDyScriptReferenceMetaInfo& p)
{
  j = nlohmann::json
  {
      {MSVSTR(sHeader_Specifier),  p.mSpecifierName},
  };
}

void from_json(_MIN_ const nlohmann::json& j, _MOUT_ PDyScriptReferenceMetaInfo& p)
{
  p.mSpecifierName = DyJsonGetValueFrom<std::string>(j, sHeader_Specifier);
}

void to_json(_MINOUT_ nlohmann::json& j, _MIN_ const PDyScriptInstanceMetaInfo& p)
{
  j = nlohmann::json
  {
      {MSVSTR(sHeader_Specifier),   p.mSpecifierName},
      {MSVSTR(sHeader_Type),        p.mScriptType},
      {MSVSTR(sHeader_Path),        p.mFilePath},
      {"Mode", p.mScriptMode},
      {MSVSTR(sHeader_IsCompressed),p.mIsCompressed}
  };
}

void from_json(_MIN_ const nlohmann::json& j, _MOUT_ PDyScriptInstanceMetaInfo& p)
{
  p.mSpecifierName  = DyJsonGetValueFrom<std::string>   (j, sHeader_Specifier);
  p.mScriptMode     = DyJsonGetValueFrom<EDyScriptMode> (j, "Mode");
  p.mScriptType     = DyJsonGetValueFrom<EDyScriptType> (j, sHeader_Type);
  p.mFilePath       = DyJsonGetValueFrom<std::string>   (j, sHeader_Path);
  p.mIsCompressed   = DyJsonGetValueFrom<bool>          (j, sHeader_IsCompressed);
}

} /// ::dy namespace