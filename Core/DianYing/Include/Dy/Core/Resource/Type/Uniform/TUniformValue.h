#ifndef GUARD_DY_CORE_RESOURCE_TYPE_UNIFORM_UniformValueTypes_H
#define GUARD_DY_CORE_RESOURCE_TYPE_UNIFORM_UniformValueTypes_H
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

#include <Dy/Core/Resource/Internal/Uniform/IUniformValueBase.h>
#include <Dy/Core/Resource/Type/Shader/XTemplateUniformType.h>

namespace dy
{

template <EUniformVariableType TType> 
class TUniformValue final : public IUniformValueBase
{
protected:
  using TValueType = typename __UniformBinder<TType>::ValueType;
public:
  TUniformValue(TI32 iId) : IUniformValueBase{iId, TType} {};
  TUniformValue(TI32 iId, const TValueType& iValue) : IUniformValueBase{iId, TType}, mValue{iValue} {};
  MDY_NOT_COPYABLE_MOVEABLE_PROPERTIES(TUniformValue);
  virtual ~TUniformValue() = default;

  /// @brief Value.
  TValueType mValue{};
};

} /// ::dy namespace

#endif /// GUARD_DY_CORE_RESOURCE_TYPE_UNIFORM_UniformValueTypes_H