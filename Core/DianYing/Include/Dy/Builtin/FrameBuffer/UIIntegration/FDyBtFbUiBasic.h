#ifndef GUARD_DY_BUITLIN_FRAMEBUFFER_UIINTEGRATION_H
#define GUARD_DY_BUITLIN_FRAMEBUFFER_UIINTEGRATION_H
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

#include <Dy/Builtin/Abstract/AGLFrameBufferResource.h>
#include <Dy/Core/Reflection/RBuiltinResources.h>

namespace dy
{

///
/// @class FDyBtFbUiBasic
/// @brief Temporal ui integration framebuffer object descriptor.
///
class FDyBtFbUiBasic final : public AGLFrameBufferResource
{
  MDY_REGISTER_RESOURCE_FRAMEBUFFER(FDyBtFbUiBasic, "dyBtFbUiBasic")
};

} /// ::dy namespace

#endif /// GUARD_DY_BUITLIN_FRAMEBUFFER_UIINTEGRATION_H