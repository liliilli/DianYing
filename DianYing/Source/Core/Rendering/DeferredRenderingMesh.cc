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
#include <Dy/Core/Rendering/DeferredRenderingMesh.h>

#include <Dy/Builtin/ShaderGl/RenderDeferredRendering.h>
#include <Dy/Core/Rendering/Helper/FrameAttachmentString.h>
#include <Dy/Component/CDyDirectionalLight.h>
#include <Dy/Management/IODataManager.h>
#include <Dy/Management/IOResourceManager.h>
#include <Dy/Management/RenderingManager.h>
#include <Dy/Management/WorldManager.h>
#include <Dy/Management/SettingManager.h>
#include <Dy/Management/Internal/FramebufferManager.h>
#include <Dy/Management/Internal/UniformBufferObjectManager.h>
#include <Dy/Builtin/Model/ScreenProjectionTriangle.h>
#include <glm/gtc/matrix_transform.inl>

//!
//! Forward declaration
//!

namespace
{

dy::DDyMatrix4x4 sSamplePvMatrix =
    glm::ortho(-20.0f, 20.0f, -20.0f, 20.0f, 0.02f, 100.0f) *
    glm::lookAt(glm::vec3(0, 20, 20), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));

} /// ::unnamed namespace

namespace dy
{

FDyDeferredRenderingMesh::FDyDeferredRenderingMesh()
{
  auto& settingManager      = MDySetting::GetInstance();
  auto& framebufferManager  = MDyFramebuffer::GetInstance();
  auto& heapManager         = MDyIOResource::GetInstance();

  ///
  /// @function CreateFramebufferAttachmentSetting
  /// @brief
  ///
  static auto CreateFramebufferAttachmentSetting = [&]()
  {
    const auto overallScreenWidth = settingManager.GetWindowSizeWidth();
    const auto overallScreenHeight = settingManager.GetWindowSizeHeight();
    const auto overallSize = DDyVectorInt2{ overallScreenWidth, overallScreenHeight };

    PDyGlFrameBufferInformation       framebufferInfo = {};
    PDyGlAttachmentInformation        attachmentInfo = {};
    PDyGlAttachmentBinderInformation  binderInfo = {};

    framebufferInfo.mFrameBufferName = sFrameBuffer_ScreenFinal;
    framebufferInfo.mFrameBufferSize = overallSize;
    framebufferInfo.mIsUsingDefaultDepthBuffer = true;

    // Rendered texture buffer
    attachmentInfo.mAttachmentName = sAttachment_ScreenFinal_Output;
    attachmentInfo.mAttachmentSize = overallSize;
    attachmentInfo.mParameterList  = {
        PDyGlTexParameterInformation\
        {EDyGlParameterName::TextureMinFilter, EDyGlParameterValue::Nearest},
        {EDyGlParameterName::TextureMagFilter, EDyGlParameterValue::Nearest},
        {EDyGlParameterName::TextureWrappingS, EDyGlParameterValue::ClampToBorder},
        {EDyGlParameterName::TextureWrappingT, EDyGlParameterValue::ClampToBorder},
    };
    attachmentInfo.mBorderColor = DDyColor{ 0, 0, 0, 0 };

    binderInfo.mAttachmentName = sAttachment_ScreenFinal_Output;
    binderInfo.mAttachmentType = EDyGlAttachmentType::Color0;
    framebufferInfo.mAttachmentList.push_back(binderInfo);

    // Push attachment buffer
    MDY_CALL_ASSERT_SUCCESS(framebufferManager.SetAttachmentInformation(attachmentInfo));
    // Create framebuffer.
    MDY_CALL_ASSERT_SUCCESS(framebufferManager.InitializeNewFrameBuffer(framebufferInfo));
  };

  // FunctionBody∨

  CreateFramebufferAttachmentSetting();
  MDY_CALL_ASSERT_SUCCESS(this->pInitializeShaderSetting());
  MDY_CALL_ASSERT_SUCCESS(this->pInitializeUboBuffers());

  //
  this->mDyBtFbScrFin = framebufferManager.GetFrameBufferPointer(MSVSTR(sFrameBuffer_ScreenFinal));
  MDY_ASSERT(MDY_CHECK_ISNOTNULL(this->mDyBtFbScrFin), "Unexpected error.");

  //
  if (MDY_CHECK_ISNULL(heapManager.GetModelResource(MSVSTR(builtin::FDyBuiltinModelScreenProjectionTriangle::sName))))
  {
    MDY_CALL_ASSERT_SUCCESS(heapManager.CreateModelResource(MSVSTR(builtin::FDyBuiltinModelScreenProjectionTriangle::sName)));
  }
  this->mScreenRenderTrianglePtr = heapManager.GetModelResource(MSVSTR(builtin::FDyBuiltinModelScreenProjectionTriangle::sName));

  for (TI32 i = 0; i < FDyDeferredRenderingMesh::sDirectionalLightCount; ++i)
  {
    this->mDirLightAvailableList.push(i);
  }
}

FDyDeferredRenderingMesh::~FDyDeferredRenderingMesh()
{
  auto& uboManager = MDyUniformBufferObject::GetInstance();
  MDY_CALL_ASSERT_SUCCESS(uboManager.RemoveUboContainer("dyBtUboDirLight"));
}

void FDyDeferredRenderingMesh::RenderScreen()
{
  MDY_ASSERT(MDY_CHECK_ISNOTNULL(this->mShaderPtr), "FDyDeferredRenderingMesh::mShaderPtr must not be nullptr.");
  MDY_ASSERT(MDY_CHECK_ISNOTNULL(this->mScreenRenderTrianglePtr), "");
  if (this->mIsAttachmentPtrBinded == false)
  {
    const auto flag = this->pTryGetAttachmentPointers();
    if (flag == DY_SUCCESS) { this->mIsAttachmentPtrBinded = true; }
    else                    { return; }
  }

  const auto& submeshList = this->mScreenRenderTrianglePtr->GetSubmeshResources();
  MDY_ASSERT(submeshList.size() == 1, "");
  // Bind vertex array
  const CDySubmeshResource& mesh = *submeshList[0];

  // Set
  glBindFramebuffer(GL_FRAMEBUFFER, this->mDyBtFbScrFin->GetFramebufferId());
  this->mShaderPtr->UseShader();
  glBindVertexArray(mesh.GetVertexArrayId());

  // Bind g-buffers as textures.
  const auto uShadowPv_Id = glGetUniformLocation(this->mShaderPtr->GetShaderProgramId(), "uShadowPv");
  glUniformMatrix4fv(uShadowPv_Id, 1, GL_FALSE, &sSamplePvMatrix[0].X);

  glActiveTexture(GL_TEXTURE0); glBindTexture(GL_TEXTURE_2D, this->mAttachmentPtr_Unlit->GetAttachmentId());
  glActiveTexture(GL_TEXTURE1); glBindTexture(GL_TEXTURE_2D, this->mAttachmentPtr_Normal->GetAttachmentId());
  glActiveTexture(GL_TEXTURE2); glBindTexture(GL_TEXTURE_2D, this->mAttachmentPtr_Specular->GetAttachmentId());
  glActiveTexture(GL_TEXTURE3); glBindTexture(GL_TEXTURE_2D, this->mAttachmentPtr_ModelPosition->GetAttachmentId());
  glActiveTexture(GL_TEXTURE4); glBindTexture(GL_TEXTURE_2D, this->mAttachmentPtr_Shadow->GetAttachmentId());
  glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, nullptr);

  // Rewind
  glBindVertexArray(0);
  this->mShaderPtr->UnuseShader();
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FDyDeferredRenderingMesh::Clear()
{
  glBindFramebuffer(GL_FRAMEBUFFER, this->mDyBtFbScrFin->GetFramebufferId());
  auto& worldManager = MDyWorld::GetInstance();

  const auto& backgroundColor = worldManager.GetValidLevelReference().GetBackgroundColor();
  glClearColor(backgroundColor.R, backgroundColor.G, backgroundColor.B, backgroundColor.A);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

std::optional<TI32> FDyDeferredRenderingMesh::GetAvailableDirectionalLightIndex() noexcept
{
  if (this->mDirLightAvailableList.empty() == true) { return std::nullopt; }
  else
  {
    const auto returnId = this->mDirLightAvailableList.front();
    this->mDirLightAvailableList.pop();

    return returnId;
  }
}

EDySuccess FDyDeferredRenderingMesh::pInitializeShaderSetting()
{
  auto& manResc = MDyIOResource::GetInstance();
  // Make deferred shader
  builtin::FDyBuiltinShaderGLRenderDeferredRendering();
  this->mShaderPtr = manResc.GetShaderResource(MSVSTR(builtin::FDyBuiltinShaderGLRenderDeferredRendering::sName));

  MDY_ASSERT(this->mShaderPtr, "FDyDeferredRenderingMesh::mShaderPtr must not be nullptr.");
  this->mShaderPtr->UseShader();

  glUniform1i(glGetUniformLocation(this->mShaderPtr->GetShaderProgramId(), "uUnlit"), 0);
  glUniform1i(glGetUniformLocation(this->mShaderPtr->GetShaderProgramId(), "uNormal"), 1);
  glUniform1i(glGetUniformLocation(this->mShaderPtr->GetShaderProgramId(), "uSpecular"), 2);
  glUniform1i(glGetUniformLocation(this->mShaderPtr->GetShaderProgramId(), "uModelPosition"), 3);
  glUniform1i(glGetUniformLocation(this->mShaderPtr->GetShaderProgramId(), "uShadow"), 4);

  this->mShaderPtr->UnuseShader();
  return DY_SUCCESS;
}

EDySuccess FDyDeferredRenderingMesh::pInitializeUboBuffers()
{
  auto& uboManager = MDyUniformBufferObject::GetInstance();
  PDyUboConstructionDescriptor desc = {};
  desc.mBindingIndex      = 1;
  desc.mUboSpecifierName  = "dyBtUboDirLight";
  desc.mBufferDrawType    = EDyBufferDrawType::DynamicDraw;
  desc.mUboElementSize    = sizeof(DDyUboDirectionalLight);
  desc.mUboArraySize      = FDyDeferredRenderingMesh::sDirectionalLightCount;
  MDY_CALL_ASSERT_SUCCESS(uboManager.CreateUboContainer(desc));
  return DY_SUCCESS;
}

EDySuccess FDyDeferredRenderingMesh::UpdateDirectionalLightValueToGpu(
    _MIN_ const TI32 index,
    _MIN_ const DDyUboDirectionalLight& container)
{ // Integrity check
  constexpr TI32 UboElementSize = sizeof(DDyUboDirectionalLight);

  auto& uboManager = MDyUniformBufferObject::GetInstance();
  MDY_CALL_ASSERT_SUCCESS(uboManager.UpdateUboContainer("dyBtUboDirLight", UboElementSize * index, UboElementSize, &container));
  return DY_SUCCESS;
}

EDySuccess FDyDeferredRenderingMesh::UnbindDirectionalLight(_MIN_ const TI32 index)
{ // Integrity check
  constexpr TI32 UboElementSize = sizeof(DDyUboDirectionalLight);

  auto& uboManager = MDyUniformBufferObject::GetInstance();
  MDY_CALL_ASSERT_SUCCESS(uboManager.ClearUboContainer("dyBtUboDirLight", UboElementSize * index, UboElementSize));
  return DY_SUCCESS;
}


EDySuccess FDyDeferredRenderingMesh::pTryGetAttachmentPointers()
{
  TI32 count = 0;

  auto& framebufferManager = MDyFramebuffer::GetInstance();
  if (MDY_CHECK_ISNULL(this->mAttachmentPtr_Unlit))
  {
    this->mAttachmentPtr_Unlit = framebufferManager.GetAttachmentPointer(sAttachment_Unlit);
  }
  if (MDY_CHECK_ISNULL(this->mAttachmentPtr_Normal))
  {
    this->mAttachmentPtr_Normal = framebufferManager.GetAttachmentPointer(sAttachment_Normal);
  }
  if (MDY_CHECK_ISNULL(this->mAttachmentPtr_Specular))
  {
    this->mAttachmentPtr_Specular = framebufferManager.GetAttachmentPointer(sAttachment_Specular);
  }
  if (MDY_CHECK_ISNULL(this->mAttachmentPtr_ModelPosition))
  {
    this->mAttachmentPtr_ModelPosition = framebufferManager.GetAttachmentPointer(sAttachment_ModelPosition);
  }
  if (MDY_CHECK_ISNULL(this->mAttachmentPtr_Shadow))
  {
    this->mAttachmentPtr_Shadow = framebufferManager.GetAttachmentPointer(sAttachment_DirectionalBasicShadow);
  }

  if (MDY_CHECK_ISNOTNULL(this->mAttachmentPtr_Unlit))        { count += 1; }
  if (MDY_CHECK_ISNOTNULL(this->mAttachmentPtr_Normal))       { count += 1; }
  if (MDY_CHECK_ISNOTNULL(this->mAttachmentPtr_Specular))     { count += 1; }
  if (MDY_CHECK_ISNOTNULL(this->mAttachmentPtr_ModelPosition)) { count += 1; }
  if (MDY_CHECK_ISNOTNULL(this->mAttachmentPtr_Shadow))       { count += 1; }

  if (count == 5) { return DY_SUCCESS; }
  else            { return DY_FAILURE; }
}

} /// ::dy namespace