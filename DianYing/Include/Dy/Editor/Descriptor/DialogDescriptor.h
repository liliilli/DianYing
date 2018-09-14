#ifndef GUARD_DY_EDITOR_DESCRIPTOR_DIALOG_DESCRIPTOR_H
#define GUARD_DY_EDITOR_DESCRIPTOR_DIALOG_DESCRIPTOR_H
#if defined(MDY_FLAG_IN_EDITOR)

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

//!
//! Forward declaration
//!

namespace dy::editor
{
class IDyGuiHasChildren;
}

//!
//! Implementation
//!

namespace dy::editor
{

///
/// @struct PDyGuiComponentDialogDescriptor
/// @brief dialog construction descriptor for gui dialog.
///
struct PDyGuiComponentDialogDescriptor final
{
  // Wrapping size of text when mIsWrappedTextLog is true.
  DDyVector2  mTextLogWrappingSize        = {};
  // Text of dialog subject header.
  std::string mDialogTitle                = "Dialog";
  // Text of dialog body.
  std::string mDialogTextBody             = "";

  // If true, texture will be wrapped, but mTextureLogWrappingSize is 0 or negative, this will do nothing.
  bool        mIsWrappedTextLog           = false;
  //
  bool*       mParentBoolFlag             = nullptr;
  //
  IDyGuiHasChildren* mParentRawPtr        = nullptr;
};

} /// ::dy::editor namespace

#endif /// MDY_FLAG_IN_EDITOR
#endif /// GUARD_DY_EDITOR_DESCRIPTOR_DIALOG_DESCRIPTOR_H