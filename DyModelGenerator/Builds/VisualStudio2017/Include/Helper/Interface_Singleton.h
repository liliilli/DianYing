#pragma once
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

#include "HelperMacroes.h"
#include "Global_EnumSuccess.h"
#include "../JuceLibraryCode/JuceHeader.h"

template <typename TType>
class Interface_Singleton
{
public:
  /// @brief Return reference of instance of MDyTime manager.
  /// This function is thread safe.
  static TType& GetInstance() noexcept
  {
    static TType instance;
    return instance;
  }

  /// @brief Initialize singleton.
  MDY_NODISCARD EDySuccess static Initialize() noexcept
  {
    jassert(Interface_Singleton<TType>::mIsInitialized == false);
    jassert(Interface_Singleton<TType>::mIsShutdown    == false);

    const auto flag = GetInstance().pfInitialize();

    bool oldValue = false;
    Interface_Singleton<TType>::mIsInitialized.compare_exchange_weak(oldValue, true);
    return flag;
  }

  /// @brief Shutdown singleton.
  MDY_NODISCARD EDySuccess static Release() noexcept
  {
    jassert(Interface_Singleton<TType>::mIsInitialized == true);
    jassert(Interface_Singleton<TType>::mIsShutdown    == false);

    const auto flag = GetInstance().pfRelease();

    bool oldValue = false;
    Interface_Singleton<TType>::mIsShutdown.compare_exchange_weak(oldValue, true);
    return flag;
  }

  /// @brief Check whether singleton is initialized.
  MDY_NODISCARD bool static IsInitialized() noexcept
  {
    return Interface_Singleton<TType>::mIsInitialized && !Interface_Singleton<TType>::mIsShutdown;
  }

  /// @brief Check whether singleton is shutdowned.
  MDY_NODISCARD bool static IsShutdowned() noexcept
  {
    return Interface_Singleton<TType>::mIsShutdown;
  }

protected:
  Interface_Singleton()                      = default;
  virtual ~Interface_Singleton()             = default;
  inline static std::atomic<bool> mIsInitialized = false;
  inline static std::atomic<bool> mIsShutdown    = false;

  MDY_SINGLETON_PROPERTIES(Interface_Singleton);
};
