#ifndef GUARD_DY_ELEMENT_CANVAS_UIOBJECT_H
#define GUARD_DY_ELEMENT_CANVAS_UIOBJECT_H
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

#include <Dy/Element/RenderableObject.h>
#include <Dy/Helper/Type/Vector2.h>
#include <Dy/Helper/Type/VectorInt2.h>

//!
//! Forward declaration
//!

namespace dy
{
enum class EDyOrigin;
class FDyUiObjectChildrenable;
} /// ::dy namespace

//!
//! Implementation
//!

namespace dy
{

///
/// @class FDyUiObject
/// @brief
///
class FDyUiObject : public FDyRenderableObject
{
public:
  FDyUiObject() = default;
  virtual ~FDyUiObject() = default;

  MDY_SET_CRC32_HASH_WITH_TYPE(FDyUiObject);
  MDY_SET_TYPEMATCH_FUNCTION(FDyRenderableObject, FDyUiObject);
  MDY_ONLY_MOVEABLE_PROPERTIES_DEFAULT(FDyUiObject)

  ///
  /// @brief Set pointer of parent ui object which is able to have children.
  /// @param parent
  ///
  void SetParentUiObject(_MIN_ FDyUiObjectChildrenable& parent)
  {
    this->mParentUiObject = &parent;
  }

  ///
  /// @brief Set central position of widget.
  /// @param position Position value.
  ///
  virtual void SetWidgetCentralPosition(_MIN_ const DDyVector2& position) noexcept;

  ///
  /// @brief  Get widget position following origin anchor input (not final position)
  /// @param  origin Anchor.
  /// @return widget position from parent.
  ///
  MDY_NODISCARD DDyVector2 GetWidgetPosition(_MIN_ const EDyOrigin& origin) const noexcept;

  ///
  /// @brief Set frame size of widget.
  /// @param size Frame size. Must be full size, not half size.
  ///
  virtual void SetWidgetFrameSize(_MIN_ const DDyVectorInt2& size) noexcept;

  ///
  /// @brief  Get frame size of widget.
  /// @return Frame size of widget.
  ///
  MDY_NODISCARD DDyVectorInt2 GetFrameSize() const noexcept;

  ///
  /// @brief Align final position of widget from parent information.
  /// @param parentFinalPosition  Final position of parent.
  /// @param parentFrameSize      Frame size of parent.
  ///
  virtual void AlignFinalPosition(_MIN_ const DDyVector2& parentFinalPosition, _MIN_ const DDyVectorInt2& parentFrameSize) = 0;

  ///
  /// @brief  Get name of UiObject instance.
  /// @return UiObject Instance.
  ///
  MDY_NODISCARD const std::string& GetUiObjectName() const noexcept
  {
    MDY_ASSERT(this->pGetObjectName().empty() == false, "Unexpected error occurred.");
    return this->pGetObjectName();
  }

private:
  FDyUiObjectChildrenable* mParentUiObject        = nullptr;
  DDyVector2               mCentralPosition       = {};
  DDyVectorInt2            mWidgetSize            = {};

protected:
  DDyVector2               mFinalCentralPosition  = {};
};

} /// ::dy namespace

#endif /// GUARD_DY_ELEMENT_CANVAS_UIOBJECT_H