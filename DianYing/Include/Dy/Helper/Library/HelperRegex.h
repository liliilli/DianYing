#ifndef GUARD_DY_HELPER_SYSTEM_REGEX_H
#define GUARD_DY_HELPER_SYSTEM_REGEX_H
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

namespace dy
{

/// @brief Generate and get parent specifier list. \n
MDY_NODISCARD std::vector<std::string> 
DyRegexCreateObjectParentSpecifierList(_MIN_ std::string iParentNameFullList); 

/// @brief Check file name is following format `Data###(\d{3}).dydat` file name.
MDY_NODISCARD bool DyRegexCheckIsCompressedDataFile(_MIN_ const std::string& iFileName);

} /// ::dy namespace

#endif /// GUARD_DY_HELPER_SYSTEM_REGEX_H