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
#include "Dy/Core/Component/Internal/MaterialType.h"

//!
//! Forward declaration
//!

namespace
{

MDY_SET_IMMUTABLE_STRING(sSampleEnglishFontPath,  "./TestResource/arial.ttf");
MDY_SET_IMMUTABLE_STRING(sSampleJapaneseFontPath, "./TestResource/msgothic.ttc");
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
  mDefaultFontContainer = new FDyFontContainer(MSVSTR(sSampleJapaneseFontPath));

  return DY_SUCCESS;
}

EDySuccess MDyFont::pfRelease()
{
  MDY_ASSERT(MDY_CHECK_ISNOTNULL(this->mDefaultFontContainer), "");
  delete this->mDefaultFontContainer;
  this->mDefaultFontContainer = nullptr;

  return DY_SUCCESS;
}

EDySuccess MDyFont::CreateFontContainer(const std::string& fontSpecifierName)
{
  if (this->IsFontContainerExist(fontSpecifierName) == true) {
    MDY_UNEXPECTED_BRANCH();
    return DY_FAILURE;
  }

  // Create font and move it.
  auto [it, isCreated] = mValidFontContainerMap.try_emplace(fontSpecifierName, MSVSTR(sSampleEnglishFontPath));
  if (isCreated == false)
  {
    MDY_UNEXPECTED_BRANCH();
    return DY_FAILURE;
  }

  return DY_SUCCESS;
}

NotNull<FDyFontContainer*> MDyFont::GetDefaultFontContainer() const noexcept
{
  MDY_ASSERT(MDY_CHECK_ISNOTNULL(this->mDefaultFontContainer), "");
  return DyMakeNotNull(this->mDefaultFontContainer);
}

} /// ::dy namespace