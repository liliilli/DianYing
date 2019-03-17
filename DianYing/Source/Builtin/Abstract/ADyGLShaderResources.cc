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
#include <Dy/Builtin/Abstract/AGLShaderResource.h>
#include <Dy/Meta/Information/GLShaderMetaInformation.h>

namespace dy
{

std::any AGLShaderResource::GetMetaInfo()
{
  PDyGLShaderInstanceMetaInfo metaInfo {};
  metaInfo.mSpecifierName = this->mSpecifierName.data();

  if (this->mVertexBuffer.empty() == false)
  {
    metaInfo.GetFragment(EDyShaderFragmentType::Vertex).mBuiltinBuffer = this->mVertexBuffer;
  }
  if (this->mHullBuffer.empty() == false)
  {
    metaInfo.GetFragment(EDyShaderFragmentType::Hull).mBuiltinBuffer = this->mHullBuffer;
  }
  if (this->mDomainBuffer.empty() == false)
  {
    metaInfo.GetFragment(EDyShaderFragmentType::Domain).mBuiltinBuffer = this->mDomainBuffer;
  }
  if (this->mGeometryBuffer.empty() == false)
  {
    metaInfo.GetFragment(EDyShaderFragmentType::Geometry).mBuiltinBuffer = this->mGeometryBuffer;
  }
  if (this->mPixelBuffer.empty() == false)
  {
    metaInfo.GetFragment(EDyShaderFragmentType::Pixel).mBuiltinBuffer = this->mPixelBuffer;
  }
  if (this->mComputeBuffer.empty() == false)
  {
    metaInfo.GetFragment(EDyShaderFragmentType::Compute).mBuiltinBuffer = this->mComputeBuffer;
    metaInfo.mIsComputeShader = true;
  }

  return metaInfo;
}

} /// ::dy namespace