#ifndef GUARD_DY_BUILTIN_FRAMEBUFFER_FDYBTFRAMEBUFFERDEFERRED_H
#define GUARD_DY_BUILTIN_FRAMEBUFFER_FDYBTFRAMEBUFFERDEFERRED_H
///
/// MIT License
/// Copyright (c) 2018 Jongmin Yun
///
/// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
/// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
/// FITNESS FOR A PARTICULAR PURPOSE AND NON INFRINGEMENT. IN NO EVENT SHALL THE
/// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
/// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
/// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
/// SOFTWARE.
///

#include <Dy/Builtin/Abstract/ADyGLFrameBufferResource.h>
#include <Dy/Core/Reflection/RDyBuiltinResources.h>

///
/// @macro MDY_REGISTER_RESOURCE_FRAMEBUFFER
/// @brief Register OpenGL framebuffer meta information source.
///
#define MDY_REGISTER_RESOURCE_FRAMEBUFFER(__MAType__, __MASpecifierName__) \
  MDY_REGISTER_RESOURCE_WITH_SPECIFIER(__MAType__, __MASpecifierName__) \
  private: \
  void ConstructBuffer(_MOUT_ PDyGlFrameBufferInstanceMetaInfo& property) noexcept; \
  public: \
  __MAType__() { this->ConstructBuffer(this->mMetaInfo); }

namespace dy
{

///
/// @class FDyBtFrameBufferDeferred
/// @brief Basic render (OPAQUE, DEFERRED) framebuffer object descriptor.
///
class FDyBtFrameBufferDeferred final : public ADyGLFrameBufferResource
{
  MDY_REGISTER_RESOURCE_FRAMEBUFFER(FDyBtFrameBufferDeferred, "dyBtBasicRender")
};

} /// ::dy namespace

#endif /// GUARD_DY_BUILTIN_FRAMEBUFFER_FDYBTFRAMEBUFFERDEFERRED_H