#ifndef GUARD_DY_CORE_TYPE_SHADER_ADYUNIFORMCONTAINER_TMP_H
#define GUARD_DY_CORE_TYPE_SHADER_ADYUNIFORMCONTAINER_TMP_H
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

#include <Dy/Core/Resource/Type/Uniform/UniformValueTypes.h>
#include <Dy/Core/Reflection/RReflection.h>

namespace dy
{

template <EDyUniformVariableType TType>
EDySuccess ADyUniformContainer::TryUpdateUniform(
  const std::string& iSpecifier, 
  const typename MDY_PRIVATE(UniformBinder)<TType>::ValueType& iValue)
{
  const auto it = this->mUniformMap.find(iSpecifier);
  if (it == this->mUniformMap.end())
  { // If not found, just insert it anyway.
    auto [createdPair, _] = this->mUniformMap.try_emplace(
      iSpecifier, 
      std::make_unique<FDyUniformValue<TType>>(-1, iValue)
    );
    DyPushLogError("Could not find uniform value but insert anyway as id -1. {}", iSpecifier);
    return DY_FAILURE;
  }
  else
  { // Check type but not matched, pass it.
    auto& [_, smtptrInstance] = *it;
    if (smtptrInstance->mType != TType)
    {
      DyPushLogError("Could not insert uniform value becasue of different type. {}", iSpecifier);
      return DY_FAILURE;
    }
    // In case of success.
    auto* ptrInstance = static_cast<FDyUniformValue<TType>*>(smtptrInstance.get());
    if (ptrInstance->mValue == iValue) { return DY_SUCCESS; }

    ptrInstance->mValue = iValue;
    this->mUpdatedItemList.emplace_back(ptrInstance);
    return DY_SUCCESS;
  }
}

#define __MDY_UNIFORM_STRUCT_COMPARE_AND_INSERT(__Type__, __ActualType__) \
  const auto& value = typeName.template GetValueOf<__ActualType__>(iContainer, varName); \
  if (auto* ptrInstance = static_cast<FDyUniformValue<EUnif::__Type__>*>(memberValue.get()); \
      ptrInstance->mValue != value) \
  { \
    ptrInstance->mValue = value; \
    this->mUpdatedStructList.emplace_back(aliasName, TI32(iIndex), ptrInstance); \
  }

template <typename TType>
EDySuccess ADyUniformContainer::TryUpdateUniformStruct(TU32 iIndex, const TType& iContainer)
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
    case ERefl::Int:      { __MDY_UNIFORM_STRUCT_COMPARE_AND_INSERT(Integer, int); } break;
    case ERefl::Float:    { __MDY_UNIFORM_STRUCT_COMPARE_AND_INSERT(Float, float); } break;
    case ERefl::Vector2:  { __MDY_UNIFORM_STRUCT_COMPARE_AND_INSERT(Vector2, DDyVector2); } break;
    case ERefl::Vector3:  { __MDY_UNIFORM_STRUCT_COMPARE_AND_INSERT(Vector3, DDyVector3); } break;
    case ERefl::Vector4:  { __MDY_UNIFORM_STRUCT_COMPARE_AND_INSERT(Vector4, DDyVector4); } break;
    case ERefl::Matrix4:  { __MDY_UNIFORM_STRUCT_COMPARE_AND_INSERT(Matrix4, DDyMatrix4x4); } break;
    case ERefl::ColorRGB: 
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

} /// ::dy namespace

#endif /// GUARD_DY_CORE_TYPE_SHADER_ADYUNIFORMCONTAINER_TMP_H