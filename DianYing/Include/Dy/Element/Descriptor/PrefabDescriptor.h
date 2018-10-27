#ifndef GUARD_DY_ELEMENT_DESCRIPTOR_CANVASDESCRIPTOR_H
#define GUARD_DY_ELEMENT_DESCRIPTOR_CANVASDESCRIPTOR_H
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
#include <vector>
#include <utility>
#include <variant>

#include <Dy/Component/Descriptor/ComponentMetaDescriptor.h>
#include <Dy/Element/Descriptor/GlobalEnums.h>

namespace dy
{

///
/// @struct PDyPrefabMetaInformation
/// @brief  Prefab meta information
///
struct PDyPrefabMetaInformation final
{
  using TChildNameList      = std::vector<std::string>;
  using TChildMetaInfoList  = std::vector<std::unique_ptr<PDyPrefabMetaInformation>>;

  /// Meta prefab's name.
  std::string         mSpecifierName          = MDY_INITILAIZE_EMPTYSTR;
  /// The type (light, pawn, pp block etc...) of object
  EDyMetaObjectType   mPrefabType             = EDyMetaObjectType::NoneError;
  /// Flag is initially activated.
  bool                mIsInitiallyActivated   = false;
  /// Children list
  TChildMetaInfoList  mChildrenList           = {};
  /// Dependency information which are vary along with mType.
  /// If mType is "EDyFDyObjectType::FdyPawn", must be converted to "PDyScriptComponentMetaInformation".
  /// If mType is ... not implemented yet!
  std::vector<std::pair<EDyComponentMetaType, std::any>> mMetaComponentInfo = {};
};

} /// ::dy namespace

#endif /// GUARD_DY_ELEMENT_DESCRIPTOR_CANVASDESCRIPTOR_H