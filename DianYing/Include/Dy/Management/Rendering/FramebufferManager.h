#ifndef GUARD_DY_MANAGEMENT_INTERNAL_FRAMEBUFFERMANAGER_H
#define GUARD_DY_MANAGEMENT_INTERNAL_FRAMEBUFFERMANAGER_H
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

#include <unordered_map>
#include <Dy/Management/Interface/ISingletonCrtp.h>
#include <Dy/Management/Type/AttachmentInformation.h>
#include <Dy/Management/Type/FramebufferInformation.h>

namespace dy
{

///
/// @class MDyFramebuffer
/// @brief Management instance of customized framebuffer.
///
class MDyFramebuffer final : public IDySingleton<MDyFramebuffer>
{
  MDY_SINGLETON_PROPERTIES(MDyFramebuffer);
  MDY_SINGLETON_DERIVED(MDyFramebuffer);
public:
  ///
  /// @brief OpenGl default framebuffer index.
  ///
  inline static constexpr TI32 sGlDefaultFramebufferId = 0;

  ///
  /// @brief
  /// @param  attachmentInfo
  /// @return
  ///
  MDY_NODISCARD EDySuccess SetAttachmentInformation(_MIN_ const PDyGlAttachmentInformation& attachmentInfo);

  ///
  /// @brief
  /// @param  bufferInfo
  /// @return
  ///
  MDY_NODISCARD EDySuccess InitializeNewFrameBuffer(_MIN_ const PDyGlFrameBufferInformation& bufferInfo);

  ///
  /// @brief
  /// @param  framebufferName
  /// @return
  ///
  MDY_NODISCARD EDySuccess RemoveFrameBuffer(_MIN_ const std::string& framebufferName);

  ///
  /// @brief
  /// @param  framebufferName
  /// @return
  ///
  MDY_NODISCARD PDyGlFrameBufferInformation* GetFrameBufferPointer(_MIN_ const std::string& framebufferName);

  ///
  /// @brief
  /// @param  attachmentName
  /// @return
  ///
  MDY_NODISCARD PDyGlAttachmentInformation* GetAttachmentPointer(_MIN_ const std::string& attachmentName);
  MDY_NODISCARD PDyGlAttachmentInformation* GetAttachmentPointer(_MIN_ const std::string_view& attachmentName);

private:
  /// Valid framebuffer container
  std::unordered_map<std::string, std::unique_ptr<PDyGlFrameBufferInformation>>  mValidFramebufferList = {};
  /// Valid attachment container
  std::unordered_map<std::string, std::unique_ptr<PDyGlAttachmentInformation>>   mValidAttachmentList  = {};


};

} /// ::dy namespace

#endif /// GUARD_DY_MANAGEMENT_INTERNAL_FRAMEBUFFERMANAGER_H