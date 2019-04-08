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

#include <Script/GlobalTestScript.h>
#include <Dy/Management/MLog.h>
#include <Dy/Management/MSetting.h>
#include <Dy/Core/Reflection/RGlobalInstanceManager.h>
#include <Dy/Builtin/GlobalInstance/FDyBtGiDebugStatus.h>

void BtScrGlobalTestScript::OnStart()
{
  DyPushLogCritical("GlobalTestScript Test. Hello world!");
  auto& refSetting = dy::MSetting::GetInstance();
  if (refSetting.IsDebugMode() == true)
  {
    //refSetting.SetRenderingMode(dy::ERenderingModelMode::WireFrame);
    refSetting.SetRenderingPhysicsCollisionShape(true);
    //refSetting.SetRenderingPhysicsCollisionAABB(true);
  }

  auto* ptrDebugStatus = MDY_GET_GLOBALINSTANCE(FDyBtGiDebugStatus);
  MDY_ASSERT_MSG(ptrDebugStatus != nullptr, "Test failed.");
}

void BtScrGlobalTestScript::OnEnd()
{
  DyPushLogCritical("GlobalTestScript Test. Bye world!");
}
