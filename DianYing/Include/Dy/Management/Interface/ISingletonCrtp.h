#ifndef GUARD_DY_MANAGEMENT_INTERFACE_ISINGLETONCRTP_H
#define GUARD_DY_MANAGEMENT_INTERFACE_ISINGLETONCRTP_H
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

#include <atomic>

#include <Dy/Helper/System/Macroes.h>

namespace dy
{
  
/// @macro MDY_SINGLETON_PROPERTIES
/// This macro must not be attached to whichever class does not inherits ISingleton<>.
#define MDY_SINGLETON_PROPERTIES(__MASingletonType__) \
public: \
    __MASingletonType__(const __MASingletonType__##&) = delete; \
    __MASingletonType__(__MASingletonType__##&&) noexcept = delete; \
    __MASingletonType__##& operator=(const __MASingletonType__##&) = delete; \
    __MASingletonType__##& operator=(__MASingletonType__##&&) noexcept = delete

/// @macro MDY_SINGLETON_DERIVED
/// This macro must not be attached to whichever class does not inherits ISingleton<>.
#define MDY_SINGLETON_DERIVED(__MADerivedSingletonType__) \
private:                                                  \
    __MADerivedSingletonType__() = default;               \
    [[nodiscard]] EDySuccess pfInitialize();              \
    [[nodiscard]] EDySuccess pfRelease();                 \
    friend class ISingleton<__MADerivedSingletonType__>;  \
public:                                                   \
    virtual ~__MADerivedSingletonType__() = default;

/// @def DY_PIMPL_SINGELTON_DERIVED
/// This macro must not be attached to whichever class does not inherit ISingleton<>.
#define DY_PIMPL_SINGELTON_DERIVED(__MADerivedSingletonType__) \
    DY_DECLARE_PIMPL();                                   \
  public:                                                 \
    __MADerivedSingletonType__() = default;               \
    [[nodiscard]] EDySuccess pfInitialize();              \
    [[nodiscard]] EDySuccess pfRelease();                 \
    friend class ISingleton<__MADerivedSingletonType__>;  \
public:                                                   \
    virtual ~__MADerivedSingletonType__(); 

template <typename TType>
class ISingleton
{
public:
  /// @brief Return reference of instance of MTime manager.
  /// This function is thread safe.
  FORCEINLINE static TType& GetInstance() noexcept
  {
    static TType instance;
    return instance;
  }

  /// @brief Initialize singleton.
  FORCEINLINE MDY_NODISCARD EDySuccess static Initialize() noexcept
  {
    MDY_ASSERT_MSG(ISingleton<TType>::mIsInitialized == false, "Singleton instance must be initialized only once.");
    MDY_ASSERT_MSG(ISingleton<TType>::mIsShutdown    == false, "Singleton instance can not be reinitialized after shutting down.");

    const auto flag = GetInstance().pfInitialize();

    bool oldValue = false;
    ISingleton<TType>::mIsInitialized.compare_exchange_weak(oldValue, true);
    return flag;
  }

  /// @brief Shutdown singleton.
  FORCEINLINE MDY_NODISCARD EDySuccess static Release() noexcept
  {
    MDY_ASSERT_MSG(ISingleton<TType>::mIsInitialized == true , "Singleton instance must be initialized before shutting down.");
    MDY_ASSERT_MSG(ISingleton<TType>::mIsShutdown    == false, "Singleton instance can not be shutted down again.");

    const auto flag = GetInstance().pfRelease();

    bool oldValue = false;
    ISingleton<TType>::mIsShutdown.compare_exchange_weak(oldValue, true);
    return flag;
  }

  /// @brief Check whether singleton is initialized.
  FORCEINLINE MDY_NODISCARD bool static IsInitialized() noexcept
  {
    return ISingleton<TType>::mIsInitialized && !ISingleton<TType>::mIsShutdown;
  }

  /// @brief Check whether singleton is shutdowned.
  FORCEINLINE MDY_NODISCARD bool static IsShutdowned() noexcept
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