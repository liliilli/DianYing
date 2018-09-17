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
#include <Dy/Core/Component/Object/RenderingMesh.h>

#include <Dy/Core/Component/Internal/ShaderType.h>

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
  auto& manInfo = MDyDataInformation::GetInstance();
  auto& manResc = MDyHeapResource::GetInstance();

  // Make triangle that can represent context.
  glGenVertexArrays(1, &this->mVao);
  glGenBuffers(1, &this->mVbo);
  glBindVertexArray(this->mVao);
  glBindBuffer(GL_ARRAY_BUFFER, this->mVbo);

  std::vector<DDyMeshInfo> mVertexInformations;
  mVertexInformations.emplace_back(DDyVector3{-1, -1, 0}, DDyVector2{0, 0});
  mVertexInformations.emplace_back(DDyVector3{ 1, -1, 0}, DDyVector2{1, 0});
  mVertexInformations.emplace_back(DDyVector3{ 1,  1, 0}, DDyVector2{1, 1});
  mVertexInformations.emplace_back(DDyVector3{-1,  1, 0}, DDyVector2{0, 1});
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

  // Make deferred shader
  PDyShaderConstructionDescriptor shaderDesc;
  {
    shaderDesc.mShaderName = "dyDeferredShader";
    {
      PDyShaderFragmentInformation vertexShaderInfo;
      vertexShaderInfo.mShaderType = EDyShaderFragmentType::Vertex;
      vertexShaderInfo.mShaderPath = "./glDeferred.vert";
      shaderDesc.mShaderFragments.emplace_back(vertexShaderInfo);
    }
    {
      PDyShaderFragmentInformation fragmentShaderInfo;
      fragmentShaderInfo.mShaderType = EDyShaderFragmentType::Pixel;
      fragmentShaderInfo.mShaderPath = "./glDeferred.frag";
      shaderDesc.mShaderFragments.emplace_back(fragmentShaderInfo);
    }
  }
  MDY_CALL_ASSERT_SUCCESS(manInfo.CreateShaderInformation(shaderDesc));
  MDY_CALL_ASSERT_SUCCESS(manResc.CreateShaderResource(shaderDesc.mShaderName));

  this->mShaderPtr = manResc.GetShaderResource(shaderDesc.mShaderName);

  PHITOS_ASSERT(this->mShaderPtr, "FDyDeferredRenderingMesh::mShaderPtr must not be nullptr.");
  this->mShaderPtr->UseShader();

  glUniform1i(glGetUniformLocation(this->mShaderPtr->GetShaderProgramId(), "uUnlit"), 0);
  glUniform1i(glGetUniformLocation(this->mShaderPtr->GetShaderProgramId(), "uNormal"), 1);
  glUniform1i(glGetUniformLocation(this->mShaderPtr->GetShaderProgramId(), "uSpecular"), 2);

  this->mShaderPtr->UnuseShader();
}

FDyDeferredRenderingMesh::~FDyDeferredRenderingMesh()
{
  if (this->mVbo) glDeleteBuffers(1, &this->mVbo);
  if (this->mVao) glDeleteVertexArrays(1, &this->mVao);

#ifdef false
  auto& manResc = MDyHeapResource::GetInstance();
#endif
}

void FDyDeferredRenderingMesh::RenderScreen()
{
  PHITOS_ASSERT(this->mShaderPtr, "FDyDeferredRenderingMesh::mShaderPtr must not be nullptr.");

  this->mShaderPtr->UseShader();
  glBindVertexArray(this->mVao);

  const auto& renderingManager = MDyRendering::GetInstance();
  // Bind g-buffers as textures.
  for (int32_t i = 0; i < renderingManager.mAttachmentBuffers.size(); ++i)
  {
    glActiveTexture(GL_TEXTURE0 + i);
    glBindTexture(GL_TEXTURE_2D, renderingManager.mAttachmentBuffers[i]);
  }

  glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
  glBindVertexArray(0);
  this->mShaderPtr->UnuseShader();
}

} /// ::dy namespace