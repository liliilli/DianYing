#include "precompiled.h"
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

#include <Include/Library/JsonTypeWriterHelper.h>
#include <boost/lexical_cast.hpp>
#include <boost/uuid/uuid_io.hpp>

namespace dy
{

void to_json(nlohmann::json& j, const DMeta& p)
{
  j = nlohmann::json
  {
    {"FontSpecifierName",  p.mFontSpecifierName},
    {"FontStyleSpecifier", p.mFontStyleSpecifier},
    {"HoriLinefeed",       p.mHoriLinefeed},
    {"Uuid",               boost::lexical_cast<std::string>(p.mUuid)}
  };
}

inline void from_json(const nlohmann::json& j, DMeta& p)
{
  p.mFontSpecifierName  = j.at("FontSpecifierName").get<std::string>();
  p.mFontStyleSpecifier = j.at("FontStyleSpecifier").get<std::string>();
  p.mHoriLinefeed       = j.at("HoriLinefeed").get<int32_t>();
  p.mUuid               = boost::lexical_cast<boost::uuids::uuid>(j.at("Uuid").get<std::string>());
}

} /// ::dy namespace