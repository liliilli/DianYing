#ifndef GUARD_DY_META_DESCRIPTOR_WIDGETCOMMONDESCRIPTOR_H
#define GUARD_DY_META_DESCRIPTOR_WIDGETCOMMONDESCRIPTOR_H
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

#include <utility>
#include <memory>
#include <unordered_map>
#include <string_view>
#include <nlohmann/json_fwd.hpp>

namespace dy
{

///
/// @enum   EDyWidgetComponentType
/// @brief
///
enum class EDyWidgetComponentType
{
  Root,
  Text,
  HorizontalBox,
  VerticalBox,
  NoneError
};

///
/// @brief
/// @param
/// @return
///
MDY_NODISCARD EDyWidgetComponentType DyGetWidgetComponentTypeFrom(_MIN_ const std::string& typeString);

struct PDyMetaWidgetChildableBaseDesc;
struct PDyMetaWidgetCommonBaseDesc;

///
/// @struct PDyMetaWidgetChildableBaseDesc
/// @brief  Widget component base descriptor type that is able to have children component.
///
struct PDyMetaWidgetChildableBaseDesc
{
  using TElementType = std::pair<EDyWidgetComponentType, std::unique_ptr<PDyMetaWidgetCommonBaseDesc>>;
  std::unordered_map<std::string, TElementType> mChildComponentList = {};
};

///
/// @struct PDyMetaWidgetCommonBaseDesc
/// @brief  Widget component base descriptor type
///
struct PDyMetaWidgetCommonBaseDesc : public PDyMetaWidgetChildableBaseDesc
{ /// Specification name
  std::string                   mComponentSpecifierName = MDY_INITILAIZE_EMPTYSTR;
  /// Widget component type
  EDyWidgetComponentType        mComponentType          = EDyWidgetComponentType::NoneError;
  /// Parent component pointer. if root, rawptr must be nullptr.
  PDyMetaWidgetCommonBaseDesc*  mParentRawPtr           = MDY_INITIALIZE_NULL;
  /// Parent component specifier name.
  std::string                   mParentSpecifierName    = MDY_INITILAIZE_EMPTYSTR;

  //!
  //! Json header file
  //!

  inline static MDY_SET_IMMUTABLE_STRING(sHeader_Name,  "Name");
  inline static MDY_SET_IMMUTABLE_STRING(sHeader_Type,  "Type");
  inline static MDY_SET_IMMUTABLE_STRING(sHeader_Parent,"Parent");
  inline static MDY_SET_IMMUTABLE_STRING(sHeader_Details,"Details");

  inline static MDY_SET_IMMUTABLE_STRING(sHeader_Script,"Script");
};

///
/// @struct PDyMetaWidgetRootDescriptor
/// @brief Text component of widget descriptor
///
struct PDyMetaWidgetRootDescriptor final : public PDyMetaWidgetChildableBaseDesc
{
  /// Widget specifier name
  std::string   mWidgetSpecifierName  = MDY_INITILAIZE_EMPTYSTR;
  /// Script (widget) specifier name
  std::string   mScriptSpecifierName  = MDY_INITILAIZE_EMPTYSTR;

  ///
  /// @brief Factory function for PDyMetaWidgetTextDescriptor.
  /// @param itemAtlas valid json atlas instance.
  /// @return If succeeded, return heap instance of PDyMetaWidgetTextDescriptor.
  ///
  static std::unique_ptr<PDyMetaWidgetRootDescriptor>
  CreateMetaInformation(_MIN_ const nlohmann::json& itemAtlas);
};

} /// ::dy namespace

#endif /// GUARD_DY_META_DESCRIPTOR_WIDGETCOMMONDESCRIPTOR_H