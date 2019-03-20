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
#include <Dy/Helper/IoHelper.h>

namespace dy
{

std::string DyGetFileNameFromPath(const std::string& path) noexcept
{
  auto start = path.find_last_of('/');
  bool isSlashFound = false;
  if (start != std::string::npos)
  {
    isSlashFound = true;
  }
  else
  {
    start = path.find_last_of('\\');
    if (start != std::string::npos)
    {
      isSlashFound = true;
    }
    else
    {
      start = 0;
    }
  }

  auto count = path.find_last_of('.');
  if (count != std::string::npos)
  {
    count -= start;
  }

  if (isSlashFound)
    return path.substr(start + 1, count - 1);
  else
    return path.substr(start, count);
}

} /// ::dy namespace