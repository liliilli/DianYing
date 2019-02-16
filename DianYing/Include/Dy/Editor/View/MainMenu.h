#ifndef GUARD_DY_GUI_MAIN_MENU_H
#define GUARD_DY_GUI_MAIN_MENU_H
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

#include <Dy/Helper/Pointer.h>

//!
//! Forward declaration
//!

namespace dy
{
class FDyBtGiDebugStatus;
}

//!
//! Implementation
//!

namespace dy::editor
{

class FDyEditor_MainMenu final
{
public:
  FDyEditor_MainMenu();
  void Draw(_MIN_ TF32 dt) noexcept;

private:
  FDyBtGiDebugStatus* mPtrGlobalInstance = nullptr;
};

} /// ::dy::editor namespace

#endif /// GUARD_DY_GUI_MAIN_MENU_H