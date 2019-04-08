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
#include <Dy/Helper/Internal/FNameGenerator.h>
#include <Dy/Helper/Library/HelperContainer.h>
#include <Dy/Helper/Library/HelperString.h>

namespace dy
{

std::string FNameGenerator::TryGetGeneratedName(const std::string& iName) noexcept
{
  if (Contains(this->mContainer, iName) == false)
  {
    this->mContainer.try_emplace(iName, 0);
    return iName;
  }
  else
  {
    const auto index = this->mContainer[iName];
    this->mContainer[iName] += 1;

    return MakeStringU8("{}_{}", iName, index);
  }
}

} /// ::dy namespace