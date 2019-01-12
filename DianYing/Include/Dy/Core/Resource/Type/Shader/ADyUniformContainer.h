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

#include <Dy/Core/Resource/Internal/ShaderType.h>
#include <Dy/Core/Resource/Type/Shader/TemplateUniformType.h>
#include <Dy/Core/Resource/Type/Shader/DDyUniformValue.h>
#include <Dy/Helper/ContainerHelper.h>
#include <Dy/Helper/System/Idioms.h>

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

#define MDY_ISUNIFORMEXIST(__MATypeSpecifier__)   pIsUniformExist##__MATypeSpecifier__
#define MDY_CREATEUNIFORM(__MATypeSpecifier__)    pCreateUniform##__MATypeSpecifier__
#define MDY_UPDATEUNIFORM(__MATypeSpecifier__)    pUpdateUniform##__MATypeSpecifier__
#define MDY_UNIFORMCONTAINER(__MATypeSpecifier__) m##__MATypeSpecifier__
#define MDY_UPDATEUNIFORMID(__MATypeSpecifier__)  MDY_PRIVATE_SPECIFIER(MDY_TOKENPASTE2(pUpdateUniformId, __MATypeSpecifier__))
#define MDY_UNIFORM_FUNCTION_CONSTRUCT(__MATypeSpecifier__) \
    TUniformMap<EDyUniformVariableType::__MATypeSpecifier__> MDY_UNIFORMCONTAINER(__MATypeSpecifier__); \
    \
    MDY_NODISCARD bool MDY_ISUNIFORMEXIST(__MATypeSpecifier__)(_MIN_ const std::string& iSpecifier) \
    { return DyIsMapContains(this->MDY_UNIFORMCONTAINER(__MATypeSpecifier__), iSpecifier); } \
    \
    const TUniformMap<EDyUniformVariableType::__MATypeSpecifier__>::value_type::second_type& \
    MDY_CREATEUNIFORM(__MATypeSpecifier__)(_MIN_ const std::string& iSpecifier, _MIN_ const TI32 id) \
    { \
      auto [it, isSucceeded] = this->MDY_UNIFORMCONTAINER(__MATypeSpecifier__).try_emplace( \
          iSpecifier, \
          std::make_unique<DDyUniformValue<EDyUniformVariableType::__MATypeSpecifier__>>(iSpecifier, id) \
      ); \
      MDY_ASSERT(isSucceeded == true, "Failed to create uniform item."); \
      return it->second; \
    } \
    \
    const TUniformMap<EDyUniformVariableType::__MATypeSpecifier__>::value_type::second_type& \
    MDY_UPDATEUNIFORMID(__MATypeSpecifier__)(_MIN_ const std::string& iSpecifier, _MIN_ const TI32 id) \
    { \
      auto& item = this->MDY_UNIFORMCONTAINER(__MATypeSpecifier__).at(iSpecifier); \
      item->mId = id; \
      return item; \
    } \
    \
    MDY_NODISCARD TPtrConvert<EDyUniformVariableType::__MATypeSpecifier__> \
    MDY_UPDATEUNIFORM(__MATypeSpecifier__)( \
        _MIN_ const std::string& iSpecifier, \
        _MIN_ const TValueType<EDyUniformVariableType::__MATypeSpecifier__>& iValue) \
    { \
      auto& item = this->MDY_UNIFORMCONTAINER(__MATypeSpecifier__).at(iSpecifier); \
      item->mValue = iValue; \
      return item.get(); \
    }

namespace dy
{

class MDY_PRIVATE_SPECIFIER(ADyUniformContainer)
{
public:
  /// @brief Try clear uniform list.
  void MDY_PRIVATE_SPECIFIER(TryClearUniformList)();
  /// @brief Try construct default uniform variable list.
  /// If already have value, do nothing.
  void MDY_PRIVATE_SPECIFIER(TryConstructDefaultUniformList)(_MIN_ const FDyShaderResource& iResc);

  template <EDyUniformVariableType TType>
  void TryUpdateUniform(
      _MIN_ const std::string& iSpecifier, 
      _MIN_ const typename MDY_PRIVATE_SPECIFIER(UniformBinder)<TType>::ValueType& iValue)
  {
    if constexpr (TType == EDyUniformVariableType::Matrix4)
    { // If not exist and not created by doing TryConstructDefaultUniformList, just create.
      if (MDY_ISUNIFORMEXIST(Matrix4)(iSpecifier) == false) { MDY_CREATEUNIFORM(Matrix4)(iSpecifier, -1); }
      const auto& item = this->MDY_UPDATEUNIFORM(Matrix4)(iSpecifier, iValue);
      this->mUpdatedItemList.emplace_back(std::make_pair(TType, item));
    }
    else if constexpr (TType == EDyUniformVariableType::Matrix4Array)
    { // If not exist and not created by doing TryConstructDefaultUniformList, just create.
      if (MDY_ISUNIFORMEXIST(Matrix4Array)(iSpecifier) == false) { MDY_CREATEUNIFORM(Matrix4Array)(iSpecifier, -1); }
      const auto& item = this->MDY_UPDATEUNIFORM(Matrix4Array)(iSpecifier, iValue);
      this->mUpdatedItemList.emplace_back(std::make_pair(TType, item));
    }
    else if constexpr (TType == EDyUniformVariableType::Matrix3)
    { // If not exist and not created by doing TryConstructDefaultUniformList, just create.
      if (MDY_ISUNIFORMEXIST(Matrix3)(iSpecifier) == false) { MDY_CREATEUNIFORM(Matrix3)(iSpecifier, -1); }
      const auto& item = this->MDY_UPDATEUNIFORM(Matrix3)(iSpecifier, iValue);
      this->mUpdatedItemList.emplace_back(std::make_pair(TType, item));
    }
    else if constexpr (TType == EDyUniformVariableType::Vector4)
    { // If not exist and not created by doing TryConstructDefaultUniformList, just create.
      if (MDY_ISUNIFORMEXIST(Vector4)(iSpecifier) == false) { MDY_CREATEUNIFORM(Vector4)(iSpecifier, -1); }
      const auto& item = this->MDY_UPDATEUNIFORM(Vector4)(iSpecifier, iValue);
      this->mUpdatedItemList.emplace_back(std::make_pair(TType, item));
    }
    else if constexpr (TType == EDyUniformVariableType::Integer)
    { // If not exist and not created by doing TryConstructDefaultUniformList, just create.
      if (MDY_ISUNIFORMEXIST(Integer)(iSpecifier) == false) { MDY_CREATEUNIFORM(Integer)(iSpecifier, -1); }
      const auto& item = this->MDY_UPDATEUNIFORM(Integer)(iSpecifier, iValue);
      this->mUpdatedItemList.emplace_back(std::make_pair(TType, item));
    }
  }

  /// @brief Try update uniform variables. \n
  /// Do nothing when update list is empty or binding flag is not set up
  /// by calling MDY_PRIVATE_SPECIFIER(TryConstructDefaultUniformList)().
  EDySuccess TryUpdateUniformList();

private:
  template <EDyUniformVariableType TType>
  using TValueType = typename MDY_PRIVATE_SPECIFIER(UniformBinder)<TType>::ValueType;

  template <EDyUniformVariableType TType>
  using TUniformMap = std::unordered_map<std::string, std::unique_ptr<DDyUniformValue<TType>>>;

  template <EDyUniformVariableType TType>
  using TPtrConvert = DDyUniformValue<TType>*;

  using TUpdatedItemList = std::vector<std::pair<EDyUniformVariableType, void*>>;

  MDY_UNIFORM_FUNCTION_CONSTRUCT(Matrix4);
  MDY_UNIFORM_FUNCTION_CONSTRUCT(Matrix3);
  MDY_UNIFORM_FUNCTION_CONSTRUCT(Vector4);
  MDY_UNIFORM_FUNCTION_CONSTRUCT(Vector3);
  MDY_UNIFORM_FUNCTION_CONSTRUCT(Vector2);
  MDY_UNIFORM_FUNCTION_CONSTRUCT(Integer);
  MDY_UNIFORM_FUNCTION_CONSTRUCT(Bool);
  MDY_UNIFORM_FUNCTION_CONSTRUCT(Texture2D);
  MDY_UNIFORM_FUNCTION_CONSTRUCT(Texture2DArray);
  MDY_UNIFORM_FUNCTION_CONSTRUCT(Matrix4Array);
  
  TUpdatedItemList  mUpdatedItemList  = {};
  bool              mIsShaderSetup    = false;
};

} /// ::dy namespace

#endif /// !GUARD_DY_CORE_TYPE_SHADER_ADYUNIFORMCONTAINER_H
