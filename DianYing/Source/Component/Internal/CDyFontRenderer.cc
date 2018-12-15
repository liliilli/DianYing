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
#include <Dy/Component/Internal/CDyFontRenderer.h>

#include <glm/gtc/matrix_transform.hpp>

#include <Dy/Builtin/ShaderGl/Font/RenderFontArraySDF.h>
#include <Dy/Element/Canvas/Text.h>
#include <Dy/Management/SettingManager.h>
#include <Dy/Core/Resource/Resource/FDyShaderResource.h>
#include <Dy/Helper/Type/Matrix4.h>

//!
//! Forward declaration & Local translation unit function data.
//!

namespace
{

/// Sample UI projection code
dy::DDyMatrix4x4 uUiProjMatrix = dy::DDyMatrix4x4{};

constexpr auto stride = sizeof(dy::DDyVector2) * 2;
GLuint mTextSampleVao = MDY_INITIALIZE_DEFUINT;
GLuint mTestVbo = MDY_INITIALIZE_DEFUINT;

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
MDY_NODISCARD std::array<dy::DDyVector2, 8>
GetCharacterVertices(_MIN_ const dy::DDyFontCharacterInfo& ch_info, _MIN_ const dy::DDyVector2& position, _MIN_ const TI32 fontSize)
{
  const TF32 scale        = fontSize / 64.0f;
  const auto charPos      = position - (ch_info.mTranslate * scale);
  const auto l = charPos.X;
  const auto r = charPos.X + (32 * fontSize / ch_info.mScale.X);
  const auto t = charPos.Y + (32 * fontSize / ch_info.mScale.Y);
  const auto b = charPos.Y;

  const auto& texLd = ch_info.mTexCoordInfo.mLeftDown;
  const auto& texRu = ch_info.mTexCoordInfo.mRightUp;

  return {
    dy::DDyVector2{r, b}, dy::DDyVector2{texRu.X, texLd.Y},
    dy::DDyVector2{r, t}, dy::DDyVector2{texRu.X, texRu.Y},
    dy::DDyVector2{l, t}, dy::DDyVector2{texLd.X, texRu.Y},
    dy::DDyVector2{l, b}, dy::DDyVector2{texLd.X, texLd.Y} };
}

///
/// @brief Actual render method.
/// This method must be called in Render__Side() method.
///
/// @param[in] vertices
///
void RenderFontCharacter(_MIN_ const std::array<dy::DDyVector2, 8>& vertices) {
	// Update content of VBO
  static constexpr TU32 size = sizeof(dy::DDyVector2) * 8;

	glBindBuffer    (GL_ARRAY_BUFFER, mTestVbo);
  glBufferSubData (GL_ARRAY_BUFFER, 0, size, &vertices[0].X);
	glBindBuffer    (GL_ARRAY_BUFFER, 0);

	// Render
  glDrawArrays    (GL_TRIANGLE_FAN, 0, 4);
}

} /// ::unnamed namespace

//!
//! Implementation
//!

namespace dy
{

EDySuccess CDyFontRenderer::Initialize(const PDyFontRendererCtorInformation& descriptor)
{
  static auto SetTemporalFontArrayBuffer = [&]
  {
    glGenVertexArrays(1, &mTextSampleVao);
    glGenBuffers(1, &mTestVbo);

    glBindVertexArray(mTextSampleVao);
    glBindBuffer(GL_ARRAY_BUFFER, mTestVbo);

    std::array<DDyVector2, 8> value =
    {
      DDyVector2{}, DDyVector2{1, 1}, DDyVector2{}, DDyVector2{1, 0},
      DDyVector2{}, DDyVector2{0, 0}, DDyVector2{}, DDyVector2{0, 1}
    };
    glBufferData(GL_ARRAY_BUFFER, stride * 4, value.data(), GL_DYNAMIC_DRAW);

    glBindVertexBuffer(0, mTestVbo, 0, stride);

    glEnableVertexAttribArray(0);
    glVertexAttribFormat(0, 2, GL_FLOAT, GL_FALSE, 0);
    glVertexAttribBinding(0, 0);

    glEnableVertexAttribArray(1);
    glVertexAttribFormat(1, 2, GL_FLOAT, GL_FALSE, 8);
    glVertexAttribBinding(1, 0);

    glBindVertexArray(0);
  };

  MDY_ASSERT(MDY_CHECK_ISNOTNULL(descriptor.mFontComponentPtr), "descriptor.mFontComponentPtr must not be null.");
  this->mFontObjectRawPtr = descriptor.mFontComponentPtr;
  this->mBinderShader.TryRequireResource(MSVSTR(builtin::FDyBuiltinShaderGLRenderFontArraySDF::sName));

  // @TODO SAMPLE CODE (TEMPORAL)
  auto& settingManager            = MDySetting::GetInstance();
  const auto overallScreenWidth   = settingManager.GetWindowSizeWidth();
  const auto overallScreenHeight  = settingManager.GetWindowSizeHeight();
  uUiProjMatrix = glm::ortho(0.f, static_cast<float>(overallScreenWidth), 0.f, static_cast<float>(overallScreenHeight), 0.2f, 10.0f);

  SetTemporalFontArrayBuffer();
  return DY_SUCCESS;
}

void CDyFontRenderer::Release()
{
  this->mFontObjectRawPtr = nullptr;
}

void CDyFontRenderer::Render()
{
  if (this->mBinderShader.IsResourceExist() == false) { return; }

  glDepthFunc(GL_ALWAYS);
  this->mBinderShader->UseShader();
  glBindVertexArray(mTextSampleVao);

  const TU32 shaderProgramId  = this->mBinderShader->GetShaderProgramId();
  const auto projectMatrixId  = glGetUniformLocation(shaderProgramId, "uUiProjMatrix");
  glUniformMatrix4fv(projectMatrixId, 1, GL_FALSE, &uUiProjMatrix[0].X);

  IDyFontContainer& container = this->mFontObjectRawPtr->GetFontContainer();
  const TI32 fontSize         = this->mFontObjectRawPtr->GetFontSize();
  const DDyVector2 initPos    = this->mFontObjectRawPtr->GetRenderPosition();

  const auto fgColorId        = glGetUniformLocation(shaderProgramId, "uFgColor");
  glUniform4fv(fgColorId, 1, &this->mFontObjectRawPtr->GetForegroundColor().R);
  const auto bgColorId        = glGetUniformLocation(shaderProgramId, "uBgColor");
  glUniform4fv(bgColorId, 1, &this->mFontObjectRawPtr->GetBackgroundColor().R);
  const auto edgeColorId      = glGetUniformLocation(shaderProgramId, "uEdgeColor");
  glUniform4fv(edgeColorId, 1, &this->mFontObjectRawPtr->GetEdgeColor().R);

  const auto uIsUsingEdgeId   = glGetUniformLocation(shaderProgramId, "uIsUsingEdge");
  glUniform1i(uIsUsingEdgeId, this->mFontObjectRawPtr->CheckIsUsingEdgeRendering());
  const auto uIsUsingBackgroundId = glGetUniformLocation(shaderProgramId, "uIsUsingBackground");
  glUniform1i(uIsUsingBackgroundId, this->mFontObjectRawPtr->CheckIsUsingBackgroundColor());

  //!
  //! Temporal code for font rendering!
  //!

  // RENDER!!
  DDyVector2 renderPosition = initPos;
  for (const TC16& ucs2Char : this->mFontObjectRawPtr->GetText()) {
    // Line feed
    if (ucs2Char == '\n')
    {
      renderPosition.X  = initPos.X;
      //renderPosition.Y -= static_cast<TF32>(container.GetLinefeedHeight(fontSize) * 0.5f);
      renderPosition.Y -= 64;
      continue;
    }
    if (container.IsCharacterGlyphExist(ucs2Char) == false) { continue; }

    // Render texture glyph
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D_ARRAY, container.GetFontTextureArrayId());

    const auto& charInfo  = container[ucs2Char];
    const auto uChannel   = glGetUniformLocation(shaderProgramId, "uChannel");
    glUniform1i(uChannel, charInfo.mTexCoordInfo.mChannel);
    const auto uMapIndex  = glGetUniformLocation(shaderProgramId, "uMapIndex");
    glUniform1i(uMapIndex, charInfo.mTexCoordInfo.mMapIndex);

    RenderFontCharacter(GetCharacterVertices(charInfo, renderPosition, fontSize));
    renderPosition.X += static_cast<TI32>(charInfo.mHorizontalAdvance * fontSize / 2);
  }

  glBindVertexArray(0);
  this->mBinderShader->DisuseShader();
  glDepthFunc(GL_LEQUAL);
}

} /// ::dy namespace