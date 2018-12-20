#ifndef GUARD_DY_MANAGEMENT_INTERNAL_MDYPROFILING_H
#define GUARD_DY_MANAGEMENT_INTERNAL_MDYPROFILING_H
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
#include <Dy/Helper/Internal/RWLockAtomic.h>

namespace dy
{

/// @class MDyProfiling
/// @brief Profiliing manager.
class MDyProfiling final : public IDySingleton<MDyProfiling>
{
  MDY_SINGLETON_DERIVED(MDyProfiling);
  MDY_SINGLETON_PROPERTIES(MDyProfiling);
public:
  /// @brief Get texture count on bind `Dy` System. \n
  /// This function is atomic.
  MDY_NODISCARD TI32 GetOnBindTextureCount() const noexcept;

private:
  /// @brief Increment & Decrement texture count that texture is on bind `Dy` System. \n
  /// This function is atomic.
  void MDY_PRIVATE_SPECIFIER(AddTextureCount)(TI32 iInput) noexcept;

  RWLockAtomic<TI32> mOnBindTextureCount = MDY_INITIALIZE_DEFUINT;

  friend class SDyProfilingHelper;
};

} /// ::dy namespace

#endif /// GUARD_DY_MANAGEMENT_INTERNAL_MDYPROFILING_H