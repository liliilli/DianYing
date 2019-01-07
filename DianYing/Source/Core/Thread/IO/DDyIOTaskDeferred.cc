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
#include <Dy/Core/Thread/IO/DDyIOTaskDeferred.h>

namespace dy
{

MDY_NODISCARD EDySuccess DDyIOTaskDeferred::TryRemoveDependenciesItem(
    _MIN_ const std::string& iSpecifier, 
    _MIN_ EDyResourceType& iType, 
    _MIN_ EDyResourceStyle& iStyle) noexcept
{
  const DConditionItem item{iSpecifier, iType, iStyle};

  for (auto it = this->mCondition.begin(); it != this->mCondition.end(); ++it)
  {
    if (*it == item) {
      const auto index = std::distance(this->mCondition.begin(), it);
      DyFastErase(this->mCondition, static_cast<TU32>(index));
      return DY_SUCCESS;
    }
  }

  return DY_FAILURE;
}

} /// ::dy namespace