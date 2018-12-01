#include <precompiled.h>
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

/// Header file
#include <Dy/Core/Rendering/UIBasicRenderer.h>

#include <Dy/Builtin/ShaderGl/RenderDefaultFont.h>
#include <Dy/Core/Component/Resource/ShaderResource.h>
#include <Dy/Core/Rendering/Helper/FrameAttachmentString.h>
#include <Dy/Element/Canvas/Text.h>
#include <Dy/Meta/Descriptor/WidgetTextMetaInformation.h>
#include <Dy/Management/SettingManager.h>
#include <Dy/Management/Rendering/FramebufferManager.h>
#include <Dy/Management/IO/IOResourceManager.h>
#include <Dy/Management/FontManager.h>
#include <Dy/Management/WindowManager.h>

#include <Dy/Element/Canvas/Widget.h>
#include "Dy/Management/IO/MetaInfoManager.h"
#include "Dy/Builtin/ShaderGl/RenderFontArraySDF.h"

//!
//! Local translation unit data
//!

namespace
{

dy::CDyShaderResource* sSampleShaderPtr = nullptr;

dy::PDyMetaWidgetTextDescriptor desc{};
std::unique_ptr<dy::FDyUiWidget> testWidget = nullptr;

} /// ::unnamed namespace

//!
//! Implementation
//!

namespace dy
{

FDyUIBasicRenderer::FDyUIBasicRenderer()
{
  auto& settingManager      = MDySetting::GetInstance();
  auto& framebufferManager  = MDyFramebuffer::GetInstance();
  const auto overallScreenWidth   = settingManager.GetWindowSizeWidth();
  const auto overallScreenHeight  = settingManager.GetWindowSizeHeight();
  const auto overallSize = DDyVectorInt2{overallScreenWidth, overallScreenHeight};

  //!
  //! Lambda function
  //!
#ifdef false
  static auto SetTemporaryInitialSetting = [&] {
    desc.mInitialString = R"dy(
[Task]        A     B     C   DRAW  TOTAL
Process : 1 0.123 0.456 0.789 0.123 0.123
ChrPre  : 1 0.123 0.456 0.789 0.456 0.123
Char    : 0 0.123 0.456 0.789 0.789 0.123
Post    : 2
BG      : 1
Canvas0 : 1
Canvas1 : 0
Camera0 : 2
)dy";
  };
#endif

  static auto CreateUIGeneralFramebuffer = [&] {
    PDyGlFrameBufferInformation       framebufferInfo = {};
    PDyGlAttachmentInformation        attachmentInfo = {};
    PDyGlAttachmentBinderInformation  binderInfo = {};

    framebufferInfo.mFrameBufferName = sFrameBuffer_UiBasic;
    framebufferInfo.mFrameBufferSize = overallSize;
    framebufferInfo.mIsUsingDefaultDepthBuffer = true;

    // Rendered texture buffer
    attachmentInfo.mAttachmentName = sAttachment_Output;
    attachmentInfo.mAttachmentSize = overallSize;
    attachmentInfo.mParameterList = {
      PDyGlTexParameterInformation\
      {EDyGlParameterName::TextureMinFilter, EDyGlParameterValue::Nearest},
      {EDyGlParameterName::TextureMagFilter, EDyGlParameterValue::Nearest},
      {EDyGlParameterName::TextureWrappingS, EDyGlParameterValue::ClampToBorder},
      {EDyGlParameterName::TextureWrappingT, EDyGlParameterValue::ClampToBorder},
    };
    attachmentInfo.mBorderColor = DDyColorRGBA{ 0, 0, 0, 0 };

    binderInfo.mAttachmentName = sAttachment_Output;
    binderInfo.mAttachmentType = EDyGlAttachmentType::Color0;
    framebufferInfo.mAttachmentList.push_back(binderInfo);

    // Push attachment buffer
    MDY_CALL_ASSERT_SUCCESS(framebufferManager.SetAttachmentInformation(attachmentInfo));

    // Create framebuffer.
    MDY_CALL_ASSERT_SUCCESS(framebufferManager.InitializeNewFrameBuffer(framebufferInfo));
    this->mDyBtFbUiBasic = framebufferManager.GetFrameBufferPointer(MSVSTR(sFrameBuffer_UiBasic));
    MDY_ASSERT(MDY_CHECK_ISNOTNULL(this->mDyBtFbUiBasic), "Unexpected error.");
  };

  //!
  //! FUNCTIONBODY∨
  //!

  CreateUIGeneralFramebuffer();

  // @TODO TEMPORAL
  MDY_CALL_ASSERT_SUCCESS(MDyFont::GetInstance().CreateFontResourceContainer("Arial"));
}

FDyUIBasicRenderer::~FDyUIBasicRenderer()
{
  // Remove framebuffer & attachment resource
  auto& framebufferManager  = MDyFramebuffer::GetInstance();
  MDY_CALL_ASSERT_SUCCESS(framebufferManager.RemoveFrameBuffer(MSVSTR(sFrameBuffer_UiBasic)));
  // Null-lize
  this->mDyBtFbUiBasic = nullptr;
}

void FDyUIBasicRenderer::RenderScreen()
{
  static bool f = false;
  if (f == false)
  {
    testWidget = std::make_unique<FDyUiWidget>();
    MDY_CALL_ASSERT_SUCCESS(testWidget->Initialize(MDyMetaInfo::GetInstance().GetWidgetMetaInformation("DebugUi")));
    f = !f;
  }

  glBindFramebuffer(GL_FRAMEBUFFER, this->mDyBtFbUiBasic->GetFramebufferId());
  testWidget->Render();
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FDyUIBasicRenderer::Clear()
{ // Integrity test
  if (MDY_CHECK_ISNULL(this->mDyBtFbUiBasic)) { return; }

  // Reset overall deferred framebuffer setting
  glBindFramebuffer(GL_FRAMEBUFFER, this->mDyBtFbUiBasic->GetFramebufferId());
  glClearColor(0, 0, 0, 0);
  glClear(GL_COLOR_BUFFER_BIT);
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

} /// ::dy namespace