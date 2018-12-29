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
#include <Dy/Component/Internal/CDyBasicGaugeBarRenderer.h>

#include <glm/gtc/matrix_transform.hpp>

#include <Dy/Builtin/ShaderGl/UI/RenderUIBasicGaugeBar.h>
#include <Dy/Builtin/Mesh/Widget/FDyBtMsUiBarQuad.h>
#include <Dy/Core/Resource/Resource/FDyShaderResource.h>
#include <Dy/Element/Canvas/FDyBasicGaugeBar.h>
#include <Dy/Management/SettingManager.h>
#include <Dy/Helper/Type/Matrix4.h>
#include <Dy/Core/Resource/Resource/FDyMeshResource.h>
#include <Dy/Core/Rendering/Wrapper/FDyGLWrapper.h>
#include <Dy/Core/Rendering/Wrapper/PDyGLBufferDescriptor.h>

//!
//! Forward declaration
//!

namespace
{

/// Sample UI projection code
dy::DDyMatrix4x4 uUiProjTempMatrix = dy::DDyMatrix4x4{};

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

} /// ::unnamed namespace

//!
//! Implementation
//!

namespace dy
{

EDySuccess CDyBasicGaugeBarRenderer::Initialize(const PDyBasicGaugeBarRendererCtorInformation& descriptor)
{
  MDY_ASSERT(MDY_CHECK_ISNOTNULL(descriptor.mPtrUiObject), "descriptor.mPtrUiObject must not be null.");
  this->mPtrBarObject  = descriptor.mPtrUiObject;
  this->mBinderShader .TryRequireResource(MSVSTR(builtin::FDyBuiltinShaderGLRenderUiBasicGaugeBar::sName));
  this->mBinderBarMesh.TryRequireResource(MSVSTR(builtin::FDyBtMsUiBarQuad::sName));

  // @TODO SAMPLE CODE (TEMPORAL)
  auto& settingManager = MDySetting::GetInstance();
  const auto overallScreenWidth   = settingManager.GetWindowSizeWidth();
  const auto overallScreenHeight  = settingManager.GetWindowSizeHeight();
  uUiProjTempMatrix = glm::ortho(0.f, static_cast<float>(overallScreenWidth), 0.f, static_cast<float>(overallScreenHeight), 0.2f, 10.0f);

  return DY_SUCCESS;
}

void CDyBasicGaugeBarRenderer::Release() { }

void CDyBasicGaugeBarRenderer::Render()
{
  MDY_ASSERT(this->mPtrBarObject != nullptr, "CDyBasicGaugeBarRenderer::mPtrBarObject must not be nullptr.");
  if (this->mBinderShader.IsResourceExist() == false
  ||  this->mBinderBarMesh.IsResourceExist() == false) { return; }

#ifdef false
  this->mBinderShader->TryUpdateUniform<>("uFillColor", &this->mPtrBarObject->GetBackgroundColor().R);
  this->mBinderShader->TryUpdateUniform<>("uUiProjMatrix", &uUiProjTempMatrix[0].X);
  /* If value is same and not changed, do nothing. */
  this->mBinderShader->TryUpdateUniform();
#endif
  glDepthFunc(GL_ALWAYS);
  this->mBinderShader->UseShader();
  glBindVertexArray(this->mBinderBarMesh->GetVertexArrayId());

  this->mBinderShader.TryUpdateUniform<EDyUniformVariableType::Matrix4>("uUiProjMatrix", uUiProjTempMatrix);

  const DDyVector2 pos     = this->mPtrBarObject->GetRenderPosition();
  const DDyVectorInt2 size = this->mPtrBarObject->GetFrameSize();
  const auto vboId = this->mBinderBarMesh->GetVertexBufferId();

  if (this->mPtrBarObject->CheckIsUsingBackgroundColor() == true)
  {
    this->mBinderShader.TryUpdateUniform<EDyUniformVariableType::Vector4>("uFillColor", this->mPtrBarObject->GetBackgroundColor());
    MDY_CALL_BUT_NOUSE_RESULT(this->mBinderShader.TryUpdateUniformList());

    const auto buffer = GetVertexPosition(pos, size);
    FDyGLWrapper::MapBuffer(EDyDirectBufferType::VertexBuffer, vboId, (void*)buffer.data(), sizeof(buffer));
    FDyGLWrapper::Draw(EDyDrawType::TriangleFan, false, 4);
  }

  {
    this->mBinderShader.TryUpdateUniform<EDyUniformVariableType::Vector4>("uFillColor", this->mPtrBarObject->GetForegroundColor());
    MDY_CALL_BUT_NOUSE_RESULT(this->mBinderShader.TryUpdateUniformList());

    const TI32 padding    = this->mPtrBarObject->GetPadding();
    const TF32 percentage = this->mPtrBarObject->GetPercentage();
    const auto buffer     = GetVertexPosition(pos, size, padding, percentage);
    FDyGLWrapper::MapBuffer(EDyDirectBufferType::VertexBuffer, vboId, (void*)buffer.data(), sizeof(buffer));
    FDyGLWrapper::Draw(EDyDrawType::TriangleFan, false, 4);
  }

  glBindVertexArray(0);
  this->mBinderShader->DisuseShader();
  glDepthFunc(GL_LEQUAL);
}

} /// ::dy namespace