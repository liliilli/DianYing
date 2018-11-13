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
#include <Dy/Builtin/Helper/BuiltinInformationDeliver.h>

//!
//! Implementation
//!

namespace dy::builtin
{

FDyBuiltinWidgetDebugUiMeta::FDyBuiltinWidgetDebugUiMeta()
{
  constexpr const char* widgetMetaString = R"dy(
{
  "Meta":
  {
    "Name": "DebugUi",
    "Script": { "Specifier": "DebugUiScriptCpp" }
  },
  "ObjectList":
  [
    {
      "Name": "DebugTestText",
      "Type": "Text",
      "Parent": "",
      "Details":
      {
        "InitialString": "Hello world!",
        "InitialPosition": { "X":0, "Y":0 },
        "InitialColor": 16777215,
        "InitialAlpha": 1.0,
        "FontSize": 12,
        "FontAlignment": "Left",
        "EdgeSize": 1,
        "FontSpecifierName": "Arial",
        "IsBold": true,
        "IsItalic": false,
        "IsUsingEdge": false,
        "IsUsingDefaultFont": true
      }
    }
  ]
}
)dy";

  MDY_CALL_ASSERT_SUCCESS(FDyBuiltinInformationDeliver::ForwardWidgetMetaInformation(widgetMetaString));
}

} /// ::dy::builtin namespace