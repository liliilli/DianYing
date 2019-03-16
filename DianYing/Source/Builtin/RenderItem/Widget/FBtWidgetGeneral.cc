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

#include <Dy/Builtin/RenderItem/Widget/FBtWidgetGeneral.h>
#include <Dy/Core/Resource/Resource/FDyFrameBufferResource.h>
#include <Dy/Management/MFont.h>
#include <Dy/Management/Rendering/MRendering.h>
#include <Dy/Element/Widget/AWidgetObject.h>

namespace dy
{

void FBtRenderWidgetGeneral::__ConstructionHelper
  ::ConstructBuffer(PDyRenderItemInstanceMetaInfo& oMeta)
{
  oMeta.mUuid = DUuid("60f1256a-a515-4bbe-9dc7-fe55f16d541e", true);
}

EDySuccess FBtRenderWidgetGeneral::OnPreRenderCheckCondition()
{
  return this->AreResourcesValid() ? DY_SUCCESS : DY_FAILURE;
}

bool FBtRenderWidgetGeneral::AreResourcesValid()
{
  return this->mBinderFrameBuffer.IsResourceExist();
}

void FBtRenderWidgetGeneral::OnSetupRenderingSetting()
{
  this->mBinderFrameBuffer->BindFrameBuffer();
  glClearColor(0, 0, 0, 0);
  glClear(GL_COLOR_BUFFER_BIT);
}

void FBtRenderWidgetGeneral::OnRender()
{
  auto& list = MRendering::GetInstance().GetUiObjectQueuelist();
  for (auto& ptrRenderUi : list)
  {
    ptrRenderUi->Render();
  }
}

void FBtRenderWidgetGeneral::OnReleaseRenderingSetting()
{
  // Unbind
  this->mBinderFrameBuffer->UnbindFrameBuffer();
}

} /// ::dy namespace
