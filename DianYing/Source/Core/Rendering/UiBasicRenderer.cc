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

#include <glm/gtc/matrix_transform.hpp>

#include <Dy/Builtin/ShaderGl/RenderDefaultFont.h>
#include <Dy/Core/Component/Resource/ShaderResource.h>
#include <Dy/Core/Rendering/Helper/FrameAttachmentString.h>
#include <Dy/Element/Canvas/Text_Deprecated.h>
#include <Dy/Helper/Type/DyString.h>
#include <Dy/Helper/Type/Matrix4.h>
#include <Dy/Management/SettingManager.h>
#include <Dy/Management/Rendering/FramebufferManager.h>
#include <Dy/Management/IO/IOResourceManager.h>
#include <Dy/Meta/Descriptor/WidgetComponentDescriptor.h>
#include <Dy/Builtin/ShaderGl/RenderFontSDF_Deprecated.h>
#include <Dy/Management/FontManager.h>
#include <Dy/Builtin/ShaderGl/RenderFontArraySDF.h>
#include <Dy/Element/Canvas/Text.h>
#include "Dy/Management/WindowManager.h"

//!
//! Local translation unit data
//!

namespace
{

dy::CDyShaderResource* sSampleShaderPtr = nullptr;

dy::PDyMetaWidgetTextDescriptor desc{};
std::unique_ptr<dy::FDyText> textComponent = nullptr;

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

  static auto SetTemporaryInitialSetting = [&] {
    builtin::FDyBuiltinShaderGLRenderFontArraySDF();

    desc.mComponentType = EDyWidgetComponentType::Text;
    desc.mComponentSpecifierName = "sampleTestText";
    desc.mIsUsingDefaultFont = true;
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
    desc.mFontSize = 12;
    desc.mInitialPosition = DDyVectorInt2{-440, 250};
    desc.mInitialColor    = DDyColor::White;
    desc.mFontSpecifierName = "Arial";
    desc.mIsUsingEdge     = true;
    desc.mEdgeColor       = DDyColor::Black;
#ifdef false
    desc.mInitialString = R"dy(Hello world!
21世紀初頭、遺伝子工学技術の進歩により、
タイレル社はロボットに代わるレプリカントと呼ばれる人造人間を発明した。
彼らは優れた体力に、創造した科学者と同等の高い知性を持っていた。)dy";
#endif
  };

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
    attachmentInfo.mBorderColor = DDyColor{ 0, 0, 0, 0 };

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

  SetTemporaryInitialSetting();
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
    textComponent = std::make_unique<FDyText>();
    MDY_CALL_ASSERT_SUCCESS(textComponent->Initialize(desc));
    f = !f;
  }

  glBindFramebuffer(GL_FRAMEBUFFER, this->mDyBtFbUiBasic->GetFramebufferId());
  textComponent->SetText(std::to_string(MDyWindow::GetInstance().GetCpuUsage()));
  textComponent->Render();
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