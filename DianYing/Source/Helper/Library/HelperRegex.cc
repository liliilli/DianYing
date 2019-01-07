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
#include <Dy/Helper/Library/HelperRegex.h>
#include <regex>

namespace dy
{

std::vector<std::string> DyRegexCreateObjectParentSpecifierList(_MIN_ std::string iParentNameFullList)
{
  if (iParentNameFullList.empty() == true) { return {}; }

  static const std::regex parentMatchRegex(R"regex((\w+)(?:\.|$))regex");
  std::smatch parentSpecifierMatch;
  std::vector<std::string> parentSpecifierList {};
  while (std::regex_search(iParentNameFullList, parentSpecifierMatch, parentMatchRegex) == true) 
  { 
    parentSpecifierList.emplace_back(parentSpecifierMatch[1].str());
    iParentNameFullList = parentSpecifierMatch.suffix();
  }

  return parentSpecifierList;
}

bool DyRegexCheckIsCompressedDataFile(_MIN_ const std::string& iFileName)
{
  if (iFileName.empty() == true) { return false; }

  static const std::regex regexPattern(R"regex((Data\d{3}).dydat)regex");
  return std::regex_match(iFileName, regexPattern);
}

} /// ::dy namespace