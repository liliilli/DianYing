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

#include <Dy/Builtin/Abstract/ARenderPipeline.h>
#include <Dy/Core/Reflection/RBuiltinResources.h>
#include <Dy/Meta/Information/MetaInfoRenderPipeline.h>

namespace dy
{

/// @class FBtRenderPipelineWidget
/// @brief Instantiate default widgetpipeline
class FBtRenderPipelineWidget final : public ARenderPipeline
{
  MDY_REGISTER_RESOURCE_RENDER_PIPELINE(FBtRenderPipelineWidget, "dyBtDefaultWidget")
public:
  EDySuccess OnPreRenderCheckCondition() override final { return EDySuccess::DY_SUCCESS; }
  void OnFailedCheckCondition() override final {}
  void OnSetupRenderingSetting() override final {}
  void OnReleaseRenderingSetting() override final {}
  void OnPostRender() override final {}
};

} /// ::dy namespace
