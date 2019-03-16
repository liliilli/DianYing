#ifndef GUARD_DY_ELEMENT_CANVAS_UIOBJECT_H
#define GUARD_DY_ELEMENT_CANVAS_UIOBJECT_H
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

#include <Dy/Element/ARenderableObject.h>
#include <Dy/Helper/Type/DVector2.h>
#include <Dy/Helper/Type/DVectorInt2.h>
#include <Dy/Meta/Type/EDyWidgetTypes.h>
#include <Dy/Element/Type/ATristateSwitcher.h>
#include <Dy/Element/Internal/TIdDistributor.h>

//!
//! Forward declaration
//!

namespace dy
{
class AWidgetUiObjectChildrenable;
} /// ::dy namespace

//!
//! Implementation
//!

namespace dy
{

/// @class AWidgetObject
/// @brief Ui object type.
class AWidgetObject : public ARenderableObject, 
    public ATristateSwitcher, 
    public TIdDistributor<TU32, AWidgetObject>
{
public:
  AWidgetObject() = default;
  virtual ~AWidgetObject() = 0;

  MDY_SET_CRC32_HASH_WITH_TYPE(AWidgetObject);
  MDY_SET_TYPEMATCH_FUNCTION(ARenderableObject, AWidgetObject);
  MDY_ONLY_MOVEABLE_PROPERTIES_DEFAULT(AWidgetObject)

  /// @brief Set pointer of parent ui object which is able to have children.
  void SetParentUiObject(AWidgetUiObjectChildrenable& parent) noexcept;
  /// @brief Check parent is exist on this UI object.
  MDY_NODISCARD bool CheckIsParentExist() const noexcept;
  /// @brief Get pointer of parent ui object as pointer.
  MDY_NODISCARD const AWidgetUiObjectChildrenable* GetPtrParentUiObject() const noexcept;

  /// @brief Set central position of widget.
  /// @param position Position value.
  virtual void SetRelativePosition(const DVector2& position) noexcept;
  /// @brief Get widget relative position following origin anchor input (not final position)
  /// @return widget position from parent.
  MDY_NODISCARD DVector2 GetRelativePosition(const EDyOrigin& origin) const noexcept;
  /// @brief Get widget final position following origin anchor input (not final position)
  /// @return widget final position of screen.
  MDY_NODISCARD DVector2 GetFinalPosition(const EDyOrigin& origin) const noexcept;

  /// @brief Set frame size of widget.
  virtual void SetFrameSize(const DVectorInt2& size) noexcept;
  /// @brief Get frame size of widget.
  MDY_NODISCARD const DVectorInt2& GetFrameSize() const noexcept;

  /// @brief Set origin axis value.
  virtual void SetOrigin(EDyOrigin iOrigin) noexcept;
  /// @brief Get origin axis to parent. 
  MDY_NODISCARD EDyOrigin GetOrigin() const noexcept;

  /// @brief Set fibot axis value. 
  virtual void SetFibot(EDyOrigin iOrigin) noexcept;
  /// @brief Get fibot axis to my widget frame size.
  MDY_NODISCARD EDyOrigin GetFibot() const noexcept;

  /// @brief Align final position of widget from parent information. \n
  /// If parent is not exist, just recalculate with fibot.
  void UpdateFinalPosition();

  /// @brief Get name of UiObject instance.
  MDY_NODISCARD const std::string& GetUiObjectName() const noexcept;

  /// @brief ZOrder value.
  TU32 mZOrder = 0;

private:
  AWidgetUiObjectChildrenable* mPtrParentUiObject = MDY_INITIALIZE_NULL;
  DVector2    mCentralRelativePosition = {};
  DVectorInt2 mFrameSize               = {};

protected:
  DVector2  mCentralFinalPosition  = {};
  EDyOrigin mOrigin = EDyOrigin::Center_Center;
  EDyOrigin mFibot  = EDyOrigin::Center_Center;
};

inline AWidgetObject::~AWidgetObject() = default;

} /// ::dy namespace

#endif /// GUARD_DY_ELEMENT_CANVAS_UIOBJECT_H