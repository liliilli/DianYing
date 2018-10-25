#ifndef GUARD_DY_MANAGEMENT_TYPE_FRAMEBUFFERINFORMATION_H
#define GUARD_DY_MANAGEMENT_TYPE_FRAMEBUFFERINFORMATION_H
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

#include <Dy/Helper/Type/VectorInt2.h>
#include <Dy/Management/Type/AttachmentInformation.h>

namespace dy
{

///
/// @struct PDyGlFrameBufferInformation
/// @brief  Framebuffer information type.
///
struct PDyGlFrameBufferInformation final
{
  std::string                                   mFrameBufferName  = MDY_INITILAIZE_EMPTYSTR;
  std::vector<PDyGlAttachmentBinderInformation> mAttachmentList   = {};
  DDyVectorInt2                                 mFrameBufferSize  = {};
  bool                                          mIsUsingDefaultDepthBuffer  = false;
  bool                                          mIsNotUsingPixelShader      = false;

  ///
  /// @brief  Get framebuffer id. If not initialized yet, just return 0.
  /// @return Framebuffer id.
  ///
  FORCEINLINE MDY_NODISCARD TU32 GetFramebufferId() const noexcept
  {
    return this->mFrameBufferId;
  }

private:
  ///
  TU32 mFrameBufferId         = MDY_INITIALIZE_DEFUINT;
  ///
  TU32 mDefaultDepthBufferId  = MDY_INITIALIZE_DEFUINT;

  friend class MDyFramebuffer;
};

} /// ::dy namespace

#endif /// GUARD_DY_MANAGEMENT_TYPE_FRAMEBUFFERINFORMATION_H