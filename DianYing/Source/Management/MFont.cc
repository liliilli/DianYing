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
  if (metaManager.IsFontMetaInformationExist(fontSpecifierName) == false)
  {
    DyPushLogCritical(
      "Failed to create font resource, {}. This font information is not exist on meta system.", 
      fontSpecifierName);
    return DY_FAILURE;
  }

  const auto& fontMetaInformation = metaManager.GetFontMetaInformation(fontSpecifierName);
  if (fontMetaInformation.mIsUsingRuntimeCreateionWhenGlyphNotExist == false)
  { // Make space
    auto [it, isSucceeded] = this->mFontResourceContainerMap.try_emplace(fontSpecifierName, nullptr);
    MDY_ASSERT_MSG(isSucceeded == true, "Font resource creation must be succeeded.");

    // Create font resource.
    auto instance = std::make_unique<FDyFontResourceContainer>(fontMetaInformation);
    { // Swapping
      std::unique_ptr<IFontContainer> tempSwap{static_cast<IFontContainer*>(instance.release())};
      it->second.swap(tempSwap);
      MDY_ASSERT_MSG(it->second.get() != nullptr, "Unexpected error occurred");
    }

    //

  }
  else
  {
    MDY_NOT_IMPLEMENTED_ASSERT();
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