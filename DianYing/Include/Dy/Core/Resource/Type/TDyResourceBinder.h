#ifndef GUARD_DY_CORE_RESOURCE_TYPE_FDYRESOURCEBINDER_H
#define GUARD_DY_CORE_RESOURCE_TYPE_FDYRESOURCEBINDER_H
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

#include <Dy/Helper/Pointer.h>
#include <Dy/Meta/Type/EDyResourceType.h>
#include <Dy/Core/Resource/Type/EDyLazy.h>
#include <Dy/Core/Resource/Type/TDyResourceBinderBase.h>
#include <Dy/Core/Resource/Type/Shader/TDyShaderBinder.h>
#include <Dy/Core/Resource/Type/Material/TDyMaterialBinder.h>

namespace dy
{

///
/// @class TDyResourceBinder
/// @brief Resource binder class.
///
template <EDyResourceType TType, EDyLazy TIsLazy>
class TDyResourceBinder;

///
/// @class TDyResourceBinder
/// @brief Not lazy version of `TDyResourceBinder`.
///
template <EDyResourceType TType>
class TDyResourceBinder<TType, EDyLazy::No> final : public __TDyResourceBinderBase<TType>
{
private:
  using TSuper = __TDyResourceBinderBase<TType>;
public:
  MDY_NOT_COPYABLE_MOVEABLE_PROPERTIES(TDyResourceBinder);

  TDyResourceBinder(_MIN_ const std::string& specifier) : TSuper{specifier}
  {
    if (const auto flag = TSuper::pTryRequireResource();
        flag == DY_FAILURE)
    { MDY_NOT_IMPLEMENTED_ASSERT(); }
    else { this->Process(); }
  }
  ~TDyResourceBinder() = default;
};

///
/// @class TDyResourceBinder
/// @brief Not lazy version of `TDyResourceBinder`.
/// User have to require resource manually. but detach is held by automatically. (RAII)
///
template <EDyResourceType TType>
class TDyResourceBinder<TType, EDyLazy::Yes> final : public __TDyResourceBinderBase<TType>
{
private:
  using TSuper = __TDyResourceBinderBase<TType>;
public:
  MDY_NOT_COPYABLE_MOVEABLE_PROPERTIES(TDyResourceBinder);
  TDyResourceBinder() = default;
  ~TDyResourceBinder() = default;

  /// @brief Try require resource with specifier name in given EDyResourceType.
  /// If resource is already bound to binder handle, detach it first and newly bind another resource into it.
  void TryRequireResource(_MIN_ const std::string& specifier)
  {
    TSuper::pSetSpecifierName(specifier);
    MDY_NOTUSED auto _1 = TSuper::pTryDetachResource();
    if (const auto flag = TSuper::pTryRequireResource();
        flag == DY_FAILURE)
    {
      /// @TODO IMEPLEMENT TEMPORAL RESOURCE BINDING. AND WAITING.
      MDY_UNEXPECTED_BRANCH();
    }
    else { this->Process(); }
  }
};

template <EDyResourceType TType>
using TDyResourceBinderInstant    = TDyResourceBinder<TType, EDyLazy::No>;
using TDyIResourceBinderShader    = TDyResourceBinderInstant<EDyResourceType::GLShader>;
using TDyIResourceBinderMesh      = TDyResourceBinderInstant<EDyResourceType::Mesh>;
using TDyIResourceBinderModel     = TDyResourceBinderInstant<EDyResourceType::Model>;
using TDyIResourceBinderTexture   = TDyResourceBinderInstant<EDyResourceType::Texture>;
using TDyIResourceBinderMaterial  = TDyResourceBinderInstant<EDyResourceType::Material>;
using TDyIResourceBinderAttachment  = TDyResourceBinderInstant<EDyResourceType::GLAttachment>;
using TDyIResourceBinderFrameBuffer = TDyResourceBinderInstant<EDyResourceType::GLFrameBuffer>;

template <EDyResourceType TType>
using TDyResourceBinderLazy = TDyResourceBinder<TType, EDyLazy::Yes>;
using TDyLResourceBinderShader    = TDyResourceBinderLazy<EDyResourceType::GLShader>;
using TDyLResourceBinderMesh      = TDyResourceBinderLazy<EDyResourceType::Mesh>;
using TDyLResourceBinderModel     = TDyResourceBinderLazy<EDyResourceType::Model>;
using TDyLResourceBinderTexture   = TDyResourceBinderLazy<EDyResourceType::Texture>;
using TDyLResourceBinderMaterial  = TDyResourceBinderLazy<EDyResourceType::Material>;
using TDyLResourceBinderAttachment  = TDyResourceBinderLazy<EDyResourceType::GLAttachment>;
using TDyLResourceBinderFrameBuffer = TDyResourceBinderLazy<EDyResourceType::GLFrameBuffer>;

} /// ::dy namespace

#endif /// GUARD_DY_CORE_RESOURCE_TYPE_FDYRESOURCEBINDER_H