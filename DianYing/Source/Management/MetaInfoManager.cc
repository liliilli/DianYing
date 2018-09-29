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
#include <Dy/Management/MetaInfoManager.h>

#include <nlohmann/json.hpp>
#include <Dy/Helper/JsonHelper.h>
#include <Dy/Element/Descriptor/LevelDescriptor.h>

//!
//! Forward declaration
//!

namespace
{

MDY_SET_IMMUTABLE_STRING(sTestPath, "./TestScene.DDat");

} /// ::unnamed namespace

//!
//! Implementation
//!

namespace dy
{

EDySuccess MDyMetaInfo::pfInitialize()
{
  const auto opJsonAtlas = DyGetJsonAtlas(sTestPath.data());
  if (!opJsonAtlas.has_value()) { return DY_FAILURE; }

  PDyLevelConstructDescriptor desc = PDyLevelConstructDescriptor::GetDescriptor(opJsonAtlas.value());
  auto [it, result] = this->mLevelInfoMap.try_emplace(desc.mLevelName, desc);
  if (!result)
  {
    PHITOS_UNEXPECTED_BRANCH();
    return DY_FAILURE;
  }

  return DY_SUCCESS;
}

EDySuccess MDyMetaInfo::pfRelease()
{

  return DY_SUCCESS;
}

const PDyLevelConstructDescriptor* MDyMetaInfo::GetLevelMetaInformation(const std::string& levelName) const noexcept
{
  const auto it = this->mLevelInfoMap.find(levelName);
  if (it == this->mLevelInfoMap.end())  { return nullptr; }
  else                                  { return &it->second; }
}

} /// ::dy namespace