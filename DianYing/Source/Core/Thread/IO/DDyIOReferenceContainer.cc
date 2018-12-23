#include <precompiled.h>
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

/// Header file
#include <Dy/Core/Thread/IO/DDyIOReferenceContainer.h>
#include <Dy/Helper/ContainerHelper.h>

namespace dy
{

bool DDyIOReferenceContainer::IsReferenceInstanceExist(_MIN_ const std::string& specifier, _MIN_ EDyResourceType type) const noexcept
{
  switch (type)
  {
  case EDyResourceType::GLShader: return DyIsMapContains(this->mMapGLShaderReference, specifier);
  case EDyResourceType::Texture:  return DyIsMapContains(this->mMapTextureReference, specifier);
  case EDyResourceType::Mesh:     return DyIsMapContains(this->mMapMeshReference, specifier);
  case EDyResourceType::Model:    return DyIsMapContains(this->mMapModelReference, specifier);
  case EDyResourceType::Material: return DyIsMapContains(this->mMapMaterialReference, specifier);
  case EDyResourceType::GLAttachment:   return DyIsMapContains(this->mMapAttachmentReference, specifier);
  case EDyResourceType::GLFrameBuffer:  return DyIsMapContains(this->mMapFrameBufferReference, specifier);
  default: MDY_UNEXPECTED_BRANCH_BUT_RETURN(false);
  }
}

bool DDyIOReferenceContainer::IsReferenceInstanceBound(_MIN_ const std::string& specifier, _MIN_ EDyResourceType type) const noexcept
{
  switch (type)
  {
  case EDyResourceType::GLShader: return this->mMapGLShaderReference.at(specifier).mIsResourceValid;
  case EDyResourceType::Texture:  return this->mMapTextureReference.at(specifier).mIsResourceValid;
  case EDyResourceType::Mesh:     return this->mMapMeshReference.at(specifier).mIsResourceValid;
  case EDyResourceType::Model:    return this->mMapModelReference.at(specifier).mIsResourceValid;
  case EDyResourceType::Material: return this->mMapMaterialReference.at(specifier).mIsResourceValid;
  case EDyResourceType::GLAttachment:   return this->mMapAttachmentReference.at(specifier).mIsResourceValid;
  case EDyResourceType::GLFrameBuffer:  return this->mMapFrameBufferReference.at(specifier).mIsResourceValid;
  default: MDY_UNEXPECTED_BRANCH_BUT_RETURN(false);
  }
}

bool DDyIOReferenceContainer::TryEnlargeResourceScope(_MIN_ EDyScope scope, _MIN_ const std::string& specifier, _MIN_ EDyResourceType type)
{
  DDyIOReferenceInstance* instance;
  switch (type)
  {
  case EDyResourceType::GLShader: { instance = &this->mMapGLShaderReference[specifier]; } break;
  case EDyResourceType::Texture:  { instance = &this->mMapTextureReference[specifier]; }  break;
  case EDyResourceType::Mesh:     { instance = &this->mMapMeshReference[specifier]; } break;
  case EDyResourceType::Model:    { instance = &this->mMapModelReference[specifier]; }    break;
  case EDyResourceType::Material: { instance = &this->mMapMaterialReference[specifier]; } break;
  case EDyResourceType::GLAttachment:   { instance = &this->mMapAttachmentReference[specifier]; } break;;
  case EDyResourceType::GLFrameBuffer:  { instance = &this->mMapFrameBufferReference[specifier]; } break;
  default: MDY_UNEXPECTED_BRANCH_BUT_RETURN(false);
  }

  if (scope > instance->mScope) { instance->mScope = scope; }
  return true;
}

EDySuccess DDyIOReferenceContainer::TryBindBinderToResourceRI(
    _MIN_ const std::string& iSpecifier,
    _MIN_ EDyResourceType iType,
    _MIN_ const __FDyBinderBase* iPtrBinder)
{
  if (this->IsReferenceInstanceExist(iSpecifier, iType) == false) { return DY_FAILURE; }

  switch (iType)
  {
  case EDyResourceType::Model:    this->mMapModelReference[iSpecifier].AttachBinder(iPtrBinder);    break;
  case EDyResourceType::GLShader: this->mMapGLShaderReference[iSpecifier].AttachBinder(iPtrBinder); break;
  case EDyResourceType::Texture:  this->mMapTextureReference[iSpecifier].AttachBinder(iPtrBinder);  break;
  case EDyResourceType::Mesh:     this->mMapMeshReference[iSpecifier].AttachBinder(iPtrBinder); break;
  case EDyResourceType::Material: this->mMapMaterialReference[iSpecifier].AttachBinder(iPtrBinder); break;
  case EDyResourceType::GLAttachment:   this->mMapAttachmentReference[iSpecifier].AttachBinder(iPtrBinder);   break;
  case EDyResourceType::GLFrameBuffer:  this->mMapFrameBufferReference[iSpecifier].AttachBinder(iPtrBinder);  break;
  default: MDY_UNEXPECTED_BRANCH_BUT_RETURN(DY_FAILURE);
  }
  return DY_SUCCESS;
}

EDySuccess DDyIOReferenceContainer::TryDetachBinderFromResourceRI(
    _MIN_ const std::string& iSpecifier, 
    _MIN_ EDyResourceType iType, 
    _MIN_ const __FDyBinderBase* iPtrBinder)
{
  if (this->IsReferenceInstanceExist(iSpecifier, iType) == false) { return DY_FAILURE; }

  switch (iType)
  {
  case EDyResourceType::Model:    this->mMapModelReference[iSpecifier].DetachBinder(iPtrBinder);    break;
  case EDyResourceType::GLShader: this->mMapGLShaderReference[iSpecifier].DetachBinder(iPtrBinder); break;
  case EDyResourceType::Texture:  this->mMapTextureReference[iSpecifier].DetachBinder(iPtrBinder);  break;
  case EDyResourceType::Mesh:     this->mMapMeshReference[iSpecifier].DetachBinder(iPtrBinder); break;
  case EDyResourceType::Material: this->mMapMaterialReference[iSpecifier].DetachBinder(iPtrBinder); break;
  case EDyResourceType::GLAttachment:   this->mMapAttachmentReference[iSpecifier].DetachBinder(iPtrBinder);   break;
  case EDyResourceType::GLFrameBuffer:  this->mMapFrameBufferReference[iSpecifier].DetachBinder(iPtrBinder);  break;
  default: MDY_UNEXPECTED_BRANCH_BUT_RETURN(DY_FAILURE);
  }
  return DY_SUCCESS;
}

EDySuccess DDyIOReferenceContainer::CreateReferenceInstance(
    _MIN_ const std::string& specifier,
    _MIN_ EDyResourceType type, _MIN_ EDyResourceStyle style, _MIN_ EDyScope scope)
{
  switch (type)
  {
  case EDyResourceType::GLShader:
  {
    auto [it, isSuccessful] = this->mMapGLShaderReference.try_emplace(specifier, specifier, style, type, scope);
    MDY_ASSERT(isSuccessful == true, "RI Container creation must be successful.");
  } break;
  case EDyResourceType::Texture:
  {
    auto [it, isSuccessful] = this->mMapTextureReference.try_emplace(specifier, specifier, style, type, scope);
    MDY_ASSERT(isSuccessful == true, "RI Container creation must be successful.");
  } break;
  case EDyResourceType::Mesh:
  {
    auto [it, isSuccessful] = this->mMapMeshReference.try_emplace(specifier, specifier, style, type, scope);
    MDY_ASSERT(isSuccessful == true, "RI Container creation must be successful.");
  } break;
  case EDyResourceType::Model:
  {
    auto [it, isSuccessful] = this->mMapModelReference.try_emplace(specifier, specifier, style, type, scope);
    MDY_ASSERT(isSuccessful == true, "RI Container creation must be successful.");
  } break;
  case EDyResourceType::Material:
  {
    auto [it, isSuccessful] = this->mMapMaterialReference.try_emplace(specifier, specifier, style, type, scope);
    MDY_ASSERT(isSuccessful == true, "RI Container creation must be successful.");
  } break;
  case EDyResourceType::GLAttachment:
  {
    auto [it, isSuccessful] = this->mMapAttachmentReference.try_emplace(specifier, specifier, style, type, scope);
    MDY_ASSERT(isSuccessful == true, "RI Container creation must be successful.");
  } break;
  case EDyResourceType::GLFrameBuffer:
  {
    auto [it, isSuccessful] = this->mMapFrameBufferReference.try_emplace(specifier, specifier, style, type, scope);
    MDY_ASSERT(isSuccessful == true, "RI Container creation must be successful.");
  } break;
  default: MDY_UNEXPECTED_BRANCH_BUT_RETURN(DY_FAILURE);
  }

  return DY_SUCCESS;
}

EDySuccess DDyIOReferenceContainer::TryUpdateValidity(EDyResourceType type, const std::string& specifier, bool isValid)
{
  TStringHashMap<DDyIOReferenceInstance>* map;
  switch (type)
  {
  case EDyResourceType::Mesh:     map = &this->mMapMeshReference;     break;
  case EDyResourceType::Model:    map = &this->mMapModelReference;    break;
  case EDyResourceType::GLShader: map = &this->mMapGLShaderReference; break;
  case EDyResourceType::Texture:  map = &this->mMapTextureReference;  break;
  case EDyResourceType::Material: map = &this->mMapMaterialReference; break;
  case EDyResourceType::GLAttachment:   map = &this->mMapAttachmentReference;   break;
  case EDyResourceType::GLFrameBuffer:  map = &this->mMapFrameBufferReference;  break;
  default: MDY_UNEXPECTED_BRANCH_BUT_RETURN(DY_FAILURE);
  }

  auto& instance = (*map)[specifier];
  if (isValid != instance.mIsResourceValid) { instance.mIsResourceValid = isValid; return DY_SUCCESS; }
  else                                      { return DY_FAILURE; }
}

} /// ::dy namespace