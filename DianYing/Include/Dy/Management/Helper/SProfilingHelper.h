#ifndef GUARD_DY_MANAGEMENT_HELPER_SDYPROFILINGHELPER_H
#define GUARD_DY_MANAGEMENT_HELPER_SDYPROFILINGHELPER_H
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

/// @class SProfilingHelper
/// @brief Helper class for communicating internal profiling manager.
class SProfilingHelper final
{
public:
  /// @brief Increase on-bind texture count.
  static void IncreaseOnBindTextureCount(TU32 iInput) noexcept;
  /// @brief Decrease on-bind texture count.
  static void DecreaseOnBindTextureCount(TU32 iInput) noexcept;

  /// @brief Increase on-bind vertex count.
  static void IncreaseOnBindVertexCount(TU32 iInput) noexcept;
  /// @brief Decrease on-bind vertex count.
  static void DecreaseOnBindVertexCount(TU32 iInput) noexcept;

  /// @brief Increase on-bind shader count.
  static void IncreaseOnBindShaderCount(TU32 iInput) noexcept;
  /// @brief Decrease on-bind shader count.
  static void DecreaseOnBindShaderCount(TU32 iInput) noexcept;

  /// @brief Increase on-bind scene actor count.
  static void IncreaseOnBindActorCount(TU32 iInput) noexcept;
  /// @brief Decrease on-bind scene actor count.
  static void DecreaseOnBindActorCount(TU32 iInput) noexcept;

  /// @brief Increase rendered scene actor count.
  static void IncreaseRenderedActorCount(TU32 iInput) noexcept;
  /// @brief Decrease rendered scene actor count.
  static void DecreaseRenderedActorCount(TU32 iInput) noexcept;

  /// @brief 
  static void AddScreenRenderedActorCount(TI32 iInput) noexcept;
  /// @brief
  static void ResetFrameDependentCounts() noexcept;
};

} /// ::dy namespace

#endif /// GUARD_DY_MANAGEMENT_HELPER_SDYPROFILINGHELPER_H