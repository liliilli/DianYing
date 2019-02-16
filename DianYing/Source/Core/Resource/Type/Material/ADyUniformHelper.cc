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
#include <Dy/Core/Resource/Type/Material/ADyUniformHelper.h>
#include <Dy/Core/Resource/Resource/FDyMaterialResource.h>

namespace dy
{

void ADyUniformHelper::UpdateUniform(
    _MIN_ FDyMaterialResource& iRefMaterial, 
    _MIN_ const std::string& iSpecifier,
    _MIN_ const MDY_PRIVATE(UniformBinder)<EDyUniformVariableType::Matrix4>::ValueType& iValue)
{
  auto& shaderBinder = iRefMaterial.GetShaderResourceBinder();
  shaderBinder.TryUpdateUniform<EDyUniformVariableType::Matrix4>(iSpecifier, iValue);
}

void ADyUniformHelper::UpdateUniform(
    _MIN_ FDyMaterialResource& iRefMaterial, 
    _MIN_ const std::string& iSpecifier, 
    _MIN_ const MDY_PRIVATE(UniformBinder)<EDyUniformVariableType::Matrix3>::ValueType& iValue)
{
  auto& shaderBinder = iRefMaterial.GetShaderResourceBinder();
  shaderBinder.TryUpdateUniform<EDyUniformVariableType::Matrix3>(iSpecifier, iValue);
}

void ADyUniformHelper::UpdateUniform(
    _MIN_ FDyMaterialResource& iRefMaterial, 
    _MIN_ const std::string& iSpecifier, 
    _MIN_ const MDY_PRIVATE(UniformBinder)<EDyUniformVariableType::Vector4>::ValueType& iValue)
{
  auto& shaderBinder = iRefMaterial.GetShaderResourceBinder();
  shaderBinder.TryUpdateUniform<EDyUniformVariableType::Vector4>(iSpecifier, iValue);
};

void ADyUniformHelper::UpdateUniform(
    _MIN_ FDyMaterialResource& iRefMaterial, 
    _MIN_ const std::string& iSpecifier, 
    _MIN_ const MDY_PRIVATE(UniformBinder)<EDyUniformVariableType::Integer>::ValueType& iValue)
{
  auto& shaderBinder = iRefMaterial.GetShaderResourceBinder();
  shaderBinder.TryUpdateUniform<EDyUniformVariableType::Integer>(iSpecifier, iValue);
}

EDySuccess ADyUniformHelper::MDY_PRIVATE(TryUpdateUniformList)(_MIN_ FDyMaterialResource& iRefMateiral)
{
  auto& shaderBinder = iRefMateiral.GetShaderResourceBinder();
  return shaderBinder.TryUpdateUniformList();
};

} /// ::dy namespace