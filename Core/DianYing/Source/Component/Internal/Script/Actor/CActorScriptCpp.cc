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

#include <Dy/Component/Internal/Actor/CActorScriptCpp.h>
#include <Dy/Management/IO/MIOMeta.h>
#include <Dy/Meta/Information/ScriptMetaInformation.h>

namespace dy
{

CActorScriptCpp::CActorScriptCpp(FActor& iRefActor, const PDyScriptInstanceMetaInfo& iDesc) 
  : CBaseActorScript{iRefActor}
{
  MDY_ASSERT_MSG(iDesc.mScriptType == EDyScriptType::Cpp,   "Script type is not matched to CActorScriptCpp.");
  MDY_ASSERT_MSG(iDesc.mScriptMode == EDyScriptMode::Actor, "Given script must be actor type.");

  MDY_ASSERT_MSG(MDY_CHECK_ISNOTNULL(iDesc.mBtInstantiationFunction), "Cpp script instantiation function must be not null.");
  this->mScriptInstance = DyConvertUniquePtrTo<AActorCppScript>(iDesc.mBtInstantiationFunction());
  MDY_ASSERT_MSG(MDY_CHECK_ISNOTNULL(this->mScriptInstance),    "Script instance could not bound to system.");

  this->mScriptName = iDesc.mSpecifierName;
  this->mScriptInstance->pfSetOutsideReference(*this);
  this->mIsScriptInstanceBinded = true;
}

AActorCppScript* CActorScriptCpp::MDY_PRIVATE(GetScriptInstance)() const noexcept
{
  MDY_ASSERT_MSG(MDY_CHECK_ISNOTEMPTY(this->mScriptInstance), "Script instance must be valid, not empty.");
  return this->mScriptInstance.get();
}

void CActorScriptCpp::Initiate()
{
  MDY_ASSERT_MSG(this->mIsScriptInstanceBinded == true, "Unexpected error occurred.");
  this->mScriptInstance->Initiate();
}

void CActorScriptCpp::Start()
{
  MDY_ASSERT_MSG(this->mIsScriptInstanceBinded == true, "Unexpected error occurred.");
  this->mScriptInstance->Start();
}

void CActorScriptCpp::Update(float dt)
{
  MDY_ASSERT_MSG(this->mIsScriptInstanceBinded == true, "Unexpected error occurred.");
  this->mScriptInstance->Update(dt);
}

void CActorScriptCpp::OnEnabled()
{
  MDY_ASSERT_MSG(this->mIsScriptInstanceBinded == true, "Unexpected error occurred.");
  this->mScriptInstance->OnEnabled();
}

void CActorScriptCpp::OnDisabled()
{
  MDY_ASSERT_MSG(this->mIsScriptInstanceBinded == true, "Unexpected error occurred.");
  this->mScriptInstance->OnDisabled();
}

void CActorScriptCpp::Destroy()
{
  MDY_ASSERT_MSG(this->mIsScriptInstanceBinded == true, "Unexpected error occurred.");
  this->mScriptInstance->Destroy();
}

} /// ::dy namespace