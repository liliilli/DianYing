#ifndef GUARD_DY_HELPER_INTERNAL_FHANDLER_H
#define GUARD_DY_HELPER_INTERNAL_FHANDLER_H
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
class FHandle;

/// @class FHandler
/// @brief Handler management type but has no value.
template <typename TType>
class FHandler
{
public:
  FHandler() = default;
  virtual ~FHandler();

  FHandler(const FHandler&) = delete;
  FHandler& operator=(const FHandler&) = delete;

  FHandler(FHandler&& iHandler) noexcept;
  FHandler& operator=(FHandler&& iHandler) noexcept;

  void AttachHandle(FHandle<TType>& iHandle);

  EDySuccess DetachHandle(FHandle<TType>& iHandle);

  /// @brief Check bound handle list is empty.
  [[nodiscard]] bool IsBeingBinded() const noexcept;

private:
  std::unordered_set<FHandle<TType>*> mBindHandleList;

  void pMoveHandler(FHandler&& iAnotherHandler) noexcept;

  friend class FHandle<TType>;
};

} /// ::dy namespace

#endif /// GUARD_DY_HELPER_INTERNAL_FHANDLER_H
#include <Dy/Helper/Internal/Inline/FHandlerTmp.inl>
