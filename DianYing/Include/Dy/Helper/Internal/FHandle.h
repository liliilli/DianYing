#ifndef GUARD_DY_HELPER_INTERNAL_FHANDLE_H
#define GUARD_DY_HELPER_INTERNAL_FHANDLE_H
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
class FHandler;

/// @class FHandle
/// @brief Safe resouce handle binder to FHandler<TType>.
template <typename TType>
class FHandle
{
public:
  FHandle() = default;
  virtual ~FHandle();

  FHandle(const FHandle& iHandle);
  FHandle& operator=(const FHandle& iHandle);

  FHandle(FHandle&& iHandle) noexcept;
  FHandle& operator=(FHandle&& iHandle) noexcept;

  TType* operator->();

  /// @brief Check this handle is binding any resource.
  /// This does not guarantee that handler resource has this instance actually.
  [[nodiscard]] bool IsBinding() const noexcept;

  /// @brief Reset binding state.
  void Reset();

private:
  FHandler<TType>* mHandlerAddress = nullptr;

  friend class FHandler<TType>;
};

} /// ::dy namespace

#endif /// GUARD_DY_HELPER_INTERNAL_FHANDLE_H
#include <Dy/Helper/Internal/Inline/FHandleTmp.inl>