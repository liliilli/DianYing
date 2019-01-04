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
#include <Dy/Builtin/Mesh/FDyBtMsUiFontQuad.h>
#include <Dy/Core/Resource/Resource/FDyShaderResource.h>
#include <Dy/Element/Canvas/Text.h>
#include <Dy/Helper/Type/Matrix4.h>
#include <Dy/Core/Resource/Resource/FDyMeshResource.h>

//!
//! Forward declaration & Local translation unit function data.
//!

namespace
{

/// Sample UI projection code
dy::DDyMatrix4x4 uUiProjTempMatrix = dy::DDyMatrix4x4{};

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
void RenderFontCharacter(_MIN_ const std::array<dy::DDyVector2, 8>& vertices, _MIN_ TU32 iVboId) {
	// Update content of VBO
  static constexpr TU32 size = sizeof(dy::DDyVector2) * 8;

	glBindBuffer    (GL_ARRAY_BUFFER, iVboId);
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

CDyFontRenderer::CDyFontRenderer(FDyText& iPtrWidget) :
  mPtrWidget{&iPtrWidget}
{
  this->mBinderFontMesh.TryRequireResource(MSVSTR(FDyBtMsUiFontQuad::sName));
  MDY_ASSERT(this->mBinderFontMesh.IsResourceExist() == true, "True");
  this->mBinderShader.TryRequireResource(MSVSTR(builtin::FDyBuiltinShaderGLRenderFontArraySDF::sName));

  uUiProjTempMatrix = glm::ortho(0.f, static_cast<float>(1280), 0.f, static_cast<float>(720), 0.2f, 10.0f);
}

void CDyFontRenderer::Render()
{
  using EUniformType = EDyUniformVariableType;
  if (this->mBinderShader.IsResourceExist() == false) { return; }

  this->mBinderShader->UseShader();
  this->mBinderShader.TryUpdateUniform<EUniformType::Matrix4>("uUiProjMatrix", uUiProjTempMatrix);
  this->mBinderShader.TryUpdateUniform<EUniformType::Vector4>("uFgColor", this->mPtrWidget->GetForegroundColor());
  this->mBinderShader.TryUpdateUniform<EUniformType::Vector4>("uBgColor", this->mPtrWidget->GetBackgroundColor());
  this->mBinderShader.TryUpdateUniform<EUniformType::Vector4>("uEdgeColor", this->mPtrWidget->GetEdgeColor());
  this->mBinderShader.TryUpdateUniform<EUniformType::Integer>("uIsUsingEdge", this->mPtrWidget->CheckIsUsingEdgeRendering());
  this->mBinderShader.TryUpdateUniform<EUniformType::Integer>("uIsUsingBackground", this->mPtrWidget->CheckIsUsingBackgroundColor());
  MDY_CALL_BUT_NOUSE_RESULT(this->mBinderShader.TryUpdateUniformList());

  glDepthFunc(GL_ALWAYS);
  glBindVertexArray(this->mBinderFontMesh->GetVertexArrayId());

  IDyFontContainer& container = this->mPtrWidget->GetFontContainer();
  const TI32 fontSize         = this->mPtrWidget->GetFontSize();
  const DDyVector2 initPos    = this->mPtrWidget->GetRenderPosition();
  DDyVector2 renderPosition   = initPos;

  for (const TC16& ucs2Char : this->mPtrWidget->GetText()) {
    if (ucs2Char == '\n')
    { // Line feed
      renderPosition.X  = initPos.X;
      //renderPosition.Y -= static_cast<TF32>(container.GetLinefeedHeight(fontSize) * 0.5f);
      renderPosition.Y -= 24;
      continue;
    }
    if (container.IsCharacterGlyphExist(ucs2Char) == false) { continue; }

    // Render texture glyph
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D_ARRAY, container.GetFontTextureArrayId());

    const auto& charInfo  = container[ucs2Char];
    this->mBinderShader.TryUpdateUniform<EUniformType::Integer>("uChannel", charInfo.mTexCoordInfo.mChannel);
    this->mBinderShader.TryUpdateUniform<EUniformType::Integer>("uMapInex", charInfo.mTexCoordInfo.mMapIndex);
    MDY_CALL_BUT_NOUSE_RESULT(this->mBinderShader.TryUpdateUniformList());

    RenderFontCharacter(GetCharacterVertices(charInfo, renderPosition, fontSize), this->mBinderFontMesh->GetVertexBufferId());
    renderPosition.X += static_cast<TI32>(charInfo.mHorizontalAdvance * fontSize / 2);
  }

  glBindVertexArray(0);
  this->mBinderShader->DisuseShader();
  glDepthFunc(GL_LEQUAL);
}

} /// ::dy namespace