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
#include <Dy/Meta/Information/ComponentMetaInformation.h>

namespace dy
{

///
/// @enum   EDyWidgetComponentType
/// @brief  Widget component object type.
///
enum class EDyWidgetComponentType
{
  Root,
  Text,
  HorizontalLayout,
  VerticalLayout,
  NoneError
};

void to_json  (_MINOUT_ nlohmann::json& j,    _MIN_ const EDyWidgetComponentType& p);
void from_json(_MIN_ const nlohmann::json& j, _MOUT_ EDyWidgetComponentType& p);

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
  std::string                   mUiObjectSpecifierName  = MDY_INITIALIZE_EMPTYSTR;
  /// Widget component type
  EDyWidgetComponentType        mComponentType          = EDyWidgetComponentType::NoneError;
  /// Parent component pointer. if root, rawptr must be nullptr.
  PDyMetaWidgetCommonBaseDesc*  mParentRawPtr           = MDY_INITIALIZE_NULL;
  /// Parent component specifier name.
  std::string                   mParentSpecifierName    = MDY_INITIALIZE_EMPTYSTR;

  //!
  //! Json header file
  //!

  inline static MDY_SET_IMMUTABLE_STRING(sHeader_Name,  "Name");
  inline static MDY_SET_IMMUTABLE_STRING(sHeader_Type,  "Type");
  inline static MDY_SET_IMMUTABLE_STRING(sHeader_Parent,"Parent");
  inline static MDY_SET_IMMUTABLE_STRING(sHeader_Details,"Details");
};

///
/// @struct PDyMetaWidgetRootDescriptor
/// @brief Text component of widget descriptor
///
struct PDyMetaWidgetRootDescriptor final : public PDyMetaWidgetChildableBaseDesc
{
  /// Widget specifier name
  std::string   mWidgetSpecifierName  = MDY_INITIALIZE_EMPTYSTR;
  /// Script component variable.
  PDyScriptComponentMetaInfo mScriptReference = {};

  ///
  /// @brief Factory function for PDyMetaWidgetTextDescriptor.
  /// @param itemAtlas valid json atlas instance.
  /// @return If succeeded, return heap instance of PDyMetaWidgetTextDescriptor.
  ///
  static std::unique_ptr<PDyMetaWidgetRootDescriptor>
  CreateMetaInformation(_MIN_ const nlohmann::json& itemAtlas);

  inline static MDY_SET_IMMUTABLE_STRING(sHeader_Name,  "Name");
  inline static MDY_SET_IMMUTABLE_STRING(sHeader_Script,"Script");
};

} /// ::dy namespace

#endif /// GUARD_DY_META_DESCRIPTOR_WIDGETCOMMONDESCRIPTOR_H