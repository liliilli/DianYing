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
#include <Dy/Management/ExternalResouceInfoManager.h>

#include <nlohmann/json.hpp>
#include <Dy/Helper/JsonHelper.h>
#include <Dy/Element/Descriptor/LevelDescriptor.h>

#define TESTCODE_20180927
#if defined(TESTCODE_20180927)
namespace
{



} /// unnamed namespace
#endif // TESTCODE_20180927

namespace dy
{

EDySuccess MDyExtRscInfo::pfInitialize()
{
#if defined(TESTCODE_20180927)
  const auto opJsonAtlas = DyGetJsonAtlas("./TestScene.DDat");
  if (!opJsonAtlas.has_value()) { return DY_FAILURE; }

  PDyLevelConstructDescriptor desc = PDyLevelConstructDescriptor::GetDescriptor(opJsonAtlas.value());
  auto [it, result] = this->mLevelInfoMap.try_emplace(desc.mLevelName, desc);
  if (!result)
  {
    PHITOS_UNEXPECTED_BRANCH();
    return DY_FAILURE;
  }

#endif // TESTCODE_20180927

  return DY_SUCCESS;
}

EDySuccess MDyExtRscInfo::pfRelease()
{

  return DY_SUCCESS;
}

} /// ::dy namespace