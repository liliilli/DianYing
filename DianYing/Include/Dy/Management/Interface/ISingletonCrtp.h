#ifndef GUARD_DY_MANAGEMENT_INTERFACE_ISINGLETONCRTP_H
#define GUARD_DY_MANAGEMENT_INTERFACE_ISINGLETONCRTP_H
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

#include <atomic>

#include <Dy/Helper/Macroes.h>

namespace dy
{

template <typename TType>
class ISingleton
{
public:
  ///
  /// @brief Return reference of instance of MDyTime manager.
  /// This function is thread safe.
  ///
  FORCEINLINE static TType& GetInstance() noexcept
  {
    static TType instance;
    return instance;
  }

  ///
  /// @brief Initialize singleton.
  ///
  [[nodiscard]] FORCEINLINE EDySuccess static Initialize() noexcept
  {
    MDY_ASSERT(ISingleton<TType>::mIsInitialized == false, "Singleton instance must be initialized only once.");
    MDY_ASSERT(ISingleton<TType>::mIsShutdown    == false, "Singleton instance can not be reinitialized after shutting down.");

    const auto flag = GetInstance().pfInitialize();

    bool oldValue = false;
    ISingleton<TType>::mIsInitialized.compare_exchange_weak(oldValue, true);
    return flag;
  }

  ///
  /// @brief Shutdown singleton.
  ///
  [[nodiscard]] FORCEINLINE EDySuccess static Release() noexcept
  {
    MDY_ASSERT(ISingleton<TType>::mIsInitialized == true , "Singleton instance must be initialized before shutting down.");
    MDY_ASSERT(ISingleton<TType>::mIsShutdown    == false, "Singleton instance can not be shutted down again.");

    const auto flag = GetInstance().pfRelease();

    bool oldValue = false;
    ISingleton<TType>::mIsShutdown.compare_exchange_weak(oldValue, true);
    return flag;
  }

  ///
  /// @brief Check whether singleton is initialized.
  ///
  [[nodiscard]] FORCEINLINE bool static IsInitialized() noexcept
  {
    return ISingleton<TType>::mIsInitialized && !ISingleton<TType>::mIsShutdown;
  }

  ///
  /// @brief Check whether singleton is shutdowned.
  ///
  [[nodiscard]] FORCEINLINE bool static IsShutdowned() noexcept
  {
    return ISingleton<TType>::mIsShutdown;
  }

protected:
  ISingleton()                      = default;
  virtual ~ISingleton()             = default;
  inline static std::atomic<bool> mIsInitialized = false;
  inline static std::atomic<bool> mIsShutdown    = false;

  MDY_SINGLETON_PROPERTIES(ISingleton);
};

} /// ::dy namespace

#endif /// GUARD_DY_MANAGEMENT_INTERFACE_ISINGLETONCRTP_H