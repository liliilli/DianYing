#ifndef GUARD_DY_BUILTIN_FRAMEBUFFER_FDyBtFbSkybox_H
#define GUARD_DY_BUILTIN_FRAMEBUFFER_FDyBtFbSkybox_H
///
/// MIT License
/// Copyright (c) 2018-2019 Jongmin Yun
///
/// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
/// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
/// FITNESS FOR A PARTICULAR PURPOSE AND NON INFRINGEMENT. IN NO EVENT SHALL THE
/// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
/// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
/// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
/// SOFTWARE.
///

#include <Dy/Builtin/Abstract/AGLFrameBufferResource.h>
#include <Dy/Core/Reflection/RBuiltinResources.h>

namespace dy::builtin
{

/// @class FDyBtFbSkybox
/// @brief Screen-Space ambient occlusion framebuffer for deferred level scene.
class FDyBtFbSkybox final : public AGLFrameBufferResource
{
  MDY_REGISTER_RESOURCE_FRAMEBUFFER(FDyBtFbSkybox, "dyBtFbSkybox")
};

} /// ::dy::builtin namespace

#endif /// GUARD_DY_BUILTIN_FRAMEBUFFER_FDyBtFbSSAO_H