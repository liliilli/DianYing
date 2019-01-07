#ifndef GUARD_DY_BUILTIN_ABSTRACT_ADYGLSHADERRESOURCE_H
#define GUARD_DY_BUILTIN_ABSTRACT_ADYGLSHADERRESOURCE_H
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

#include <string_view>
#include <Dy/Builtin/Interface/IDyResource.h>
#include <Dy/Core/Reflection/RDyBuiltinResources.h>

namespace dy
{

struct ADyGLShaderResource : public IDyResource
{
  virtual ~ADyGLShaderResource() = default;
  static constexpr auto value = EDyResourceType::GLShader;

  std::string_view mSpecifierName   = "";
  std::string_view mVertexBuffer    = "";
  std::string_view mHullBuffer      = "";
  std::string_view mDomainBuffer    = "";
  std::string_view mGeometryBuffer  = "";
  std::string_view mPixelBuffer     = "";
  std::string_view mComputeBuffer   = "";

  /// @brief Return meta info, that has buffer pointer of shader fragment.
  std::any GetMetaInfo() override final;
};

} /// ::dy namespace

#endif /// GUARD_DY_BUILTIN_ABSTRACT_ADYGLSHADERRESOURCE_H