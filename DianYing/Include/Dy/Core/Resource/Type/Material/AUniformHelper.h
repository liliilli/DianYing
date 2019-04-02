#ifndef GUARD_DY_CORE_RESOURCE_TYPE_MATERIAL_ADYUNIFORMHELPER_H
#define GUARD_DY_CORE_RESOURCE_TYPE_MATERIAL_ADYUNIFORMHELPER_H
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

#include <Dy/Core/Resource/Type/Shader/XTemplateUniformType.h>

//!
//! Forward declaration
//!

namespace dy
{
class FResourceMaterial;
class DDyMatrix4;
} /// ::dy namespace

//!
//! Implementation
//!

namespace dy
{

class AUniformHelper 
{
public:
  AUniformHelper() = default;
  virtual ~AUniformHelper() = 0;

  using TEnum = EUniformVariableType;
protected:
  void UpdateUniform(
    FResourceMaterial& iRefMaterial, 
    const std::string& iSpecifier, 
    const MDY_PRIVATE(UniformBinder)<TEnum::Matrix4>::ValueType& iValue);

  void UpdateUniform(
    FResourceMaterial& iRefMaterial, 
    const std::string& iSpecifier, 
    const MDY_PRIVATE(UniformBinder)<TEnum::Matrix4Array>::ValueType& iValue);

  void UpdateUniform(
    FResourceMaterial& iRefMaterial, 
    const std::string& iSpecifier, 
    const MDY_PRIVATE(UniformBinder)<TEnum::Matrix3>::ValueType& iValue);

  void UpdateUniform(
    FResourceMaterial& iRefMaterial, 
    const std::string& iSpecifier, 
    const MDY_PRIVATE(UniformBinder)<TEnum::Vector4>::ValueType& iValue);
 
  void UpdateUniform(
    FResourceMaterial& iRefMaterial, 
    const std::string& iSpecifier, 
    const MDY_PRIVATE(UniformBinder)<TEnum::Vector3>::ValueType& iValue);
    
  void UpdateUniform(
    FResourceMaterial& iRefMaterial, 
    const std::string& iSpecifier, 
    const MDY_PRIVATE(UniformBinder)<TEnum::Vector3Array>::ValueType& iValue);
 
  void UpdateUniform(
    FResourceMaterial& iRefMaterial, 
    const std::string& iSpecifier, 
    const MDY_PRIVATE(UniformBinder)<TEnum::Vector2>::ValueType& iValue);
 
  void UpdateUniform(
    FResourceMaterial& iRefMaterial, 
    const std::string& iSpecifier, 
    const MDY_PRIVATE(UniformBinder)<TEnum::Integer>::ValueType& iValue);
     
  void UpdateUniform(
    FResourceMaterial& iRefMaterial, 
    const std::string& iSpecifier, 
    const MDY_PRIVATE(UniformBinder)<TEnum::Float>::ValueType& iValue);
     
  void UpdateUniform(
    FResourceMaterial& iRefMaterial, 
    const std::string& iSpecifier, 
    const MDY_PRIVATE(UniformBinder)<TEnum::Bool>::ValueType& iValue);
 
  /// @brief Try update uniform list of valid mateiral instance.
  MDY_NODISCARD EDySuccess 
  __TryUpdateUniformList(FResourceMaterial& iRefMateiral);
};

inline AUniformHelper::~AUniformHelper() = default;

} /// ::dy namespace

#endif // !GUARD_DY_CORE_RESOURCE_TYPE_MATERIAL_ADYUNIFORMHELPER_H
