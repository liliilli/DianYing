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
#include <Dy/Component/CDyScript.h>
#include <Dy/Management/WorldManager.h>

namespace dy
{

CDyScript::CDyScript(FDyActor& actorReference) : ADyBaseComponent(actorReference)
{

}

void CDyScript::Activate() noexcept
{
  ADyBaseComponent::Activate();
  //
}

void CDyScript::Deactivate() noexcept
{
  ADyBaseComponent::Deactivate();
  //
}

void CDyScript::Initiate()
{

}

void CDyScript::Start()
{

}

void CDyScript::Update(float dt)
{

}

void CDyScript::OnEnabled()
{

}

void CDyScript::OnDisabled()
{

}

void CDyScript::Destroy()
{

}

std::string CDyScript::ToString()
{
  return "CDyScript::ToString NOT IMPLEMENTED YET!";
}

EDySuccess CDyScript::Initialize(const DDyScriptMetaInformation& metaInfo)
{
  // @TODO ASSERT THAT SCRIPT COMPONENT IS ACTIVATED EVEN WHEN FIRST TIME.
  this->mScriptPath             = metaInfo.mScriptPath;
  const auto activatedIndex     = MDyWorld::GetInstance().pfEnrollActiveScript(DyMakeNotNull(this));
  this->mActivatedUpdateListId  = activatedIndex;

  return DY_SUCCESS;
}

void CDyScript::Release()
{
  MDyWorld::GetInstance().pfUnenrollActiveScript(this->mActivatedUpdateListId);
}

} /// ::dy namespace