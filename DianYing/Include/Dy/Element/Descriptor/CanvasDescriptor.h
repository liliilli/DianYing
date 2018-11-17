#ifndef GUARD_DY_ELEMENT_DESCRIPTOR_CANVASDESCRIPTOR_H
#define GUARD_DY_ELEMENT_DESCRIPTOR_CANVASDESCRIPTOR_H
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

#include <Dy/Helper/Type/Vector2.h>
#include <Dy/Helper/Type/ColorRGBA.h>
#include <Dy/Helper/Type/DyString.h>

//!
//! Forward declaration
//!

namespace dy
{
struct PDyMetaWidgetTextDescriptor;
} /// ::dy namespace

//!
//! Implementation
//!

namespace dy
{

enum class EDyTextAlignmentType : int
{
  // Alignable instance would be displayed to the point started from position.
  Left,
  // Alignable instance would be displayed to the position is center of instance.
  Center,
  // Alignable instance would be displayed to the point ended to position.
  Right
};

///
/// @brief This enum is need for calculating originable instance's final rendering position. \n
/// IOriginable instance has a feature of calculating final poisiton mechanism with UiObject, or MFontManager. \n
///
/// \(7)|(8)/(9) At first, method gets Origin value of child which is left and calculate \n
/// -(4).(5)-(6) with parent_x, y, width, height to be aligned Parent's region without \n
/// /(1)|(2)\(3) applying child's local position. \n
///
/// Afterward, this method sets final rendering position of child and apply changed value to
/// children of child calling SetUiParentPosition() of children of child.
///
enum class EDyOrigin : int
{
  UP_LEFT = 7,      UP_CENTER = 8,      UP_RIGHT = 9,
  CENTER_LEFT = 4,  CENTER_CENTER = 5,  CENTER_RIGHT = 6,
  DOWN_LEFT = 1,    DOWN_CENTER = 2,    DOWN_RIGHT = 3
};

} /// ::dy namespace

#endif /// GUARD_DY_ELEMENT_DESCRIPTOR_CANVASDESCRIPTOR_H