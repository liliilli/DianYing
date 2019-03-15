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
#include <Dy/Core/Resource/Resource/FDyShaderResource.h>

namespace dy
{

void ADyUniformHelper::UpdateUniform(
    _MIN_ FDyMaterialResource& iRefMaterial, 
    _MIN_ const std::string& iSpecifier,
    _MIN_ const MDY_PRIVATE(UniformBinder)<EUniformVariableType::Matrix4>::ValueType& iValue)
{
  auto& shaderBinder = iRefMaterial.GetShaderResourceBinder();
  shaderBinder->TryUpdateUniform<EUniformVariableType::Matrix4>(iSpecifier, iValue);
}

void ADyUniformHelper::UpdateUniform(
    _MIN_ FDyMaterialResource& iRefMaterial, 
    _MIN_ const std::string& iSpecifier,
    _MIN_ const __UniformBinder<TEnum::Matrix4Array>::ValueType& iValue)
{
  auto& shaderBinder = iRefMaterial.GetShaderResourceBinder();
  shaderBinder->TryUpdateUniform<EUniformVariableType::Matrix4Array>(iSpecifier, iValue);
}

void ADyUniformHelper::UpdateUniform(
    _MIN_ FDyMaterialResource& iRefMaterial, 
    _MIN_ const std::string& iSpecifier, 
    _MIN_ const MDY_PRIVATE(UniformBinder)<EUniformVariableType::Matrix3>::ValueType& iValue)
{
  auto& shaderBinder = iRefMaterial.GetShaderResourceBinder();
  shaderBinder->TryUpdateUniform<EUniformVariableType::Matrix3>(iSpecifier, iValue);
}

void ADyUniformHelper::UpdateUniform(
    _MIN_ FDyMaterialResource& iRefMaterial, 
    _MIN_ const std::string& iSpecifier, 
    _MIN_ const MDY_PRIVATE(UniformBinder)<EUniformVariableType::Vector4>::ValueType& iValue)
{
  auto& shaderBinder = iRefMaterial.GetShaderResourceBinder();
  shaderBinder->TryUpdateUniform<EUniformVariableType::Vector4>(iSpecifier, iValue);
};

void ADyUniformHelper::UpdateUniform(
    _MIN_ FDyMaterialResource& iRefMaterial, 
    _MIN_ const std::string& iSpecifier,
    _MIN_ const __UniformBinder<TEnum::Vector3>::ValueType& iValue)
{
  auto& shaderBinder = iRefMaterial.GetShaderResourceBinder();
  shaderBinder->TryUpdateUniform<EUniformVariableType::Vector3>(iSpecifier, iValue);
}

void ADyUniformHelper::UpdateUniform(
    _MIN_ FDyMaterialResource& iRefMaterial, 
    _MIN_ const std::string& iSpecifier,
    _MIN_ const __UniformBinder<TEnum::Vector3Array>::ValueType& iValue)
{
  auto& shaderBinder = iRefMaterial.GetShaderResourceBinder();
  shaderBinder->TryUpdateUniform<EUniformVariableType::Vector3Array>(iSpecifier, iValue);
}

void ADyUniformHelper::UpdateUniform(
    _MIN_ FDyMaterialResource& iRefMaterial, 
    _MIN_ const std::string& iSpecifier,
    _MIN_ const __UniformBinder<TEnum::Vector2>::ValueType& iValue)
{
  auto& shaderBinder = iRefMaterial.GetShaderResourceBinder();
  shaderBinder->TryUpdateUniform<EUniformVariableType::Vector2>(iSpecifier, iValue);
}

void ADyUniformHelper::UpdateUniform(
    _MIN_ FDyMaterialResource& iRefMaterial, 
    _MIN_ const std::string& iSpecifier, 
    _MIN_ const MDY_PRIVATE(UniformBinder)<EUniformVariableType::Integer>::ValueType& iValue)
{
  auto& shaderBinder = iRefMaterial.GetShaderResourceBinder();
  shaderBinder->TryUpdateUniform<EUniformVariableType::Integer>(iSpecifier, iValue);
}

void ADyUniformHelper::UpdateUniform(
    _MIN_ FDyMaterialResource& iRefMaterial, 
    _MIN_ const std::string& iSpecifier,
    _MIN_ const __UniformBinder<TEnum::Float>::ValueType& iValue)
{
  auto& shaderBinder = iRefMaterial.GetShaderResourceBinder();
  shaderBinder->TryUpdateUniform<EUniformVariableType::Float>(iSpecifier, iValue);
}

void ADyUniformHelper::UpdateUniform(
    _MIN_ FDyMaterialResource& iRefMaterial, 
    _MIN_ const std::string& iSpecifier,
    _MIN_ const __UniformBinder<TEnum::Bool>::ValueType& iValue)
{
  auto& shaderBinder = iRefMaterial.GetShaderResourceBinder();
  shaderBinder->TryUpdateUniform<EUniformVariableType::Bool>(iSpecifier, iValue);
}

EDySuccess ADyUniformHelper::MDY_PRIVATE(TryUpdateUniformList)(_MIN_ FDyMaterialResource& iRefMateiral)
{
  auto& shaderBinder = iRefMateiral.GetShaderResourceBinder();
  return shaderBinder->TryUpdateUniformList();
};

} /// ::dy namespace