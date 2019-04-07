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
#include <Dy/Core/Thread/IO/DRescIODeferredTask.h>

namespace dy
{

EDySuccess DRescIODeferredTask::TryRemoveDependentItem(
  const std::string& iSpecifier, 
  EResourceType& iType, 
  EResourceStyle& iStyle) noexcept
{
  const DConditionItem item{iSpecifier, iType, iStyle};

  for (auto it = this->mCondition.begin(); it != this->mCondition.end(); ++it)
  {
    // Check Condition item informations are same to each other.
    if (*it == item) 
    {
      FaseErase(this->mCondition, it);
      return EDySuccess::DY_SUCCESS;
    }
  }

  return EDySuccess::DY_FAILURE;
}

} /// ::dy namespace