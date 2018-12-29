#include <precompiled.h>
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

/// Header file
#include <Dy/Core/Resource/Type/Shader/ADyUniformContainer.h>
#include <Dy/Core/Resource/Resource/FDyShaderResource.h>

#define MDY_DOCONSTRUCTUNIFORMLIST(__MATypeSpecifier__, __MASpecifier__, __MAId__) \
    if (MDY_ISUNIFORMEXIST(__MATypeSpecifier__)(__MASpecifier__) == true) { MDY_UPDATEUNIFORMID(__MATypeSpecifier__)(__MASpecifier__, __MAId__); } \
    else \
    { \
      auto& item = MDY_CREATEUNIFORM(__MATypeSpecifier__)(__MASpecifier__, __MAId__); \
      this->mUpdatedItemList.emplace_back(std::make_pair(type, item.get())); \
    }

namespace dy
{

void MDY_PRIVATE_SPECIFIER(ADyUniformContainer)::MDY_PRIVATE_SPECIFIER(TryClearUniformList)()
{
  this->mUpdatedItemList.clear();
  MDY_UNIFORMCONTAINER(Matrix4).clear();
  MDY_UNIFORMCONTAINER(Matrix3).clear();
  MDY_UNIFORMCONTAINER(Vector4).clear();
  MDY_UNIFORMCONTAINER(Vector3).clear();
  MDY_UNIFORMCONTAINER(Vector2).clear();
  MDY_UNIFORMCONTAINER(Integer).clear();
  MDY_UNIFORMCONTAINER(Bool).clear();
  MDY_UNIFORMCONTAINER(Texture2D).clear();
  MDY_UNIFORMCONTAINER(Texture2DArray).clear();
  this->mIsShaderSetup = false;
}

void MDY_PRIVATE_SPECIFIER(ADyUniformContainer)::
MDY_PRIVATE_SPECIFIER(TryConstructDefaultUniformList)(const FDyShaderResource& iResc)
{
  const auto& uniformList = iResc.GetUniformVariableList();
  for (const auto& [specifier, _, type, id] : uniformList)
  {
    // Check there is already specifier value, if exist do nothing
    // but not exist, insert with default value and bring it to update list.
    switch (type)
    {
    case EDyUniformVariableType::Matrix4:   { MDY_DOCONSTRUCTUNIFORMLIST(Matrix4, specifier, id); } break;
    case EDyUniformVariableType::Matrix3:   { MDY_DOCONSTRUCTUNIFORMLIST(Matrix3, specifier, id); } break;
    case EDyUniformVariableType::Vector4:   { MDY_DOCONSTRUCTUNIFORMLIST(Vector4, specifier, id); } break;
    case EDyUniformVariableType::Vector3:   { MDY_DOCONSTRUCTUNIFORMLIST(Vector3, specifier, id); } break;
    case EDyUniformVariableType::Vector2:   { MDY_DOCONSTRUCTUNIFORMLIST(Vector2, specifier, id); } break;
    case EDyUniformVariableType::Integer:   { MDY_DOCONSTRUCTUNIFORMLIST(Integer, specifier, id); } break;
    case EDyUniformVariableType::Bool:      { MDY_DOCONSTRUCTUNIFORMLIST(Bool, specifier, id); } break;
    case EDyUniformVariableType::Texture2D:      { MDY_DOCONSTRUCTUNIFORMLIST(Texture2D, specifier, id); } break;
    case EDyUniformVariableType::Texture2DArray: { MDY_DOCONSTRUCTUNIFORMLIST(Texture2DArray, specifier, id); } break;
    default: MDY_UNEXPECTED_BRANCH(); break;
    }
  }
  
  this->mIsShaderSetup = true;
}

EDySuccess MDY_PRIVATE_SPECIFIER(ADyUniformContainer)::TryUpdateUniformList()
{
  if (this->mIsShaderSetup == false) { return DY_FAILURE; }
  if (this->mUpdatedItemList.empty() == true) { return DY_FAILURE; }
  
  // We have to insert all variables into shader when updated, 
  // because shader is shared by any objects which want to render with it,
  // but each shader program has only one uniform variable status.

  for (auto& [type, ptrItem] : this->mUpdatedItemList)
  {
    switch (type)
    {
    case EDyUniformVariableType::Matrix4: 
    {
      const auto* item = static_cast<TPtrConvert<EDyUniformVariableType::Matrix4>>(ptrItem);
      if (item->mId == -1) { continue; }
      glUniformMatrix4fv(item->mId, 1, GL_FALSE, &item->mValue[0].X);
    } break;
    case EDyUniformVariableType::Matrix3: 
    {
      const auto* item = static_cast<TPtrConvert<EDyUniformVariableType::Matrix3>>(ptrItem);
      if (item->mId == -1) { continue; }

    } break;
    case EDyUniformVariableType::Vector4: 
    {
      const auto* item = static_cast<TPtrConvert<EDyUniformVariableType::Vector4>>(ptrItem);
      if (item->mId == -1) { continue; }
      glUniform4fv(item->mId, 1, &item->mValue.X);
    } break;
    case EDyUniformVariableType::Vector3: 
    {
      const auto* item = static_cast<TPtrConvert<EDyUniformVariableType::Vector3>>(ptrItem);
      if (item->mId == -1) { continue; }

    } break;
    case EDyUniformVariableType::Vector2: 
    {
      const auto* item = static_cast<TPtrConvert<EDyUniformVariableType::Vector2>>(ptrItem);
      if (item->mId == -1) { continue; }

    } break;
    case EDyUniformVariableType::Integer:
    {
      const auto* item = static_cast<TPtrConvert<EDyUniformVariableType::Integer>>(ptrItem);
      if (item->mId == -1) { continue; }
      glUniform1i(item->mId, item->mValue);
    } break;
    case EDyUniformVariableType::Bool:
    {
      const auto* item = static_cast<TPtrConvert<EDyUniformVariableType::Bool>>(ptrItem);
      if (item->mId == -1) { continue; }
      glUniform1i(item->mId, TI32(item->mValue));
    }
    case EDyUniformVariableType::Texture2D: 
    {
      const auto* item = static_cast<TPtrConvert<EDyUniformVariableType::Texture2D>>(ptrItem);
      if (item->mId == -1) { continue; }

    } break;
    case EDyUniformVariableType::Texture2DArray:
    {

    } break;
    default: MDY_UNEXPECTED_BRANCH_BUT_RETURN(DY_FAILURE);
    }
  }
  this->mUpdatedItemList.clear();
  return DY_SUCCESS;
}

} /// ::dy namespace