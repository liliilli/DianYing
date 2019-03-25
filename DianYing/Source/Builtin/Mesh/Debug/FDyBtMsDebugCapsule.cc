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
#include <Dy/Builtin/Mesh/Debug/FDyBtMsDebugCapsule.h>

namespace dy::builtin
{

void FDyBtMsDebugCapsule::ConstructBuffer(_MOUT_ PDyMeshInstanceMetaInfo& property) noexcept
{
  property.mSpecifierName = sName;
  property.mMeshUsage     = EDyMeshUsage::DynamicDraw;

  // (Position, Texture coord) stride.
  // DVector3{} : POSITION, 
  property.mCustomMeshBuffer.resize(3 * 100);

  /*
    glBindVertexBuffer(0, mTestVbo, 0, stride);

    glEnableVertexAttribArray(0);
    glVertexAttribFormat(0, 3, GL_FLOAT, GL_FALSE, 0);
    glVertexAttribBinding(0, 0);
   */

  property.mVAOBindingInfo.mIsUsingDefaultDyAttributeModel = false;
  property.mVAOBindingInfo.mOffsetByteSize = 0;
  property.mVAOBindingInfo.mStrideByteSize = sizeof(DVector3);
  property.mVAOBindingInfo.mAttributeFormatList.emplace_back(EDyGLPixelFormatType::Float, false, 3, 0);

  for (TU32 i = 0; i < 50; ++i) { property.mIndiceBuffer.emplace_back(i); }
  property.mIndiceBuffer.emplace_back(0xFFFFFFFF); // For glPrimitiveRestartIndex.
  for (TU32 i = 0; i < 50; ++i) { property.mIndiceBuffer.emplace_back(i + 50); }
}

} /// ::dy::builtin namespace