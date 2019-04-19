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
#include <Expr/XMacroes.h>
#include <Expr/ESuccess.h>

namespace dy::expr
{
   
/// @macro EXPR_SINGLETON_PROPERTIES
/// @brief This macro must not be attached to whichever class does not inherits ISingleton<>.
#define EXPR_SINGLETON_PROPERTIES(__MASingletonType__) \
public: \
    __MASingletonType__(const EXPR_MAKENAME(__MASingletonType__,&)) = delete; \
    __MASingletonType__(EXPR_MAKENAME(__MASingletonType__,&&)) noexcept = delete; \
    EXPR_MAKENAME(__MASingletonType__,&) operator=(const EXPR_MAKENAME(__MASingletonType__,&)) = delete; \
    EXPR_MAKENAME(__MASingletonType__,&) operator=(EXPR_MAKENAME(__MASingletonType__,&&)) noexcept = delete

/// @macro EXPR_SINGLETON_DERIVED
/// @brief This macro must not be attached to whichever class does not inherits ISingleton<>.
#define EXPR_SINGLETON_DERIVED(__MADerivedSingletonType__) \
private:                                                  \
    __MADerivedSingletonType__() = default;               \
    [[nodiscard]] EDySuccess pfInitialize();              \
    [[nodiscard]] EDySuccess pfRelease();                 \
    friend class ISingleton<__MADerivedSingletonType__>;  \
public:                                                   \
    virtual ~__MADerivedSingletonType__() = default;

/// @macro EXPR_SGT
/// @brief Singleton type.
#define EXPR_SGT(__SingletonType__) \
  __SingletonType__::GetInstance()
  
/// @class ISingleton
/// @brief Singleton type interface.
template <typename TType>
class ISingleton
{
public:
  /// @brief Return reference of instance of MTime manager.
  /// This function is thread safe.
  static TType& GetInstance() noexcept;

  /// @brief Initialize singleton.
  EDySuccess static Initialize() noexcept;

  /// @brief Shutdown singleton.
  EDySuccess static Release() noexcept;

  /// @brief Check whether singleton is initialized.
  [[nodiscard]] bool static IsInitialized() noexcept;

  /// @brief Check whether singleton is shutdowned.
  [[nodiscard]] bool static IsReleased() noexcept;

protected:
  ISingleton() = default;
  virtual ~ISingleton() = default;
  inline static std::atomic<bool> mIsInitialized = false;

  EXPR_SINGLETON_PROPERTIES(ISingleton);
};

} /// ::dy::expr namespace
#include <Expr/Inline/ISingleton.inl>