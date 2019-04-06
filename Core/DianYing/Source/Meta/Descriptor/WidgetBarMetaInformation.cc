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
#include <Dy/Meta/Descriptor/WidgetBarMetaInformation.h>
#include <Dy/Helper/Library/HelperJson.h>
#include <Dy/Helper/Type/DColorRGB24.h>

namespace dy
{

std::unique_ptr<PDyMetaWidgetBarDescriptor>
PDyMetaWidgetBarDescriptor::CreateMetaInformation(const nlohmann::json& itemAtlas)
{
  /* Template
    { "Name": "BasicBarTest", "Type": "BasicBar", "Parent": "", "ZOrder": 0, "IsActivated": true,
      "Details": {
        "InitialPosition": { "X": 0, "Y": 32 },
        "WidgetSize": { "X": 600, "Y": 32 },
        "Origin": "Center_Bottom",
        "ForegroundColor": { "R": 1.0, "G": 1.0, "B": 1.0, "A": 1.0 },
        "BackgroundColor": { "R": 1.0, "G": 1.0, "B": 1.0, "A": 1.0 },
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
  instance->mUiObjectSpecifierName  = json::GetValueFrom<std::string>(itemAtlas, TPDyMWCBD::sHeader_Name);
  instance->mComponentType          = EDyWidgetComponentType::BasicGaugeBar;
  instance->mParentSpecifierName    = json::GetValueFrom<std::string>(itemAtlas, TPDyMWCBD::sHeader_Parent);
  json::GetValueFromTo(itemAtlas, "IsActivated",  instance->mIsActivated);
  json::GetValueFromTo(itemAtlas, "ZOrder", instance->mZOrder);

  const auto& detailAtlas = itemAtlas[(TPDyMWCBD::sHeader_Details)];
  instance->mInitialPosition    = json::GetValueFrom<DIVec2>(detailAtlas, "InitialPosition");
  instance->mOrigin             = json::GetValueFrom<EDyOrigin>(detailAtlas, "Origin");
  instance->mWidgetSize         = json::GetValueFrom<DIVec2>(detailAtlas, "WidgetSize");

  instance->mForegroundColor    = json::GetValueFrom<DColorRGBA>(detailAtlas, "ForegroundColor");
  instance->mBackgroundColor    = json::GetValueFrom<DColorRGBA>(detailAtlas, "BackgroundColor");
  instance->mPadding            = json::GetValueFrom<TI32>(detailAtlas, "Padding");
  instance->mMin                = json::GetValueFrom<TF32>(detailAtlas, "Min");
  instance->mMax                = json::GetValueFrom<TF32>(detailAtlas, "Max");
  instance->mIsUsingBackground  = json::GetValueFrom<bool>(detailAtlas, "IsUsingBackgroundColor");

  return instance;
}

} /// ::dy namespace