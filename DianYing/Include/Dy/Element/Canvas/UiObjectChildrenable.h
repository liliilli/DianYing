#ifndef GUARD_DY_ELEMENT_CANVAS_UIOBJECTCHILDRENABLE_H
#define GUARD_DY_ELEMENT_CANVAS_UIOBJECTCHILDRENABLE_H
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

#include <Dy/Element/Canvas/UiObject.h>
#include "Dy/Helper/Pointer.h"

namespace dy
{

///
/// @enum  EDySearchMode
/// @brief Search mode enumeration value.
///
enum class EDySearchMode
{
  Default,
  Recursive
};

///
/// @class FDyUiObjectChildrenable
/// @brief Ui object which has chidlren ui object.
///
class FDyUiObjectChildrenable : public FDyUiObject
{
  MDY_SET_CRC32_HASH_WITH_TYPE(FDyUiObjectChildrenable);
  MDY_SET_TYPEMATCH_FUNCTION(FDyUiObject, FDyUiObjectChildrenable);
  MDY_ONLY_MOVEABLE_PROPERTIES_DEFAULT(FDyUiObjectChildrenable)
public:
  FDyUiObjectChildrenable() = default;
  virtual ~FDyUiObjectChildrenable() = default;

  ///
  /// @TODO IMPLEMENT THIS & AUTOMATIC SPECIFIER NAME CREATION.
  template <typename TUiObjectType, typename TArgument>
  TUiObjectType* AddUiObject(const TArgument& arg)
  {
    static_assert(std::is_base_of_v<FDyUiObject, TUiObjectType> == true, "TUiObjectType is not derived from FDyUiObject.");

    auto object = std::make_unique<TUiObjectType>();
    MDY_CALL_ASSERT_SUCCESS(object->Initialize(arg));
    object->SetParentUiObject(*this);

    auto& it = this->mUiObjectList.emplace_back(static_cast<FDyUiObject*>(object.release()));
    return static_cast<TUiObjectType*>(it.get());
  }

  ///
  /// @TODO IMPLEMENT THIS
  /// @brief UiObject version.
  ///
  MDY_NODISCARD FDyUiObject* GetUiObject(const std::string& objectName, EDySearchMode searchMode = EDySearchMode::Default);

  ///
  /// @brief Template version. Able to retrieve object as a TType.
  ///
  template <typename TType>
  MDY_NODISCARD TType* GetUiObject(const std::string& objectName, EDySearchMode searchMode = EDySearchMode::Default)
  {
    static_assert(std::is_base_of_v<FDyUiObject, TType> == true, "TType is not derived from FDyUiObject.");
    FDyUiObject* objectPtr = this->GetUiObject(objectName, searchMode);
    return static_cast<TType*>(objectPtr);
  }

  ///
  /// @TODO IMPLEMENT THIS
  void RemoveUiObject();

  ///
  /// @brief
  ///
  void Render() override;

private:
  /// @brief Ui object list.
  std::vector<std::unique_ptr<FDyUiObject>> mUiObjectList = {};
};

} /// ::dy namespace

#endif /// GUARD_DY_ELEMENT_CANVAS_UIOBJECTCHILDRENABLE_H