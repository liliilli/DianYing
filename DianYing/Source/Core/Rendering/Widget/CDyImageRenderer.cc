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
#include <Dy/Component/Internal/CDyImageRenderer.h>

#include <glm/gtc/matrix_transform.hpp>
#include <Dy/Builtin/ShaderGl/UI/RenderUIImage.h>
#include <Dy/Builtin/Mesh/FDyBtMsUiImgQuad.h>
#include <Dy/Core/Resource/Resource/FDyShaderResource.h>
#include <Dy/Core/Resource/Resource/FDyMeshResource.h>
#include <Dy/Core/Rendering/Wrapper/FDyGLWrapper.h>
#include <Dy/Element/Canvas/FDyImage.h>
#include <Dy/Core/Resource/Resource/FDyTextureResource.h>
#include <Dy/Core/Thread/SDyIOConnectionHelper.h>
#include <Dy/Meta/Information/MetaInfoMaterial.h>
#include "Dy/Core/Resource/Resource/FDyMaterialResource.h"

namespace dy
{

///
/// @brief The method gets character quad vertices to be needed for rendering.
/// @return Character glyph render vertices information.
/// @see https://www.freetype.org/freetype2/docs/tutorial/step2.html
///
MDY_NODISCARD std::array<dy::DDyVector2, 4>
GetVertexPosition(
    _MIN_ const dy::DDyVector2& position, _MIN_ const dy::DDyVectorInt2& size,
    _MIN_ const TI32 padding = 0, _MIN_ const TF32 percentage = 1.0f)
{
  const auto lb = dy::DyGetPositionWithOrigin(position, size, dy::EDyOrigin::Left_Bottom) + dy::DDyVector2( padding );
  auto       ru = dy::DyGetPositionWithOrigin(position, size, dy::EDyOrigin::Right_Top) - dy::DDyVector2( padding );
  ru.X          = (ru.X - lb.X) * percentage + lb.X;

  return { dy::DDyVector2{ru.X, lb.Y}, ru, dy::DDyVector2{lb.X, ru.Y}, lb };
}

/// Sample UI projection code
dy::DDyMatrix4x4 uUiProjTempMatrix = dy::DDyMatrix4x4{};

CDyImageRenderer::CDyImageRenderer(FDyImage& mRefObject) : 
    mPtrObject{&mRefObject}
{
  this->mBinderQuadMesh.TryRequireResource(MSVSTR(FDyBtMsUiImageQuad::sName));
  this->mPtrObject->SetShaderSpecifier(MSVSTR(builtin::FDyBuiltinShaderGLRenderUiImage::sName));
  uUiProjTempMatrix = glm::ortho(0.f, static_cast<float>(1280), 0.f, static_cast<float>(720), 0.2f, 10.0f);
}

void CDyImageRenderer::Render()
{
  using EUniformType = EDyUniformVariableType;
  if (this->mBinderMaterial.IsResourceExist() == false
  ||  this->mBinderQuadMesh.IsResourceExist() == false) { return; }

  auto& sourceBinder = this->mBinderMaterial->GetShaderResourceBinder();
  sourceBinder->UseShader();
  sourceBinder.TryUpdateUniform<EUniformType::Matrix4>("uUiProjMatrix", uUiProjTempMatrix);
  sourceBinder.TryUpdateUniform<EUniformType::Vector4>("uTintColor", this->mPtrObject->GetTintColor());
  sourceBinder.TryUpdateUniformList();

  glDepthFunc(GL_ALWAYS);
  this->mBinderQuadMesh->BindVertexArray();
  this->mBinderMaterial->TryUpdateTextureList();

  const auto buffer = GetVertexPosition(this->mPtrObject->GetFinalPosition(EDyOrigin::Center_Center), this->mPtrObject->GetFrameSize());
  FDyGLWrapper::MapBufferExt(
      EDyDirectBufferType::VertexBuffer, 
      this->mBinderQuadMesh->GetVertexBufferId(), 
      (void*)buffer.data(), 
      sizeof(buffer),
      8, 8, 0);
  FDyGLWrapper::Draw(EDyDrawType::TriangleFan, false, 4);

  FDyGLWrapper::UnbindVertexArrayObject();
  sourceBinder->DisuseShader();
  glDepthFunc(GL_LEQUAL);
}

void CDyImageRenderer::UpdateMaterial() noexcept
{
  PDyMaterialInstanceMetaInfo info{};
  info.mShaderSpecifier = mPtrObject->GetShaderSpecifier();
  info.mTextureNames[0] = mPtrObject->GetImageName();
  info.mBlendMode       = EDyMaterialBlendMode::Opaque;
  SDyIOConnectionHelper::PopulateInstantMaterialResource(info, this->mBinderMaterial);
}

} /// ::dy namespace