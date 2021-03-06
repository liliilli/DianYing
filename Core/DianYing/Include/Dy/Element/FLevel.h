#ifndef GUARD_DY_ELEMENT_LEVEL_H
#define GUARD_DY_ELEMENT_LEVEL_H
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

#include <Dy/Helper/Type/DColorRGBA.h>
#include <Dy/Element/AWorldObject.h>
#include <Dy/Element/FActor.h>
#include <Dy/Element/Interface/IUpdatable.h>
#include <Dy/Helper/Internal/FNameGenerator.h>

namespace dy {

/// @class FLevel
/// @brief Level class type for managing run-time interactive world space.
class FLevel final : 
  public AWorldObject, 
  public IUpdatable, 
  public FNameGenerator,
  public TIdDistributor<TU32, FLevel>
{
public:
  /// Initialize level context with valid descriptor.
  FLevel(_MIN_ const PDyLevelConstructMetaInfo& desc);
  /// Release level by release all subobjects in this level storing information or signalling something.
  virtual ~FLevel();

  /// Update level.
  void Update(_MIN_ float dt) override final;

  /// @brief  Get background color of this scene.
  /// @return background color [0, 1] (RGBA)
  MDY_NODISCARD const DColorRGBA& GetBackgroundColor() const noexcept;

  /// @brief  Get present level name.
  /// @return Level name.
  MDY_NODISCARD const std::string& GetLevelName() const noexcept;

  /// @brief  Set background color of this scene.
  /// @param  backgroundColor New backgrond color value.
  void SetBackgroundColor(_MIN_ const DColorRGBA& backgroundColor) noexcept;

  /// @brief Align position of actors.
  void MDY_PRIVATE(AlignActorsPosition)() noexcept;

  /// @brief  Get valid level reference.
  /// @return Valid level reference. when level is not specified, unexpected behaviour.
  MDY_NODISCARD std::vector<NotNull<FActor*>> 
  GetAllActorsWithTag(_MIN_ const std::string& iTagSpecifier) const noexcept;
  /// @brief Get all actors with tag. Tag must be valid. \n
  /// If iTagSpecifier is empty, this function get all actors which is not specified any tag. \n
  /// and this function search all actor of object tree from root to leaf, so might take some time.
  MDY_NODISCARD std::vector<NotNull<FActor*>>
  GetAllActorsWithTagRecursive(_MIN_ const std::string& iTagSpecifier) const noexcept;
  /// @brief Get all actors with matched name within only one depth of level object tree. \n
  /// If iNameSpecifier is empty, just return empty list.
  MDY_NODISCARD std::vector<NotNull<FActor*>>
  GetAllActorsWithName(_MIN_ const std::string& iNameSpecifier) const noexcept; 
  /// @brief Get all actors with matched name within overall level object tree. \n
  /// If iNameSpecifier is empty, just return empty list.
  MDY_NODISCARD std::vector<NotNull<FActor*>>
  GetAllActorsWithNameRecursive(_MIN_ const std::string& iNameSpecifier) const noexcept; 
  /// @brief Get pointer of actor with object id.
  /// If not found, just return nullptr.
  FActor* GetActorWithObjectId(_MIN_ TU32 iObjectId) noexcept;

  /// @brief Try get actor with given full name, from root to actor.
  /// If iFullName did not satisfy full name format, just return nullptr.
  MDY_NODISCARD FActor* GetActorWithFullName(const std::string& iFullName) const noexcept;
  /// @brief Try get actor with given full name, from root to actor.
  /// If iFullName did not satisfy full name format, just return nullptr.
  MDY_NODISCARD FActor* GetActorWithFullName(const std::vector<std::string>& iKeywords) const noexcept;

  /// @brief Return level actor container.
  MDY_NODISCARD FActor::TActorMap& GetActorContainer() noexcept;

  /// @brief Create actor instantly in this level.
  void CreateActorInstantly(const PActorCreationDescriptor& iDescriptor);

private:
  /// Level's name. not modifiable
  std::string         mLevelName            = MDY_INITIALIZE_EMPTYSTR;
  /// Level basic color
  DColorRGBA        mLevelBackgroundColor = DColorRGBA::White;
  /// Actor list (hierarchial version)
  FActor::TActorMap mActorMap             = {};
  /// Check if level is initialized or released. Level is active when only mInitialized is true.
  bool                mInitialized          = false;

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
      MDY_ASSERT_MSG(result, "Object did not be made properly.");
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
      MDY_ASSERT_MSG(result, "Object did not be made properly.");
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
