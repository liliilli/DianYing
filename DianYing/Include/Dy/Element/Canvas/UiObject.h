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

//!
//! Forward declaration
//!

namespace dy
{
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
  /// @brief  Get name of UiObject instance.
  /// @return UiObject Instance.
  ///
  MDY_NODISCARD const std::string& GetUiObjectName() noexcept
  {
    MDY_ASSERT(this->pGetObjectName().empty() == false, "Unexpected error occurred.");
    return this->pGetObjectName();
  }

private:
  FDyUiObjectChildrenable* mParentUiObject = nullptr;
};

} /// ::dy namespace

#endif /// GUARD_DY_ELEMENT_CANVAS_UIOBJECT_H