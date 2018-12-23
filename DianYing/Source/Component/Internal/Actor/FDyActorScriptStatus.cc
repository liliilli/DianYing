#include <precompiled.h>
///
/// MIT License
/// Copyright (c) 2018 Jongmin Yun
///
/// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
/// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
/// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
/// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
/// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
/// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
/// SOFTWARE.
///

/// Header file
#include <Dy/Component/Internal/Actor/FDyActorScriptStatus.h>
#include <Dy/Component/Internal/Actor/CDyActorScriptBase.h>
#include <Dy/Component/Internal/Actor/CDyActorScriptCpp.h>
#include <Dy/Meta/Information/ScriptMetaInformation.h>

namespace dy
{

void FDyActorScriptState::CallScriptFunction(_MIN_ const float dt) noexcept
{
  MDY_ASSERT(MDY_CHECK_ISNOTEMPTY(this->mScriptInstance),"Script instace must be activated!");
  MDY_ASSERT(this->mStatus != EDyScriptState::NoneError, "FDyActorScriptState must be initialized!");

  switch (this->mStatus)
  {
  case EDyScriptState::CalledNothing:
    this->mScriptInstance->Initiate();
    this->mStatus = EDyScriptState::CalledInitiate;
    break;
  case EDyScriptState::CalledInitiate:
    this->mScriptInstance->Start();
    this->mScriptInstance->Update(dt);
    this->mStatus = EDyScriptState::CalledUpdate;
    break;
  case EDyScriptState::CalledUpdate:
    this->mScriptInstance->Update(dt);
    break;
  default: MDY_UNEXPECTED_BRANCH(); break;
  }
}

} /// ::dy namespace