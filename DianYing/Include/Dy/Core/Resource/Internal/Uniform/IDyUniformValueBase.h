#ifndef GUARD_DY_CORE_RESOURCE_INTENRAL_UNIFORM_IDyUniformValueBase_H
#define GUARD_DY_CORE_RESOURCE_INTENRAL_UNIFORM_IDyUniformValueBase_H
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

namespace dy
{

/// @struct IDyUniformValueBase
/// @brief Uniform value base type.
struct IDyUniformValueBase
{
  IDyUniformValueBase(TI32 iId, EDyUniformVariableType iType) 
    : mId{iId}, mType{iType} 
  { };
  virtual ~IDyUniformValueBase() = 0;

  TI32                    mId   = -1;
  EDyUniformVariableType  mType = EDyUniformVariableType::NoneError;
  std::string             mSpecifier;
};

  inline IDyUniformValueBase::~IDyUniformValueBase() = default;

} /// ::dy namespace

#endif /// GUARD_DY_CORE_RESOURCE_INTENRAL_UNIFORM_IDyUniformValueBase_H