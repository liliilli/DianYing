#ifndef GUARD_DY_CORE_RESOURCE_INFORMATION_FDYFRAMEBUFFERRESOURCE_H
#define GUARD_DY_CORE_RESOURCE_INFORMATION_FDYFRAMEBUFFERRESOURCE_H
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

namespace dy
{
class FDyFrameBufferInformation;
} /// ::dy namespace

//!
//! Implementation
//!

namespace dy
{
class FDyFrameBufferResource final
{
public:
  FDyFrameBufferResource(_MIN_ const FDyFrameBufferInformation& iInformation);
  ~FDyFrameBufferResource();

  /// @brief Get specifier name of framebuffer resource.
  MDY_NODISCARD const std::string& GetSpecifierName() const noexcept { return this->mSpecifierName; }

  /// @brief Get framebuffer id.
  MDY_NODISCARD TU32 GetFrameBufferId() const noexcept { return this->mFrameBufferId; }

private:
  std::string mSpecifierName = MDY_INITIALIZE_EMPTYSTR;
  TU32        mFrameBufferId = MDY_INITIALIZE_DEFUINT;
};

} /// ::dy namespace

#endif /// GUARD_DY_CORE_RESOURCE_INFORMATION_FDYFRAMEBUFFERRESOURCE_H
