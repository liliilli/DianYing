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
    this->mUpdatedItemList.emplace_back(smtptrInstance.get());

namespace dy
{

void MDY_PRIVATE_SPECIFIER(ADyUniformContainer)::__TryClearUniformList()
{
  this->mUniformMap.clear();
  this->mUpdatedItemList.clear();
  this->mIsShaderSetuped = false;
}

#define MDY_NOUSEVAR MDY_TOKENPASTE2(_, __LINE__)

void MDY_PRIVATE_SPECIFIER(ADyUniformContainer)::__TryConstructDefaultUniformList(const FDyShaderResource& iResc)
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
    case TEnum::Bool:          { MDY_DOCONSTRUCTUNIFORMLIST(Bool, specifier, id); } break;
    case TEnum::Float:         { MDY_DOCONSTRUCTUNIFORMLIST(Float, specifier, id); } break;
    case TEnum::Texture2D:             { MDY_DOCONSTRUCTUNIFORMLISTTEXTURE(Texture2D, specifier, id); } break;
    case TEnum::Texture2DArray:        { MDY_DOCONSTRUCTUNIFORMLISTTEXTURE(Texture2DArray, specifier, id); } break;
    case TEnum::Texture2DRectangle:    { MDY_DOCONSTRUCTUNIFORMLISTTEXTURE(Texture2DRectangle, specifier, id); } break;
    case TEnum::Texture2DShadowArray:  { MDY_DOCONSTRUCTUNIFORMLISTTEXTURE(Texture2DShadowArray, specifier, id); } break;
    case TEnum::Texture2DCubemap:      { MDY_DOCONSTRUCTUNIFORMLISTTEXTURE(Texture2DCubemap, specifier, id); } break;
    default: MDY_UNEXPECTED_BRANCH(); break;
    }
  }
  
  this->mIsShaderSetuped = true;
}

EDySuccess MDY_PRIVATE_SPECIFIER(ADyUniformContainer)::TryUpdateUniformList()
{
  if (this->mIsShaderSetuped == false) { return DY_FAILURE; }
  if (this->mUpdatedItemList.empty() == true) { return DY_FAILURE; }
  
  // We have to insert all variables into shader when updated, 
  // because shader is shared by any objects which want to render with it,
  // but each shader program has only one uniform variable status.

  using TEnum = EDyUniformVariableType;
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
    case EDyUniformVariableType::Float:
    { const auto* item = static_cast<FDyUniformValue<TEnum::Float>*>(ptrItem);
      FDyGLWrapper::UpdateUniformFloat(item->mId, item->mValue);
    } break;
    case EDyUniformVariableType::Bool:
    { const auto* item = static_cast<FDyUniformValue<TEnum::Bool>*>(ptrItem);
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
  return DY_SUCCESS;
}

} /// ::dy namespace