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

#include <cstdint>
#include <EPlatform.h>
#include <AHandlesBase.h>

namespace dy
{

/// @class APlatformBase
/// @brief Platform base type. 
/// This manages platform dependent instance and do bootstrapping of DyEngine.
class APlatformBase
{
public:
  APlatformBase(EXPR_E(EPlatform) platform);
  virtual ~APlatformBase() = 0; 

  virtual void SetWindowTitle(const std::string& newTitle) = 0;

  virtual std::string GetWindowTitle() const = 0;

  virtual uint32_t GetWindowHeight() const = 0;

  virtual uint32_t GetWindowWidth() const = 0;

  virtual void ResizeWindow(uint32_t width, uint32_t height) = 0;

  EXPR_E(EPlatform) GetPlatformType() const noexcept;

protected:
  std::unique_ptr<AHandlesBase> mHandle = nullptr;

private:
  EXPR_E(EPlatform) mPlatform;
};

} /// ::dy namespace