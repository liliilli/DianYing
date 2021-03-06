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
#include <Dy/Element/Abstract/ABaseComponent.h>

namespace dy
{

ABaseComponent::ABaseComponent(FActor& actorReference) : mBindedActor(&actorReference)
{ }

ABaseComponent::ABaseComponent(ABaseComponent&& instance) noexcept :
    mBindedActor(instance.mBindedActor),
    mActivatedUpdateListId(instance.mActivatedUpdateListId)
{
  instance.mBindedActor           = nullptr;
  instance.mActivatedUpdateListId = MDY_INITIALIZE_DEFINT;
}

ABaseComponent& ABaseComponent::operator=(ABaseComponent&& instance) noexcept
{
  if (this == &instance) { return *this; }

  // Move instance properties.
  this->mBindedActor              = instance.mBindedActor;
  this->mActivatedUpdateListId    = instance.mActivatedUpdateListId;

  // Reset moved instance's properties.
  instance.mBindedActor           = nullptr;
  instance.mActivatedUpdateListId = MDY_INITIALIZE_DEFINT;

  return *this;
}

bool ABaseComponent::IsComponentActivated() const noexcept
{
  return this->IsActivated();
}

} /// ::dy namespace