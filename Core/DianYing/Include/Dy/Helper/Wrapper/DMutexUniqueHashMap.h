#ifndef GUARD_DY_HELPER_WRAPPER_MUTEXUNORDEREDMAP_H
#define GUARD_DY_HELPER_WRAPPER_MUTEXUNORDEREDMAP_H
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

#include <memory>
#include <shared_mutex>
#include <unordered_map>
#include <string>

#include <Dy/Helper/System/TypeTraits.h>
#include <Dy/Helper/System/Pointer.h>
#include <Dy/Helper/Library/HelperContainer.h>

namespace dy
{

/// @class DMutexUniqueHashMap
/// @brief HashMap that wraps std::unordered_map 
/// and check atomic operation using shared RW lock.
template <typename TKeyType, typename TValueType>
class DMutexUniqueHashMap final
{
  using TSmtValueType = std::unique_ptr<TValueType>;

public:
  /// @brief Move unique_ptr instance and insert container atomically.
  void MoveInsert(const TKeyType& iKey, std::unique_ptr<TValueType>&& ioValue) noexcept
  {
    this->pMoveInsert(iKey, std::move(ioValue));
  }

  /// @brief Overload version of plain `MoveInsert`.
  template <
    typename TDerivedType, 
    typename = std::enable_if_t<std::is_same_v<TDerivedType, TValueType> == false>
  >
  void MoveInsert(const TKeyType& iKey, std::unique_ptr<TDerivedType>&& ioValue) noexcept
  {
    this->pMoveInsert(iKey, DyConvertUniquePtrTo<TValueType>(std::move(ioValue)));
  }

  /// @brief Check given item which has name key is valid and not empty.
  MDY_NODISCARD bool IsExistAndNotEmpty(const TKeyType& key) const noexcept
  {
    std::shared_lock lock(this->mSharedMutex);
    return Contains(this->mContainer, key);
  }

  /// @brief
  NotNull<TValueType*> GetInstancePtr(const TKeyType& key) noexcept
  {
    std::shared_lock lock(this->mSharedMutex);
    return this->mContainer.at(key).get();
  }

  /// @brief
  NotNull<const TValueType*> GetInstancePtr(const TKeyType& key) const noexcept
  {
    std::shared_lock lock(this->mSharedMutex);
    return this->mContainer.at(key).get();
  }

  /// @brief
  TValueType* TryGetInstancePtr(const TKeyType& key) noexcept
  {
    std::shared_lock lock(this->mSharedMutex);
    if (this->IsExistAndNotEmpty(key) == false) { return nullptr; }

    return this->mContainer.at(key).get();
  }

  /// @brief
  const TValueType* TryGetInstancePtr(const TKeyType& key) const noexcept
  {
    std::shared_lock lock(this->mSharedMutex);
    if (this->IsExistAndNotEmpty(key) == false) { return nullptr; }

    return this->mContainer.at(key).get();
  }

  /// @brief
  MDY_NODISCARD TSmtValueType RemoveAndGetInstance(const TKeyType& key) noexcept
  {
    TSmtValueType smtptrReturnInstance = MDY_INITIALIZE_NULL;
    {
      std::unique_lock lock{this->mSharedMutex};
      smtptrReturnInstance = std::move(this->mContainer[key]);
      this->mContainer.erase(key);
    }
    return std::move(smtptrReturnInstance);
  }

  /// @brief
  EDySuccess Remove(const TKeyType& key) noexcept
  {
    std::unique_lock lock{this->mSharedMutex};
    this->mContainer.erase(key);
    return EDySuccess::DY_SUCCESS;
  }

private:
  void pMoveInsert(const TKeyType& iKey, std::unique_ptr<TValueType>&& ioValue) noexcept
  {
    std::unique_lock lock{this->mSharedMutex};
    mContainer.insert_or_assign(iKey, std::move(ioValue));
  }

  mutable std::shared_mutex mSharedMutex;
  std::unordered_map<TKeyType, TSmtValueType> mContainer;
};

} /// ::dy namespace

#endif /// GUARD_DY_HELPER_WRAPPER_MUTEXUNORDEREDMAP_H