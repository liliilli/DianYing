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

bool DDyIOReferenceContainer::IsReferenceInstanceExist(_MIN_ const std::string& specifier, _MIN_ EDyResourceType type)
{
  switch (type)
  {
  case EDyResourceType::GLShader: return DyIsMapContains(this->mMapGLShaderReference, specifier);
  case EDyResourceType::Texture:  return DyIsMapContains(this->mMapTextureReference, specifier);
  case EDyResourceType::Script:
  case EDyResourceType::Model:
  case EDyResourceType::Material:
    MDY_NOT_IMPLEMENTED_ASSERT();
    break;
  default: MDY_UNEXPECTED_BRANCH(); break;
  }

  MDY_UNEXPECTED_BRANCH_BUT_RETURN(false);
}

bool DDyIOReferenceContainer::TryEnlargeResourceScope(_MIN_ EDyScope scope, _MIN_ const std::string& specifier, _MIN_ EDyResourceType type)
{
  switch (type)
  {
  case EDyResourceType::GLShader:
  {
    auto& instance = this->mMapGLShaderReference[specifier];
    if (scope > instance.mScope) { instance.mScope = scope; }
  } break;
  case EDyResourceType::Texture:
  {
    auto& instance = this->mMapTextureReference[specifier];
    if (scope > instance.mScope) { instance.mScope = scope; }
  } break;
  case EDyResourceType::Script:
  case EDyResourceType::Model:
  case EDyResourceType::Material:
    MDY_NOT_IMPLEMENTED_ASSERT();
    break;
  default: MDY_UNEXPECTED_BRANCH(); break;
  }

  MDY_UNEXPECTED_BRANCH_BUT_RETURN(false);
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
  case EDyResourceType::Script:
  case EDyResourceType::Model:
  case EDyResourceType::Material:
    MDY_NOT_IMPLEMENTED_ASSERT();
    break;
  default: MDY_UNEXPECTED_BRANCH(); break;
  }

  return DY_SUCCESS;
}

} /// ::dy namespace