#ifndef GUARD_DY_MANAGEMENT_HELPER_SDYPROFILINGHELPER_H
#define GUARD_DY_MANAGEMENT_HELPER_SDYPROFILINGHELPER_H
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

///
/// @class SDyProfilingHelper
/// @brief Helper class for communicating internal profiling manager.
///
class SDyProfilingHelper final
{
public:
  /// @brief Increase on-bind texture count.
  static void IncreaseOnBindTextureCount(_MIN_ TU32 iInput) noexcept;
  /// @brief Decrease on-bind texture count.
  static void DecreaseOnBindTextureCount(_MIN_ TU32 iInput) noexcept;

};

} /// ::dy namespace

#endif /// GUARD_DY_MANAGEMENT_HELPER_SDYPROFILINGHELPER_H