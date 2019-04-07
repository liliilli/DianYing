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
#include <Dy/Builtin/Mesh/FDyBtMsUiFontQuad.h>

namespace dy
{

void FDyBtMsUiFontQuad::ConstructBuffer(_MOUT_ PDyMeshInstanceMetaInfo& property) noexcept
{
  property.mSpecifierName         = sName;
  property.mMeshUsage             = EDyMeshUsage::DynamicDraw;

  // (Position, Texture coord) stride.
  //DVec2{}, DVec2{1, 1}, channel, map.
  //DVec2{}, DVec2{1, 0}, channel, map.
  //DVec2{}, DVec2{0, 0}, 
  //DVec2{}, DVec2{0, 1}
  property.mCustomMeshBuffer.emplace_back(0.f);
  property.mCustomMeshBuffer.emplace_back(0.f);
  property.mCustomMeshBuffer.emplace_back(1.f);
  property.mCustomMeshBuffer.emplace_back(1.f);
  property.mCustomMeshBuffer.emplace_back(0.f); // For channel.
  property.mCustomMeshBuffer.emplace_back(0.f); // For map index.

  property.mCustomMeshBuffer.emplace_back(0.f);
  property.mCustomMeshBuffer.emplace_back(0.f);
  property.mCustomMeshBuffer.emplace_back(1.f);
  property.mCustomMeshBuffer.emplace_back(0.f);
  property.mCustomMeshBuffer.emplace_back(0.f);
  property.mCustomMeshBuffer.emplace_back(0.f);

  property.mCustomMeshBuffer.emplace_back(0.f);
  property.mCustomMeshBuffer.emplace_back(0.f);
  property.mCustomMeshBuffer.emplace_back(0.f);
  property.mCustomMeshBuffer.emplace_back(0.f);
  property.mCustomMeshBuffer.emplace_back(0.f);
  property.mCustomMeshBuffer.emplace_back(0.f);

  property.mCustomMeshBuffer.emplace_back(0.f);
  property.mCustomMeshBuffer.emplace_back(0.f);
  property.mCustomMeshBuffer.emplace_back(0.f);
  property.mCustomMeshBuffer.emplace_back(1.f);
  property.mCustomMeshBuffer.emplace_back(0.f);
  property.mCustomMeshBuffer.emplace_back(0.f);

  /*
   *glBufferData(GL_ARRAY_BUFFER,  sizeof(DVec2) * 3, value.data(), GL_DYNAMIC_DRAW);
    glBindVertexBuffer(0, mTestVbo, 0, stride);

    glEnableVertexAttribArray(0);
    glVertexAttribFormat(0, 2, GL_FLOAT, GL_FALSE, 0);
    glVertexAttribBinding(0, 0);

    glEnableVertexAttribArray(1);
    glVertexAttribFormat(1, 2, GL_FLOAT, GL_FALSE, 8);
    glVertexAttribBinding(1, 0);
    
    glEnableVertexAttribArray(2);
    glVertexAttribFormat(2, 2, GL_FLOAT, GL_FALSE, 16);
    glVertexAttribBinding(2, 0);
   */

  property.mVAOBindingInfo.mIsUsingDefaultDyAttributeModel = false;
  property.mVAOBindingInfo.mOffsetByteSize = 0;
  property.mVAOBindingInfo.mStrideByteSize = sizeof(DVec2) * 3;
  property.mVAOBindingInfo.mAttributeFormatList.emplace_back(EGLPixelFormatType::Float, false, 2, 0);
  property.mVAOBindingInfo.mAttributeFormatList.emplace_back(EGLPixelFormatType::Float, false, 2, 8);
  property.mVAOBindingInfo.mAttributeFormatList.emplace_back(EGLPixelFormatType::Float, false, 2, 16);
}

} /// ::dy namespace