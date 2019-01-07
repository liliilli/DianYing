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

/// Header file
#include <Dy/Element/Abstract/ADyBaseComponent.h>

namespace dy
{

ADyBaseComponent::ADyBaseComponent(FDyActor& actorReference) : mBindedActor(&actorReference)
{ }

ADyBaseComponent::ADyBaseComponent(ADyBaseComponent&& instance) noexcept :
    mActivateFlag(instance.mActivateFlag),
    mBindedActor(instance.mBindedActor),
    mActivatedUpdateListId(instance.mActivatedUpdateListId)
{
  instance.mActivateFlag          = DDy3StateBool{};
  instance.mBindedActor           = nullptr;
  instance.mActivatedUpdateListId = MDY_INITIALIZE_DEFINT;
}

ADyBaseComponent& ADyBaseComponent::operator=(ADyBaseComponent&& instance) noexcept
{
  if (this == &instance) { return *this; }

  // Move instance properties.
  this->mActivateFlag             = instance.mActivateFlag;
  this->mBindedActor              = instance.mBindedActor;
  this->mActivatedUpdateListId    = instance.mActivatedUpdateListId;

  // Reset moved instance's properties.
  instance.mActivateFlag          = DDy3StateBool{};
  instance.mBindedActor           = nullptr;
  instance.mActivatedUpdateListId = MDY_INITIALIZE_DEFINT;

  return *this;
}

void ADyBaseComponent::Activate() noexcept
{
  this->mActivateFlag.UpdateInput(true);
}

void ADyBaseComponent::Deactivate() noexcept
{
  this->mActivateFlag.UpdateInput(false);
}

void ADyBaseComponent::pPropagateParentActorActivation(const DDy3StateBool& actorBool) noexcept
{
  this->mActivateFlag.UpdateParent(actorBool.GetOutput());
}

bool ADyBaseComponent::IsComponentActivated() const noexcept
{
  return this->mActivateFlag.GetOutput();
}

} /// ::dy namespace