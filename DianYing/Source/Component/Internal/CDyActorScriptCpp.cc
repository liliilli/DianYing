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

#include <Dy/Component/Internal/CDyActorScriptCpp.h>
#include <Dy/Management/IO/MetaInfoManager.h>
#include <Dy/Core/Reflection/RDyCppScript.h>

namespace dy
{

EDySuccess CDyActorScriptCpp::Initialize(const PDyScriptComponentMetaInfo& descriptor)
{
  const auto& metaInfo = MDyMetaInfo::GetInstance().GetScriptMetaInformation(descriptor.mDetails.mSpecifierName);
  MDY_ASSERT(metaInfo.mScriptType == EDyScriptType::Cpp, "Script type is not matched to CDyScriptCpp.");

  this->mScriptInstance = DyRefGetActorScriptInstance(metaInfo.mSpecifierName);
  this->mScriptInstance->pfSetOutsideReference(*this);
  MDY_ASSERT(this->mScriptInstance != nullptr, "Script instance could not bound to system.");

  return DY_SUCCESS;
}

void CDyActorScriptCpp::Release()
{
  this->mScriptInstance = nullptr;
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