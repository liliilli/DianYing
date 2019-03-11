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

#include <Dy/Helper/ContainerHelper.h>
#include <Dy/Helper/System/Idioms.h>
#include <Dy/Core/Resource/Internal/ShaderType.h>
#include <Dy/Core/Resource/Internal/Uniform/IDyUniformValueBase.h>
#include <Dy/Core/Resource/Type/Shader/TemplateUniformType.h>
#include <Dy/Core/Resource/Type/Uniform/UniformValueTypes.h>
#include <Dy/Core/Resource/Internal/Uniform/IDyUniformStruct.h>
#include <Dy/Core/Reflection/RReflection.h>
#include <Dy/Helper/Type/ColorRGB.h>

//!
//! Forward declaration
//!

namespace dy
{
class FDyShaderResource;
} /// ::dy namespace

//!
//! Implementation
//!

namespace dy
{

class ADyUniformContainer
{
public:
  virtual ~ADyUniformContainer() = 0;

  /// @brief Try clear uniform list.
  void __TryClearUniformList();
  /// @brief Try construct default uniform variable list.
  /// If already have value, do nothing.
  void __TryConstructDefaultUniformList(const FDyShaderResource& iResc);

  /// @brief Try update uniform value.
  /// If uniform value is not exist on binding shader, just insert value with -1 id (garbage value)
  /// and if uniform value is exist but value is same, do not update value for performance.
  /// If uniform value is exsit but need to be updated, overwrite value and push value item into queue
  /// for updating uniform values.
  template <EDyUniformVariableType TType>
  EDySuccess TryUpdateUniform(
    const std::string& iSpecifier, 
    const typename MDY_PRIVATE(UniformBinder)<TType>::ValueType& iValue);

  /// @brief Try update with structurized value instance.
  template <typename TType>
  EDySuccess TryUpdateUniformStruct(
    TU32 iIndex,
    const TType& iContainer)
  {
    const auto& aliasName = reflect::RUniformReflection::GetFirstAliasOf(TType::__sTypeName);
    if (DyIsMapContains(this->mUniformStructListMap, aliasName) == false) { return DY_FAILURE; }

    const auto& typeName = reflect::RUniformReflection::GetData(TType::__sTypeName);
    auto& data = this->mUniformStructListMap.at(aliasName);
    for (auto& memberValue : data.mItems[iIndex].mMemberValues)
    {
      const auto& varName = typeName.GetVarNameOf(memberValue->mSpecifier);
      using ERefl = reflect::EReflectScopeType; 
      using EUnif = EDyUniformVariableType;
      const ERefl varType = typeName.GetTypeOf(varName);
      switch (varType)
      {
      case ERefl::Int: 
      {
        const auto& value = typeName.template GetValueOf<int>(iContainer, varName);
        if (auto* ptrInstance = static_cast<FDyUniformValue<EUnif::Integer>*>(memberValue.get());
            ptrInstance->mValue != value)
        {
          ptrInstance->mValue = value;
          this->mUpdatedStructList.emplace_back(aliasName, TI32(iIndex), ptrInstance);
        }
      } break;
      case reflect::EReflectScopeType::Float:
      {
        const auto& value = typeName.template GetValueOf<float>(iContainer, varName);
        if (auto* ptrInstance = static_cast<FDyUniformValue<EUnif::Float>*>(memberValue.get());
            ptrInstance->mValue != value)
        {
          ptrInstance->mValue = value;
          this->mUpdatedStructList.emplace_back(aliasName, TI32(iIndex), ptrInstance);
        }
      } break;
      case reflect::EReflectScopeType::Vector2:
      {
        const auto& value = typeName.template GetValueOf<DDyVector2>(iContainer, varName);
        if (auto* ptrInstance = static_cast<FDyUniformValue<EUnif::Vector2>*>(memberValue.get());
            ptrInstance->mValue != value)
        {
          ptrInstance->mValue = value;
          this->mUpdatedStructList.emplace_back(aliasName, TI32(iIndex), ptrInstance);
        }
      } break;
      case reflect::EReflectScopeType::Vector3:
      {
        const auto& value = typeName.template GetValueOf<DDyVector3>(iContainer, varName);
        if (auto* ptrInstance = static_cast<FDyUniformValue<EUnif::Vector3>*>(memberValue.get());
            ptrInstance->mValue != value)
        {
          ptrInstance->mValue = value;
          this->mUpdatedStructList.emplace_back(aliasName, TI32(iIndex), ptrInstance);
        }
      } break;
      case reflect::EReflectScopeType::Vector4:
      {
        const auto& value = typeName.template GetValueOf<DDyVector4>(iContainer, varName);
        if (auto* ptrInstance = static_cast<FDyUniformValue<EUnif::Vector4>*>(memberValue.get());
            ptrInstance->mValue != value)
        {
          ptrInstance->mValue = value;
          this->mUpdatedStructList.emplace_back(aliasName, TI32(iIndex), ptrInstance);
        }
      } break;
      case reflect::EReflectScopeType::Matrix4:
      {
        const auto& value = typeName.template GetValueOf<DDyMatrix4x4>(iContainer, varName);
        if (auto* ptrInstance = static_cast<FDyUniformValue<EUnif::Matrix4>*>(memberValue.get());
            ptrInstance->mValue != value)
        {
          ptrInstance->mValue = value;
          this->mUpdatedStructList.emplace_back(aliasName, TI32(iIndex), ptrInstance);
        }
      } break;
      case reflect::EReflectScopeType::ColorRGB: 
      {
        const auto& value = typeName.template GetValueOf<DDyColorRGB>(iContainer, varName);
        if (auto* ptrInstance = static_cast<FDyUniformValue<EUnif::Vector3>*>(memberValue.get());
            ptrInstance->mValue != static_cast<DDyVector3>(value))
        {
          ptrInstance->mValue = static_cast<DDyVector3>(value);
          this->mUpdatedStructList.emplace_back(aliasName, TI32(iIndex), ptrInstance);
        }
      } break;
      default: MDY_NOT_IMPLEMENTED_ASSERT(); break;
      }
    }

    return DY_SUCCESS;
  }

  /// @brief Try insert texture requisition. \n
  /// If required `insertId` is out of bound of count of available texture, just do nothing but return DY_FAILURE.
  /// When successful, find proper texture type from container, make requisition issue item and insert to queue. \n
  /// This function does not update texture binding status but need to call `TryUpdateUniformList` function.
  EDySuccess TryInsertTextureRequisition(TU32 insertId, TU32 textureId);

  /// @brief Get the type of given string of uniform variable.
  MDY_NODISCARD EDyUniformVariableType GetTypeOfUniform(const std::string& iSpecifier) const noexcept;

  /// @brief Try update uniform variables and new texture requisition. \n
  /// Do nothing when update list is empty or binding flag is not set up
  /// by calling MDY_PRIVATE(TryConstructDefaultUniformList)().
  EDySuccess TryUpdateUniformList();

  /// @brief Get available texture count.
  MDY_NODISCARD TU32 GetAvailableTextureCount() const noexcept;

private:
  void pTryConstructDefaultUniformVariables(
    const std::vector<DDyUniformVariableInformation>& iUniformList);
  void pTryConstructDefaultUniformStructLists(
    const std::unordered_map<
    std::string, 
    std::pair<std::string, std::vector<DDyUniformStructVarInformation>>>& iUniformList);
  void pTryConstructDefaultUniformItemList(
    const std::unordered_map<
    std::string, 
    std::pair<std::string, DDyUniformStructVarInformation>>& iUniformList);

  void pTryUpdateUniformVariables();
  void pTryUpdateUniformTextureVars();
  void pTryUpdateUniformStructVars();

  /// @struct DTextureUpdateItem
  /// @brief Texture binding requisition item type. This type is used only in ADyUniformContainer.
  struct DTextureUpdateItem final
  {
    TU32 mInsertId  = 0;
    EDyUniformVariableType mType = EDyUniformVariableType::NoneError;
    TU32 mTextureId = 0;

    DTextureUpdateItem(TU32 insertId, EDyUniformVariableType type, TU32 textureId) 
      : mInsertId{insertId}, mType{type}, mTextureId{textureId} 
    {};
  };

  template <EDyUniformVariableType TType> using TValueType 
    = typename MDY_PRIVATE(UniformBinder)<TType>::ValueType;

  using TUniformMap         = std::unordered_map<std::string, std::unique_ptr<IDyUniformValueBase>>;
  using TUpdatedItemList    = std::vector<IDyUniformValueBase*>;
  using TUpdatedStructList  = std::vector<std::tuple<std::string, TI32, IDyUniformValueBase*>>;
  using TUpdatedTextureList = std::vector<DTextureUpdateItem>;

  TUniformMap mUniformMap {};
  std::unordered_map<std::string, IDyUniformStructItem> mUniformStructItemMap;
  std::unordered_map<std::string, IDyUniformStructList> mUniformStructListMap;

  TUpdatedItemList    mUpdatedItemList = {};
  TUpdatedStructList  mUpdatedStructList = {};
  TUpdatedTextureList mUpdatedTextureList = {};

  bool mIsShaderSetuped = false;
  TU32 mAvailableTextureCount = 0;
};

inline ADyUniformContainer::~ADyUniformContainer() = default;

} /// ::dy namespace

#endif /// !GUARD_DY_CORE_TYPE_SHADER_ADYUNIFORMCONTAINER_H
#include <Dy/Core/Resource/Type/Inline/AUniformContainerTmp.inl>