#ifndef GUARD_DY_CORE_RESOURCE_INFORMATION_FDYFRAMEBUFFERINFORMATION_H
#define GUARD_DY_CORE_RESOURCE_INFORMATION_FDYFRAMEBUFFERINFORMATION_H
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
struct PDyGlFrameBufferInstanceMetaInfo;
} /// ::dy namespace

//!
//! Implementation
//!

namespace dy
{

///
/// @class FDyFrameBufferInformation
/// @brief FrameBuffer information resource.
///
class FDyFrameBufferInformation final
{
public:
  MDY_ONLY_MOVEABLE_PROPERTIES_DEFAULT(FDyFrameBufferInformation);

  /// @brief Construct framebuffer information.
  FDyFrameBufferInformation(_MIN_ const PDyGlFrameBufferInstanceMetaInfo& metaInfo);
  ~FDyFrameBufferInformation() = default;

  /// @brief Get attachment's specifier name.
  FORCEINLINE MDY_NODISCARD const std::string& GetSpecifierName() const noexcept { return this->mSpecifierName; }

private:
  std::string mSpecifierName = MDY_INITIALIZE_EMPTYSTR;
};

} /// ::dy namespace

#endif /// GUARD_DY_CORE_RESOURCE_INFORMATION_FDYFRAMEBUFFERINFORMATION_H
