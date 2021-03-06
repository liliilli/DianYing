#include <precompiled.h>
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

/// Header file
#include <Dy/Core/Resource/Type/Shader/AUniformValueContainer.h>

#include <regex>
#include <Dy/Core/Resource/Resource/FResourceShader.h>
#include <Dy/Core/Rendering/Wrapper/XGLWrapper.h>
#include <Dy/Core/Resource/Type/Uniform/TUniformValue.h>
#include <Dy/Core/Resource/Type/TResourceBinder.h>
#include <Dy/Core/Resource/Resource/AResourceAttachmentBase.h>
#include <Dy/Core/Reflection/RReflection.h>

#include <Dy/Include/GlInclude.h>

#define MDY_CONSTRUCT_UNIFORM_ITEM(__MATypeSpecifier__, __MASpecifier__, __MAId__) \
    auto [it2, MDY_NOUSEVAR] = this->mUniformMap.try_emplace( \
      __MASpecifier__, std::make_unique<TUniformValue<TEnum::__MATypeSpecifier__>>(__MAId__)); \
      it2->second->mSpecifier = __MASpecifier__; \
    auto& [MDY_NOUSEVAR##_2, smtptrInstance] = *it2; \
    /* Insert default to update list. */ \
    this->mUpdatedItemList.emplace_back(smtptrInstance.get());

#define MDY_CONSTRUCT_UNIFORM_TEXTURE_ITEM(__MATypeSpecifier__, __MASpecifier__, __MAId__) \
    /* Check uniform name. */ \
    std::unique_ptr<TUniformValue<TEnum::__MATypeSpecifier__>> instance = nullptr; \
    if (std::regex_match(__MASpecifier__, textureRegex) == true) \
    { \
      const auto value = std::stoi(__MASpecifier__.substr(8)); \
      instance = std::make_unique<TUniformValue<TEnum::__MATypeSpecifier__>>(__MAId__, value); \
    } \
    else { instance = std::make_unique<TUniformValue<TEnum::__MATypeSpecifier__>>(__MAId__); } \
    /* Insert instance. */ \
    auto [it2, MDY_NOUSEVAR] = this->mUniformMap.try_emplace(__MASpecifier__, std::move(instance)); \
    it2->second->mSpecifier = __MASpecifier__; \
    auto& [MDY_NOUSEVAR##_2, smtptrInstance] = *it2; \
    /* Insert default to update list. */ \
    this->mUpdatedItemList.emplace_back(smtptrInstance.get()); \
    this->mAvailableTextureCount++;

#define MDY_CONSTRUCT_UNIFORM_STRUCT_ITEM(__Container__, __MATypeSpecifier__, __MAPrefix__, __MAPost__, __MAId__) \
    auto& ref = __Container__.emplace_back( \
      std::make_unique<TUniformValue<TEnum::__MATypeSpecifier__>>(__MAId__)); \
    ref->mSpecifier = __MAPost__; \
    this->mUpdatedStructList.emplace_back(std::tuple(__MAPrefix__, -1, ref.get()));

#define MDY_CONSTRUCT_UNIFORM_STRUCT_LIST_ITEM(__Container__, __MATypeSpecifier__, __MAPrefix__, __MAPost__, __MAId__, __Index__) \
    auto& ref = __Container__.emplace_back( \
      std::make_unique<TUniformValue<TEnum::__MATypeSpecifier__>>(__MAId__)); \
    ref->mSpecifier = __MAPost__; \
    this->mUpdatedStructList.emplace_back(std::tuple(__MAPrefix__, __Index__, ref.get()));

namespace dy
{

void AUniformValueContainer::__TryClearUniformList()
{
  this->mUniformMap.clear();
  this->mUniformStructItemMap.clear();
  this->mUniformStructListMap.clear();
  this->mUpdatedItemList.clear();
  this->mUpdatedTextureList.clear();
  this->mUpdatedStructList.clear();
  this->mIsShaderSetuped = false;
  this->mAvailableTextureCount = 0;
}

#define MDY_NOUSEVAR MDY_TOKENPASTE2(_, __LINE__)

void AUniformValueContainer::__TryConstructDefaultUniformList(const FResourceShader& iResc)
{
  this->pTryConstructDefaultUniformVariables(iResc.GetUniformVariableList());
  this->pTryConstructDefaultUniformStructLists(iResc.GetUniformStructListMap());
  this->pTryConstructDefaultUniformItemList(iResc.GetUniformStructItemMap());
  
  this->mIsShaderSetuped = true;
}

void AUniformValueContainer::pTryConstructDefaultUniformVariables(
const std::vector<DUniformVariableInformation>& iUniformList)
{
  static std::regex textureRegex{"uTexture\\d+(_\\w*)?"};

  for (const auto& [specifier, MDY_NOUSEVAR, type, id] : iUniformList)
  {
    // Check there is already specifier value, if exist do nothing
    // but not exist, insert with default value and bring it to update list.
    using TEnum = EUniformVariableType;
    switch (type)
    {
    case TEnum::Matrix4:       { MDY_CONSTRUCT_UNIFORM_ITEM(Matrix4, specifier, id); } break;
    case TEnum::Matrix4Array:  { MDY_CONSTRUCT_UNIFORM_ITEM(Matrix4Array, specifier, id); } break;
    case TEnum::Matrix3:       { MDY_CONSTRUCT_UNIFORM_ITEM(Matrix3, specifier, id); } break;
    case TEnum::Vector4:       { MDY_CONSTRUCT_UNIFORM_ITEM(Vector4, specifier, id); } break;
    case TEnum::Vector3:       { MDY_CONSTRUCT_UNIFORM_ITEM(Vector3, specifier, id); } break;
    case TEnum::Vector3Array:  { MDY_CONSTRUCT_UNIFORM_ITEM(Vector3Array, specifier, id); } break;
    case TEnum::Vector2:       { MDY_CONSTRUCT_UNIFORM_ITEM(Vector2, specifier, id); } break;
    case TEnum::Integer:       { MDY_CONSTRUCT_UNIFORM_ITEM(Integer, specifier, id); } break;
    case TEnum::Unsigned:      { MDY_CONSTRUCT_UNIFORM_ITEM(Unsigned, specifier, id); } break;
    case TEnum::Bool:          { MDY_CONSTRUCT_UNIFORM_ITEM(Bool, specifier, id); } break;
    case TEnum::Float:         { MDY_CONSTRUCT_UNIFORM_ITEM(Float, specifier, id); } break;
    case TEnum::FloatArray:    { MDY_CONSTRUCT_UNIFORM_ITEM(FloatArray, specifier, id); } break;
    case TEnum::Texture1D:            { MDY_CONSTRUCT_UNIFORM_TEXTURE_ITEM(Texture1D, specifier, id); } break;
    case TEnum::Texture2D:            { MDY_CONSTRUCT_UNIFORM_TEXTURE_ITEM(Texture2D, specifier, id); } break;
    case TEnum::Texture2DArray:       { MDY_CONSTRUCT_UNIFORM_TEXTURE_ITEM(Texture2DArray, specifier, id); } break;
    case TEnum::Texture2DRectangle:   { MDY_CONSTRUCT_UNIFORM_TEXTURE_ITEM(Texture2DRectangle, specifier, id); } break;
    case TEnum::Texture2DShadowArray: { MDY_CONSTRUCT_UNIFORM_TEXTURE_ITEM(Texture2DShadowArray, specifier, id); } break;
    case TEnum::Texture2DCubemap:     { MDY_CONSTRUCT_UNIFORM_TEXTURE_ITEM(Texture2DCubemap, specifier, id); } break;
    default: MDY_UNEXPECTED_BRANCH(); break;
    }
  }
  
  this->mUpdatedTextureList.reserve(this->mAvailableTextureCount);
}

void AUniformValueContainer::pTryConstructDefaultUniformStructLists(
  const std::unordered_map<
    std::string, 
    std::pair<std::string, std::vector<DUniformStructVarInformation>>>& iUniformList)
{
  for (const auto& [prefix, uniformItem] : iUniformList)
  {
    // Get information from list, and make List.
    auto [it, MDY_NOUSEVAR] = this->mUniformStructListMap.try_emplace(prefix, prefix);

    auto& [MDY_NOUSEVAR, information] = uniformItem;
    auto& structList = it->second;
    structList.mItems.reserve(information.size());

    for (size_t i = 0, size = information.size(); i < size; ++i)
    {
      auto& instance = structList.mItems.emplace_back();

      // setup each item. (Should be refactored)
      for (const auto& memberValue : information[i].mMemberValues)
      {
        using TEnum = EUniformVariableType;
        const auto& postfix  = memberValue.mVariableName;
        const auto uniformId = memberValue.mVariableLocation;
        switch (memberValue.mVariableType)
        {
        case TEnum::Matrix4: 
        { MDY_CONSTRUCT_UNIFORM_STRUCT_LIST_ITEM(
            instance.mMemberValues, Matrix4, prefix, postfix, uniformId, i); } break;
        case TEnum::Matrix3: 
        { MDY_CONSTRUCT_UNIFORM_STRUCT_LIST_ITEM(
            instance.mMemberValues, Matrix3, prefix, postfix, uniformId, i); } break;
        case TEnum::Vector4: 
        { MDY_CONSTRUCT_UNIFORM_STRUCT_LIST_ITEM(
            instance.mMemberValues, Vector4, prefix, postfix, uniformId, i); } break;
        case TEnum::Vector3: 
        { MDY_CONSTRUCT_UNIFORM_STRUCT_LIST_ITEM(
            instance.mMemberValues, Vector3, prefix, postfix, uniformId, i); } break;
        case TEnum::Vector2: 
        { MDY_CONSTRUCT_UNIFORM_STRUCT_LIST_ITEM(
            instance.mMemberValues, Vector2, prefix, postfix, uniformId, i); } break;
        case TEnum::Integer: 
        { MDY_CONSTRUCT_UNIFORM_STRUCT_LIST_ITEM(
            instance.mMemberValues, Integer, prefix, postfix, uniformId, i); } break;
        case TEnum::Unsigned: 
        { MDY_CONSTRUCT_UNIFORM_STRUCT_LIST_ITEM(
            instance.mMemberValues, Unsigned, prefix, postfix, uniformId, i); } break;
        case TEnum::Bool: 
        { MDY_CONSTRUCT_UNIFORM_STRUCT_LIST_ITEM(
            instance.mMemberValues, Bool, prefix, postfix, uniformId, i); } break;
        case TEnum::Float: 
        { MDY_CONSTRUCT_UNIFORM_STRUCT_LIST_ITEM(
            instance.mMemberValues, Float, prefix, postfix, uniformId, i); } break;
        default: MDY_UNEXPECTED_BRANCH(); break;
        }
      }
    }
  }
}

void AUniformValueContainer::pTryConstructDefaultUniformItemList(
  const std::unordered_map<
    std::string, 
    std::pair<std::string, DUniformStructVarInformation>>& iUniformList)
{
  for (const auto& [prefix, value] : iUniformList)
  {
    // get information from list, and make Item.
    auto& [MDY_NOUSEVAR, information] = value;
    auto [it, MDY_NOUSEVAR] = this->mUniformStructItemMap.try_emplace(prefix, prefix);
    auto& structItem = it->second;

    // and set type with postfix, 
    for (const auto& memberValue : information.mMemberValues)
    {
      const auto& postfix   = memberValue.mVariableName;
      const auto uniformId  = memberValue.mVariableLocation;
      using TEnum = EUniformVariableType;
      switch (memberValue.mVariableType) 
      {
      case TEnum::Matrix4: 
      { MDY_CONSTRUCT_UNIFORM_STRUCT_ITEM(structItem.mMemberValues, Matrix4, prefix, postfix, uniformId); } break;
      case TEnum::Matrix3: 
      { MDY_CONSTRUCT_UNIFORM_STRUCT_ITEM(structItem.mMemberValues, Matrix3, prefix, postfix, uniformId); } break;
      case TEnum::Vector4: 
      { MDY_CONSTRUCT_UNIFORM_STRUCT_ITEM(structItem.mMemberValues, Vector4, prefix, postfix, uniformId); } break;
      case TEnum::Vector3: 
      { MDY_CONSTRUCT_UNIFORM_STRUCT_ITEM(structItem.mMemberValues, Vector3, prefix, postfix, uniformId); } break;
      case TEnum::Vector2: 
      { MDY_CONSTRUCT_UNIFORM_STRUCT_ITEM(structItem.mMemberValues, Vector2, prefix, postfix, uniformId); } break;
      case TEnum::Integer: 
      { MDY_CONSTRUCT_UNIFORM_STRUCT_ITEM(structItem.mMemberValues, Integer, prefix, postfix, uniformId); } break;
      case TEnum::Unsigned: 
      { MDY_CONSTRUCT_UNIFORM_STRUCT_ITEM(structItem.mMemberValues, Unsigned, prefix, postfix, uniformId); } break;
      case TEnum::Bool: 
      { MDY_CONSTRUCT_UNIFORM_STRUCT_ITEM(structItem.mMemberValues, Bool, prefix, postfix, uniformId); } break;
      case TEnum::Float: 
      { MDY_CONSTRUCT_UNIFORM_STRUCT_ITEM(structItem.mMemberValues, Float, prefix, postfix, uniformId); } break;
      default: MDY_UNEXPECTED_BRANCH(); break;
      }
    }
  }
}

EDySuccess AUniformValueContainer::TryInsertTextureRequisition(TU32 insertId, TU32 textureId)
{
  // Find
  using TEnum = EUniformVariableType;
  static std::array<TEnum, 6> textureTypes = {
      TEnum::Texture1D, TEnum::Texture2D, TEnum::Texture2DArray, 
      TEnum::Texture2DCubemap, TEnum::Texture2DRectangle, TEnum::Texture2DShadowArray};
  for (auto& [specifier, item] : this->mUniformMap)
  {
    if (Contains(textureTypes, item->mType) == false) { continue; }
    // Insert.
    switch (item->mType)
    {
    case TEnum::Texture1D: 
    { auto& ref = static_cast<TUniformValue<TEnum::Texture1D>&>(*item);
      if (ref.mValue == TI32(insertId))
      {
        this->mUpdatedTextureList.emplace_back(insertId, item->mType, textureId);
        return EDySuccess::DY_SUCCESS;
      }
    } break;
    case EUniformVariableType::Texture2D:
    { auto& ref = static_cast<TUniformValue<TEnum::Texture2D>&>(*item);
      if (ref.mValue == TI32(insertId))
      {
        this->mUpdatedTextureList.emplace_back(insertId, item->mType, textureId);
        return EDySuccess::DY_SUCCESS;
      }
    } break;
    case EUniformVariableType::Texture2DArray:
    { auto& ref = static_cast<TUniformValue<TEnum::Texture2DArray>&>(*item);
      if (ref.mValue == TI32(insertId))
      {
        this->mUpdatedTextureList.emplace_back(insertId, item->mType, textureId);
        return EDySuccess::DY_SUCCESS;
      }
    } break;
    case EUniformVariableType::Texture2DShadowArray:
    { auto& ref = static_cast<TUniformValue<TEnum::Texture2DShadowArray>&>(*item);
      if (ref.mValue == TI32(insertId))
      {
        this->mUpdatedTextureList.emplace_back(insertId, item->mType, textureId);
        return EDySuccess::DY_SUCCESS;
      }
    } break;
    case EUniformVariableType::Texture2DRectangle:
    { auto& ref = static_cast<TUniformValue<TEnum::Texture2DRectangle>&>(*item);
      if (ref.mValue == TI32(insertId))
      {
        this->mUpdatedTextureList.emplace_back(insertId, item->mType, textureId);
        return EDySuccess::DY_SUCCESS;
      }
    } break;
    case EUniformVariableType::Texture2DCubemap: 
    { auto& ref = static_cast<TUniformValue<TEnum::Texture2DCubemap>&>(*item);
      if (ref.mValue == TI32(insertId))
      {
        this->mUpdatedTextureList.emplace_back(insertId, item->mType, textureId);
        return EDySuccess::DY_SUCCESS;
      }
    } break;
    default: MDY_UNEXPECTED_BRANCH(); break;
    }
  }

  return EDySuccess::DY_FAILURE;
}

EUniformVariableType AUniformValueContainer::GetTypeOfUniform(const std::string& iSpecifier) const noexcept
{
  if (Contains(this->mUniformMap, iSpecifier) == false)
  {
    return EUniformVariableType::NoneError;
  }

  return this->mUniformMap.at(iSpecifier)->mType;
}

EDySuccess AUniformValueContainer::TryUpdateUniformList()
{
  if (this->mIsShaderSetuped == false) { return EDySuccess::DY_FAILURE; }
  
  // We have to insert all variables into shader when updated, 
  // because shader is shared by any objects which want to render with it,
  // but each shader program has only one uniform variable status.
  if (this->mUpdatedItemList.empty() == false) 
  { 
    this->pTryUpdateUniformVariables();
    this->mUpdatedItemList.clear();
  }

  // Update texture also
  if (this->mUpdatedTextureList.empty() == false) 
  { 
    this->pTryUpdateUniformTextureVars();
    this->mUpdatedTextureList.clear();
    this->mUpdatedTextureList.reserve(this->mAvailableTextureCount);
  };

  // Update uniform structure variable info
  if (this->mUpdatedStructList.empty() == false)
  {
    this->pTryUpdateUniformStructVars();
    this->mUpdatedStructList.clear();
  }

  return EDySuccess::DY_SUCCESS;
}

void AUniformValueContainer::pTryUpdateUniformVariables()
{
  using TEnum = EUniformVariableType;
  for (auto& ptrItem : this->mUpdatedItemList)
  {
    switch (ptrItem->mType)
    {
    case TEnum::Matrix4: 
    { const auto* item = static_cast<TUniformValue<TEnum::Matrix4>*>(ptrItem);
      XGLWrapper::UpdateUniformMatrix4(item->mId, item->mValue);
    } break;
    case EUniformVariableType::Matrix4Array: 
    { const auto* item = static_cast<TUniformValue<TEnum::Matrix4Array>*>(ptrItem);
      XGLWrapper::UpdateUniformMatrix4Array(item->mId, item->mValue);
    } break;
    case EUniformVariableType::Matrix3: 
    { const auto* item = static_cast<TUniformValue<TEnum::Matrix3>*>(ptrItem);
      XGLWrapper::UpdateUniformMatrix3(item->mId, item->mValue);
    } break;
    case EUniformVariableType::Vector4: 
    { const auto* item = static_cast<TUniformValue<TEnum::Vector4>*>(ptrItem);
      XGLWrapper::UpdateUniformVector4(item->mId, item->mValue);
    } break;
    case EUniformVariableType::Vector3: 
    { const auto* item = static_cast<TUniformValue<TEnum::Vector3>*>(ptrItem);
      XGLWrapper::UpdateUniformVector3(item->mId, item->mValue);
    } break;
    case EUniformVariableType::Vector3Array: 
    { const auto* item = static_cast<TUniformValue<TEnum::Vector3Array>*>(ptrItem);
      XGLWrapper::UpdateUniformVector3Array(item->mId, item->mValue);
    } break;
    case EUniformVariableType::Vector2: 
    { const auto* item = static_cast<TUniformValue<TEnum::Vector2>*>(ptrItem);
      XGLWrapper::UpdateUniformVector2(item->mId, item->mValue);
    } break;
    case EUniformVariableType::Integer:
    { const auto* item = static_cast<TUniformValue<TEnum::Integer>*>(ptrItem);
      XGLWrapper::UpdateUniformInteger(item->mId, item->mValue);
    } break;
    case EUniformVariableType::Unsigned:
    { const auto* item = static_cast<TUniformValue<TEnum::Unsigned>*>(ptrItem);
      XGLWrapper::UpdateUniformUnsigned(item->mId, item->mValue);
    } break;
    case EUniformVariableType::Float:
    { const auto* item = static_cast<TUniformValue<TEnum::Float>*>(ptrItem);
      XGLWrapper::UpdateUniformFloat(item->mId, item->mValue);
    } break;
    case EUniformVariableType::FloatArray:
    { const auto* item = static_cast<TUniformValue<TEnum::FloatArray>*>(ptrItem);
      XGLWrapper::UpdateUniformFloatArray(item->mId, item->mValue);
    } break;
    case EUniformVariableType::Bool:
    { const auto* item = static_cast<TUniformValue<TEnum::Bool>*>(ptrItem);
      XGLWrapper::UpdateUniformInteger(item->mId, item->mValue);
    } break;
    case EUniformVariableType::Texture1D: 
    { const auto* item = static_cast<TUniformValue<TEnum::Texture1D>*>(ptrItem);
      XGLWrapper::UpdateUniformInteger(item->mId, item->mValue);
    } break;
    case EUniformVariableType::Texture2D: 
    { const auto* item = static_cast<TUniformValue<TEnum::Texture2D>*>(ptrItem);
      XGLWrapper::UpdateUniformInteger(item->mId, item->mValue);
    } break;
    case EUniformVariableType::Texture2DArray:
    { const auto* item = static_cast<TUniformValue<TEnum::Texture2DArray>*>(ptrItem);
      XGLWrapper::UpdateUniformInteger(item->mId, item->mValue);
    } break;
    case EUniformVariableType::Texture2DRectangle: 
    { const auto* item = static_cast<TUniformValue<TEnum::Texture2DRectangle>*>(ptrItem);
      XGLWrapper::UpdateUniformInteger(item->mId, item->mValue);
    } break;
    case EUniformVariableType::Texture2DShadowArray:
    { const auto* item = static_cast<TUniformValue<TEnum::Texture2DShadowArray>*>(ptrItem);
      XGLWrapper::UpdateUniformInteger(item->mId, item->mValue);
    } break;
    case EUniformVariableType::Texture2DCubemap:
    { const auto* item = static_cast<TUniformValue<TEnum::Texture2DCubemap>*>(ptrItem);
      XGLWrapper::UpdateUniformInteger(item->mId, item->mValue);
    } break;
    default: MDY_UNEXPECTED_BRANCH();
    }
  }
}

void AUniformValueContainer::pTryUpdateUniformTextureVars()
{
  using TEnum = EUniformVariableType;
  static const constexpr std::array<std::pair<TEnum, GLenum>, 6> textureTypes = {
    std::pair(TEnum::Texture1D, GL_TEXTURE_1D), 
    std::pair(TEnum::Texture2D, GL_TEXTURE_2D), 
    std::pair(TEnum::Texture2DArray, GL_TEXTURE_2D_ARRAY), 
    std::pair(TEnum::Texture2DCubemap, GL_TEXTURE_CUBE_MAP), 
    std::pair(TEnum::Texture2DRectangle, GL_TEXTURE_RECTANGLE), 
    std::pair(TEnum::Texture2DShadowArray, GL_TEXTURE_2D_ARRAY)
  };

  // Bind texture.
  for (const auto& [insertId, type, textureId] : this->mUpdatedTextureList)
  {
    // Find appropriate texture type enum value.
    GLenum internalTextureType = GL_NONE;
    for (auto& [itemType, enumValue] : textureTypes)
    {
      if (itemType == type) { internalTextureType = enumValue; break; }
    }
    MDY_ASSERT_MSG_FORCE(internalTextureType != GL_NONE, "Failed to find texture type.");

    {
      // Critical section?
      glActiveTexture(GL_TEXTURE0 + insertId);
      glBindTexture(internalTextureType, textureId);
    }
  }
}

void AUniformValueContainer::pTryUpdateUniformStructVars()
{
 using TEnum = EUniformVariableType;
  for (auto& [prefix, index, ptrItem] : this->mUpdatedStructList)
  {
    switch (ptrItem->mType)
    {
    case TEnum::Matrix4: 
    { const auto* item = static_cast<TUniformValue<TEnum::Matrix4>*>(ptrItem);
      XGLWrapper::UpdateUniformMatrix4(item->mId, item->mValue);
    } break;
    case EUniformVariableType::Matrix3: 
    { const auto* item = static_cast<TUniformValue<TEnum::Matrix3>*>(ptrItem);
      XGLWrapper::UpdateUniformMatrix3(item->mId, item->mValue);
    } break;
    case EUniformVariableType::Vector4: 
    { const auto* item = static_cast<TUniformValue<TEnum::Vector4>*>(ptrItem);
      XGLWrapper::UpdateUniformVector4(item->mId, item->mValue);
    } break;
    case EUniformVariableType::Vector3: 
    { const auto* item = static_cast<TUniformValue<TEnum::Vector3>*>(ptrItem);
      XGLWrapper::UpdateUniformVector3(item->mId, item->mValue);
    } break;
    case EUniformVariableType::Vector2: 
    { const auto* item = static_cast<TUniformValue<TEnum::Vector2>*>(ptrItem);
      XGLWrapper::UpdateUniformVector2(item->mId, item->mValue);
    } break;
    case EUniformVariableType::Integer:
    { const auto* item = static_cast<TUniformValue<TEnum::Integer>*>(ptrItem);
      XGLWrapper::UpdateUniformInteger(item->mId, item->mValue);
    } break;
    case EUniformVariableType::Unsigned:
    { const auto* item = static_cast<TUniformValue<TEnum::Unsigned>*>(ptrItem);
      XGLWrapper::UpdateUniformUnsigned(item->mId, item->mValue);
    } break;
    case EUniformVariableType::Float:
    { const auto* item = static_cast<TUniformValue<TEnum::Float>*>(ptrItem);
      XGLWrapper::UpdateUniformFloat(item->mId, item->mValue);
    } break;
    case EUniformVariableType::Bool:
    { const auto* item = static_cast<TUniformValue<TEnum::Bool>*>(ptrItem);
      XGLWrapper::UpdateUniformInteger(item->mId, item->mValue);
    } break;
    default: MDY_UNEXPECTED_BRANCH();
    }
  }
}

TU32 AUniformValueContainer::GetAvailableTextureCount() const noexcept
{
  return this->mAvailableTextureCount;
}

} /// ::dy namespace