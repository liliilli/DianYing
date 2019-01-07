#ifndef GUARD_DY_HELPER_INTERNAL_RWLOCKATOMIC_H
#define GUARD_DY_HELPER_INTERNAL_RWLOCKATOMIC_H
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

#include <shared_mutex>

namespace dy
{

template <typename TType, typename = std::enable_if_t<std::is_arithmetic_v<TType>>>
class RWLockAtomic final
{
public:
  RWLockAtomic() = default;
  RWLockAtomic(_MIN_ TType value) : mValue{value} {};

  /// @brief Add. This function is unique and mutual exclusive as writers.
  void Add(_MIN_ TType value) noexcept
  {
    std::unique_lock lock(this->mMutex);
    this->mValue += value;
  }

  /// @brief Get value. This function is shared as readers.
  MDY_NODISCARD TType Get() const noexcept
  {
    std::shared_lock lock(this->mMutex);
    return this->mValue;
  }

private:
  MDY_TRANSIENT std::shared_mutex mMutex;
  TType mValue = TType{};
};

} /// ::dy namespace

#endif /// GUARD_DY_HELPER_INTERNAL_RWLOCKATOMIC_H