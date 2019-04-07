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
#include <Dy/Core/Rendering/Wrapper/XGLWrapper.h>
#include <Dy/Meta/Type/Mesh/DGlVaoBindInformation.h>
#include <Dy/Core/Resource/Information/FInformationModel.h>

namespace dy
{

const DGlVaoBindInformation& XGLWrapper::GetDefaultAttributeFormatDescriptor() noexcept
{
  static DGlVaoBindInformation descriptor;
  static bool mIsInitialized = false;
  if (mIsInitialized == false)
  {
    descriptor.mIsUsingDefaultDyAttributeModel = false;
    descriptor.mOffsetByteSize = 0;
    descriptor.mStrideByteSize = sizeof(DDefaultVertexInfo);
    descriptor.mAttributeFormatList.emplace_back(EGLPixelFormatType::Float, false, 3, offsetof(DDefaultVertexInfo, mPosition));
    descriptor.mAttributeFormatList.emplace_back(EGLPixelFormatType::Float, false, 3, offsetof(DDefaultVertexInfo, mNormal));
    descriptor.mAttributeFormatList.emplace_back(EGLPixelFormatType::Float, false, 2, offsetof(DDefaultVertexInfo, mTexCoord0));
    descriptor.mAttributeFormatList.emplace_back(EGLPixelFormatType::Float, false, 2, offsetof(DDefaultVertexInfo, mTexCoord1));
    descriptor.mAttributeFormatList.emplace_back(EGLPixelFormatType::Float, false, 3, offsetof(DDefaultVertexInfo, mTangent));
    descriptor.mAttributeFormatList.emplace_back(EGLPixelFormatType::Float, false, 3, offsetof(DDefaultVertexInfo, mBitangent));
    descriptor.mAttributeFormatList.emplace_back(EGLPixelFormatType::Int,   false, 4, offsetof(DDefaultVertexInfo, mBoneId));
    descriptor.mAttributeFormatList.emplace_back(EGLPixelFormatType::Float, false, 4, offsetof(DDefaultVertexInfo, mWeights));
    mIsInitialized = true;
  }

  return descriptor;
}

} /// ::dy namespace