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
#include <Dy/Component/Internal/WidgetRenderer/CRendererBasicGaugeBar.h>

#include <Dy/Builtin/ShaderGl/UI/RenderUIBasicGaugeBar.h>
#include <Dy/Builtin/Mesh/Widget/FDyBtMsUiBarQuad.h>
#include <Dy/Core/Resource/Resource/FDyShaderResource.h>
#include <Dy/Element/Canvas/FDyBasicGaugeBar.h>
#include <Dy/Management/MSetting.h>
#include <Dy/Core/Resource/Resource/FDyMeshResource.h>
#include <Dy/Core/Rendering/Wrapper/XGLWrapper.h>
#include <Dy/Core/Rendering/Wrapper/PGLBufferDescriptor.h>
#include <Dy/Management/Rendering/MRendering.h>

//!
//! Forward declaration
//!

namespace
{

///
/// @brief The method gets character quad vertices to be needed for rendering.
/// @return Character glyph render vertices information.
/// @see https://www.freetype.org/freetype2/docs/tutorial/step2.html
///
MDY_NODISCARD std::array<dy::DVector2, 4>
GetVertexPosition(
    _MIN_ const dy::DVector2& position, _MIN_ const dy::DVectorInt2& size,
    _MIN_ const TI32 padding = 0, _MIN_ const TF32 percentage = 1.0f)
{
  const auto lb = dy::DyGetPositionWithOrigin(position, size, dy::EDyOrigin::Left_Bottom) + dy::DVector2( padding );
  auto       ru = dy::DyGetPositionWithOrigin(position, size, dy::EDyOrigin::Right_Top) - dy::DVector2( padding );
  ru.X          = (ru.X - lb.X) * percentage + lb.X;

  return { dy::DVector2{ru.X, lb.Y}, ru, dy::DVector2{lb.X, ru.Y}, lb };
}

} /// ::unnamed namespace

//!
//! Implementation
//!

namespace dy
{

EDySuccess CRendererBasicGaugeBar::Initialize(const PCtorBasicGaugeBarRendererInformation& descriptor)
{
  MDY_ASSERT_MSG(MDY_CHECK_ISNOTNULL(descriptor.mPtrUiObject), "descriptor.mPtrUiObject must not be null.");
  this->mPtrBarObject  = descriptor.mPtrUiObject;
  this->mBinderShader .TryRequireResource((builtin::FDyBuiltinShaderGLRenderUiBasicGaugeBar::sName));
  this->mBinderBarMesh.TryRequireResource((builtin::FDyBtMsUiBarQuad::sName));

  return DY_SUCCESS;
}

void CRendererBasicGaugeBar::Release() { }

void CRendererBasicGaugeBar::Render()
{
  using EUniformType = EUniformVariableType;
  MDY_ASSERT_MSG(this->mPtrBarObject != nullptr, "CRendererBasicGaugeBar::mPtrBarObject must not be nullptr.");
  if (this->mBinderShader.IsResourceExist() == false
  ||  this->mBinderBarMesh.IsResourceExist() == false) { return; }

  this->mBinderShader->TryUpdateUniform<EUniformType::Matrix4>("uUiProjMatrix", MRendering::GetInstance().GetGeneralUiProjectionMatrix());
  const DVector2 pos     = this->mPtrBarObject->GetRenderPosition();
  const DVectorInt2 size = this->mPtrBarObject->GetFrameSize();
  const auto vboId = this->mBinderBarMesh->GetVertexBufferId();

  this->mBinderShader->UseShader();
  glDepthFunc(GL_ALWAYS);
  glBindVertexArray(this->mBinderBarMesh->GetVertexArrayId());

  if (this->mPtrBarObject->CheckIsUsingBackgroundColor() == true)
  { // If backgroud is used, try render.
    this->mBinderShader->TryUpdateUniform<EUniformType::Vector4>("uFillColor", this->mPtrBarObject->GetBackgroundColor());
    MDY_CALL_BUT_NOUSE_RESULT(this->mBinderShader->TryUpdateUniformList());

    const auto buffer = GetVertexPosition(pos, size);
    XGLWrapper::MapBuffer(EDirectBufferType::VertexBuffer, vboId, (void*)buffer.data(), sizeof(buffer));
    XGLWrapper::Draw(EDrawType::TriangleFan, false, 4);
  }

  { // Foreground render.
    this->mBinderShader->TryUpdateUniform<EUniformType::Vector4>("uFillColor", this->mPtrBarObject->GetForegroundColor());
    MDY_CALL_BUT_NOUSE_RESULT(this->mBinderShader->TryUpdateUniformList());

    const TI32 padding    = this->mPtrBarObject->GetPadding();
    const TF32 percentage = this->mPtrBarObject->GetPercentage();
    const auto buffer     = GetVertexPosition(pos, size, padding, percentage);
    XGLWrapper::MapBuffer(EDirectBufferType::VertexBuffer, vboId, (void*)buffer.data(), sizeof(buffer));
    XGLWrapper::Draw(EDrawType::TriangleFan, false, 4);
  }

  glBindVertexArray(0);
  this->mBinderShader->DisuseShader();
  glDepthFunc(GL_LEQUAL);
}

} /// ::dy namespace