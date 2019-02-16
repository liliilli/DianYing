#ifndef GUARD_DY_CORE_RESOURCE_TYPE_MATERIAL_TDYMATERIALBINDER_H
#define GUARD_DY_CORE_RESOURCE_TYPE_MATERIAL_TDYMATERIALBINDER_H
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

#include <Dy/Core/Resource/Type/TDyResourceBinderBase.h>
#include <Dy/Core/Resource/Type/Material/ADyUniformHelper.h>
#include <Dy/Meta/Type/EDyResourceType.h>

namespace dy
{

/// @struct __TDyResourceBinderMaterial
/// @brief Binder base class for each supporting resource type.
struct __TDyResourceBinderMaterial : 
    public __TDyResourceBinderBase<EDyResourceType::Material>,
    public ADyUniformHelper
{
public:
  MDY_NOT_COPYABLE_MOVEABLE_PROPERTIES(__TDyResourceBinderMaterial);

  /// @brief Release binder instance and detach it from specified Reference Instance.
  __TDyResourceBinderMaterial() = default;
  virtual ~__TDyResourceBinderMaterial() = default;

  template <EDyUniformVariableType TType>
  void TryUpdateUniform(
      _MIN_ const std::string& iSpecifier, 
      _MIN_ const typename MDY_PRIVATE(UniformBinder)<TType>::ValueType& iValue)
  {
    if (this->IsResourceExist() == true) { return; }
    this->UpdateUniform(*Get(), iSpecifier, iValue);
  }

  /// @brief Try update uniform variables. \n
  /// Do nothing when update list is empty or binding flag is not set up
  /// by calling MDY_PRIVATE(TryConstructDefaultUniformList)().
  MDY_NODISCARD EDySuccess TryUpdateUniformList();
};

} /// ::dy namespace

#endif /// GUARD_DY_CORE_RESOURCE_TYPE_MATERIAL_TDYMATERIALBINDER_H