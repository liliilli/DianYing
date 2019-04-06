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
#include <Dy/Component/Internal/WidgetRenderer/CRendererImage.h>

#include <Dy/Builtin/Mesh/FDyBtMsUiImgQuad.h>
#include <Dy/Core/Resource/Resource/FResourceShader.h>
#include <Dy/Core/Resource/Resource/FResourceMesh.h>
#include <Dy/Core/Rendering/Wrapper/XGLWrapper.h>
#include <Dy/Element/Widget/FWidgetImage.h>
#include <Dy/Core/Resource/Resource/AResourceTextureBase.h>
#include <Dy/Core/Thread/SIOConnectionHelper.h>
#include <Dy/Core/Resource/Resource/FResourceMaterial.h>
#include <Dy/Management/Rendering/MRendering.h>

namespace dy
{

///
/// @brief The method gets character quad vertices to be needed for rendering.
/// @return Character glyph render vertices information.
/// @see https://www.freetype.org/freetype2/docs/tutorial/step2.html
///
MDY_NODISCARD std::array<dy::DVec2, 4>
GetVertexPosition(
    _MIN_ const dy::DVec2& position, _MIN_ const dy::DIVec2& size,
    _MIN_ const TI32 padding = 0, _MIN_ const TF32 percentage = 1.0f)
{
  const auto lb = dy::DyGetPositionWithOrigin(position, size, dy::EDyOrigin::Left_Bottom) + dy::DVec2( padding );
  auto       ru = dy::DyGetPositionWithOrigin(position, size, dy::EDyOrigin::Right_Top) - dy::DVec2( padding );
  ru.X          = (ru.X - lb.X) * percentage + lb.X;

  return { dy::DVec2{ru.X, lb.Y}, ru, dy::DVec2{lb.X, ru.Y}, lb };
}

CRendererImage::CRendererImage(FWidgetImage& mRefObject) 
  : mPtrObject{&mRefObject}
{
  this->mBinderRenderable = std::make_unique<TDyResourceBinderMaterial>();
  this->mBinderQuadMesh.TryRequireResource((FDyBtMsUiImageQuad::sName));
}

void CRendererImage::Render()
{
  using EUniformType = EUniformVariableType;
  if (const auto type = this->mBinderRenderable->mRenderType; type == EResourceType::Texture)
  {
    auto& refTexture = static_cast<TDyResourceBinderTexture&>(*this->mBinderRenderable);  // NOLINT(cppcoreguidelines-pro-type-static-cast-downcast)
    if (refTexture.IsResourceExist() == false || this->mBinderQuadMesh.IsResourceExist() == false) { return; }

    // Set shader uniforms
    mDefaultImageShader->TryUpdateUniform<EUniformType::Matrix4>("uUiProjMatrix", MRendering::GetInstance().GetGeneralUiProjectionMatrix());
    mDefaultImageShader->TryUpdateUniform<EUniformType::Vector4>("uTintColor", this->mPtrObject->GetTintColor());
    MDY_ASSERT_MSG(this->mDefaultImageShader->GetAvailableTextureCount() == 1, "Unexpected error occurred.");
    mDefaultImageShader->TryInsertTextureRequisition(0, refTexture->GetTextureId());
    // Set buffer.
    const auto buffer = GetVertexPosition(this->mPtrObject->GetFinalPosition(EDyOrigin::Center_Center), this->mPtrObject->GetFrameSize());

    // Render
    { 
      mDefaultImageShader->UseShader();
      mDefaultImageShader->TryUpdateUniformList();

      glDepthFunc(GL_ALWAYS);
      this->mBinderQuadMesh->BindVertexArray();

      XGLWrapper::MapBufferExt(
          EDirectBufferType::VertexBuffer, 
          this->mBinderQuadMesh->GetVertexBufferId(), 
          (void*)buffer.data(), sizeof(buffer), 8, 8, 0);
      XGLWrapper::Draw(EDrawType::TriangleFan, false, 4);

      XGLWrapper::UnbindVertexArrayObject();
      mDefaultImageShader->DisuseShader();
      glDepthFunc(GL_LEQUAL);
    }
  }
  else if (type == EResourceType::Material)
  {
    auto& refMaterial = static_cast<TDyResourceBinderMaterial&>(*this->mBinderRenderable);  // NOLINT(cppcoreguidelines-pro-type-static-cast-downcast)
    if (refMaterial.IsResourceExist() == false || this->mBinderQuadMesh.IsResourceExist() == false) { return; }

    // Set shader uniforms
    auto& refMatShader = refMaterial->GetShaderResourceBinder();
    refMatShader->TryUpdateUniform<EUniformType::Matrix4>("uUiProjMatrix", MRendering::GetInstance().GetGeneralUiProjectionMatrix());
    refMatShader->TryUpdateUniform<EUniformType::Vector4>("uTintColor", this->mPtrObject->GetTintColor());
    // Set buffer.
    const auto buffer = GetVertexPosition(this->mPtrObject->GetFinalPosition(EDyOrigin::Center_Center), this->mPtrObject->GetFrameSize());

    // Render
    { 
      refMatShader->UseShader();
      refMatShader->TryUpdateUniformList();

      glDepthFunc(GL_ALWAYS);
      this->mBinderQuadMesh->BindVertexArray();
      refMaterial->TryUpdateTextureList();

      XGLWrapper::MapBufferExt(
          EDirectBufferType::VertexBuffer, 
          this->mBinderQuadMesh->GetVertexBufferId(), 
          (void*)buffer.data(), sizeof(buffer), 8, 8, 0);
      XGLWrapper::Draw(EDrawType::TriangleFan, false, 4);

      XGLWrapper::UnbindVertexArrayObject();
      refMatShader->DisuseShader();
      glDepthFunc(GL_LEQUAL);
    }
  }
}

/*
 * OLD CODE
#ifdef false
  PDyMaterialInstanceMetaInfo info{};
  info.mShaderSpecifier = mDefaultImageShader->GetSpecifierName();
  info.mTextureNames[0] = {mPtrObject->GetRenderableImageName().first, ETextureMapType::Unknown};
  info.mBlendMode       = EMaterialBlendMode::Opaque;
  SIOConnectionHelper::PopulateInstantMaterialResource(info, static_cast<TDyResourceBinderMaterial&>(*this->mBinderRenderable));
#endif
 */
void CRendererImage::UpdateRenderableTarget() noexcept
{
  const auto& [specifier, isMaterial] = this->mPtrObject->GetRenderableImageName();
  if (isMaterial == true)
  {
    if (this->mBinderRenderable->mRenderType == EResourceType::Material)
    {
      auto& refBinder = static_cast<TDyResourceBinderMaterial&>(*this->mBinderRenderable);
      refBinder.TryRequireResource(specifier);
    }
    else
    {
      this->mBinderRenderable = nullptr;
      this->mBinderRenderable = std::make_unique<TDyResourceBinderMaterial>(specifier);
    }
  }
  else
  {
    if (this->mBinderRenderable->mRenderType == EResourceType::Texture)
    {
      auto& refBinder = static_cast<TDyResourceBinderTexture&>(*this->mBinderRenderable);
      refBinder.TryRequireResource(specifier);
    }
    else
    {
      this->mBinderRenderable = nullptr;
      this->mBinderRenderable = std::make_unique<TDyResourceBinderTexture>(specifier);
    }
  }
}

TDyResourceBinderMaterial* CRendererImage::GetUsingMaterial()
{
  // Check
  if (this->mBinderRenderable == nullptr
  ||  this->mBinderRenderable->mRenderType != EResourceType::Material)
  {
    return nullptr;
  }

  // Return
  return &static_cast<TDyResourceBinderMaterial&>(*this->mBinderRenderable);
}

} /// ::dy namespace