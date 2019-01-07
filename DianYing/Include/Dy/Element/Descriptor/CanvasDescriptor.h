#ifndef GUARD_DY_ELEMENT_DESCRIPTOR_CANVASDESCRIPTOR_H
#define GUARD_DY_ELEMENT_DESCRIPTOR_CANVASDESCRIPTOR_H
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

} /// ::dy namespace

#endif /// GUARD_DY_ELEMENT_DESCRIPTOR_CANVASDESCRIPTOR_H