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

#include <ALowInput.h>

namespace dy
{

/// @class FWindowsLowInput
/// @brief Windows OS (WIN32) low-input level management type.
class FWindowsLowInput final : public base::ALowInput
{
public:
  FWindowsLowInput();
  virtual ~FWindowsLowInput() = default;

private:
  /// @brief Implementation function.
  void UpdateKeyboard(void* descriptor) override final;

  /// @brief Implementation function.
  void UpdateMouseButton(void* descriptor) override final;

  /// @brief Implementated function.
  void UpdateMousePos(void* descriptor) override;
};

} /// ::dy namespace