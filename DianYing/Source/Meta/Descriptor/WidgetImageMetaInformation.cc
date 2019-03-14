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
   *  {
        "Name": "TestImage",
        "Type": "Image",
        "Parent": "",
        "Details": {
          "InitialPosition": { "X": 0, "Y": 0 },
          "WidgetSize": { "X": 200, "Y": 100 },
          "Origin": "Center_Center",
          "ImageRegion": {
            "LeftDown": { "X": 0.0, "Y": 0.0 },
            "RightUp" : { "X": 1.0, "Y": 1.0 }
          },
          "ImageSpecifierName": "TestImageImage",
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
  resultInstance->mUiObjectSpecifierName  = json::GetValueFrom<std::string>(itemAtlas, TPDyMWCBD::sHeader_Name);
  resultInstance->mComponentType          = EDyWidgetComponentType::Image;
  resultInstance->mParentSpecifierName    = json::GetValueFrom<std::string>(itemAtlas, TPDyMWCBD::sHeader_Parent);
  json::GetValueFromTo(itemAtlas, "ZOrder", resultInstance->mZOrder);

  const auto& detailAtlas           = itemAtlas[(TPDyMWCBD::sHeader_Details)];
  resultInstance->mInitialPosition  = json::GetValueFrom<DVectorInt2>(detailAtlas, "InitialPosition");
  resultInstance->mOrigin           = json::GetValueFrom<EDyOrigin>(detailAtlas, "Origin");
  resultInstance->mWidgetSize       = json::GetValueFrom<DVectorInt2>(detailAtlas, "WidgetSize");

  resultInstance->mImageRegion        = json::GetValueFrom<DArea2D>(detailAtlas, "ImageRegion");
  resultInstance->mImageSpecifierName = json::GetValueFrom<std::string>(detailAtlas, "ImageSpecifierName");
  resultInstance->mIsSizeToContent  = json::GetValueFrom<bool>(detailAtlas, "IsSizeToContent");
  resultInstance->mTintColor        = json::GetValueFrom<DColorRGBA>(detailAtlas, "TintColor");;


  return resultInstance;
}

} /// ::dy namespace