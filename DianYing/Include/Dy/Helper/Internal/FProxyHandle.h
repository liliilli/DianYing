#ifndef GUARD_DY_HELPER_INTERNAL_FPROXYHANDLE_H
#define GUARD_DY_HELPER_INTERNAL_FPROXYHANDLE_H
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

template <typename TType>
class FProxyHandler;

/// @class FProxyHandle
/// @brief Safe resouce handle binder to FProxyHandler<TType>.
template <typename TType>
class FProxyHandle
{
public:
  FProxyHandle() = default;
  virtual ~FProxyHandle();

  FProxyHandle(const FProxyHandle& iHandle);
  FProxyHandle& operator=(const FProxyHandle& iHandle);

  FProxyHandle(FProxyHandle&& iHandle) noexcept;
  FProxyHandle& operator=(FProxyHandle&& iHandle) noexcept;

  TType* operator->();

  /// @brief Check this handle is binding any resource.
  /// This does not guarantee that handler resource has this instance actually.
  [[nodiscard]] bool IsBinding() const noexcept;

  /// @brief Reset binding state.
  void Reset();

private:
  FProxyHandler<TType>* mHandlerAddress = nullptr;

  friend class FProxyHandler<TType>;
};

} /// ::dy namespace

#endif /// GUARD_DY_HELPER_INTERNAL_FPROXYHANDLE_H
#include <Dy/Helper/Internal/Inline/FProxyHandleTmp.inl>