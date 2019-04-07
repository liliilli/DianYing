#ifndef GUARD_DY_HELPER_LIBRARY_HelperString_INL
#define GUARD_DY_HELPER_LIBRARY_HelperString_INL
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

#include <Dy/Helper/Library/HelperJson.h>
#define FMT_HEADER_ONLY
#include <spdlog/fmt/fmt.h>

namespace dy
{

template <typename... TArgs>
std::string MakeStringU8(const std::string& iStringFormat, TArgs&&... iArgs)
{
  return fmt::format(iStringFormat, std::forward<TArgs>(iArgs)...);
}

} /// ::dy namespace

#endif /// GUARD_DY_HELPER_LIBRARY_HelperString_INL