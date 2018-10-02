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
#include <Dy/Element/Abstract/ADyBaseComponent.h>

namespace dy
{

void ADyBaseComponent::Activate() noexcept
{
  this->mActivateFlag.UpdateInput(true);
}

void ADyBaseComponent::Deactivate() noexcept
{
  this->mActivateFlag.UpdateInput(false);

#ifdef false
  // If component is being binded to MDyWorld component activate list.
  if (this->mActivatedUpdateListId != MDY_INITIALIZE_DEFINT)
  {

  }
#endif
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