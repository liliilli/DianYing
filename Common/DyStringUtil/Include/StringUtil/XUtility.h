#pragma once
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

#include <string>
#include <sstream>

#ifdef _WIN32
  __pragma(warning(push))
  __pragma(warning(disable:4244))
#endif

namespace dy
{

/// @brief Convert string (basic_string<char>) to wstring (basic_string<wchar_t>).
inline std::wstring ConvertStringToWString(const std::string& string)
{
  std::wstring wideStr;
  const auto& ctfacet = std::use_facet<std::ctype<wchar_t>>(std::wstringstream().getloc());

  for (const auto& currChar : string)
  {
    wideStr.push_back(ctfacet.widen(currChar));
  } 

  return wideStr;
}

/// @brief Convert wstring (basic_string<wchar_t>) to string (basic_string<char>).
inline std::string ConvertWStringToString(const std::wstring& wstring)
{
  std::string string;
  const auto& ctfacet = std::use_facet<std::ctype<char>>(std::stringstream().getloc());

  for (const auto& currChar : wstring)
  {
    string.push_back(ctfacet.narrow(currChar));
  } 

  return string;
}

} /// ::dy namespace

#ifdef _WIN32
  __pragma(warning(pop))
#endif
