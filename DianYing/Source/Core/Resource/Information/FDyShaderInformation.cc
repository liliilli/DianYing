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
#include <Dy/Core/Resource/Information/FDyShaderInformation.h>
#include <Dy/Meta/Information/GLShaderMetaInformation.h>

namespace dy
{

FDyShaderInformation::FDyShaderInformation(_MIN_ const PDyGLShaderInstanceMetaInfo& metaInfo)
{
  this->mSpecifierName = metaInfo.mSpecifierName;
  for (int i = 0; i < 6; ++i)
  {
    PDyShaderFragmentInformation shader{};
    shader.mIsEnabledRawLoadShaderCode = metaInfo.mSourceType == EDyResourceSource::Builtin ? true : false;
    const auto& p = metaInfo.GetFragment(static_cast<EDyShaderFragmentType>(i));
    if (p.mExternalFilePath.empty() == true && p.mBuiltinBuffer.empty() == true) { continue; }

    shader.mShaderPath    = p.mExternalFilePath;
    shader.mShaderRawCode = p.mBuiltinBuffer;
    shader.mShaderType    = static_cast<EDyShaderFragmentType>(i);
    this->mShaderFragmentList.emplace_back(shader);
  }
}


} /// ::dy namespace