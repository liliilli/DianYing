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
MDY_NODISCARD std::array<dy::DDyVector2, 12>
GetCharacterVertices(
    _MIN_ const dy::DDyFontCharacterInfo& ch_info, 
    _MIN_ const dy::DDyVector2& position, 
    _MIN_ const TI32 fontSize)
{
  const TF32 scale        = fontSize / 64.0f;
  const auto charPos      = position - (ch_info.mTranslate * scale);
  const auto l = charPos.X;
  const auto r = charPos.X + (32 * fontSize / ch_info.mScale.X);
  const auto t = charPos.Y + (32 * fontSize / ch_info.mScale.Y);
  const auto b = charPos.Y;

  const auto& texLd = ch_info.mTexCoordInfo.mLeftDown;
  const auto& texRu = ch_info.mTexCoordInfo.mRightUp;

  const auto chanMap = dy::DDyVector2{
      static_cast<TF32>(ch_info.mTexCoordInfo.mChannel), 
      static_cast<TF32>(ch_info.mTexCoordInfo.mMapIndex)};

  return {
    dy::DDyVector2{r, b}, dy::DDyVector2{texRu.X, texLd.Y}, chanMap,
    dy::DDyVector2{r, t}, dy::DDyVector2{texRu.X, texRu.Y}, chanMap,
    dy::DDyVector2{l, t}, dy::DDyVector2{texLd.X, texRu.Y}, chanMap,
    dy::DDyVector2{l, b}, dy::DDyVector2{texLd.X, texLd.Y}, chanMap };
}

MDY_NODISCARD std::vector<std::array<dy::DDyVector2, 12>> 
GetCharacterVertices(
    dy::IDyFontContainer& container, 
    const std::vector<std::pair<TC16, dy::DDyVector2>>& lineList, 
    TI32 fontSize)
{
   std::vector<std::array<dy::DDyVector2, 12>> result;
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
  this->mBinderFontMesh.TryRequireResource((FDyBtMsUiFontQuad::sName));
  MDY_ASSERT(this->mBinderFontMesh.IsResourceExist() == true, "True");
  this->mBinderShader.TryRequireResource((builtin::FDyBuiltinShaderGLRenderFontArraySDF::sName));
}

#ifdef false
    glVertexBindingDivisor(0, 1);
    // https://forums.khronos.org/showthread.php/81978
    // https://gamedev.stackexchange.com/questions/104239/instanced-rendering-with-arb-vertex-attrib-binding
    // https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glVertexBindingDivisor.xhtml
    // https://stackoverflow.com/questions/50650457/what-is-the-difference-between-glvertexattribdivisor-and-glvertexbindingdivisor
    glEnableVertexAttribArray(0);
    glVertexBindingDivisor(0, 1);
    glEnableVertexAttribArray(1);
    glVertexBindingDivisor(1, 1);
    glEnableVertexAttribArray(2);
    glVertexBindingDivisor(2, 1);

    glBindBuffer(GL_ARRAY_BUFFER, this->mBinderFontMesh->GetVertexBufferId());
    glBindVertexBuffer(0, this->mBinderFontMesh->GetVertexBufferId(), 0, sizeof(DDyVector2) * 2);
    VertexAttribDivisor // DO NOT USE IN CASE OF THiS.
#endif
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

  using TLineCharCodeList = std::vector<std::pair<TC16, DDyVector2>>;
  std::vector<TLineCharCodeList>  charCodeList{};
  std::vector<TI32>               lineActualWidthList{};

  { // Make char-code list with calculating lineActualWidth of each line..
    TLineCharCodeList lineCharCodeList{};
    TI32              lineActualWidth = 0;
    for (const TC16& ucs2Char : string)
    {
      if (ucs2Char != '\n')
      { 
        const auto& charInfo = container[ucs2Char];
        if (container.IsCharacterGlyphExist(ucs2Char) == false) { continue; }
        // Insert and relocate next position.
        lineCharCodeList.emplace_back(std::pair(ucs2Char, DDyVector2{renderPosition.X, renderPosition.Y}));
        // Calculate width.
        const auto calculatedWidth = static_cast<TI32>(charInfo.mHorizontalAdvance * fontSize / 2);
        renderPosition.X += calculatedWidth;
        lineActualWidth  += calculatedWidth;
      }
      else 
      { // If line feed is exist.
        charCodeList.emplace_back(lineCharCodeList);
        lineActualWidthList.emplace_back(lineActualWidth);
        lineCharCodeList.clear();
        lineActualWidth = 0;
        // Realign.
        renderPosition.X = initPos.X; renderPosition.Y -= fontSize * 2.5f;
        //renderPosition.Y -= static_cast<TF32>(container.GetLinefeedHeight(fontSize) * 0.5f);
      }
    }
    // and add final string if exist.
    if (lineCharCodeList.empty() == false)
    {
      charCodeList.emplace_back(lineCharCodeList);
      lineActualWidthList.emplace_back(lineActualWidth);
      lineCharCodeList.clear();
    }
  }
 
  // Check alignment value, and get alignment pixel using actual width of text line.
  switch (this->mPtrWidget->GetAlignment())
  {
  case EDyHorizontalAlignment::Left: /* Do nothing */ break;
  case EDyHorizontalAlignment::Center: 
  { // We need to subtract half calculated width (advances?)
    for (TU32 lineId = 0, lineNum = charCodeList.size(); lineId < lineNum; ++lineId)
    {
      const auto offsetWidth = lineActualWidthList[lineId] / 2;
      for (auto& [charCode, position] : charCodeList[lineId]) { position.X -= offsetWidth; }
    }
  } break;
  case EDyHorizontalAlignment::Right: 
  { // We need to subtract full caculated width.
    for (TU32 lineId = 0, lineNum = charCodeList.size(); lineId < lineNum; ++lineId)
    {
      const auto offsetWidth = lineActualWidthList[lineId];
      for (auto& [charCode, position] : charCodeList[lineId]) { position.X -= offsetWidth; }
    }
  } break;
  }
  
  // Set actual charcode list for rendering with calculated rendering position.
  TLineCharCodeList actualCharCodeList = {}; 
  actualCharCodeList.reserve(string.GetLength());
  // Insertion
  for (const auto& listItem : charCodeList)
  {
    actualCharCodeList.insert(actualCharCodeList.end(), MDY_BIND_BEGIN_END(listItem));
  }

  // Render
  using TBuffer = std::array<DDyVector2, 12>;
  const std::vector<TBuffer> buffer = GetCharacterVertices(container, actualCharCodeList, fontSize);
  std::vector<TU32> indices = {};
  for (TU32 i = 0, num = static_cast<TU32>(buffer.size()); i < num; ++i)
  {
    indices.emplace_back(i * 4 + 0);
    indices.emplace_back(i * 4 + 1);
    indices.emplace_back(i * 4 + 2);
    indices.emplace_back(i * 4 + 3);
    // Separator
    indices.emplace_back(0xFFFFFFFF);
  }

  { 
    glDepthFunc(GL_ALWAYS);

    this->mBinderShader->UseShader();
    this->mBinderFontMesh->BindVertexArray();

    this->mBinderShader->TryUpdateUniformList();
    glBindBuffer(GL_ARRAY_BUFFER, this->mBinderFontMesh->GetVertexBufferId());
    glBufferData(GL_ARRAY_BUFFER, buffer.size() * sizeof(TBuffer), buffer.data()->data(), GL_DYNAMIC_DRAW);
    // Render texture glyph
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D_ARRAY, container.GetFontTextureArrayId());

    //const auto numInstance = static_cast<TU32>(buffer.size());
    glDrawElements(GL_TRIANGLE_FAN, indices.size(), GL_UNSIGNED_INT, indices.data());
    this->mBinderShader->DisuseShader();
    glDepthFunc(GL_LEQUAL);
  }
}

} /// ::dy namespace