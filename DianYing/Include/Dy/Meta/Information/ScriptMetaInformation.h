#ifndef GUARD_DY_META_INFORMATION_SCRIPTMETAINFORMATION_H
#define GUARD_DY_META_INFORMATION_SCRIPTMETAINFORMATION_H
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

#include <nlohmann/json_fwd.hpp>

namespace dy
{

///
/// @enum   EDyScriptType
/// @brief  Meta script type. `NoneError` must not be valid and seen on runtime.
///
enum class EDyScriptType
{
  Cpp,
  Lua,
  NoneError
};

void to_json(_MINOUT_ nlohmann::json& j, _MIN_ const EDyScriptType& p);
void from_json(_MIN_ const nlohmann::json& j, _MINOUT_ EDyScriptType& p);

///
/// @struct PDyScriptReferenceMetaInfo
/// @brief  Reference meta information to `PDyScriptInstanceMetaInfo`. \n
/// specifier must be valid on runtime.
///
struct PDyScriptReferenceMetaInfo final
{
  std::string mSpecifierName = MDY_INITILAIZE_EMPTYSTR;
};

void to_json(_MINOUT_ nlohmann::json& j, _MIN_ const PDyScriptReferenceMetaInfo& p);
void from_json(_MIN_ const nlohmann::json& j, _MINOUT_ PDyScriptReferenceMetaInfo& p);

///
/// @struct PDyScriptInstanceMetaInfo
/// @brief  Instance meta information of `CDyScript` base type.
///
struct PDyScriptInstanceMetaInfo final
{
  EDyScriptType mScriptType    = EDyScriptType::NoneError;
  std::string   mSpecifierName = MDY_INITILAIZE_EMPTYSTR;
  std::string   mFilePath      = "";
  bool          mIsCompressed  = false;
};

void to_json(_MINOUT_ nlohmann::json& j, _MIN_ const PDyScriptInstanceMetaInfo& p);
void from_json(_MIN_ const nlohmann::json& j, _MINOUT_ PDyScriptInstanceMetaInfo& p);

} /// ::dy namespace

#endif /// GUARD_DY_META_INFORMATION_SCRIPTMETAINFORMATION_H