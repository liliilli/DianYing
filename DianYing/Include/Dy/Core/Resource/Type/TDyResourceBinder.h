#ifndef GUARD_DY_CORE_RESOURCE_TYPE_FDYRESOURCEBINDER_H
#define GUARD_DY_CORE_RESOURCE_TYPE_FDYRESOURCEBINDER_H
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

#include <Dy/Helper/Pointer.h>
#include <Dy/Meta/Type/EDyResourceType.h>
#include <Dy/Core/Resource/Type/EDyLazy.h>
#include <Dy/Core/Resource/Type/TDyResourceBinderBase.h>
#include <Dy/Core/Resource/Type/Shader/TDyShaderBinder.h>
#include <Dy/Core/Resource/Type/Material/TDyMaterialBinder.h>

namespace dy
{

/// @brief Resource binder class.
/// @brief Not lazy version of `TDyResourceBinder`.
/// User have to require resource manually. but detach is held by automatically. (RAII)
template <EDyResourceType TType>
class TDyResourceBinder final : public 
  std::conditional_t<TType == EDyResourceType::Material, 
      __TDyResourceBinderMaterial, 
      std::conditional_t<TType == EDyResourceType::GLShader,
          __TDyResourceBinderShader,
          __TDyResourceBinderBase<TType>
      >
  >
{
private:
  using TSuper = __TDyResourceBinderBase<TType>;
public:
  MDY_NOT_COPYABLE_MOVEABLE_PROPERTIES(TDyResourceBinder);
  TDyResourceBinder(_MIN_ const std::string& iNewSpecifier)
  {
    TryRequireResource(iNewSpecifier);
  }
  TDyResourceBinder() = default;
  ~TDyResourceBinder() = default;

  /// @brief Try require resource with specifier name in given EDyResourceType.
  /// If resource is already bound to binder handle, detach it first and newly bind another resource into it.
  void TryRequireResource(_MIN_ const std::string& iNewSpecifier)
  {
    // If resource is already exist and bound by something. 
    // Let it be until new resource is bounded soon.
    // If flag == DY_FAILURE, iNewSpecifier will be stored as deferred resource specifier.
    if (const auto flag = TSuper::pTryRequireResource(iNewSpecifier); flag == DY_SUCCESS) 
    { 
      this->mSpecifierName = iNewSpecifier;
      this->Process(); 
    }
  }
};

using TDyLResourceBinderShader      = TDyResourceBinder<EDyResourceType::GLShader>;
using TDyLResourceBinderMesh        = TDyResourceBinder<EDyResourceType::Mesh>;
using TDyLResourceBinderModel       = TDyResourceBinder<EDyResourceType::Model>;
using TDyLResourceBinderTexture     = TDyResourceBinder<EDyResourceType::Texture>;
using TDyLResourceBinderMaterial    = TDyResourceBinder<EDyResourceType::Material>;
using TDyLResourceBinderAttachment  = TDyResourceBinder<EDyResourceType::GLAttachment>;
using TDyLResourceBinderFrameBuffer = TDyResourceBinder<EDyResourceType::GLFrameBuffer>;

} /// ::dy namespace

#endif /// GUARD_DY_CORE_RESOURCE_TYPE_FDYRESOURCEBINDER_H