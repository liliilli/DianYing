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

#include <Dy/Core/Resource/Type/TBaseResourceBinder.h>
#include <Dy/Core/Resource/Type/Material/AUniformHelper.h>
#include <Dy/Meta/Type/EResourceType.h>

namespace dy
{

/// @struct __TResourceBinderMaterial
/// @brief Base binder class for each supporting mateiral resource type.
struct __TResourceBinderMaterial : 
    public __TBaseResourceBinder<EResourceType::Material>,
    public AUniformHelper
{
public:
  MDY_NOT_COPYABLE_MOVEABLE_PROPERTIES(__TResourceBinderMaterial);

  /// @brief Release binder instance and detach it from specified Reference Instance.
  __TResourceBinderMaterial() = default;
  virtual ~__TResourceBinderMaterial() = default;

  template <EUniformVariableType TType>
  void TryUpdateUniform(
    const std::string& iSpecifier, 
    const typename MDY_PRIVATE(UniformBinder)<TType>::ValueType& iValue)
  {
    // Check
    if (this->IsResourceExist() == false) { return; }
    // Insert
    this->UpdateUniform(*Get(), iSpecifier, iValue);
  }

  /// @brief Try update uniform variables. \n
  /// Do nothing when update list is empty or binding flag is not set up
  /// by calling MDY_PRIVATE(TryConstructDefaultUniformList)().
  MDY_NODISCARD EDySuccess TryUpdateUniformList();
};

} /// ::dy namespace

#endif /// GUARD_DY_CORE_RESOURCE_TYPE_MATERIAL_TDYMATERIALBINDER_H