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

#include <Dy/Meta/Type/EDyResourceType.h>
#include <Dy/Core/Resource/Type/TDyResourceBinderBase.h>
#include <Dy/Core/Resource/Type/Shader/TDyShaderBinder.h>
#include <Dy/Core/Resource/Type/Material/TDyMaterialBinder.h>
#include <Dy/Core/Resource/Interface/IDyRenderableBinder.h>

namespace dy
{

/// @brief Resource binder class.
/// @brief Not lazy version of `TDyResourceBinder`.
/// User have to require resource manually. but detach is held by automatically. (RAII)
template <EDyResourceType TType>
class TDyResourceBinder final : public 
    std::conditional_t<TType == EDyResourceType::GLShader, 
    __TDyResourceBinderShader,        // Then 
    __TDyResourceBinderBase<TType> >  // Else
{
private:
  using TSuper = __TDyResourceBinderBase<TType>;
public:
  MDY_NOT_COPYABLE_MOVEABLE_PROPERTIES(TDyResourceBinder);
  TDyResourceBinder(_MIN_ const std::string& iNewSpecifier) { TryRequireResource(iNewSpecifier); }
  TDyResourceBinder() = default;
  virtual ~TDyResourceBinder() = default;

  /// @brief Try require resource with specifier name in given EDyResourceType.
  /// If resource is already bound to binder handle, detach it first and newly bind another resource into it.
  void TryRequireResource(_MIN_ const std::string& iNewSpecifier)
  {
    // If resource is already exist and bound by something. 
    // Let it be until new resource is bounded soon.
    // If flag == DY_FAILURE, iNewSpecifier will be stored as deferred resource specifier.
    if (const auto flag = TSuper::pTryRequireResource(iNewSpecifier); flag == DY_SUCCESS) 
    { 
      this->mSpecifierName = iNewSpecifier; this->Process(); 
    }
  }
};

/// @brief Resource binder class.
/// @brief Not lazy version of `TDyResourceBinder`.
/// User have to require resource manually. but detach is held by automatically. (RAII)
template <>
class TDyResourceBinder<EDyResourceType::Material> final : 
    public __TDyResourceBinderMaterial
  , public IDyRenderableBinder
{
public:
  MDY_NOT_COPYABLE_MOVEABLE_PROPERTIES(TDyResourceBinder);
  TDyResourceBinder(_MIN_ const std::string& iNewSpecifier) : IDyRenderableBinder{EDyResourceType::Material} { TryRequireResource(iNewSpecifier); }
  TDyResourceBinder() : IDyRenderableBinder{EDyResourceType::Material} {};
  virtual ~TDyResourceBinder() = default;

  /// @brief Try require resource with specifier name in given EDyResourceType.
  /// If resource is already bound to binder handle, detach it first and newly bind another resource into it.
  void TryRequireResource(_MIN_ const std::string& iNewSpecifier)
  {
    if (const auto flag = pTryRequireResource(iNewSpecifier); flag == DY_SUCCESS) 
    { 
      this->mSpecifierName = iNewSpecifier; this->Process(); 
    }
  }
};

/// @brief Resource binder class.
/// @brief Not lazy version of `TDyResourceBinder`.
/// User have to require resource manually. but detach is held by automatically. (RAII)
template <>
class TDyResourceBinder<EDyResourceType::Texture> final : 
    public __TDyResourceBinderBase<EDyResourceType::Texture>
  , public IDyRenderableBinder
{
public:
  MDY_NOT_COPYABLE_MOVEABLE_PROPERTIES(TDyResourceBinder);
  TDyResourceBinder(_MIN_ const std::string& iNewSpecifier) : IDyRenderableBinder{EDyResourceType::Texture} { TryRequireResource(iNewSpecifier); }
  TDyResourceBinder() : IDyRenderableBinder{EDyResourceType::Texture} {};
  virtual ~TDyResourceBinder() = default;

  /// @brief Try require resource with specifier name in given EDyResourceType.
  /// If resource is already bound to binder handle, detach it first and newly bind another resource into it.
  void TryRequireResource(_MIN_ const std::string& iNewSpecifier)
  {
    if (const auto flag = pTryRequireResource(iNewSpecifier); flag == DY_SUCCESS) 
    { 
      this->mSpecifierName = iNewSpecifier; this->Process(); 
    }
  }
};

using TDyResourceBinderShader      = TDyResourceBinder<EDyResourceType::GLShader>;
using TDyResourceBinderMesh        = TDyResourceBinder<EDyResourceType::Mesh>;
using TDyResourceBinderModel       = TDyResourceBinder<EDyResourceType::Model>;
using TDyResourceBinderTexture     = TDyResourceBinder<EDyResourceType::Texture>;
using TDyResourceBinderMaterial    = TDyResourceBinder<EDyResourceType::Material>;
using TDyResourceBinderAttachment  = TDyResourceBinder<EDyResourceType::GLAttachment>;
using TDyResourceBinderFrameBuffer = TDyResourceBinder<EDyResourceType::GLFrameBuffer>;

} /// ::dy namespace

#endif /// GUARD_DY_CORE_RESOURCE_TYPE_FDYRESOURCEBINDER_H