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
#include <Dy/Element/Canvas/Text.h>
#include <Dy/Helper/Type/DyString.h>
#include <Dy/Helper/Type/Matrix4.h>
#include <Dy/Management/SettingManager.h>
#include <Dy/Management/Internal/FramebufferManager.h>
#include <Dy/Management/HeapResourceManager.h>
#include <Dy/Meta/Descriptor/WidgetComponentDescriptor.h>
#include <Dy/Builtin/ShaderGl/RenderFontSDF.h>

//!
//! Local translation unit data
//!

namespace
{

dy::CDyShaderResource* sSampleShaderPtr = nullptr;

/// Sample UI projection code
dy::DDyMatrix4x4 uUiProjMatrix = dy::DDyMatrix4x4{};

constexpr auto stride = sizeof(dy::DDyVector2) * 2;
dy::PDyMetaWidgetTextDescriptor desc{};
std::unique_ptr<dy::FDyText> textComponent = nullptr;

GLuint mTextSampleVao = MDY_INITIALIZE_DEFUINT;
GLuint mTextSampleVbo = MDY_INITIALIZE_DEFUINT;

///
/// @brief The method gets character quad vertices to be needed for rendering.
///
/// @param[in] ch_info Specific character glyph information.
/// @param[in] position The position that character which will be rendered.
/// @param[in] fontSize Scale value to magnify or minify character render size.
///
/// @return Character glyph render vertices information.
/// @see https://www.freetype.org/freetype2/docs/tutorial/step2.html
///
MDY_NODISCARD std::array<dy::DDyVector2, 4>
GetCharacterVertices(_MIN_ const dy::DDyFontCharacter& ch_info, _MIN_ const dy::DDyVector2& position, _MIN_ const TI32 fontSize)
{
  const TF32 scale        = fontSize / 64.0f;
  const auto charPos      = position - (ch_info.mTranslate * scale);
  const auto l = charPos.X;
  const auto r = charPos.X + (32 * fontSize / ch_info.mGlyphScale.X);
  const auto t = charPos.Y + (32 * fontSize / ch_info.mGlyphScale.Y);
  const auto b = charPos.Y;

  return { dy::DDyVector2{r, b}, dy::DDyVector2{r, t}, dy::DDyVector2{l, t}, dy::DDyVector2{l, b} };
}

///
/// @brief Actual render method.
/// This method must be called in Render__Side() method.
///
/// @param[in] charInfo
/// @param[in] vertices
///
void Render(_MIN_ const dy::DDyFontCharacter& charInfo, _MIN_ const std::array<dy::DDyVector2, 4>& vertices) {
	// Render texture glyph
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, charInfo.mTextureId);

	// Update content of VBO
	glBindBuffer    (GL_ARRAY_BUFFER, mTextSampleVbo);
	glBufferSubData (GL_ARRAY_BUFFER, stride * 0, 8, &vertices[0].X);
	glBufferSubData (GL_ARRAY_BUFFER, stride * 1, 8, &vertices[1].X);
	glBufferSubData (GL_ARRAY_BUFFER, stride * 2, 8, &vertices[2].X);
	glBufferSubData (GL_ARRAY_BUFFER, stride * 3, 8, &vertices[3].X);
	glBindBuffer    (GL_ARRAY_BUFFER, 0);

	// Render
  glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
}

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
    builtin::FDyBuiltinShaderGLRenderFontSDF();

    // @TODO SAMPLE CODE (TEMPORAL)
    uUiProjMatrix = glm::ortho(
        -static_cast<float>(overallScreenWidth) / 2,
        static_cast<float>(overallScreenWidth) / 2,
        -static_cast<float>(overallScreenHeight) / 2,
        static_cast<float>(overallScreenHeight) / 2,
        0.2f, 10.0f);
    sSampleShaderPtr = MDyHeapResource::GetInstance().GetShaderResource(MSVSTR(builtin::FDyBuiltinShaderGLRenderFontSDF::sName));

    desc.mComponentType = EDyWidgetComponentType::Text;
    desc.mComponentSpecifierName = "sampleTestText";
    desc.mIsUsingDefaultFont = true;
#ifdef false
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
#endif
    desc.mFontSize = 16;
    desc.mInitialPosition = DDyVectorInt2{-440, 250};
    desc.mInitialColor    = DDyColor::White;
    desc.mIsUsingEdge     = true;
    desc.mEdgeColor       = DDyColor::Black;
    desc.mInitialString = R"dy(Hello world!
21世紀初頭、遺伝子工学技術の進歩により、
タイレル社はロボットに代わるレプリカントと呼ばれる人造人間を発明した。
彼らは優れた体力に、創造した科学者と同等の高い知性を持っていた。)dy";

  };

  static auto SetTemporalFontArrayBuffer = [&] {
    glGenVertexArrays(1, &mTextSampleVao);
    glGenBuffers(1, &mTextSampleVbo);

    glBindVertexArray(mTextSampleVao);
    glBindBuffer(GL_ARRAY_BUFFER, mTextSampleVbo);

    std::array<DDyVector2, 8> value =
    {
      DDyVector2{}, DDyVector2{1, 1},
      DDyVector2{}, DDyVector2{1, 0},
      DDyVector2{}, DDyVector2{0, 0},
      DDyVector2{}, DDyVector2{0, 1}
    };
    glBufferData(GL_ARRAY_BUFFER, stride * 4, value.data(), GL_DYNAMIC_DRAW);

    glBindVertexBuffer(0, mTextSampleVbo, 0, stride);

    glEnableVertexAttribArray(0);
    glVertexAttribFormat(0, 2, GL_FLOAT, GL_FALSE, 0);
    glVertexAttribBinding(0, 0);

    glEnableVertexAttribArray(1);
    glVertexAttribFormat(1, 2, GL_FLOAT, GL_FALSE, 8);
    glVertexAttribBinding(1, 0);

    glBindVertexArray(0);
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
  SetTemporalFontArrayBuffer();
  CreateUIGeneralFramebuffer();
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
  glDepthFunc(GL_ALWAYS);
  sSampleShaderPtr->UseShader();
  glBindVertexArray(mTextSampleVao);

  const auto shaderid     = glGetUniformLocation(sSampleShaderPtr->GetShaderProgramId(), "uUiProjMatrix");
  glUniformMatrix4fv(shaderid, 1, GL_FALSE, &uUiProjMatrix[0].X);
  //glUniform4fv(glGetUniformLocation(sSampleShaderPtr->GetShaderProgramId(), "uTextColor"), 1, &uTextColor.R);
  //const auto pxRangeId    = glGetUniformLocation(sSampleShaderPtr->GetShaderProgramId(), "pxRange");
  //glUniform1f(pxRangeId, 4.0f);

  FDyFontContainer& container = textComponent->GetFontContainer();
  const TI32 fontSize         = textComponent->GetFontSize();
  const DDyVector2 initPos    = textComponent->GetRenderPosition();
  const TU32 shaderProgramId  = sSampleShaderPtr->GetShaderProgramId();

  const auto fgColorId        = glGetUniformLocation(shaderProgramId, "uFgColor");
  glUniform4fv(fgColorId, 1, &textComponent->GetForegroundColor().R);
  const auto bgColorId        = glGetUniformLocation(shaderProgramId, "uBgColor");
  glUniform4fv(bgColorId, 1, &textComponent->GetBackgroundColor().R);
  const auto edgeColorId      = glGetUniformLocation(shaderProgramId, "uEdgeColor");
  glUniform4fv(edgeColorId, 1, &textComponent->GetEdgeColor().R);

  const auto uIsUsingEdgeId   = glGetUniformLocation(shaderProgramId, "uIsUsingEdge");
  glUniform1i(uIsUsingEdgeId, textComponent->CheckIsUsingEdgeRendering());
  const auto uIsUsingBackgroundId = glGetUniformLocation(shaderProgramId, "uIsUsingBackground");
  glUniform1i(uIsUsingBackgroundId, textComponent->CheckIsUsingBackgroundColor());

  //!
  //! Temporal code for font rendering!
  //!

  // RENDER!!
  DDyVector2 renderPosition   = initPos;
  for (const TC16& ucs2Char : textComponent->GetText()) {
    // Line feed
    if (ucs2Char == '\n')
    {
      renderPosition.X  = initPos.X;
      renderPosition.Y -= static_cast<TF32>(container.GetScaledLinefeedHeight(fontSize) * 0.5f);
      continue;
    }
    // If character on font is not exist yet, generate new one.
    if (container.IsFontGlyphExist(ucs2Char) == false) { MDY_CALL_ASSERT_SUCCESS(container.CreateFontGlyph(ucs2Char)); }
    const auto& charInfo = container[ucs2Char];
    Render(charInfo, GetCharacterVertices(charInfo, renderPosition, fontSize));
    renderPosition.X += static_cast<TI32>(charInfo.mAdvance * fontSize / 2);
  }

  glBindVertexArray(0);
  sSampleShaderPtr->UnuseShader();
  glDepthFunc(GL_LEQUAL);
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