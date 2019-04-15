#pragma once
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

#include <array>
#include <DInputButton.h>

namespace dy::base
{

/// @enum ELowMouseButton
/// @brief Low-level mouse enum value. (keycode)
/// This enum is inspired from GLFW3 input system.
enum ELowMouseButton
{
  DyMouse__Error = -1,

  DyMouseButton1 = 0, // Left
  DyMouseButton2 = 1, // Right
  DyMouseButton3 = 2, // Middle
  DyMouseButton4 = 3,
  DyMouseButton5 = 4,
  DyMouseButton6 = 5,
  DyMouseButton7 = 6,
  DyMouseButton8 = 7,

  DyMouse__Sum
};

/// @brief Button states management container.
extern std::array<DInputButtonItem, ELowMouseButton::DyMouse__Sum> sLowMouseButtons;

} /// ::dy::base namespace