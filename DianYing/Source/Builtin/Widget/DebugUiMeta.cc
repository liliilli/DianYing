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
#include <Dy/Builtin/Widget/DebugUiMeta.h>

namespace
{

MDY_SET_IMMUTABLE_STRING(sBuffer, R"dy(
{
  "Meta":
  {
    "Name": "DebugUi", "ZOrder" : 102, "IsUsingScript": true, "IsActivated": true,
    "Script": {
      "Type" : "Script", "Activated" : true,
      "Details" : { "Specifier": "FDyBuiltinDebugUiScript" }
    }
  },
  "ObjectList":
  [
    { "Name": "DebugTestText", "Type": "Text", "Parent": "", "ZOrder": 0, "IsActivated": true,
      "Details":
      { "InitialPosition": { "X":32, "Y": -64 },
        "WidgetSize": { "X": 1, "Y": 1 },
        "Origin": "Left_Top",
        "InitialString": "", 
        "InitialColor": { "R": 1.0, "G": 1.0, "B": 1.0, "A": 1.0 },
        "EdgeColor": { "R": 0.0, "G": 0.0, "B": 0.0 },
        "FontSize": 9, "Alignment": "Left",
        "FontSpecifierName": "Arial", "IsUsingEdge": true
      }
    },
    { "Name": "JoystickTest", "Type": "Text", "Parent": "", "ZOrder": 0, "IsActivated": true,
      "Details":
      { "InitialPosition": { "X":32, "Y": 64 },
        "WidgetSize": { "X": 1, "Y": 1 },
        "Origin": "Left_Bottom",
        "InitialString": "", 
        "InitialColor": { "R": 1.0, "G": 1.0, "B": 0.0, "A": 1.0 },
        "EdgeColor": { "R": 0.0, "G": 0.0, "B": 0.0 },
        "FontSize": 9, "Alignment": "Left",
        "FontSpecifierName": "Arial", "IsUsingEdge": true
      }
    },
    { "Name": "BasicBarTest", "Type": "BasicBar", "Parent": "", "ZOrder": 0, "IsActivated": true,
      "Details": {
        "InitialPosition": { "X": 0, "Y": -32 },
        "WidgetSize": { "X": 320, "Y": 8 },
        "Origin": "Center_Top",
        "ForegroundColor": { "R": 1.0, "G": 1.0, "B": 0.0, "A": 1.0 },
        "BackgroundColor": { "R": 0.0, "G": 0.0, "B": 0.0, "A": 0.85 },
        "Padding": 2, "Min": 0, "Max": 100,
        "IsUsingBackgroundColor": true
      }
    },
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