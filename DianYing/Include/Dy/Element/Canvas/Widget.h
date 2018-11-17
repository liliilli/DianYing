#ifndef GUARD_DY_ELEMENT_CANVAS_CANVAS_H
#define GUARD_DY_ELEMENT_CANVAS_CANVAS_H
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
#include <Dy/Element/Canvas/UiObject.h>
#include <Dy/Helper/Pointer.h>
#include <Dy/Meta/Descriptor/WidgetCommonDescriptor.h>
#include <Dy/Component/Interface/IDyInitializeHelper.h>
#include <Dy/Component/CDyScript.h>
#include <Dy/Component/UI/CDyWidgetScript.h>

namespace dy
{

///
/// @class FDyUiWidget
/// @brief FFF
///
class FDyUiWidget final : public FDyRenderableObject, public IDyInitializeHelper<PDyMetaWidgetRootDescriptor>
{
  using TUiObject       = std::unique_ptr<FDyUiObject>;
	using TUiObjectMap    = std::unordered_map<std::string, TUiObject>;

  MDY_SET_CRC32_HASH_WITH_TYPE(FDyUiWidget);
  MDY_SET_TYPEMATCH_FUNCTION(FDyRenderableObject, FDyUiWidget);
public:
  FDyUiWidget() = default;

  ///
  /// @brief Initialize FDyUiWidget.
  /// @param widgetMetaDesc Meta descriptor information instance for FDyUiWidget.
  /// @return Success / Failure flag.
  ///
  EDySuccess Initialize(_MIN_ const PDyMetaWidgetRootDescriptor& widgetMetaDesc) override final;

  /// @brief Release widget instance.
  void Release() override final;

  /// @brief Activate FDyUiWidget instance.
  void Activate() noexcept;

  /// @brief Deactivate FDyUiWidget instance.
  void Deactivate() noexcept;

  ///
  /// @brief Render object.
  ///
  void Render() override final;

  ///
  /// @brief  Check FDyUiWidget is activated or not.
  /// @return Check flag for activation checking.
  ///
  MDY_NODISCARD FORCEINLINE bool IsActivated() const noexcept
  {
    return this->mActivationFlag.GetOutput();
  }

  ///
  /// @brief Get present actor name on runtime.
  /// @return Actor name of this instance.
  ///
  MDY_NODISCARD const std::string& GetActorName() const noexcept
  {
    return this->pGetObjectName();
  }

  ///
  /// @brief Return actor's information
  /// @return information string.
  ///
  MDY_NODISCARD std::string ToString() override
  {
    return MDY_INITIALIZE_EMPTYSTR;
  }

  ///
  /// @brief  Get script component pointer from script list using scriptName to verify.
  /// @return The pointer instance of CDyScript. If not found, return just no value.
  ///
  MDY_NODISCARD std::optional<CDyWidgetScript*> GetScript() noexcept;

private:
  ///
  /// @brief
  ///
  void pPropagateActivationFlag() noexcept;

  DDy3StateBool                     mActivationFlag = {};
  /// Parent FDyUiWidget raw-pointer data.
  FDyUiWidget*                      mParentFDyUiWidgetRawPtr= MDY_INITIALIZE_NULL;
  /// Component list (randomly) which attached to FDyUiWidget instance (this!)
  TUiObjectMap                      mObjectList     = {};
  /// Script list (specialized!)
  std::unique_ptr<CDyWidgetScript>  mWidgetScript   = nullptr;
};

} /// ::dy namespace

#endif /// GUARD_DY_ELEMENT_CANVAS_CANVAS_H