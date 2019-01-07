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

#include <Dy/Core/Resource/Type/Shader/TemplateUniformType.h>

//!
//! Forward declaration
//!

namespace dy
{
class FDyMaterialResource;
class DDyMatrix4;
} /// ::dy namespace

//!
//! Implementation
//!

namespace dy
{

class ADyUniformHelper 
{
protected:
  void UpdateUniform(
      _MIN_ FDyMaterialResource& iRefMaterial, 
      _MIN_ const std::string& iSpecifier, 
      _MIN_ const MDY_PRIVATE_SPECIFIER(UniformBinder)<EDyUniformVariableType::Matrix4>::ValueType& iValue);

  void UpdateUniform(
      _MIN_ FDyMaterialResource& iRefMaterial, 
      _MIN_ const std::string& iSpecifier, 
      _MIN_ const MDY_PRIVATE_SPECIFIER(UniformBinder)<EDyUniformVariableType::Matrix3>::ValueType& iValue);

  void UpdateUniform(
      _MIN_ FDyMaterialResource& iRefMaterial, 
      _MIN_ const std::string& iSpecifier, 
      _MIN_ const MDY_PRIVATE_SPECIFIER(UniformBinder)<EDyUniformVariableType::Vector4>::ValueType& iValue);
  
  void UpdateUniform(
      _MIN_ FDyMaterialResource& iRefMaterial, 
      _MIN_ const std::string& iSpecifier, 
      _MIN_ const MDY_PRIVATE_SPECIFIER(UniformBinder)<EDyUniformVariableType::Integer>::ValueType& iValue);
  
  /// @brief Try update uniform list of valid mateiral instance.
  MDY_NODISCARD EDySuccess 
  MDY_PRIVATE_SPECIFIER(TryUpdateUniformList)(_MIN_ FDyMaterialResource& iRefMateiral);
};

} /// ::dy namespace

#endif // !GUARD_DY_CORE_RESOURCE_TYPE_MATERIAL_ADYUNIFORMHELPER_H
