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
#include <Dy/Meta/Descriptor/WidgetImageMetaInformation.h>
#include <Dy/Helper/Library/HelperJson.h>
#include <Dy/Helper/Type/DColorRGB24.h>

namespace dy
{

std::unique_ptr<PDyMetaWidgetImageDescriptor>
PDyMetaWidgetImageDescriptor::CreateMetaInformation(const nlohmann::json& itemAtlas)
{
  /*  Template
    { "Name": "TestImage", "Type": "Image", "Parent": "", "ZOrder": 0, "IsActivated": true,
      "Details": {
        "InitialPosition": { "X": -80, "Y": 80 },
        "WidgetSize": { "X": 128, "Y": 128 },
        "Origin": "Right_Bottom",
        "ImageRegion": {
          "Start": { "X": 0.0, "Y": 0.0 },
          "Length" : { "X": 1.0, "Y": 1.0 }
        },
        "ImageSpecifierName": "dyBtTextureChecker",
        "TintColor": { "R": 1.0, "G": 1.0, "B": 1.0, "A": 1.0 },
        "IsSizeToContent": false
      }
    }
   */

  using TPDyMWCBD = PDyMetaWidgetCommonBaseDesc;

  //! - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  //! FUNCTIONBODY ∨
  //! - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  auto resultInstance = std::make_unique<PDyMetaWidgetImageDescriptor>();
  resultInstance->mComponentType = EDyWidgetComponentType::Image;

  json::GetValueFromTo(itemAtlas, "Name", resultInstance->mUiObjectSpecifierName);
  json::GetValueFromTo(itemAtlas, "Parent", resultInstance->mParentSpecifierName);
  json::GetValueFromTo(itemAtlas, "IsActivated", resultInstance->mIsActivated);
  json::GetValueFromTo(itemAtlas, "ZOrder", resultInstance->mZOrder);

  const auto& detailAtlas           = itemAtlas[(TPDyMWCBD::sHeader_Details)];
  resultInstance->mInitialPosition  = json::GetValueFrom<DIVec2>(detailAtlas, "InitialPosition");
  resultInstance->mOrigin           = json::GetValueFrom<EDyOrigin>(detailAtlas, "Origin");
  resultInstance->mWidgetSize       = json::GetValueFrom<DIVec2>(detailAtlas, "WidgetSize");

  resultInstance->mImageRegion      = json::GetValueFrom<DArea2D>(detailAtlas, "ImageRegion");
  resultInstance->mImageSpecifierName = json::GetValueFrom<std::string>(detailAtlas, "ImageSpecifierName");
  resultInstance->mIsSizeToContent  = json::GetValueFrom<bool>(detailAtlas, "IsSizeToContent");
  resultInstance->mTintColor        = json::GetValueFrom<DColorRGBA>(detailAtlas, "TintColor");;


  return resultInstance;
}

} /// ::dy namespace