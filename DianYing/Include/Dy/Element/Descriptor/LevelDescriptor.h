#ifndef GUARD_DY_ELELMENT_DESCRIPTOR_LEVELDESCRIPTOR_H
#define GUARD_DY_ELELMENT_DESCRIPTOR_LEVELDESCRIPTOR_H
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

#include <any>
#include <Dy/Helper/Type/Color.h>
#include <nlohmann/json_fwd.hpp>
#include "Dy/Component/Descriptor/ComponentMetaDescriptor.h"

namespace dy
{

///
/// @enum EDyMetaObjectType
/// @brief
///
enum class EDyMetaObjectType
{
  Actor,
  SceneScriptor,
  Object,
  NoneError
};

///
/// @struct DDyObjectInformation
/// @brief object meta information container
///
struct DDyObjectInformation final
{
  /// Meta index for classification of objects. Might not be used in game runtime.
  TI32              mMetaIndex        = MDY_INITIALIZE_DEFINT;
  /// Meta object's name.
  std::string       mObjectName       = MDY_INITILAIZE_EMPTYSTR;
  /// The type (light, pawn, pp block etc...) of object
  EDyMetaObjectType mObjectType       = EDyMetaObjectType::NoneError;
  /// Meta object's hash value to verify it from other information instance.
  std::string       mHashValue        = MDY_INITILAIZE_EMPTYSTR;
  /// Parent meta hash name of this object.
  std::string       mParentHashValue  = MDY_INITILAIZE_EMPTYSTR;

  /// Dependency information which are vary along with mType.
  /// If mType is "EDyFDyObjectType::FdyPawn", must be converted to "DDyScriptMetaInformation".
  /// If mType is ... not implemented yet!
  std::vector<std::pair<EDyComponentMetaType, std::any>> mMetaComponentInfo;
};

///
/// @struct PDyLevelConstructDescriptor
/// @brief Level construction descriptor.
///
struct PDyLevelConstructDescriptor final
{
  /// Initial level name
  std::string                       mLevelName                  = MDY_INITILAIZE_EMPTYSTR;
  /// Initial background of scene to create
  DDyColor                          mLevelBackgroundColor       = DDyColor::White;
  /// Object information
  std::vector<DDyObjectInformation> mLevelObjectInformations    = {};

  /// Make descriptor
  static MDY_NODISCARD PDyLevelConstructDescriptor CreateDescriptor(const nlohmann::json& jsonAtlas);
};

} /// ::dy namespace

#endif /// GUARD_DY_ELELMENT_DESCRIPTOR_LEVELDESCRIPTOR_H