#ifndef GUARD_DY_CORE_RESOURCE_TYPE_SHADER_DDYUNIFORMVALUE_H
#define GUARD_DY_CORE_RESOURCE_TYPE_SHADER_DDYUNIFORMVALUE_H
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

#include <Dy/Core/Resource/Internal/ShaderType.h>
#include <Dy/Core/Resource/Type/Shader/TemplateUniformType.h>

namespace dy
{

template <EDyUniformVariableType TType>
struct DDyUniformValue final
{
  using ValueType = typename MDY_PRIVATE_SPECIFIER(UniformBinder)<TType>::ValueType;

  std::string mSpecifierName = {};
  TI32        mId;
  ValueType   mValue;

  DDyUniformValue(
      _MIN_ const std::string& iSpecifierName, 
      _MIN_ const TI32 iId) :
      mSpecifierName{iSpecifierName},
      mId{iId} {};
};

} /// ::dy namespace

#endif /// GUARD_DY_CORE_RESOURCE_TYPE_SHADER_DDYUNIFORMVALUE_H