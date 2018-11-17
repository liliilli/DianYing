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
#include <Dy/Management/FontManager.h>

#include <string>
#include <Dy/Core/Component/Internal/MaterialType.h>
#include <Dy/Management/IO/MetaInfoManager.h>
#include <Dy/Management/Type/FontResourceContainer.h>

//!
//! Forward declaration
//!

namespace
{

MDY_SET_IMMUTABLE_STRING(sSampleEnglishFontPath,  "./TestResource/Inconsolata-Regular.ttf");
MDY_SET_IMMUTABLE_STRING(sSampleJapaneseFontPath, "./TestResource/UDDigiKyokashoN-R.ttc");
//MDY_SET_IMMUTABLE_STRING(sSampleJapaneseFontPath, "./TestResource/msgothic.ttc");
MDY_SET_IMMUTABLE_STRING(sSampleHangleFontPath,   "./TestResource/KoPubDotumMedium.ttf");

} /// ::unnamed namespace

//!
//! Implementation
//!

namespace dy
{

EDySuccess MDyFont::pfInitialize()
{
  // Create font and move it.
  mDefaultFontContainer_Deprecated = new FDyFontContainer_Deprecated(MSVSTR(sSampleJapaneseFontPath));

  return DY_SUCCESS;
}

EDySuccess MDyFont::pfRelease()
{
  MDY_ASSERT(MDY_CHECK_ISNOTNULL(this->mDefaultFontContainer_Deprecated), "");
  delete this->mDefaultFontContainer_Deprecated;
  this->mDefaultFontContainer_Deprecated = nullptr;

  return DY_SUCCESS;
}

EDySuccess MDyFont::CreateFontResourceContainer(const std::string& fontSpecifierName)
{
  if (this->IsFontResourceContainerExist(fontSpecifierName) == true)
  {
    MDY_UNEXPECTED_BRANCH(); return DY_FAILURE;
  }

  // Create font information and move it.
  auto& metaManager = MDyMetaInfo::GetInstance();
  if (metaManager.IsFontMetaInformationExist(fontSpecifierName) == false)
  {
    return DY_FAILURE;
  }

  const auto& fontMetaInformation = metaManager.GetFontMetaInformation(fontSpecifierName);
  if (fontMetaInformation.mIsUsingRuntimeCreateionWhenGlyphNotExist == false)
  { // Make space
    auto [it, isSucceeded] = this->mFontResourceContainerMap.try_emplace(fontSpecifierName, nullptr);
    MDY_ASSERT(isSucceeded == true, "Font resource creation must be succeeded.");

    // Create font resource.
    auto instance = std::make_unique<FDyFontResourceContainer>(fontMetaInformation);
    { // Swapping
      std::unique_ptr<IDyFontContainer> tempSwap{static_cast<IDyFontContainer*>(instance.release())};
      it->second.swap(tempSwap);
      MDY_ASSERT(it->second.get() != nullptr, "Unexpected error occurred");
    }

    //

  }
  else
  {
    MDY_NOT_IMPLEMENTED_ASSERT();
  }

  return DY_SUCCESS;
}

NotNull<FDyFontContainer_Deprecated*> MDyFont::GetDefaultFontContainer() const noexcept
{
  MDY_ASSERT(MDY_CHECK_ISNOTNULL(this->mDefaultFontContainer_Deprecated), "");
  return DyMakeNotNull(this->mDefaultFontContainer_Deprecated);
}

IDyFontContainer* MDyFont::GetFontResourceContainer(_MIN_ const std::string& specifierName)
{
  if (this->IsFontResourceContainerExist(specifierName) == false) { return nullptr; }

  return this->mFontResourceContainerMap[specifierName].get();
}

EDySuccess MDyFont::CreateFontContainer_Deprecated(const std::string& fontSpecifierName)
{
  if (this->IsFontContainerExist_Deprecated(fontSpecifierName) == true)
  {
    MDY_UNEXPECTED_BRANCH(); return DY_FAILURE;
  }

  // Create font and move it.
  auto [it, isCreated] = mValidFontContainerMap_Deprecated.try_emplace(fontSpecifierName, MSVSTR(sSampleEnglishFontPath));
  if (isCreated == false)
  {
    MDY_UNEXPECTED_BRANCH(); return DY_FAILURE;
  }

  return DY_SUCCESS;
}

} /// ::dy namespace