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

#include <Dy/Builtin/RenderItem/FBtOpaqueDefault.h>

namespace dy
{

void FBtRenderItemOpaqueDefault::__ConstructionHelper
  ::ConstructBuffer(PDyRenderItemInstanceMetaInfo& oMeta)
{
  oMeta.mUuid = DUuid("aaa116de-6210-46fb-a551-a50554a08d95", true);
}

EDySuccess FBtRenderItemOpaqueDefault::OnPreRenderCheckCondition()
{
  return DY_FAILURE;
}

void FBtRenderItemOpaqueDefault::OnFailedCheckCondition()
{

}

void FBtRenderItemOpaqueDefault::OnSetupRenderingSetting()
{

}

void FBtRenderItemOpaqueDefault::OnRender()
{

}

void FBtRenderItemOpaqueDefault::OnReleaseRenderingSetting()
{

}

void FBtRenderItemOpaqueDefault::OnPostRender()
{

}

} /// ::dy namespace
