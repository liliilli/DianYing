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
#include <Dy/Core/Rendering/FinalScreenDisplayRenderer.h>
#include <Dy/Core/Rendering/Helper/FrameAttachmentString.h>
#include <Dy/Management/Rendering/FramebufferManager.h>
#include <Dy/Management/SettingManager.h>
#include <Dy/Management/IO/IOResourceManager.h>
#include <Dy/Management/WorldManager.h>
#include <Dy/Builtin/ShaderGl/RenderScreenOutput.h>
#include <Dy/Builtin/Model/ScreenProjectionTriangle.h>

namespace dy
{

FDyFinalScreenDisplayRenderer::FDyFinalScreenDisplayRenderer()
{
  auto& framebufferManager  = MDyFramebuffer::GetInstance();
  auto& heapManager         = MDyIOResource::GetInstance();

  //!
  //! Lambda function
  //!

  static auto GetPointerOfAttachmentTextures = [&]() -> EDySuccess
  {
    if (MDY_CHECK_ISNULL(this->mAttachmentPtr_Scene))
    {
      this->mAttachmentPtr_Scene = framebufferManager.GetAttachmentPointer(sAttachment_ScreenFinal_Output);
    }
    if (MDY_CHECK_ISNULL(this->mAttachmentPtr_Ui))
    {
      this->mAttachmentPtr_Ui   = framebufferManager.GetAttachmentPointer(sAttachment_Output);
    }

    if (MDY_CHECK_ISNULL(this->mAttachmentPtr_Scene)) { return DY_FAILURE; }
    if (MDY_CHECK_ISNULL(this->mAttachmentPtr_Ui))    { return DY_FAILURE; }
    return DY_SUCCESS;
  };

  static auto ProceedTriangleSetting = [&]() -> EDySuccess
  {
    //
    using TFDBMSPT = builtin::FDyBuiltinModelScreenProjectionTriangle;

    if (MDY_CHECK_ISNULL(heapManager.GetModelResource(MSVSTR(TFDBMSPT::sName))))
    {
      MDY_CALL_ASSERT_SUCCESS(heapManager.CreateModelResource(MSVSTR(TFDBMSPT::sName)));
    }
    this->mScreenRenderTrianglePtr = heapManager.GetModelResource(MSVSTR(TFDBMSPT::sName));

    return DY_SUCCESS;
  };

  static auto ProceedShaderSetting = [&]() -> EDySuccess
  {
    // Make shader
    builtin::FDyBuiltinShaderGLRenderScreenOutput();
    this->mShaderPtr = heapManager.GetShaderResource(MSVSTR(builtin::FDyBuiltinShaderGLRenderScreenOutput::sName));

    MDY_ASSERT(MDY_CHECK_ISNOTNULL(this->mShaderPtr), "FDyFinalScreenDisplayRenderer::mShaderPtr must not be nullptr.");

    this->mShaderPtr->UseShader();
    glUniform1i(glGetUniformLocation(this->mShaderPtr->GetShaderProgramId(), "uSceneTexture"), 0);
    glUniform1i(glGetUniformLocation(this->mShaderPtr->GetShaderProgramId(), "uUiTexture")   , 1);
    this->mShaderPtr->UnuseShader();

    return DY_SUCCESS;
  };

  //!
  //! FunctionBody ∨
  //!

  MDY_CALL_ASSERT_SUCCESS(GetPointerOfAttachmentTextures());
  MDY_CALL_ASSERT_SUCCESS(ProceedTriangleSetting());
  MDY_CALL_ASSERT_SUCCESS(ProceedShaderSetting());
}

FDyFinalScreenDisplayRenderer::~FDyFinalScreenDisplayRenderer()
{
  this->mAttachmentPtr_Scene = nullptr;
  this->mAttachmentPtr_Ui = nullptr;
  this->mScreenRenderTrianglePtr = nullptr;
  this->mShaderPtr = nullptr;
}

void FDyFinalScreenDisplayRenderer::RenderScreen()
{
  MDY_ASSERT(MDY_CHECK_ISNOTNULL(this->mShaderPtr), "FDyFinalScreenDisplayRenderer::mShaderPtr must not be nullptr.");
  MDY_ASSERT(MDY_CHECK_ISNOTNULL(this->mScreenRenderTrianglePtr), "");

  const auto& submeshList = this->mScreenRenderTrianglePtr->GetSubmeshResources();
  MDY_ASSERT(submeshList.size() == 1, "");
  // Bind vertex array
  const CDySubmeshResource& mesh = *submeshList[0];

  // Set
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  this->mShaderPtr->UseShader();
  glBindVertexArray(mesh.GetVertexArrayId());

  // Bind g-buffers as textures and draw.
  glActiveTexture(GL_TEXTURE0); glBindTexture(GL_TEXTURE_2D, this->mAttachmentPtr_Scene->GetAttachmentId());
  glActiveTexture(GL_TEXTURE1); glBindTexture(GL_TEXTURE_2D, this->mAttachmentPtr_Ui->GetAttachmentId());
  glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, nullptr);

  // Rewind
  glBindVertexArray(0);
  this->mShaderPtr->UnuseShader();
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FDyFinalScreenDisplayRenderer::Clear()
{
  glBindFramebuffer(GL_FRAMEBUFFER, 0);

  glClearColor(0, 0, 0, 0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

} /// ::dy namespace