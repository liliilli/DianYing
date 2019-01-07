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
#include <Dy/Core/Rendering/Wrapper/FDyGLWrapper.h>
#include <Dy/Meta/Type/Mesh/DDyGLVaoBindInformation.h>
#include <Dy/Core/Resource/Information/FDyModelInformation.h>

namespace dy
{

const DDyGLVaoBindInformation& FDyGLWrapper::GetDefaultAttributeFormatDescriptor() noexcept
{
  static DDyGLVaoBindInformation descriptor;
  static bool mIsInitialized = false;
  if (mIsInitialized == false)
  {
    descriptor.mIsUsingDefaultDyAttributeModel = false;
    descriptor.mOffsetByteSize = 0;
    descriptor.mStrideByteSize = sizeof(DDyVertexInformation);
    descriptor.mAttributeFormatList.emplace_back(EDyGLPixelFormatType::Float, false, 3, offsetof(DDyVertexInformation, mPosition));
    descriptor.mAttributeFormatList.emplace_back(EDyGLPixelFormatType::Float, false, 3, offsetof(DDyVertexInformation, mNormal));
    descriptor.mAttributeFormatList.emplace_back(EDyGLPixelFormatType::Float, false, 2, offsetof(DDyVertexInformation, mTexCoords));
    descriptor.mAttributeFormatList.emplace_back(EDyGLPixelFormatType::Int,   false, 4, offsetof(DDyVertexInformation, mVertexBoneData.mBoneId));
    descriptor.mAttributeFormatList.emplace_back(EDyGLPixelFormatType::Float, false, 4, offsetof(DDyVertexInformation, mVertexBoneData.mWeights));
    mIsInitialized = true;
  }

  return descriptor;
}

} /// ::dy namespace