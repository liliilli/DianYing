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
  using TContainer      = std::unordered_set<FHandle<TType>*>;
  using iterator        = typename TContainer::iterator;
  using const_iterator  = typename TContainer::const_iterator;

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

  iterator begin() noexcept { return this->mBindHandleList.begin(); }
  const_iterator begin() const noexcept { return this->mBindHandleList.begin(); }
  const_iterator cbegin() const noexcept { return this->mBindHandleList.cbegin(); }

  iterator end() noexcept { return this->mBindHandleList.end(); }
  const_iterator end() const noexcept { return this->mBindHandleList.end(); }
  const_iterator cend() const noexcept { return this->mBindHandleList.cend(); }

private:
  TContainer mBindHandleList;

  void pMoveHandler(FHandler&& iAnotherHandler) noexcept;

  friend class FHandle<TType>;
};

} /// ::dy namespace

#endif /// GUARD_DY_HELPER_INTERNAL_FHANDLER_H
#include <Dy/Helper/Internal/Inline/FHandlerTmp.inl>
