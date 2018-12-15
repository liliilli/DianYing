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

#include <filesystem>
#include <Dy/Meta/Information/GLShaderMetaInformation.h>
#include <Dy/Helper/IoHelper.h>

namespace dy
{

FDyShaderInformation::FDyShaderInformation(_MIN_ const PDyGLShaderInstanceMetaInfo& metaInfo)
{
  this->mSpecifierName = metaInfo.mSpecifierName;
  for (int i = 0; i < 6; ++i)
  {
    PDyShaderFragmentInformation shader{};
    shader.mIsEnabledRawLoadShaderCode_Deprecated = metaInfo.mSourceType == EDyResourceSource::Builtin ? true : false;
    const auto& p = metaInfo.GetFragment(static_cast<EDyShaderFragmentType>(i));
    if (p.mExternalFilePath.empty() == true && p.mBuiltinBuffer.empty() == true) { continue; }
    if (p.mExternalFilePath.empty() == false)
    {
      MDY_ASSERT(std::filesystem::exists(p.mExternalFilePath) == true, "OpenGL Shader external file path exist but not valid.");
      auto ptrBuffer = DyReadBinaryFileAll(p.mExternalFilePath);
      MDY_ASSERT(ptrBuffer.has_value() == true, "Unexpected error occurred while reading file.");
      shader.mShaderFragmentCode = ptrBuffer.value().data();
    }
    else
    {
      shader.mShaderFragmentCode = p.mBuiltinBuffer;
    }

    shader.mShaderType    = static_cast<EDyShaderFragmentType>(i);
    this->mShaderFragmentList.emplace_back(shader);
  }
}


} /// ::dy namespace