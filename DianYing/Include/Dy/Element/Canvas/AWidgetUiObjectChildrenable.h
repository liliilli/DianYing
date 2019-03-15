#ifndef GUARD_DY_ELEMENT_CANVAS_UIOBJECTCHILDRENABLE_H
#define GUARD_DY_ELEMENT_CANVAS_UIOBJECTCHILDRENABLE_H
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

#include <Dy/Element/Canvas/AWidgetObject.h>

namespace dy
{

/// @enum  EDySearchMode
/// @brief Search mode enumeration value.
enum class EDySearchMode
{
  Default,
  Recursive
};

/// @class AWidgetUiObjectChildrenable
/// @brief Ui object which has children ui object.
class AWidgetUiObjectChildrenable : public AWidgetObject
{
  MDY_SET_CRC32_HASH_WITH_TYPE(AWidgetUiObjectChildrenable);
  MDY_SET_TYPEMATCH_FUNCTION(AWidgetObject, AWidgetUiObjectChildrenable);
  MDY_ONLY_MOVEABLE_PROPERTIES_DEFAULT(AWidgetUiObjectChildrenable)
public:
  AWidgetUiObjectChildrenable() = default;
  virtual ~AWidgetUiObjectChildrenable() = 0;

  /// @brief Set central position of widget.
  /// @param position Position value.
  void SetRelativePosition(const DVector2& position) noexcept override final;
  /// @brief Set frame size of widget.
  /// @param size Frame size. Must be full size, not half size.
  void SetFrameSize(const DVectorInt2& size) noexcept override final;
  /// @brief Set origin axis value.
  void SetOrigin(EDyOrigin iOrigin) noexcept override final;
  /// @brief Set fibot axis value. 
  void SetFibot(EDyOrigin iOrigin) noexcept override final;

  /// @brief Set propagation mode.
  void SetPropagateMode(bool isEnabled, EDySearchMode mode);
  /// @brief Propagate information to children. If `CheckIsPropagable` is not true, do nothing.
  void TryPropagatePositionToChildren();
  /// @brief Check this widget is propagable position to children.
  MDY_NODISCARD bool CheckIsPropagable() const noexcept;

  ///
  /// @TODO IMPLEMENT THIS & AUTOMATIC SPECIFIER NAME CREATION.
  template <typename TUiObjectType, typename TArgument>
  TUiObjectType* AddUiObject(const TArgument& arg)
  {
    static_assert(std::is_base_of_v<AWidgetObject, TUiObjectType> == true, "TUiObjectType is not derived from AWidgetObject.");

    auto object = std::make_unique<TUiObjectType>();
    MDY_CALL_ASSERT_SUCCESS(object->Initialize(arg));
    object->SetParentUiObject(*this);

    auto& it = this->mUiObjectList.emplace_back(static_cast<AWidgetObject*>(object.release()));
    return static_cast<TUiObjectType*>(it.get());
  }

  ///
  /// @TODO IMPLEMENT THIS
  /// @brief UiObject version.
  ///
  MDY_NODISCARD AWidgetObject* GetUiObject(const std::string& objectName, EDySearchMode searchMode = EDySearchMode::Default);

  ///
  /// @brief Template version. Able to retrieve object as a TType.
  ///
  template <typename TType>
  MDY_NODISCARD TType* GetUiObject(const std::string& objectName, EDySearchMode searchMode = EDySearchMode::Default)
  {
    static_assert(std::is_base_of_v<AWidgetObject, TType> == true, "TType is not derived from AWidgetObject.");
    AWidgetObject* objectPtr = this->GetUiObject(objectName, searchMode);
    return static_cast<TType*>(objectPtr);
  }

  ///
  /// @TODO IMPLEMENT THIS
  void RemoveUiObject();

  /// @brief
  void Render() override;

  /// @brief Propagate activation flag into low-level hierarchical widget tree.
  void PropagateActivationFlag(bool iParentFlag);

protected:
  void TryActivateInstance() override;
  void TryDeactivateInstance() override;

private:
  std::vector<std::unique_ptr<AWidgetObject>> mUiObjectList = {};
  std::vector<std::unique_ptr<AWidgetUiObjectChildrenable>> mUiChildrenableObjectList = {};

  bool mIsCanPropagatePosition = false;
};

inline AWidgetUiObjectChildrenable::~AWidgetUiObjectChildrenable() = default;

} /// ::dy namespace

#endif /// GUARD_DY_ELEMENT_CANVAS_UIOBJECTCHILDRENABLE_H