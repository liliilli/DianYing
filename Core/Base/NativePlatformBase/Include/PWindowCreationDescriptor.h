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

#include <string>

namespace dy
{

/// @struct PWindowCreationDescriptor
/// @brief Descriptor for creating window.
struct PWindowCreationDescriptor final
{
  /// @brief Created window is resizable.
  /// This feature will be not supported by OS.
  bool mIsWindowResizable   = false;
  /// @brief Created window is visible or not.
  /// This feature will be not supported by OS.
  bool mIsWindowVisible     = false;
  /// @brief Created window is full-screended. 
  /// This feature will be not supported by OS.
  bool mIsWindowFullScreen  = false; /* NOT SUPPORTED YET */
  /// @brief Created window must be focused when created.
  /// This feature will be not supported by OS.
  bool mIsWindowShouldFocus = false;

  /// @brief Window horizontal size.
  /// This value must be bigger than 0.
  unsigned mWindowWidth = 0;
  /// @brief Window vertical size.
  /// This value must be bigger than 0.
  unsigned mWindowHeight = 0;
  
  /// @brief Window titlebar name. (and key)
  std::string mWindowName;
};

} /// ::dy namespace