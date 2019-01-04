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
#include <Dy/Component/Internal/ScriptState.h>

#include <Dy/Component/CDyScript.h>

namespace dy
{

EDySuccess FDyScriptState::Initialize(_MIN_ const PDyScriptStateDescriptor& descriptor)
{
  this->mScriptPtr = descriptor.mScriptPtr;
  this->mState     = EDyScriptState::CalledNothing;
  return DY_SUCCESS;
}

void FDyScriptState::Release()
{
  this->mScriptPtr = MDY_INITIALIZE_NULL;
}

void FDyScriptState::CallScriptFunction(_MIN_ const float dt) noexcept
{
  MDY_ASSERT(this->mScriptPtr->IsComponentActivated() == true, "Script instace must be activated!");
  MDY_ASSERT(this->mState != EDyScriptState::NoneError, "FDyScriptState must be initialized!");

  switch (this->mState)
  {
  case EDyScriptState::CalledNothing:
    this->mScriptPtr->Initiate();
    this->mState = EDyScriptState::CalledInitiate;
    break;
  case EDyScriptState::CalledInitiate:
    this->mScriptPtr->Start();
    this->mScriptPtr->Update(dt);
    this->mState = EDyScriptState::CalledUpdate;
    break;
  case EDyScriptState::CalledUpdate:
    this->mScriptPtr->Update(dt);
    break;
  default: MDY_UNEXPECTED_BRANCH(); break;
  }
}

} /// ::dy namespace