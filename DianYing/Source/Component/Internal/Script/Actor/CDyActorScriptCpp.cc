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

#include <Dy/Component/Internal/Actor/CDyActorScriptCpp.h>
#include <Dy/Management/IO/MetaInfoManager.h>

namespace dy
{

CDyActorScriptCpp::CDyActorScriptCpp(_MIN_ FDyActor& iRefActor, _MIN_ const PDyScriptInstanceMetaInfo& iDesc) :
    CDyActorScriptBase{iRefActor}
{
  MDY_ASSERT(iDesc.mScriptType == EDyScriptType::Cpp,   "Script type is not matched to CDyActorScriptCpp.");
  MDY_ASSERT(iDesc.mScriptMode == EDyScriptMode::Actor, "Given script must be actor type.");

  MDY_ASSERT(MDY_CHECK_ISNOTNULL(iDesc.mBtInstantiationFunction), "Cpp script instantiation function must be not null.");
  this->mScriptInstance = DyConvertUniquePtrTo<ADyActorCppScript>(iDesc.mBtInstantiationFunction());
  MDY_ASSERT(MDY_CHECK_ISNOTNULL(this->mScriptInstance),    "Script instance could not bound to system.");

  this->mScriptName = iDesc.mSpecifierName;
  this->mScriptInstance->pfSetOutsideReference(*this);
  this->mIsScriptInstanceBinded = true;
}

ADyActorCppScript* CDyActorScriptCpp::MDY_PRIVATE(GetScriptInstance)() const noexcept
{
  MDY_ASSERT(MDY_CHECK_ISNOTEMPTY(this->mScriptInstance), "Script instance must be valid, not empty.");
  return this->mScriptInstance.get();
}

void CDyActorScriptCpp::Initiate()
{
  MDY_ASSERT(this->mIsScriptInstanceBinded == true, "Unexpected error occurred.");
  this->mScriptInstance->Initiate();
}

void CDyActorScriptCpp::Start()
{
  MDY_ASSERT(this->mIsScriptInstanceBinded == true, "Unexpected error occurred.");
  this->mScriptInstance->Start();
}

void CDyActorScriptCpp::Update(_MIN_ float dt)
{
  MDY_ASSERT(this->mIsScriptInstanceBinded == true, "Unexpected error occurred.");
  this->mScriptInstance->Update(dt);
}

void CDyActorScriptCpp::OnEnabled()
{
  MDY_ASSERT(this->mIsScriptInstanceBinded == true, "Unexpected error occurred.");
  this->mScriptInstance->OnEnabled();
}

void CDyActorScriptCpp::OnDisabled()
{
  MDY_ASSERT(this->mIsScriptInstanceBinded == true, "Unexpected error occurred.");
  this->mScriptInstance->OnDisabled();
}

void CDyActorScriptCpp::Destroy()
{
  MDY_ASSERT(this->mIsScriptInstanceBinded == true, "Unexpected error occurred.");
  this->mScriptInstance->Destroy();
}

} /// ::dy namespace