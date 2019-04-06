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

#include <Dy/Builtin/RenderPipeline/Widget/FBtWidget.h>
#include <Dy/Builtin/RenderItem/Widget/FBtWidgetGeneral.h>

namespace dy
{

void FBtRenderPipelineWidget::__ConstructionHelper
  ::ConstructBuffer(PDyRenderPipelineInstanceMetaInfo& oMeta)
{
  oMeta.mSpecifierName = sName;
  oMeta.mUuid          = DUuid("7eebeb61-4e08-43f7-b68c-41b40ae3106b");

  oMeta.mLocalRenderItemNames.emplace_back(FBtRenderWidgetGeneral::sName);
}

} /// ::dy namespace