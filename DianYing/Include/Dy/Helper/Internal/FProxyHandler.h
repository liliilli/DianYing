#ifndef GUARD_DY_HELPER_INTERNAL_FPROXYHANDLER_H
#define GUARD_DY_HELPER_INTERNAL_FPROXYHANDLER_H
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

#include <unordered_set>

namespace dy
{

template <typename TType>
class FProxyHandle;

template <typename TType>
class FProxyHandler
{
public:
  template <typename... TArgs>
  FProxyHandler(TArgs&&... iArgs);

  explicit FProxyHandler(TType* iRawInstance);

  virtual ~FProxyHandler();

  FProxyHandler(const FProxyHandler&) = delete;
  FProxyHandler& operator=(const FProxyHandler&) = delete;

  FProxyHandler(FProxyHandler&& iHandler) noexcept;
  FProxyHandler& operator=(FProxyHandler&& iHandler) noexcept;

  void AttachHandle(FProxyHandle<TType>& iHandle);

  EDySuccess DetachHandle(FProxyHandle<TType>& iHandle);

  /// @brief Check bound handle list is empty.
  [[nodiscard]] bool IsBeingBinded() const noexcept;

private:
  TType* mValue = nullptr;
  std::unordered_set<FProxyHandle<TType>*> mBindHandleList;

  void pMoveHandler(FProxyHandler&& iAnotherHandler) noexcept;

  friend class FProxyHandle<TType>;
};

} /// ::dy namespace

#endif /// GUARD_DY_HELPER_INTERNAL_FPROXYHANDLER_H
#include <Dy/Helper/Internal/Inline/FProxyHandlerTmp.inl>
