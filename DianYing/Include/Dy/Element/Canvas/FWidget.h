#ifndef GUARD_DY_ELEMENT_CANVAS_CANVAS_H
#define GUARD_DY_ELEMENT_CANVAS_CANVAS_H
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

#include <Dy/Element/Canvas/AWidgetUiObjectChildrenable.h>
#include <Dy/Element/Canvas/AWidgetObject.h>
#include <Dy/Meta/Descriptor/WidgetCommonDescriptor.h>
#include <Dy/Component/Widget/CWidgetScript.h>
#include <Dy/Helper/Type/DTristateBool.h>
#include <Dy/Helper/System/Pointer.h>

//!
//! Forward declaration
//!

namespace dy
{
class DWidgetBinder;
class CBaseWidgetScript;
} /// ::dy namespace

//!
//! Implementation
//!

namespace dy
{
/// @class FWidget
/// @brief Root UI Widget or Children but wrapped UI Widget object.
class FWidget final : public AWidgetUiObjectChildrenable 
{
  using TUiObject    = std::unique_ptr<AWidgetObject>;
	using TUiObjectMap = std::unordered_map<std::string, TUiObject>;

  MDY_SET_CRC32_HASH_WITH_TYPE(FWidget);
  MDY_SET_TYPEMATCH_FUNCTION(AWidgetUiObjectChildrenable, FWidget);
public:
  /// @brief Initialize FWidget.
  /// @param widgetMetaDesc Meta descriptor information instance for FWidget.
  /// @return Success / Failure flag.
  FWidget(const PDyMetaWidgetRootDescriptor& widgetMetaDesc);
  virtual ~FWidget();

  /// @brief Render object.
  void Render() override final;

  /// @brief Get present actor name on runtime.
  /// @return Actor name of this instance.
  MDY_NODISCARD const std::string& GetActorName() const noexcept
  {
    return this->pGetObjectName();
  }

  /// @brief Return actor's information
  /// @return information string.
  MDY_NODISCARD std::string ToString() override
  {
    return MDY_INITIALIZE_EMPTYSTR;
  }

  /// @brief Get script component pointer from script list using scriptName to verify.
  /// @return The pointer instance of CDyScript. If not found, return just no value.
  MDY_NODISCARD CBaseWidgetScript* GetScript() noexcept;

  /// @brief Attach DWidgetBinder.
  void MDY_PRIVATE(TryAttachBinderFromBinder)(_MIN_ DWidgetBinder& iRefUiBinder);
  /// @brief Detach DWidgetBinder. This function called from Binder.
  void MDY_PRIVATE(TryDetachBinderFromBinder)(_MIN_ DWidgetBinder& iRefUiBinder);

  /// @brief Set name internally.
  void MDY_PRIVATE(SetName)(_MIN_ const std::string& iNewName);

private:
  void TryActivateInstance() override final;
  void TryDeactivateInstance() override final;

  /// Parent FWidget raw-pointer data.
  FWidget*                      mParentFDyUiWidgetRawPtr= MDY_INITIALIZE_NULL;
  /// Component list (randomly) which attached to FWidget instance (this!)
  TUiObjectMap                      mObjectList     = {};
  /// @brief 
  std::unique_ptr<CWidgetScript>  mWidgetScript = MDY_INITIALIZE_NULL;

  /// @brief Managing DWidgetBinder instance.
  std::vector<NotNull<DWidgetBinder*>> mBoundUiBinderList {};
};

} /// ::dy namespace

#endif /// GUARD_DY_ELEMENT_CANVAS_CANVAS_H