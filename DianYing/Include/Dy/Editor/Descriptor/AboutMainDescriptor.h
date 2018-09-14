#ifndef GUARD_DY_EDITOR_DESCRIPTOR_ABOUT_MAIN_DESCRIPTOR_H
#define GUARD_DY_EDITOR_DESCRIPTOR_ABOUT_MAIN_DESCRIPTOR_H
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

//!
//! Forward declaration
//!

namespace dy::editor
{
class IDyGuiHasChildren;
} /// ::dy::editor namespace

//!
//! Implementation
//!

namespace dy::editor
{

///
/// @struct PDyGuiAboutMainDescriptor
/// @brief dialog construction descriptor for gui dialog.
///
struct PDyGuiAboutMainDescriptor final
{
  //
  bool*               mParentBoolFlag = nullptr;
  //
  IDyGuiHasChildren*  mParentRawPtr   = nullptr;
};

} /// ::dy::editor namespace

#endif /// MDY_FLAG_IN_EDITOR
#endif /// GUARD_DY_EDITOR_DESCRIPTOR_ABOUT_MAIN_DESCRIPTOR_H