#ifndef GUARD_DY_HELPER_WRAPPER_MUTEXUNORDEREDMAP_H
#define GUARD_DY_HELPER_WRAPPER_MUTEXUNORDEREDMAP_H
///
/// MIT License
/// Copyright (c) 2018 Jongmin Yun
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

#include <Dy/Helper/System/TypeTraits.h>
#include <Dy/Helper/Pointer.h>
#include <Dy/Helper/ContainerHelper.h>

namespace dy
{

template <typename TKeyType, typename TValueType>
class DDyMutexUniqueHashMap final
{
  using TSmtValueType = std::unique_ptr<TValueType>;

public:
  /// @brief Move unique_ptr instance and insert container atomically.
  void MoveInsert(_MIN_ const std::string& key, _MINOUT_ std::unique_ptr<TKeyType>&& value) noexcept
  {
    this->pMoveInsert(key, std::move(value));
  }

  /// @brief Overload version of plain `MoveInsert`.
  template <typename TDerivedType>
  void MoveInsert(_MIN_ const std::string& key, _MINOUT_ std::unique_ptr<TDerivedType>&& value) noexcept
  {
    this->pMoveInsert(key, DyConvertUniquePtrTo<TKeyType>(std::move(value)));
  }

  /// @brief Check given item which has name key is valid and not empty.
  MDY_NODISCARD bool IsExistAndNotEmpty(_MIN_ const std::string& key) const noexcept
  {
    std::shared_lock lock(this->mSharedMutex);
    return DyIsMapContains(this->mContainer, key);
  }

  /// @brief
  MDY_NODISCARD NotNull<TValueType*> GetInstancePtr(_MIN_ const std::string& key) noexcept
  {
    std::shared_lock lock(this->mSharedMutex);
    return this->mContainer[key].get();
  }

  /// @brief
  MDY_NODISCARD NotNull<const TValueType*> GetInstancePtr(_MIN_ const std::string& key) const noexcept
  {
    std::shared_lock lock(this->mSharedMutex);
    return this->mContainer[key].get();
  }

  /// @brief
  MDY_NODISCARD TValueType* TryGetInstancePtr(_MIN_ const std::string& key) noexcept
  {
    std::shared_lock lock(this->mSharedMutex);
    if (this->IsExistAndNotEmpty(key) == false) { return nullptr; }

    return this->mContainer[key].get();
  }

  MDY_NODISCARD const TValueType* TryGetInstancePtr(_MIN_ const std::string& key) const noexcept
  {
    std::shared_lock lock(this->mSharedMutex);
    if (this->IsExistAndNotEmpty(key) == false) { return nullptr; }

    return this->mContainer[key].get();
  }

  /// @brief
  MDY_NODISCARD TSmtValueType RemoveAndGetInstance(_MIN_ const std::string& key) noexcept
  {
    TSmtValueType smtptrReturnInstance = MDY_INITIALIZE_NULL;
    {
      std::unique_lock lock{this->mSharedMutex};
      smtptrReturnInstance = std::move(this->mContainer[key]);
      this->mContainer.erase(key);
    }
    return std::move(smtptrReturnInstance);
  }

private:
  FORCEINLINE void pMoveInsert(_MIN_ const std::string& key, _MINOUT_ std::unique_ptr<TKeyType>&& value) noexcept
  {
    std::unique_lock lock{this->mSharedMutex};
    mContainer.insert_or_assign(key, std::move(value));
  }

  MDY_TRANSIENT std::shared_mutex mSharedMutex;
  std::unordered_map<TKeyType, TSmtValueType> mContainer;
};

} /// ::dy namespace

#endif /// GUARD_DY_HELPER_WRAPPER_MUTEXUNORDEREDMAP_H