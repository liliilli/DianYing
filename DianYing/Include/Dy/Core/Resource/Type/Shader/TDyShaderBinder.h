#ifndef GUARD_DY_CORE_RESOURCE_TYPE_TDYSHADERBINDER_H
#define GUARD_DY_CORE_RESOURCE_TYPE_TDYSHADERBINDER_H
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
#include <Dy/Core/Resource/Type/Shader/ADyUniformContainer.h>

namespace dy
{

/// @struct __TDyResourceBinderShader
/// @brief Binder base class for each supporting resource type.
struct __TDyResourceBinderShader : 
    public __TDyResourceBinderBase<EDyResourceType::GLShader>, 
    public MDY_PRIVATE(ADyUniformContainer)
{
public:
  MDY_NOT_COPYABLE_MOVEABLE_PROPERTIES(__TDyResourceBinderShader);
  __TDyResourceBinderShader() = default;
  virtual ~__TDyResourceBinderShader() = default;

  /// @brief Process funtion after binding.
  void Process() noexcept override
  {
    __TryClearUniformList();
    MDY_PRIVATE(TryConstructDefaultUniformList)(*this->mPtrResource);
  }
};

} /// ::dy namespace

#endif /// GUARD_DY_CORE_RESOURCE_TYPE_TDYSHADERBINDER_H