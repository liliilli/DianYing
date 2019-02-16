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
#include <Dy/Helper/ContainerHelper.h>
#include <Dy/Helper/System/Idioms.h>
#include <Dy/Core/Resource/Internal/Uniform/IDyUniformValueBase.h>
#include <Dy/Core/Resource/Type/Uniform/UniformValueTypes.h>

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

class __ADyUniformContainer
{
public:
  /// @brief Try clear uniform list.
  void __TryClearUniformList();
  /// @brief Try construct default uniform variable list.
  /// If already have value, do nothing.
  void __TryConstructDefaultUniformList(_MIN_ const FDyShaderResource& iResc);

  template <EDyUniformVariableType TType>
  void TryUpdateUniform(
      _MIN_ const std::string& iSpecifier, 
      _MIN_ const typename MDY_PRIVATE(UniformBinder)<TType>::ValueType& iValue)
  {
    auto it = this->mUniformMap.find(iSpecifier);
    if (it == this->mUniformMap.end())
    { //
      auto [createdPair, _] = this->mUniformMap.try_emplace(iSpecifier, std::make_unique<FDyUniformValue<TType>>(-1, iValue)); \
    }
    else
    { //
      auto& [_, smtptrInstance] = *it;
      if (smtptrInstance->mType != TType)
      {
        MDY_LOG_ERROR("Could not insert uniform value becasue of different type. {}", iSpecifier);
        return;
      }
      // 
      auto* ptrInstance = static_cast<FDyUniformValue<TType>*>(smtptrInstance.get());
      if (ptrInstance->mValue == iValue) { return; }

      ptrInstance->mValue = iValue;
      this->mUpdatedItemList.emplace_back(ptrInstance);
    }
  }

  /// @brief Try update uniform variables. \n
  /// Do nothing when update list is empty or binding flag is not set up
  /// by calling MDY_PRIVATE(TryConstructDefaultUniformList)().
  EDySuccess TryUpdateUniformList();

private:
  template <EDyUniformVariableType TType> using TValueType = typename MDY_PRIVATE(UniformBinder)<TType>::ValueType;

  using TUniformMap       = std::unordered_map<std::string, std::unique_ptr<IDyUniformValueBase>>;
  using TUpdatedItemList  = std::vector<IDyUniformValueBase*>;

  TUniformMap       mUniformMap {};
  TUpdatedItemList  mUpdatedItemList  = {};
  bool              mIsShaderSetuped    = false;
};

} /// ::dy namespace

#endif /// !GUARD_DY_CORE_TYPE_SHADER_ADYUNIFORMCONTAINER_H
