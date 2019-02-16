#ifndef GUARD_DY_MANAGEMENT_INTERNAL_MDYPROFILING_H
#define GUARD_DY_MANAGEMENT_INTERNAL_MDYPROFILING_H
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
  /// @brief Get texture count on bind `Dy` System. This function is atomic.
  MDY_NODISCARD TI32 GetOnBindTextureCount() const noexcept;
  /// @brief Get vertex count on bind `Dy` System. This function is atomic.
  MDY_NODISCARD TI32 GetOnBindVertexCount() const noexcept;
  /// @brief Get shader count on bind `Dy` System. This function is atomic.
  MDY_NODISCARD TI32 GetOnBindShaderCount() const noexcept;
  /// @brief Get total actor count of level on bind `Dy` System. This function is atomic.
  MDY_NODISCARD TI32 GetOnBindActorCount() const noexcept;
  /// @brief Get screen rendered actour count. This function is not atomic.
  MDY_NODISCARD TI32 GetScreenRenderedActorCount() const noexcept;

private:
  /// @brief Increment & Decrement texture count that texture is on bind `Dy` System. \n
  /// This function is atomic.
  void MDY_PRIVATE(AddTextureCount)(_MIN_ TI32 iInput) noexcept;
  /// @brief Increment & Decrement on-bind vertex count of `Dy` System. This function is atomic.
  void MDY_PRIVATE(AddOnBindVertexCount)(_MIN_ TI32 iInput) noexcept;
  /// @brief Increment & Decrement on-bind shader count of `Dy` System. This function is atomic.
  void MDY_PRIVATE(AddOnBindShaderCount)(_MIN_ TI32 iInput) noexcept;
  /// @brief Increment & Decrement on-bind shader count of `Dy` System. This function is atomic.
  void MDY_PRIVATE(AddOnBindActorCount)(_MIN_ TI32 iInput) noexcept;
  /// @brief Increment & Decrement `mScreenRenderedActorCount` by iInput.
  void MDY_PRIVATE(AddScreenRenderedActorCount)(_MIN_ TI32 iInput) noexcept;

  /// @brief Reset `mScreenRenderedActorCount` to 0. This function is not atomic.
  void MDY_PRIVATE(ResetScreenRenderedActorCount)() noexcept;
  /// @brief Reset frame dependent count.
  /// `mScreenRenderedActorCount` will be reset.
  void MDY_PRIVATE(ResetFrameDependentCounts)() noexcept;

  RWLockAtomic<TI32> mOnBindTextureCount  = MDY_INITIALIZE_DEFUINT;

  RWLockAtomic<TI32> mOnBindVertexCount   = MDY_INITIALIZE_DEFUINT;
  RWLockAtomic<TI32> mScreenVertexCount   = MDY_INITIALIZE_DEFUINT;

  RWLockAtomic<TI32> mOnBindShaderCount   = MDY_INITIALIZE_DEFUINT;
  RWLockAtomic<TI32> mOnBindActorCount    = MDY_INITIALIZE_DEFUINT;
  TI32               mScreenRenderedActorCount = MDY_INITIALIZE_DEFUINT;

  friend class SDyProfilingHelper;
};

} /// ::dy namespace

#endif /// GUARD_DY_MANAGEMENT_INTERNAL_MDYPROFILING_H