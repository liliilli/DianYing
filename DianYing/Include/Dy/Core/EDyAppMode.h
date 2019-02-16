#ifndef GUARD_DY_CORE_EDYAPPMODE_H
#define GUARD_DY_CORE_EDYAPPMODE_H
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

namespace dy
{

/// @enum  EDyAppMode
/// @brief Global application mode.
enum class EDyAppMode
{
  // If -d not setup, just do this that.
  ModeRuntime,
};

/// @enum  EDyFileLoadingMode
/// @brief File loading mode.
enum class EDyFileLoadingMode
{
  LoadSeperatedFile,  // Load separated file. When `-d` is enabled, this mode is called.
  /// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  /// @macro MDY_FLAG_MODE_POPULATE_COMPRESSED_DATAFILE
  /// @brief If this flag is set, neither initialization a project nor running game,
  /// but just bind every resources into compressed files, running procedure monitoring window.
  ///
  /// When run project with this, any specified file name is exist, procedure will not take off.
  /// - Data###.dydat (^Data(\t){3}.dydat$)
  /// 
  /// Compressed .dydat file will be detected by application,
  /// when MDY_FLAG_LOAD_COMPRESSED_DATAFILE is set.
  ///
  /// ** SEQUENCE... **
  /// 1. define MDY_FLAG_MODE_POPULATE_COMPRESSED_DATAFILE.
  /// 2. M_PATH_PLAIN_PATH_OF_SETTING_JSON string literal must specify the path of "Setting.json"
  /// 2. build project and run program. Mode chagned to compression mode, wait until procedure finished.
  /// 3. undefine MDY_FLAG_MODE_POPULATE_COMPRESSED_DATAFILE
  /// 4. define MDY_FLAG_LOAD_COMPRESSED_DATAFILE, so compressed `Data###.dydat` must be loaded instead of plain json and resources. 
  /// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  LoadCompressedFile, // Load compressed file.
};

} /// ::dy namespace

#endif /// GUARD_DY_CORE_EDYAPPMODE_H
