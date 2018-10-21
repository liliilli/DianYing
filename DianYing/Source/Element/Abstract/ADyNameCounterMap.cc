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
#include <Dy/Element/Abstract/ADyNameCounterMap.h>

namespace dy
{

ADyNameCounterMap::~ADyNameCounterMap() = default;

std::string ADyNameCounterMap::CreateObjectName(const std::string& name) noexcept
{
  if (const auto it = mNameCounterMap.find(name); it != mNameCounterMap.end())
  {
    auto& count = mNameCounterMap[name];
    count += 1;

    return fmt::format("{0}_{1}", name, count);
  }
  else
  {
    mNameCounterMap[name] = 0;
    return name;
  }
}

} /// ::dy namespace