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
#include <Dy/Core/Resource/Type/Shader/ADyUniformContainer.h>

#include <regex>
#include <Dy/Core/Resource/Resource/FDyShaderResource.h>
#include <Dy/Core/Rendering/Wrapper/FDyGLWrapper.h>
#include <Dy/Core/Resource/Type/Uniform/UniformValueTypes.h>

#define MDY_DOCONSTRUCTUNIFORMLIST(__MATypeSpecifier__, __MASpecifier__, __MAId__) \
    auto [it2, MDY_NOUSEVAR] = this->mUniformMap.try_emplace(__MASpecifier__, std::make_unique<FDyUniformValue<TEnum::__MATypeSpecifier__>>(__MAId__)); \
    auto& [MDY_NOUSEVAR##_2, smtptrInstance] = *it2; \
    /* Insert default to update list. */ \
    this->mUpdatedItemList.emplace_back(smtptrInstance.get());

#define MDY_DOCONSTRUCTUNIFORMLISTTEXTURE(__MATypeSpecifier__, __MASpecifier__, __MAId__) \
    /* Check uniform name. */ \
    std::unique_ptr<FDyUniformValue<TEnum::__MATypeSpecifier__>> instance = nullptr; \
    if (std::regex_match(__MASpecifier__, textureRegex) == true) \
    { \
      const auto value = std::stoi(__MASpecifier__.substr(8)); \
      instance = std::make_unique<FDyUniformValue<TEnum::__MATypeSpecifier__>>(__MAId__, value); \
    } \
    else { instance = std::make_unique<FDyUniformValue<TEnum::__MATypeSpecifier__>>(__MAId__); } \
    /* Insert instance. */ \
    auto [it2, MDY_NOUSEVAR] = this->mUniformMap.try_emplace(__MASpecifier__, std::move(instance)); \
    auto& [MDY_NOUSEVAR##_2, smtptrInstance] = *it2; \
    /* Insert default to update list. */ \
    this->mUpdatedItemList.emplace_back(smtptrInstance.get()); \
    this->mAvailableTextureCount++;

namespace dy
{

void ADyUniformContainer::__TryClearUniformList()
{
  this->mUniformMap.clear();
  this->mUpdatedItemList.clear();
  this->mUpdatedTextureList.clear();
  this->mIsShaderSetuped = false;
  this->mAvailableTextureCount = 0;
}

#define MDY_NOUSEVAR MDY_TOKENPASTE2(_, __LINE__)

void ADyUniformContainer::__TryConstructDefaultUniformList(const FDyShaderResource& iResc)
{
  const auto& uniformList = iResc.GetUniformVariableList();
  static std::regex textureRegex{"uTexture\\d+(_\\w*)?"};

  for (const auto& [specifier, _, type, id] : uniformList)
  {
    // Check there is already specifier value, if exist do nothing
    // but not exist, insert with default value and bring it to update list.
    using TEnum = EDyUniformVariableType;
    switch (type)
    {
    case TEnum::Matrix4:       { MDY_DOCONSTRUCTUNIFORMLIST(Matrix4, specifier, id); } break;
    case TEnum::Matrix4Array:  { MDY_DOCONSTRUCTUNIFORMLIST(Matrix4Array, specifier, id); } break;
    case TEnum::Matrix3:       { MDY_DOCONSTRUCTUNIFORMLIST(Matrix3, specifier, id); } break;
    case TEnum::Vector4:       { MDY_DOCONSTRUCTUNIFORMLIST(Vector4, specifier, id); } break;
    case TEnum::Vector3:       { MDY_DOCONSTRUCTUNIFORMLIST(Vector3, specifier, id); } break;
    case TEnum::Vector3Array:  { MDY_DOCONSTRUCTUNIFORMLIST(Vector3Array, specifier, id); } break;
    case TEnum::Vector2:       { MDY_DOCONSTRUCTUNIFORMLIST(Vector2, specifier, id); } break;
    case TEnum::Integer:       { MDY_DOCONSTRUCTUNIFORMLIST(Integer, specifier, id); } break;
    case TEnum::Unsigned:      { MDY_DOCONSTRUCTUNIFORMLIST(Unsigned, specifier, id); } break;
    case TEnum::Bool:          { MDY_DOCONSTRUCTUNIFORMLIST(Bool, specifier, id); } break;
    case TEnum::Float:         { MDY_DOCONSTRUCTUNIFORMLIST(Float, specifier, id); } break;
    case TEnum::Texture1D:             { MDY_DOCONSTRUCTUNIFORMLISTTEXTURE(Texture1D, specifier, id); } break;
    case TEnum::Texture2D:             { MDY_DOCONSTRUCTUNIFORMLISTTEXTURE(Texture2D, specifier, id); } break;
    case TEnum::Texture2DArray:        { MDY_DOCONSTRUCTUNIFORMLISTTEXTURE(Texture2DArray, specifier, id); } break;
    case TEnum::Texture2DRectangle:    { MDY_DOCONSTRUCTUNIFORMLISTTEXTURE(Texture2DRectangle, specifier, id); } break;
    case TEnum::Texture2DShadowArray:  { MDY_DOCONSTRUCTUNIFORMLISTTEXTURE(Texture2DShadowArray, specifier, id); } break;
    case TEnum::Texture2DCubemap:      { MDY_DOCONSTRUCTUNIFORMLISTTEXTURE(Texture2DCubemap, specifier, id); } break;
    default: MDY_UNEXPECTED_BRANCH(); break;
    }
  }
  
  this->mUpdatedTextureList.reserve(this->mAvailableTextureCount);
  this->mIsShaderSetuped = true;
}

EDySuccess ADyUniformContainer::TryInsertTextureRequisition(_MIN_ TU32 insertId, _MIN_ TU32 textureId)
{
  // Check
  if (insertId >= this->GetAvailableTextureCount()) 
  { 
    MDY_LOG_ERROR("Failed to binding texture to given shader because not available insertId. {}", insertId);
    return DY_FAILURE; 
  }

  // Find
  using TEnum = EDyUniformVariableType;
  static std::array<TEnum, 6> textureTypes = {
      TEnum::Texture1D, TEnum::Texture2D, TEnum::Texture2DArray, 
      TEnum::Texture2DCubemap, TEnum::Texture2DRectangle, TEnum::Texture2DShadowArray};
  for (auto& [specifier, item] : this->mUniformMap)
  {
    if (Contains(textureTypes, item->mType) == false) { continue; }
    // Insert.
    this->mUpdatedTextureList.emplace_back(insertId, item->mType, textureId);
  }

  return DY_SUCCESS;
}

EDySuccess ADyUniformContainer::TryUpdateUniformList()
{
  using TEnum = EDyUniformVariableType;
  if (this->mIsShaderSetuped == false) { return DY_FAILURE; }
  
  // We have to insert all variables into shader when updated, 
  // because shader is shared by any objects which want to render with it,
  // but each shader program has only one uniform variable status.
  if (this->mUpdatedItemList.empty() == true) { goto ADYUNIFORMCONTAINER_TRYUPDATEUNIFORMLIST_TEXTURE; }

  for (auto& ptrItem : this->mUpdatedItemList)
  {
    switch (ptrItem->mType)
    {
    case TEnum::Matrix4: 
    { const auto* item = static_cast<FDyUniformValue<TEnum::Matrix4>*>(ptrItem);
      FDyGLWrapper::UpdateUniformMatrix4(item->mId, item->mValue);
    } break;
    case EDyUniformVariableType::Matrix4Array: 
    { const auto* item = static_cast<FDyUniformValue<TEnum::Matrix4Array>*>(ptrItem);
      FDyGLWrapper::UpdateUniformMatrix4Array(item->mId, item->mValue);
    } break;
    case EDyUniformVariableType::Matrix3: 
    { const auto* item = static_cast<FDyUniformValue<TEnum::Matrix3>*>(ptrItem);
      FDyGLWrapper::UpdateUniformMatrix3(item->mId, item->mValue);
    } break;
    case EDyUniformVariableType::Vector4: 
    { const auto* item = static_cast<FDyUniformValue<TEnum::Vector4>*>(ptrItem);
      FDyGLWrapper::UpdateUniformVector4(item->mId, item->mValue);
    } break;
    case EDyUniformVariableType::Vector3: 
    { const auto* item = static_cast<FDyUniformValue<TEnum::Vector3>*>(ptrItem);
      FDyGLWrapper::UpdateUniformVector3(item->mId, item->mValue);
    } break;
    case EDyUniformVariableType::Vector3Array: 
    { const auto* item = static_cast<FDyUniformValue<TEnum::Vector3Array>*>(ptrItem);
      FDyGLWrapper::UpdateUniformVector3Array(item->mId, item->mValue);
    } break;
    case EDyUniformVariableType::Vector2: 
    { const auto* item = static_cast<FDyUniformValue<TEnum::Vector2>*>(ptrItem);
      //FDyGLWrapper::UpdateUniformVector2(item->mId, item->mValue);
    } break;
    case EDyUniformVariableType::Integer:
    { const auto* item = static_cast<FDyUniformValue<TEnum::Integer>*>(ptrItem);
      FDyGLWrapper::UpdateUniformInteger(item->mId, item->mValue);
    } break;
    case EDyUniformVariableType::Unsigned:
    { const auto* item = static_cast<FDyUniformValue<TEnum::Unsigned>*>(ptrItem);
      FDyGLWrapper::UpdateUniformUnsigned(item->mId, item->mValue);
    } break;
    case EDyUniformVariableType::Float:
    { const auto* item = static_cast<FDyUniformValue<TEnum::Float>*>(ptrItem);
      FDyGLWrapper::UpdateUniformFloat(item->mId, item->mValue);
    } break;
    case EDyUniformVariableType::Bool:
    { const auto* item = static_cast<FDyUniformValue<TEnum::Bool>*>(ptrItem);
      FDyGLWrapper::UpdateUniformInteger(item->mId, item->mValue);
    } break;
    case EDyUniformVariableType::Texture1D: 
    { const auto* item = static_cast<FDyUniformValue<TEnum::Texture1D>*>(ptrItem);
      FDyGLWrapper::UpdateUniformInteger(item->mId, item->mValue);
    } break;
    case EDyUniformVariableType::Texture2D: 
    { const auto* item = static_cast<FDyUniformValue<TEnum::Texture2D>*>(ptrItem);
      FDyGLWrapper::UpdateUniformInteger(item->mId, item->mValue);
    } break;
    case EDyUniformVariableType::Texture2DArray:
    { const auto* item = static_cast<FDyUniformValue<TEnum::Texture2DArray>*>(ptrItem);
      FDyGLWrapper::UpdateUniformInteger(item->mId, item->mValue);
    } break;
    case EDyUniformVariableType::Texture2DRectangle: 
    { const auto* item = static_cast<FDyUniformValue<TEnum::Texture2DRectangle>*>(ptrItem);
      FDyGLWrapper::UpdateUniformInteger(item->mId, item->mValue);
    } break;
    case EDyUniformVariableType::Texture2DShadowArray:
    { const auto* item = static_cast<FDyUniformValue<TEnum::Texture2DShadowArray>*>(ptrItem);
      FDyGLWrapper::UpdateUniformInteger(item->mId, item->mValue);
    } break;
    case EDyUniformVariableType::Texture2DCubemap:
    { const auto* item = static_cast<FDyUniformValue<TEnum::Texture2DCubemap>*>(ptrItem);
      FDyGLWrapper::UpdateUniformInteger(item->mId, item->mValue);
    } break;
    default: MDY_UNEXPECTED_BRANCH_BUT_RETURN(DY_FAILURE);
    }
  }
  this->mUpdatedItemList.clear();

ADYUNIFORMCONTAINER_TRYUPDATEUNIFORMLIST_TEXTURE:
  // Update texture also
  if (this->mUpdatedTextureList.empty() == true) { goto ADYUNIFORMCONTAINER_TRYUPDATEUNIFORMLIST_RETURN; };

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
    MDY_ASSERT_FORCE(internalTextureType != GL_NONE, "Failed to find texture type.");

    {
      // Critical section?
      glActiveTexture(GL_TEXTURE0 + insertId);
      glBindTexture(internalTextureType, textureId);
    }
  }

  // Clear list.
  this->mUpdatedTextureList.clear();
  this->mUpdatedTextureList.reserve(this->mAvailableTextureCount);

ADYUNIFORMCONTAINER_TRYUPDATEUNIFORMLIST_RETURN:
  return DY_SUCCESS;
}

TU32 ADyUniformContainer::GetAvailableTextureCount() const noexcept
{
  return this->mAvailableTextureCount;
}

} /// ::dy namespace