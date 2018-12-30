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
#include <Dy/Builtin/Widget/DebugUiMeta.h>

namespace
{

MDY_SET_IMMUTABLE_STRING(sBuffer, R"dy(
{
  "Meta":
  {
    "Name": "DebugUi",
    "Script": {
      "Type"      : "Script",
      "Activated" : true,
      "Details"   : { "Specifier": "FDyBuiltinDebugUiScript" }
    }
  },
  "ObjectList":
  [
    {
      "Name": "DebugTestText",
      "Type": "Text",
      "Parent": "",
      "Details":
      {
        "InitialPosition": { "X":32, "Y": -64 },
        "WidgetSize": { "X": 1, "Y": 1 },
        "Origin": "Left_Top",
        "InitialString": "", "InitialColor": 16777215, "InitialAlpha": 1.0,
        "EdgeColor": 0, "FontSize": 9, "FontAlignment": "Left",
        "FontSpecifierName": "Arial", "IsUsingEdge": true
      }
    },
    {   
      "Name": "JoystickTest",
      "Type": "Text",
      "Parent": "",
      "Details":
      {
        "InitialPosition": { "X":32, "Y": 64 },
        "WidgetSize": { "X": 1, "Y": 1 },
        "Origin": "Left_Bottom",
        "InitialString": "", "InitialColor": 16776960, "InitialAlpha": 1.0,
        "EdgeColor": 0, "FontSize": 9, "FontAlignment": "Left",
        "FontSpecifierName": "Arial", "IsUsingEdge": true
      }
    },
    {
      "Name": "BasicBarTest",
      "Type": "BasicBar",
      "Parent": "",
      "Details": {
        "InitialPosition": { "X": 0, "Y": -32 },
        "WidgetSize": { "X": 320, "Y": 8 },
        "Origin": "Center_Top",
        "ForegroundColor": 16776960,
        "ForegroundAlpha": 1.0,
        "BackgroundColor": 0,
        "BackgroundAlpha": 0.85,
        "Padding": 2,
        "Min": 0,
        "Max": 100,
        "IsUsingBackgroundColor": true
      }
    },
    {
      "Name": "TestImage",
      "Type": "Image",
      "Parent": "",
      "Details": {
        "InitialPosition": { "X": -80, "Y": 80 },
        "WidgetSize": { "X": 128, "Y": 128 },
        "Origin": "Right_Bottom",
        "ImageRegion": {
          "LeftDown": { "X": 0.0, "Y": 0.0 },
          "RightUp" : { "X": 1.0, "Y": 1.0 }
        },
        "ImageSpecifierName": "dyBtTextureChecker",
        "ImageTintColor": 16777215,
        "ImageAlpha": 1.0,
        "IsSizeToContent": false
      }
    }
  ]
}
)dy");

} /// ::unnamed namespace

//!
//! Implementation
//!

namespace dy::builtin
{

FDyBuiltinWidgetDebugUiMeta::FDyBuiltinWidgetDebugUiMeta()
{
  this->mMetaBuffer = sBuffer;
}

} /// ::dy::builtin namespace