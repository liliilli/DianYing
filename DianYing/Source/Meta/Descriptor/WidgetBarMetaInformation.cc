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
#include <Dy/Meta/Descriptor/WidgetBarMetaInformation.h>
#include <Dy/Helper/Library/HelperJson.h>
#include <Dy/Helper/Type/ColorRGB24.h>

namespace dy
{

std::unique_ptr<PDyMetaWidgetBarDescriptor>
PDyMetaWidgetBarDescriptor::CreateMetaInformation(const nlohmann::json& itemAtlas)
{
  /* Template
   * {
        "Name": "BasicBarTest",
        "Type": "BasicBar",
        "Parent": "",
        "Details": {
          "InitialPosition": { "X": 0, "Y": 32 },
          "WidgetSize": { "X": 600, "Y": 32 },
          "Origin": "Center_Bottom",
          "ForegroundColor": 16777215,
          "ForegroundAlpha": 1.0,
          "BackgroundColor": 16777215,
          "BackgroundAlpha": 1.0,
          "Padding": 4,
          "Min": 0,
          "Max": 100,
          "IsUsingBackgroundColor": false
        }
      }
   */

  using TPDyMWCBD = PDyMetaWidgetCommonBaseDesc;

  //! - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  //! FUNCTIONBODY ∨
  //! - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  // Common
  auto instance = std::make_unique<PDyMetaWidgetBarDescriptor>();
  instance->mUiObjectSpecifierName  = DyJsonGetValueFrom<std::string>(itemAtlas, TPDyMWCBD::sHeader_Name);
  instance->mComponentType          = EDyWidgetComponentType::BasicGaugeBar;
  instance->mParentSpecifierName    = DyJsonGetValueFrom<std::string>(itemAtlas, TPDyMWCBD::sHeader_Parent);

  const auto& detailAtlas = itemAtlas[MSVSTR(TPDyMWCBD::sHeader_Details)];
  instance->mInitialPosition    = DyJsonGetValueFrom<DDyVectorInt2>(detailAtlas, "InitialPosition");
  instance->mOrigin             = DyJsonGetValueFrom<EDyOrigin>(detailAtlas, "Origin");
  instance->mWidgetSize         = DyJsonGetValueFrom<DDyVectorInt2>(detailAtlas, "WidgetSize");

  DDyColorRGBA foregroundColor  = DyJsonGetValueFrom<DDyColorRGB24>(detailAtlas, "ForegroundColor");
  foregroundColor.A             = DyJsonGetValueFrom<TF32>(detailAtlas, "ForegroundAlpha");
  DDyColorRGBA backgroundColor  = DyJsonGetValueFrom<DDyColorRGB24>(detailAtlas, "BackgroundColor");
  backgroundColor.A             = DyJsonGetValueFrom<TF32>(detailAtlas, "BackgroundAlpha");
  instance->mForegroundColor    = foregroundColor;
  instance->mBackgroundColor    = backgroundColor;

  instance->mPadding            = DyJsonGetValueFrom<TI32>(detailAtlas, "Padding");
  instance->mMin                = DyJsonGetValueFrom<TF32>(detailAtlas, "Min");
  instance->mMax                = DyJsonGetValueFrom<TF32>(detailAtlas, "Max");
  instance->mIsUsingBackground  = DyJsonGetValueFrom<bool>(detailAtlas, "IsUsingBackgroundColor");

  return instance;
}

} /// ::dy namespace