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
#include <Dy/Core/Resource/Object/Grid.h>
#include <Dy/Builtin/ShaderGl/Editor/RenderGrid.h>
#include <Dy/Helper/Type/DVector3.h>

namespace dy
{

FDyGrid::FDyGrid()
{
  glGenVertexArrays(1, &this->mVao);
  glGenBuffers(1, &this->mVbo);
  glBindVertexArray(this->mVao);

  std::vector<DVector3> mPointers;
  {
    const float start     = -6.f;
    const float width     = std::abs(start) * 2.f;
    const float interval  = width / mStep;

    for (int32_t i = 0; i <= mStep; ++i)
    {
      // Horizontal
      mPointers.emplace_back(start,         0.f, start + interval * i);
      mPointers.emplace_back(start + width, 0.f, start + interval * i);
      // Vertical
      mPointers.emplace_back(start + interval * i, 0.f,         start);
      mPointers.emplace_back(start + interval * i, 0.f, start + width);
    }
  }

  glBindBuffer(GL_ARRAY_BUFFER, this->mVbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(DVector3) * mPointers.size(), mPointers.data(), GL_STATIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(DVector3), nullptr);
  glEnableVertexAttribArray(0);
  glBindVertexArray(0);

  builtin::FDyBuiltinShaderGLRenderGrid();
  //this->mShaderPtr = MDyIOResource_Deprecated::GetInstance().GetShaderResource(builtin::FDyBuiltinShaderGLRenderGrid::sName.data());
}

void FDyGrid::RenderGrid() noexcept
{
#ifdef false
  glDisable(GL_TEXTURE_2D);
  glDisable(GL_BLEND);

  this->mShaderPtr->UseShader();
  glBindVertexArray(this->mVao);

  // @todo temporal
  const auto viewMatrix = glGetUniformLocation(this->mShaderPtr->GetShaderProgramId(), "viewMatrix");
  const auto projMatirx = glGetUniformLocation(this->mShaderPtr->GetShaderProgramId(), "projectionMatrix");

  auto& sceneManager = MWorld::GetInstance();
  auto* camera = sceneManager.GetMainCameraPtr();
  if (camera)
  {
    glUniformMatrix4fv(viewMatrix, 1, GL_FALSE, &camera->GetViewMatrix()[0].X);
    glUniformMatrix4fv(projMatirx, 1, GL_FALSE, &camera->GetProjectionMatrix()[0].X);
  }

  glDrawArrays(GL_LINES, 0, (mStep + 1) * 4);
  glBindVertexArray(0);
  this->mShaderPtr->UnuseShader();

  glEnable(GL_BLEND);
#endif
}

} /// ::dy namespace