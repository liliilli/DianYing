#pragma once
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
/// @class FDyBtFbTransIntegration
/// @brief Integration phase of opaque scene and transcluent textures.
///
class FDyBtFbTransIntegration final : public AGLFrameBufferResource
{
  MDY_REGISTER_RESOURCE_FRAMEBUFFER(FDyBtFbTransIntegration, "dyBtFbIntgLevelTrans")
};

} /// ::dy namespace
