#ifndef GUARD_DY_HELPER_DBG_CHECK_USAGE_H
#define GUARD_DY_HELPER_DBG_CHECK_USAGE_H
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

#include <Dy/Management/Interface/ISingletonCrtp.h>

namespace dy
{

class MDyUsageChecker final : public ISingleton<MDyUsageChecker>
{
  MDY_SINGLETON_PROPERTIES(MDyUsageChecker);
  MDY_SINGLETON_DERIVED(MDyUsageChecker);
public:

  ///
  /// @brief Update cpu usage graph with dt.
  ///
  void UpdateCpuUsageGraph(float dt);

  ///
  /// @brief Update cpu usgae graph with dt.
  ///
  void UpdateRanUsageGraph(float dt);

  ///
  /// @brief Reset cpu usage value array.
  ///
  void ResetCpuUsageGraph();

  ///
  /// @brief Reset cpu usage value array.
  ///
  void ResetRamUsageGraph();

private:


};

} /// ::dy namespace

#endif /// GUARD_DY_HELPER_DBG_CHECK_USAGE_H