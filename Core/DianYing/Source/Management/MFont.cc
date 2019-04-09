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
#include <Dy/Management/MFont.h>

#include <string>
#include <Dy/Management/IO/MIOMeta.h>
#include <Dy/Management/Internal/Font/FFontContainerPlain.h>
#include <Dy/Management/Internal/Font/FFontContainerBuiltin.h>
#include <Dy/Helper/Library/HelperContainer.h>
#include <Dy/Helper/System/Assertion.h>
#include <Dy/Management/MLog.h>

namespace dy
{

EDySuccess MFont::pfInitialize()
{
  return EDySuccess::DY_SUCCESS;
}

EDySuccess MFont::pfRelease()
{
  this->mFontResourceContainerMap.clear();
  return EDySuccess::DY_SUCCESS;
}

EDySuccess MFont::CreateFontResourceContainer(const std::string& identifier)
{
  if (this->IsFontResourceContainerExist(identifier) == true)
  {
    DyPushLogError(
      "Failed to create font resource, {}. This font is already created in container.", 
      identifier);
    return EDySuccess::DY_SUCCESS;
  }

  // Create font information and move it.
  auto& metaManager = MIOMeta::GetInstance();
  MDY_ASSERT_FORCE(metaManager.IsFontMetaInformationExist(identifier) == true);

  const auto& fontInfo = metaManager.GetFontMetaInformation(identifier);
  using ELoadingType = PDyMetaFontInformation::ELoadingType;
  switch (fontInfo.mLoadingType)
  {
  case ELoadingType::ExternalPlain: 
  { 
    const auto& details = std::get<
      PDyMetaFontInformation::ToDetailType<ELoadingType::ExternalPlain>>(fontInfo.mDetails);
    // Create font resource.
    this->mFontResourceContainerMap.try_emplace(
      fontInfo.mSpecifierName, 
      std::make_unique<FFontContainerPlain>(details));
  } break;
  case ELoadingType::ExternalCompressed: 
  case ELoadingType::Runtime: 
    MDY_NOT_IMPLEMENTED_ASSERT(); break;
  case ELoadingType::Builtin:
  {
    const auto& details = std::get<
      PDyMetaFontInformation::ToDetailType<ELoadingType::Builtin>>(fontInfo.mDetails);
    // Create font resource.
    this->mFontResourceContainerMap.try_emplace(
      fontInfo.mSpecifierName, 
      std::make_unique<FFontContainerBuiltin>(details));
  } break;
  default: MDY_UNEXPECTED_BRANCH(); break;
  }

  return EDySuccess::DY_SUCCESS;
}

bool MFont::IsFontResourceContainerExist(const std::string& identifier)
{
  return Contains(this->mFontResourceContainerMap, identifier);
}

EDySuccess MFont::RemoveFontContainer(const std::string& identifier)
{
  return EDySuccess::DY_SUCCESS;
}

AFontContainer* MFont::GetFontResourceContainer(const std::string& identifier)
{
  if (this->IsFontResourceContainerExist(identifier) == false) { return nullptr; }

  return this->mFontResourceContainerMap[identifier].get();
}

} /// ::dy namespace