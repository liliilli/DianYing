#include <precompiled.h>
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

#include <Dy/Builtin/RenderPipeline/FBtDefault.h>

namespace dy
{

void FBtRenderPipelineDefault::__ConstructionHelper
  ::ConstructBuffer(PDyRenderPipelineInstanceMetaInfo& oMeta)
{
  oMeta.mSpecifierName    = sName;
  oMeta.mUuid             = DUuid("a6506841-bc7c-45b4-bb4e-5f614cff2a1c");
}

EDySuccess FBtRenderPipelineDefault::OnPreRenderCheckCondition()
{
  return DY_FAILURE;
}

void FBtRenderPipelineDefault::OnFailedCheckCondition()
{

}

void FBtRenderPipelineDefault::OnSetupRenderingSetting()
{

}

void FBtRenderPipelineDefault::OnReleaseRenderingSetting()
{

}

void FBtRenderPipelineDefault::OnPostRender()
{

}

} /// ::dy namespace