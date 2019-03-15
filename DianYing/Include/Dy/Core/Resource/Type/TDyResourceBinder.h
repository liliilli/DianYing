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

#include <Dy/Meta/Type/EResourceType.h>
#include <Dy/Core/Resource/Type/TDyResourceBinderBase.h>
#include <Dy/Core/Resource/Type/Material/TDyMaterialBinder.h>
#include <Dy/Core/Resource/Interface/IDyRenderableBinder.h>

namespace dy
{

/// @brief Resource binder class.
/// @brief Not lazy version of `TDyResourceBinder`.
/// User have to require resource manually. but detach is held by automatically. (RAII)
template <EResourceType TType>
class TDyResourceBinder final : public __TDyResourceBinderBase<TType>
{
private:
  using TSuper = __TDyResourceBinderBase<TType>;
public:
  MDY_NOT_COPYABLE_MOVEABLE_PROPERTIES(TDyResourceBinder);
  TDyResourceBinder(_MIN_ const std::string& iNewSpecifier) { TryRequireResource(iNewSpecifier); }
  TDyResourceBinder() = default;
  virtual ~TDyResourceBinder() = default;

  /// @brief Try require resource with specifier name in given EResourceType.
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
class TDyResourceBinder<EResourceType::Material> final : 
    public __TDyResourceBinderMaterial
  , public IDyRenderableBinder
{
public:
  MDY_NOT_COPYABLE_MOVEABLE_PROPERTIES(TDyResourceBinder);
  TDyResourceBinder(_MIN_ const std::string& iNewSpecifier) : IDyRenderableBinder{EResourceType::Material} { TryRequireResource(iNewSpecifier); }
  TDyResourceBinder() : IDyRenderableBinder{EResourceType::Material} {};
  virtual ~TDyResourceBinder() = default;

  /// @brief Try require resource with specifier name in given EResourceType.
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
class TDyResourceBinder<EResourceType::Texture> final : 
    public __TDyResourceBinderBase<EResourceType::Texture>
  , public IDyRenderableBinder
{
public:
  MDY_NOT_COPYABLE_MOVEABLE_PROPERTIES(TDyResourceBinder);
  TDyResourceBinder(_MIN_ const std::string& iNewSpecifier) : IDyRenderableBinder{EResourceType::Texture} { TryRequireResource(iNewSpecifier); }
  TDyResourceBinder() : IDyRenderableBinder{EResourceType::Texture} {};
  virtual ~TDyResourceBinder() = default;

  /// @brief Try require resource with specifier name in given EResourceType.
  /// If resource is already bound to binder handle, detach it first and newly bind another resource into it.
  void TryRequireResource(_MIN_ const std::string& iNewSpecifier)
  {
    if (const auto flag = pTryRequireResource(iNewSpecifier); flag == DY_SUCCESS) 
    { 
      this->mSpecifierName = iNewSpecifier; this->Process(); 
    }
  }
};

using TDyResourceBinderShader      = TDyResourceBinder<EResourceType::GLShader>;
using TDyResourceBinderMesh        = TDyResourceBinder<EResourceType::Mesh>;
using TDyResourceBinderModel       = TDyResourceBinder<EResourceType::Model>;
using TDyResourceBinderTexture     = TDyResourceBinder<EResourceType::Texture>;
using TDyResourceBinderMaterial    = TDyResourceBinder<EResourceType::Material>;
using TDyResourceBinderAttachment  = TDyResourceBinder<EResourceType::GLAttachment>;
using TDyResourceBinderFrameBuffer = TDyResourceBinder<EResourceType::GLFrameBuffer>;

} /// ::dy namespace

#endif /// GUARD_DY_CORE_RESOURCE_TYPE_FDYRESOURCEBINDER_H