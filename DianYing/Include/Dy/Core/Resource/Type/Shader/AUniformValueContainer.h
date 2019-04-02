#ifndef GUARD_DY_CORE_TYPE_SHADER_ADYUNIFORMCONTAINER_H
#define GUARD_DY_CORE_TYPE_SHADER_ADYUNIFORMCONTAINER_H
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

#include <Dy/Helper/Library/HelperContainer.h>
#include <Dy/Helper/System/Idioms.h>
#include <Dy/Core/Resource/Internal/XShaderTypes.h>
#include <Dy/Core/Resource/Internal/Uniform/IUniformValueBase.h>
#include <Dy/Core/Resource/Type/Shader/XTemplateUniformType.h>
#include <Dy/Core/Resource/Internal/Uniform/IUniformStructs.h>
#include <Dy/Helper/Type/DColorRGB.h>

//!
//! Forward declaration
//!

namespace dy
{
class FResourceShader;
} /// ::dy namespace

//!
//! Implementation
//!

namespace dy
{

/// @class AUniformValueContainer
/// @brief Store uniform value & structure values and update when calling proper API.
class AUniformValueContainer
{
public:
  virtual ~AUniformValueContainer() = 0;

  /// @brief Try clear uniform list.
  void __TryClearUniformList();
  /// @brief Try construct default uniform variable list.
  /// If already have value, do nothing.
  void __TryConstructDefaultUniformList(const FResourceShader& iResc);

  /// @brief Try update uniform value.
  /// If uniform value is not exist on binding shader, just insert value with -1 id (garbage value)
  /// and if uniform value is exist but value is same, do not update value for performance.
  /// If uniform value is exsit but need to be updated, overwrite value and push value item into queue
  /// for updating uniform values.
  template <EUniformVariableType TType>
  EDySuccess TryUpdateUniform(
    const std::string& iSpecifier, 
    const typename MDY_PRIVATE(UniformBinder)<TType>::ValueType& iValue);

  /// @brief Try update with structurized value instance.
  template <typename TType>
  EDySuccess TryUpdateUniformStruct(TU32 iIndex, const TType& iContainer);

  /// @brief Try update with structurized value instance but without item index.
  template <typename TType>
  EDySuccess TryUpdateUniformStruct(const TType& iContainer);

  /// @brief Try insert texture requisition. \n
  /// If required `insertId` is out of bound of count of available texture, just do nothing but return DY_FAILURE.
  /// When successful, find proper texture type from container, make requisition issue item and insert to queue. \n
  /// This function does not update texture binding status but need to call `TryUpdateUniformList` function.
  EDySuccess TryInsertTextureRequisition(TU32 insertId, TU32 textureId);

  /// @brief Get the type of given string of uniform variable.
  MDY_NODISCARD EUniformVariableType GetTypeOfUniform(const std::string& iSpecifier) const noexcept;

  /// @brief Try update uniform variables and new texture requisition. \n
  /// Do nothing when update list is empty or binding flag is not set up
  /// by calling MDY_PRIVATE(TryConstructDefaultUniformList)().
  EDySuccess TryUpdateUniformList();

  /// @brief Get available texture count.
  MDY_NODISCARD TU32 GetAvailableTextureCount() const noexcept;

private:
  void pTryConstructDefaultUniformVariables(
    const std::vector<DUniformVariableInformation>& iUniformList);
  void pTryConstructDefaultUniformStructLists(
    const std::unordered_map<
    std::string, 
    std::pair<std::string, std::vector<DUniformStructVarInformation>>>& iUniformList);
  void pTryConstructDefaultUniformItemList(
    const std::unordered_map<
    std::string, 
    std::pair<std::string, DUniformStructVarInformation>>& iUniformList);

  void pTryUpdateUniformVariables();
  void pTryUpdateUniformTextureVars();
  void pTryUpdateUniformStructVars();

  /// @struct DTextureUpdateItem
  /// @brief Texture binding requisition item type. This type is used only in AUniformValueContainer.
  struct DTextureUpdateItem final
  {
    TU32 mInsertId  = 0;
    EUniformVariableType mType = EUniformVariableType::NoneError;
    TU32 mTextureId = 0;

    DTextureUpdateItem(TU32 insertId, EUniformVariableType type, TU32 textureId) 
      : mInsertId{insertId}, mType{type}, mTextureId{textureId} 
    {};
  };

  template <EUniformVariableType TType> using TValueType 
    = typename MDY_PRIVATE(UniformBinder)<TType>::ValueType;

  using TUniformMap         = std::unordered_map<std::string, std::unique_ptr<IUniformValueBase>>;
  using TUpdatedItemList    = std::vector<IUniformValueBase*>;
  using TUpdatedStructList  = std::vector<std::tuple<std::string, TI32, IUniformValueBase*>>;
  using TUpdatedTextureList = std::vector<DTextureUpdateItem>;

  TUniformMap mUniformMap {};
  std::unordered_map<std::string, IUniformStructItem> mUniformStructItemMap;
  std::unordered_map<std::string, IUniformStructList> mUniformStructListMap;

  TUpdatedItemList    mUpdatedItemList = {};
  TUpdatedStructList  mUpdatedStructList = {};
  TUpdatedTextureList mUpdatedTextureList = {};

  bool mIsShaderSetuped = false;
  TU32 mAvailableTextureCount = 0;
};

inline AUniformValueContainer::~AUniformValueContainer() = default;

} /// ::dy namespace

#endif /// !GUARD_DY_CORE_TYPE_SHADER_ADYUNIFORMCONTAINER_H
#include <Dy/Core/Resource/Type/Inline/AUniformContainerTmp.inl>