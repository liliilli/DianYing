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
#include <Dy/Core/Component/Object/DeferredRenderingMesh.h>

#include <Dy/Builtin/ShaderGl/RenderDeferredRendering.h>
#include <Dy/Core/Rendering/Helper/FrameAttachmentString.h>
#include <Dy/Component/CDyDirectionalLight.h>
#include <Dy/Management/Internal/FramebufferManager.h>
#include <Dy/Management/DataInformationManager.h>
#include <Dy/Management/HeapResourceManager.h>
#include <Dy/Management/RenderingManager.h>

namespace
{

struct DDyMeshInfo final
{
  dy::DDyVector3 mPosition = {};
  dy::DDyVector2 mTexCoord = {};

  DDyMeshInfo(const dy::DDyVector3& position, const dy::DDyVector2& texCoord) :
      mPosition{position}, mTexCoord{texCoord}
  {};
};

} /// ::unnamed namespace

namespace dy
{

FDyDeferredRenderingMesh::FDyDeferredRenderingMesh()
{
  MDY_CALL_ASSERT_SUCCESS(this->pInitializeGeometries());
  MDY_CALL_ASSERT_SUCCESS(this->pInitializeShaderSetting());
  MDY_CALL_ASSERT_SUCCESS(this->pInitializeUboBuffers());

  for (TI32 i = 0; i < FDyDeferredRenderingMesh::sDirectionalLightCount; ++i)
  {
    this->mAvailableList.push(i);
  }
}

FDyDeferredRenderingMesh::~FDyDeferredRenderingMesh()
{
  if (this->mDirLight) glDeleteBuffers(1, &this->mDirLight);
  if (this->mVbo) glDeleteBuffers(1, &this->mVbo);
  if (this->mVao) glDeleteVertexArrays(1, &this->mVao);

#ifdef false
  auto& manResc = MDyHeapResource::GetInstance();
#endif
}

void FDyDeferredRenderingMesh::RenderScreen()
{
  MDY_ASSERT(this->mShaderPtr, "FDyDeferredRenderingMesh::mShaderPtr must not be nullptr.");
  if (this->mIsAttachmentPtrBinded == false)
  {
    const auto flag = this->pTryGetAttachmentPointers();
    if (flag == DY_SUCCESS) { this->mIsAttachmentPtrBinded = true; }
    else                    { return; }
  }

  this->mShaderPtr->UseShader();
  glBindVertexArray(this->mVao);

  // Bind g-buffers as textures.
  glActiveTexture(GL_TEXTURE0); glBindTexture(GL_TEXTURE_2D, this->mAttachmentPtr_Unlit->GetAttachmentId());
  glActiveTexture(GL_TEXTURE1); glBindTexture(GL_TEXTURE_2D, this->mAttachmentPtr_Normal->GetAttachmentId());
  glActiveTexture(GL_TEXTURE2); glBindTexture(GL_TEXTURE_2D, this->mAttachmentPtr_Specular->GetAttachmentId());
  glActiveTexture(GL_TEXTURE3); glBindTexture(GL_TEXTURE_2D, this->mAttachmentPtr_ViewPosition->GetAttachmentId());

  glDrawArrays(GL_TRIANGLES, 0, 3);
  glBindVertexArray(0);
  this->mShaderPtr->UnuseShader();
}

std::optional<TI32> FDyDeferredRenderingMesh::GetAvailableDirectionalLightIndex() noexcept
{
  if (this->mAvailableList.empty() == true) { return std::nullopt; }
  else
  {
    const auto returnId = this->mAvailableList.front();
    this->mAvailableList.pop();

    return returnId;
  }
}

EDySuccess FDyDeferredRenderingMesh::UpdateDirectionalLightValueToGpu(
    _MIN_ const TI32 index,
    _MIN_ const DDyUboDirectionalLight& container)
{ // Integrity check
  if (index >= FDyDeferredRenderingMesh::sDirectionalLightCount)
  {
    MDY_LOG_ERROR("Directional light index is out of bound.");
    return DY_FAILURE;
  }

  constexpr TI32 UboElementSize = sizeof(DDyUboDirectionalLight);
  glBindBuffer    (GL_UNIFORM_BUFFER, this->mDirLight);
  glBufferSubData (GL_UNIFORM_BUFFER, UboElementSize * index, UboElementSize, &container);
  glBindBuffer    (GL_UNIFORM_BUFFER, 0);
  return DY_SUCCESS;
}

EDySuccess FDyDeferredRenderingMesh::UnbindDirectionalLight(_MIN_ const TI32 index)
{ // Integrity check
  if (index >= FDyDeferredRenderingMesh::sDirectionalLightCount
      || index <= MDY_INITIALIZE_DEFINT)
  {
    MDY_LOG_ERROR("Directional light index is out of bound.");
    return DY_FAILURE;
  }

  constexpr TI32  UboElementSize = sizeof(DDyUboDirectionalLight);
  constexpr float nullValue[1]   = {0.f};
  glBindBuffer    (GL_UNIFORM_BUFFER, this->mDirLight);
  glClearBufferSubData(GL_UNIFORM_BUFFER, GL_RGBA, UboElementSize * index, UboElementSize, GL_RED, GL_FLOAT, &nullValue);
  glBindBuffer    (GL_UNIFORM_BUFFER, 0);

  this->mAvailableList.push(index);
  return DY_SUCCESS;
}

EDySuccess FDyDeferredRenderingMesh::pInitializeGeometries()
{
  // Make triangle that can represent context.
  glGenVertexArrays(1, &this->mVao);
  glGenBuffers(1, &this->mVbo);
  glBindVertexArray(this->mVao);
  glBindBuffer(GL_ARRAY_BUFFER, this->mVbo);

  std::vector<DDyMeshInfo> mVertexInformations;
  mVertexInformations.emplace_back(DDyVector3{-1, -1, 0}, DDyVector2{0, 0});
  mVertexInformations.emplace_back(DDyVector3{ 3, -1, 0}, DDyVector2{2, 0});
  mVertexInformations.emplace_back(DDyVector3{-1,  3, 0}, DDyVector2{0, 2});
  glBufferData(GL_ARRAY_BUFFER, sizeof(DDyMeshInfo) * 4, &mVertexInformations[0], GL_STATIC_DRAW);
  glBindVertexBuffer(0, this->mVbo, 0, sizeof(DDyMeshInfo));

  // DDyMeshInfo.mPosition (DDyVector3)
  glEnableVertexAttribArray(0);
  glVertexAttribFormat(0, 3, GL_FLOAT, GL_FALSE, offsetof(DDyMeshInfo, mPosition));
  glVertexAttribBinding(0, 0);
  // DDyMeshInfo.mTexCoord (DDyVector2)
  glEnableVertexAttribArray(1);
  glVertexAttribFormat(1, 2, GL_FLOAT, GL_FALSE, offsetof(DDyMeshInfo, mTexCoord));
  glVertexAttribBinding(1, 0);

  glBindVertexArray(0);
  return DY_SUCCESS;
}

EDySuccess FDyDeferredRenderingMesh::pInitializeShaderSetting()
{
  auto& manResc = MDyHeapResource::GetInstance();
  // Make deferred shader
  builtin::FDyBuiltinShaderGLRenderDeferredRendering();
  this->mShaderPtr = manResc.GetShaderResource(builtin::FDyBuiltinShaderGLRenderDeferredRendering::sName.data());

  MDY_ASSERT(this->mShaderPtr, "FDyDeferredRenderingMesh::mShaderPtr must not be nullptr.");
  this->mShaderPtr->UseShader();

  glUniform1i(glGetUniformLocation(this->mShaderPtr->GetShaderProgramId(), "uUnlit"), 0);
  glUniform1i(glGetUniformLocation(this->mShaderPtr->GetShaderProgramId(), "uNormal"), 1);
  glUniform1i(glGetUniformLocation(this->mShaderPtr->GetShaderProgramId(), "uSpecular"), 2);
#ifdef false
  glUniform1i(glGetUniformLocation(this->mShaderPtr->GetShaderProgramId(), "uSsaoOcclusion"), 3);
#endif

  this->mShaderPtr->UnuseShader();
  return DY_SUCCESS;
}

EDySuccess FDyDeferredRenderingMesh::pInitializeUboBuffers()
{
  // Make uniform buffer object buffer space
  glGenBuffers(1, &this->mDirLight);
  glBindBuffer(GL_UNIFORM_BUFFER, this->mDirLight);
  glBufferData(GL_UNIFORM_BUFFER, sizeof(DDyUboDirectionalLight) * FDyDeferredRenderingMesh::sDirectionalLightCount, nullptr, GL_DYNAMIC_DRAW);
  glBindBufferBase(GL_UNIFORM_BUFFER, 1, this->mDirLight);

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
  if (MDY_CHECK_ISNULL(this->mAttachmentPtr_ViewPosition))
  {
    this->mAttachmentPtr_ViewPosition = framebufferManager.GetAttachmentPointer(sAttachment_ViewPosition);
  }

  if (MDY_CHECK_ISNOTNULL(this->mAttachmentPtr_Unlit))        { count += 1; }
  if (MDY_CHECK_ISNOTNULL(this->mAttachmentPtr_Normal))       { count += 1; }
  if (MDY_CHECK_ISNOTNULL(this->mAttachmentPtr_Specular))     { count += 1; }
  if (MDY_CHECK_ISNOTNULL(this->mAttachmentPtr_ViewPosition)) { count += 1; }

  if (count == 4) { return DY_SUCCESS; }
  else            { return DY_FAILURE; }
}

} /// ::dy namespace