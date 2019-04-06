#ifndef GUARD_DY_JSONTYPEWRITERHELPER_H
#define GUARD_DY_JSONTYPEWRITERHELPER_H
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

#include <nlohmann/json.hpp>

#include <boost/uuid/uuid.hpp>

namespace dy
{

struct DMeta final
{
  std::string         mFontSpecifierName;
  std::string         mFontStyleSpecifier;
  int32_t             mHoriLinefeed{};
  boost::uuids::uuid  mUuid{};
};

void to_json(nlohmann::json& j, const DMeta& p);
void from_json(const nlohmann::json& j, DMeta& p);

} /// ::dy namespace

#endif /// GUARD_DY_JSONTYPEWRITERHELPER_H