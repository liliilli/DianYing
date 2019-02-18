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
#include <Dy/Builtin/Widget/FDyBtWdTestLoadingUi.h>

//!
//! Forward declaration
//!

namespace 
{

MDY_SET_IMMUTABLE_STRING(sBuffer, R"dy(
{
  "Meta":
  { "Name": "dyBtWdTestLoadingUi", "ZOrder" : 100,
    "Script": {
      "Type" : "Script", "Activated" : true,
      "Details" : { "Specifier": "FDyBtScTestLoadingUi" }
    }
  },
  "ObjectList":
  [
    { "Name": "LoadingText", "Type": "Text", "Parent": "", "ZOrder" : 0,
      "Details":
      {
        "InitialPosition": { "X":0, "Y": 0 },
        "WidgetSize": { "X": 1, "Y": 1 },
        "Origin": "Center_Center",
        "InitialString": "LOADING...", "InitialColor": 16777215, "InitialAlpha": 1.0,
        "EdgeColor": 0, "FontSize": 18, "Alignment": "Center",
        "FontSpecifierName": "Arial", "IsUsingEdge": false
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

FDyBtWdTestLoadingUi::FDyBtWdTestLoadingUi()
{
  this->mMetaBuffer = sBuffer;
}

} /// ::dy namespace