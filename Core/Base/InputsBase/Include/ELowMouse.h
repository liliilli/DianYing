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

/// @enum ELowMousePosState
/// @brief Low-level mouse position state binding enum value.
enum class [[nodiscard]] ELowMousePosState
{
  Normal,   // Mouse position binding range will be set with screen size.
  Unlimited,// Mouse position binding range will be unlimited but cursor not move from center.
  Off       // Mouse position binding feature will be off.
};



} /// ::dy::base namespace