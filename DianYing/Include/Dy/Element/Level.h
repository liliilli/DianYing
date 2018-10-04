#ifndef GUARD_DY_ELEMENT_LEVEL_H
#define GUARD_DY_ELEMENT_LEVEL_H
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

#include <Dy/Helper/Type/Color.h>
#include <Dy/Element/Object.h>
#include <Dy/Element/Actor.h>
#include <Dy/Element/Abstract/ADyNameCounterMap.h>
#include <Dy/Element/Descriptor/LevelDescriptor.h>
#include <Dy/Element/Interface/IDyUpdatable.h>

namespace dy {

///
/// @class FDyLevel
/// @brief Level class type for managing run-time interactive world space.
///
class FDyLevel final : public FDyObject, public IDyUpdatable, public ADyNameCounterMap
{
  using TActorSmtPtr      = std::unique_ptr<FDyActor>;
  using TActorMap         = std::unordered_map<std::string, TActorSmtPtr>;
  using TNameCounterMap   = std::unordered_map<std::string, int32_t>;

public:
  /// Initialize level context with valid descriptor.
  void Initialize(_MIN_ const PDyLevelConstructDescriptor& desc);

  /// Release level by release all subobjects in this level storing information or signalling something.
  void Release();

  /// Update level.
  void Update(_MIN_ float dt) override final;

  ///
  /// @brief  Get present level name.
  /// @return Level name.
  ///
  MDY_NODISCARD const std::string& GetLevelName() const noexcept
  {
    return this->mLevelName;
  }

private:
  /// Level's name. not modifiable
  std::string     mLevelName            = MDY_INITILAIZE_EMPTYSTR;
  /// Level's hash value for identifying scene in world's array.
  TU32            mLevelHashIdentifier  = MDY_INITIALIZE_DEFUINT;
  /// Scene basic color
  DDyColor        mLevelBackgroundColor = DDyColor::White;
  /// Actor list (hierarchial version)
  TActorMap       mActorMap             = {};
  /// Check if level is initialized or released. Level is active when only mInitialized is true.
  bool            mInitialized          = false;

public:
  /// Level information as string.
  MDY_NODISCARD std::string ToString() override final;

#ifdef false
  ///
  /// @brief
	/// Instantiate game object in scene by moving object instance to container.
  /// If success, return pointer of moved object but nullptr if failed.
  ///
  /// @param[in] object_name Object name to speicify.
  /// @param[in] object_smtptr Unique smart pointer reference of object.
  ///
  /// @return If success, raw pointer of moved object; otherwise nullptr.
  ///
	template <
    class TCObjectType,
    typename = std::enable_if_t<IsCObjectBase<TCObjectType>>
  >
  TCObjectType* CreateGameObject(const std::string& object_name,
                            std::unique_ptr<TCObjectType>& object_smtptr) {
    const auto object_final_name = CreateChildTag(object_name);

    auto [result_pair, result] = m_object_list.try_emplace(
        object_final_name,
        nullptr);
    if (!result) {
      PHITOS_ASSERT(result, "Object did not be made properly.");
      return nullptr;
    }

    result_pair->second = std::move(object_smtptr);
    TGameObjectSmtPtr& object_ref = result_pair->second;
    object_ref->pSetHash(object_final_name);
    return static_cast<TCObjectType*>(object_ref.get());
	}

  ///
  /// @brief
	/// Instantiate game object in scene by specifing arguments
  /// to create in container.
  ///
  /// If success, return pointer of moved object but nullptr if failed.
  ///
  /// @param[in] object_name Object name to speicify.
  /// @param[in] args Argument values for constructor of each object type.
  ///
  /// @return If success, raw pointer of moved object; otherwise nullptr.
  ///
  template <
    class TCObjectType,
    class... TConstructionArgs,
    typename = std::enable_if_t<IsCObjectBase<TCObjectType>>
  >
  TCObjectType* CreateGameObject(const std::string& object_name, TConstructionArgs&&... args) {
    const auto object_final_name = CreateChildTag(object_name);

    auto [result_pair, result] = m_object_list.try_emplace(object_final_name, nullptr);
    if (!result) {
      PHITOS_ASSERT(result, "Object did not be made properly.");
      return nullptr;
    }

    result_pair->second = std::make_unique<TCObjectType>(std::forward<TConstructionArgs>(args)...);
    TGameObjectSmtPtr& object_ref = result_pair->second;
    object_ref->pSetHash(object_final_name);

    return static_cast<TCObjectType*>(object_ref.get());
	}

	///
	/// @brief Get specific object with tag.
	///
  [[nodiscard]]
	std::optional<FDyPawn*> GetPawn(const std::string& objectName, bool isRecursive = false);

	///
	/// @brief Destroy object has unique tag key but not recursively.
	/// @param[in] object_name Object name.
	/// @return Success/Failed tag.
  /// If arbitary m_object_list has been destroied, return ture.
	///
  [[nodiscard]] EDySuccess DestroyPawn(const std::string& object_name);

  ///
  /// @brief Destory child object with address.
  /// @param[in] objectRef    Object reference.
  /// @param[in] isRecursive  Flag for destruction of specified object recursively.
	/// @return Success/Failed tag.
  /// If arbitary m_object_list has been destroyed, return ture.
  ///
  [[nodiscard]] EDySuccess DestroyPawn(const FDyPawn& objectRef, bool isRecursive = false);

private:
  ///
  /// @brief Create child object name.
  /// @param[in] name
  /// @return
  ///
  inline std::string CreateChildTag(const std::string& name) noexcept {
    std::string item_tag = name;

    if (m_name_counter.find(name) != m_name_counter.end()) {
      auto& count = m_name_counter[name];
      count += 1;
      item_tag.append('_' + std::to_string(count));
    }
    else { m_name_counter[name] = 0; }

    return item_tag;
  }

  CObject* GetGameObjectResursively(const std::string& object_name) noexcept;

  component::CCamera* m_main_camera = nullptr;
  DColor m_background_color = DColor::Black;

  TGameObjectMap  m_object_list;
  TNameCounterMap m_name_counter;
#endif
};

} /// ::dy namespace

#endif /// GUARD_DY_ELEMENT_LEVEL_H
