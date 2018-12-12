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
#include <Dy/Element/Canvas/FDyBasicGaugeBar.h>
#include <Dy/Management/IO/IOResourceManager_Deprecated.h>
#include <Dy/Management/SettingManager.h>
#include "Dy/Element/Canvas/Text.h"

//!
//! Forward declaration
//!

namespace
{

/// Sample UI projection code
dy::DDyMatrix4x4 uUiProjMatrix = dy::DDyMatrix4x4{};

GLuint mTempVao = MDY_INITIALIZE_DEFUINT;
GLuint mTestVbo = MDY_INITIALIZE_DEFUINT;

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

///
/// @brief Actual render method.
/// This method must be called in Render__Side() method.
///
/// @param[in] vertices
///
void RenderBar(_MIN_ const std::array<dy::DDyVector2, 4>& vertices) {
  // Update content of VBO
  static constexpr TU32 size = sizeof(vertices);

  glBindBuffer(GL_ARRAY_BUFFER, mTestVbo);
  glBufferSubData(GL_ARRAY_BUFFER, 0, size, &vertices[0].X);
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  // Render
  glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
}

} /// ::unnamed namespace

//!
//! Implementation
//!

namespace dy
{

EDySuccess CDyBasicGaugeBarRenderer::Initialize(const PDyBasicGaugeBarRendererCtorInformation& descriptor)
{
  static auto SetTestBuffer = [&]
  {
    glGenVertexArrays(1, &mTempVao);
    glGenBuffers(1, &mTestVbo);

    glBindVertexArray(mTempVao);
    glBindBuffer(GL_ARRAY_BUFFER, mTestVbo);

    std::array<DDyVector2, 4> value = { DDyVector2{}, DDyVector2{}, DDyVector2{}, DDyVector2{} };
    glBufferData(GL_ARRAY_BUFFER, sizeof(value), value.data(), GL_DYNAMIC_DRAW);

    glBindVertexBuffer(0, mTestVbo, 0, 8);

    glEnableVertexAttribArray(0);
    glVertexAttribFormat  (0, 2, GL_FLOAT, GL_FALSE, 0);
    glVertexAttribBinding (0, 0);
    glBindVertexArray     (0);
  };

  MDY_ASSERT(MDY_CHECK_ISNOTNULL(descriptor.mPtrUiObject), "descriptor.mPtrUiObject must not be null.");
  this->mPtrBarObject  = descriptor.mPtrUiObject;
  this->mPtrShader     = MDyIOResource_Deprecated::GetInstance().GetShaderResource(MSVSTR(builtin::FDyBuiltinShaderGLRenderUiBasicGaugeBar::sName));

  // @TODO SAMPLE CODE (TEMPORAL)
  auto& settingManager = MDySetting::GetInstance();
  const auto overallScreenWidth   = settingManager.GetWindowSizeWidth();
  const auto overallScreenHeight  = settingManager.GetWindowSizeHeight();
  uUiProjMatrix = glm::ortho(0.f, static_cast<float>(overallScreenWidth), 0.f, static_cast<float>(overallScreenHeight), 0.2f, 10.0f);

  SetTestBuffer();
  return DY_SUCCESS;
}

void CDyBasicGaugeBarRenderer::Release()
{
  glDeleteBuffers(1, &mTestVbo);
  glDeleteVertexArrays(1, &mTempVao);
  this->mPtrBarObject = nullptr;

  MDY_NOT_IMPLEMENTED_ASSERT();
}

void CDyBasicGaugeBarRenderer::Render()
{
  MDY_ASSERT(this->mPtrBarObject != nullptr, "CDyBasicGaugeBarRenderer::mPtrBarObject must not be nullptr.");
  if (MDY_CHECK_ISNULL(this->mPtrShader)) { return; }

  glDepthFunc(GL_ALWAYS);
  this->mPtrShader->UseShader();
  glBindVertexArray(mTempVao);

  const TU32 shaderProgramId  = this->mPtrShader->GetShaderProgramId();
  const auto fillColorId      = glGetUniformLocation(shaderProgramId, "uFillColor");
  const auto shaderid         = glGetUniformLocation(shaderProgramId, "uUiProjMatrix");
  glUniformMatrix4fv(shaderid, 1, GL_FALSE, &uUiProjMatrix[0].X);

  const DDyVector2 pos        = this->mPtrBarObject->GetRenderPosition();
  const DDyVectorInt2 size    = this->mPtrBarObject->GetFrameSize();
  const TI32 padding          = this->mPtrBarObject->GetPadding();
  const TF32 percentage       = this->mPtrBarObject->GetPercentage();

  if (this->mPtrBarObject->CheckIsUsingBackgroundColor() == true)
  {
    glUniform4fv(fillColorId, 1, &this->mPtrBarObject->GetBackgroundColor().R);
    RenderBar(GetVertexPosition(pos, size));
  }

  glUniform4fv(fillColorId, 1, &this->mPtrBarObject->GetForegroundColor().R);
  RenderBar(GetVertexPosition(pos, size, padding, percentage));

  glBindVertexArray(0);
  this->mPtrShader->UnuseShader();
  glDepthFunc(GL_LEQUAL);
}

} /// ::dy namespace