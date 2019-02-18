#include <precompiled.h>
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

/// Header file
#include <Dy/Component/Internal/CDyFontRenderer.h>

#include <Dy/Builtin/ShaderGl/Font/RenderFontArraySDF.h>
#include <Dy/Builtin/Mesh/FDyBtMsUiFontQuad.h>
#include <Dy/Core/Resource/Resource/FDyShaderResource.h>
#include <Dy/Core/Resource/Resource/FDyMeshResource.h>
#include <Dy/Element/Canvas/Text.h>
#include <Dy/Management/Rendering/RenderingManager.h>
#include "Dy/Core/Rendering/Wrapper/FDyGLWrapper.h"

//!
//! Forward declaration & Local translation unit function data.
//!

namespace
{

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

MDY_NODISCARD std::vector<std::array<dy::DDyVector2, 8>> 
GetCharacterVertices(
    dy::IDyFontContainer& container, 
    const std::vector<std::pair<TC16, dy::DDyVector2>>& lineList, 
    TI32 fontSize)
{
   std::vector<std::array<dy::DDyVector2, 8>> result;
   result.reserve(lineList.size());
   for (const auto& [charCode, position] : lineList)
   {
     result.emplace_back(GetCharacterVertices(container[charCode], position, fontSize));
   }
   return result;
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
}

void CDyFontRenderer::Render()
{
  using EUniformType = EDyUniformVariableType;
  if (this->mBinderShader.IsResourceExist() == false) { return; }

  const auto& string = this->mPtrWidget->GetText();
  if (string.GetLength() == 0) { return; }
  
  this->mBinderShader->TryUpdateUniform<EUniformType::Matrix4>("uUiProjMatrix", MDyRendering::GetInstance().GetGeneralUiProjectionMatrix());
  this->mBinderShader->TryUpdateUniform<EUniformType::Vector4>("uFgColor", this->mPtrWidget->GetForegroundColor());
  this->mBinderShader->TryUpdateUniform<EUniformType::Vector4>("uBgColor", this->mPtrWidget->GetBackgroundColor());
  this->mBinderShader->TryUpdateUniform<EUniformType::Vector4>("uEdgeColor", this->mPtrWidget->GetEdgeColor());
  this->mBinderShader->TryUpdateUniform<EUniformType::Bool>("uIsUsingEdge", this->mPtrWidget->CheckIsUsingEdgeRendering());
  this->mBinderShader->TryUpdateUniform<EUniformType::Bool>("uIsUsingBackground", this->mPtrWidget->CheckIsUsingBackgroundColor());
  
  IDyFontContainer& container = this->mPtrWidget->GetFontContainer();
  const TI32 fontSize         = this->mPtrWidget->GetFontSize();
  const DDyVector2 initPos    = this->mPtrWidget->GetRenderPosition();
  DDyVector2 renderPosition   = initPos;

  // Char code, x position.
  using TLineCharCodeList = std::vector<std::pair<TC16, DDyVector2>>;
  std::vector<TLineCharCodeList> lineCharCodeList = {};
  
  // Make list.
  TLineCharCodeList lineList;
  for (const TC16& ucs2Char : string)
  {
    if (ucs2Char != '\n')
    { 
      const auto& charInfo = container[ucs2Char];
      if (container.IsCharacterGlyphExist(ucs2Char) == false) { continue; }
      // Insert
      lineList.emplace_back(std::pair(ucs2Char, DDyVector2{renderPosition.X, renderPosition.Y}));
      // Relocate next position.
      renderPosition.X += static_cast<TI32>(charInfo.mHorizontalAdvance * fontSize / 2);
    }
    else
    {
      renderPosition.X = initPos.X; renderPosition.Y -= 24;
      //renderPosition.Y -= static_cast<TF32>(container.GetLinefeedHeight(fontSize) * 0.5f);
      lineCharCodeList.emplace_back(lineList);
      lineList.clear();
    }
  }
  // Final line check...
  if (lineList.empty() == false)
  {
    lineCharCodeList.emplace_back(lineList);
    lineList.clear();
  }

  // Render
  for (const auto& lineInformation : lineCharCodeList)
  {
    // Set uniform (TEMPORARY)
    const auto& charInfo = container[lineInformation.front().first];
    this->mBinderShader->TryUpdateUniform<EUniformType::Integer>("uChannel", charInfo.mTexCoordInfo.mChannel);
    this->mBinderShader->TryUpdateUniform<EUniformType::Integer>("uMapIndex", charInfo.mTexCoordInfo.mMapIndex);

    using TBuffer = std::array<DDyVector2, 8>;
    const std::vector<TBuffer> buffer = GetCharacterVertices(container, lineInformation, fontSize);

    { MDY_GRAPHIC_SET_CRITICALSECITON();
      glDepthFunc(GL_ALWAYS);

      this->mBinderShader->UseShader();
      this->mBinderFontMesh->BindVertexArray();

      this->mBinderShader->TryUpdateUniformList();
      glBindBuffer(GL_ARRAY_BUFFER, this->mBinderFontMesh->GetVertexBufferId());
      glBufferData(GL_ARRAY_BUFFER, buffer.size() * sizeof(TBuffer), buffer.data()->data(), GL_DYNAMIC_DRAW);

      glEnableVertexAttribArray(0);
      glVertexBindingDivisor(0, 1);
      glEnableVertexAttribArray(1);
      glVertexBindingDivisor(1, 1);

#ifdef false
      glBindBuffer(GL_ARRAY_BUFFER, this->mBinderFontMesh->GetVertexBufferId());
      glBindVertexBuffer(0, this->mBinderFontMesh->GetVertexBufferId(), 0, sizeof(DDyVector2) * 2);
      glVertexAttribFormat(0, 2, GL_FLOAT, GL_FALSE, 0);
      VertexAttribDivisor // DO NOT USE IN CASE OF THiS.

      glVertexAttribFormat(1, 2, GL_FLOAT, GL_FALSE, 8);


      //glDrawArrays(GL_TRIANGLE_FAN, 0, 4);  
      glBindVertexArray(0);
      glDepthFunc(GL_LEQUAL);
#endif
      // Render texture glyph
      glActiveTexture(GL_TEXTURE0);
      glBindTexture(GL_TEXTURE_2D_ARRAY, container.GetFontTextureArrayId());
      glDrawArraysInstanced(GL_TRIANGLE_FAN, 0, 4, buffer.size());  
      this->mBinderShader->DisuseShader();
    }
  }
}

} /// ::dy namespace