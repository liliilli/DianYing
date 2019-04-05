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
#include <Dy/Management/Type/FontResourceContainer.h>
#include <Dy/Helper/Library/HelperContainer.h>
#include <Dy/Management/MLog.h>

namespace dy
{

EDySuccess MFont::pfInitialize()
{
  return DY_SUCCESS;
}

EDySuccess MFont::pfRelease()
{
  this->mFontResourceContainerMap.clear();
  return DY_SUCCESS;
}

EDySuccess MFont::CreateFontResourceContainer(const std::string& fontSpecifierName)
{
  if (this->IsFontResourceContainerExist(fontSpecifierName) == true)
  {
    DyPushLogError(
      "Failed to create font resource, {}. This font is already created in container.", 
      fontSpecifierName);
    return DY_SUCCESS;
  }

  // Create font information and move it.
  auto& metaManager = MIOMeta::GetInstance();
  MDY_ASSERT_FORCE(metaManager.IsFontMetaInformationExist(fontSpecifierName) == true);

  const auto& fontInfo = metaManager.GetFontMetaInformation(fontSpecifierName);
  using ELoadingType = PDyMetaFontInformation::ELoadingType;
  switch (fontInfo.mLoadingType)
  {
  case ELoadingType::ExternalPlain: 
  { 
    const auto& details = std::get<PDyMetaFontInformation::ToDetailType<ELoadingType::ExternalPlain>>(fontInfo.mDetails);
    // Create font resource.
    this->mFontResourceContainerMap.try_emplace(fontInfo.mSpecifierName, std::make_unique<FDyFontResourceContainer>(fontInfo));
  } break;
  case ELoadingType::ExternalCompressed: 
  case ELoadingType::Runtime: 
  case ELoadingType::Builtin:
  default: MDY_UNEXPECTED_BRANCH(); break;
  }

  return DY_SUCCESS;
}

bool MFont::IsFontResourceContainerExist(const std::string& specifierName)
{
  return Contains(this->mFontResourceContainerMap, specifierName);
}

  IFontContainer* MFont::GetFontResourceContainer(_MIN_ const std::string& specifierName)
{
  if (this->IsFontResourceContainerExist(specifierName) == false) { return nullptr; }

  return this->mFontResourceContainerMap[specifierName].get();
}

} /// ::dy namespace