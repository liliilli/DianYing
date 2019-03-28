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
#include <Dy/Builtin/Mesh/Debug/FDyBtMsDebugAABB.h>
#include <Dy/Builtin/Constant/GeneralValue.h>

namespace dy::builtin
{

void FDyBtMsDebugAABB::ConstructBuffer(_MOUT_ PDyMeshInstanceMetaInfo& property) noexcept
{
  property.mSpecifierName = sName;
  property.mMeshUsage     = EDyMeshUsage::DynamicDraw;

  // (Position, Texture coord) stride.
  // DVec3{} : POSITION, 
  /*
   *   1----0    Indice must be 
   *  /|    |\    0 1 2 3 F 
   * 2--------3   0 3 6 5 F 0 5 4 1 F 1 4 7 2 F 2 7 6 3 F 
   * | 4----5 |   4 5 6 7
   * |/      \|
   * 7--------6
   */
  property.mCustomMeshBuffer.resize(8 * 3);

  /*
    glBindVertexBuffer(0, mTestVbo, 0, stride);

    glEnableVertexAttribArray(0);
    glVertexAttribFormat(0, 3, GL_FLOAT, GL_FALSE, 0);
    glVertexAttribBinding(0, 0);
   */

  const auto f = kIndiceSeparator;
  property.mIndiceBuffer = decltype(property.mIndiceBuffer){
      0,1,2,3,f,
      0,3,6,5,f,0,5,4,1,f,1,4,7,2,f,2,7,6,3,f,
      5,6,7,4
  };

  property.mVAOBindingInfo.mIsUsingDefaultDyAttributeModel = false;
  property.mVAOBindingInfo.mOffsetByteSize = 0;
  property.mVAOBindingInfo.mStrideByteSize = sizeof(DVec3);
  property.mVAOBindingInfo.mAttributeFormatList.emplace_back(EDyGLPixelFormatType::Float, false, 3, 0);
}

} /// ::dy::builtin namespace