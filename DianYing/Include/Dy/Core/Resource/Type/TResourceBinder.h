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
#include <Dy/Core/Resource/Type/TBaseResourceBinder.h>
#include <Dy/Core/Resource/Type/Material/TResourceBinderMaterial.h>
#include <Dy/Core/Resource/Interface/IRenderableBinder.h>

namespace dy
{

/// @brief Resource binder class.
/// @brief Not lazy version of `TResourceBinder`.
/// User have to require resource manually. but detach is held by automatically. (RAII)
template <EResourceType TType>
class TResourceBinder final : public __TBaseResourceBinder<TType>
{
private:
  using TSuper = __TBaseResourceBinder<TType>;
public:
  MDY_NOT_COPYABLE_MOVEABLE_PROPERTIES(TResourceBinder);
  TResourceBinder(_MIN_ const std::string& iNewSpecifier) { TryRequireResource(iNewSpecifier); }
  TResourceBinder() = default;
  virtual ~TResourceBinder() = default;

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
/// @brief Not lazy version of `TResourceBinder`.
/// User have to require resource manually. but detach is held by automatically. (RAII)
template <>
class TResourceBinder<EResourceType::Material> final : 
    public __TResourceBinderMaterial
  , public IRenderableBinder
{
public:
  MDY_NOT_COPYABLE_MOVEABLE_PROPERTIES(TResourceBinder);
  TResourceBinder(_MIN_ const std::string& iNewSpecifier) 
    : IRenderableBinder{EResourceType::Material} { TryRequireResource(iNewSpecifier); }
  TResourceBinder() : IRenderableBinder{EResourceType::Material} {};
  virtual ~TResourceBinder() = default;

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
/// @brief Not lazy version of `TResourceBinder`.
/// User have to require resource manually. but detach is held by automatically. (RAII)
template <>
class TResourceBinder<EResourceType::Texture> final : 
    public __TBaseResourceBinder<EResourceType::Texture>
  , public IRenderableBinder
{
public:
  MDY_NOT_COPYABLE_MOVEABLE_PROPERTIES(TResourceBinder);
  TResourceBinder(_MIN_ const std::string& iNewSpecifier)
    : IRenderableBinder{EResourceType::Texture} { TryRequireResource(iNewSpecifier); }
  TResourceBinder() : IRenderableBinder{EResourceType::Texture} {};
  virtual ~TResourceBinder() = default;

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

using TDyResourceBinderShader      = TResourceBinder<EResourceType::GLShader>;
using TDyResourceBinderMesh        = TResourceBinder<EResourceType::Mesh>;
using TDyResourceBinderModel       = TResourceBinder<EResourceType::Model>;
using TDyResourceBinderTexture     = TResourceBinder<EResourceType::Texture>;
using TDyResourceBinderMaterial    = TResourceBinder<EResourceType::Material>;
using TDyResourceBinderAttachment  = TResourceBinder<EResourceType::GLAttachment>;
using TDyResourceBinderFrameBuffer = TResourceBinder<EResourceType::GLFrameBuffer>;

} /// ::dy namespace

#endif /// GUARD_DY_CORE_RESOURCE_TYPE_FDYRESOURCEBINDER_H