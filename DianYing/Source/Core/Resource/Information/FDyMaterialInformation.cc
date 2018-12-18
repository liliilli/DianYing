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
#include <Dy/Core/Resource/Information/FDyMaterialInformation.h>
#include <Dy/Meta/Information/MetaInfoMaterial.h>
#include <Dy/Management/IO/MDyIOData.h>
#include <Dy/Helper/System/Idioms.h>

namespace dy
{

FDyMaterialInformation::FDyMaterialInformation(_MIN_ const PDyMaterialInstanceMetaInfo& metaInfo) :
    mSpecifierName{metaInfo.mSpecifierName},
    mBlendMode{metaInfo.mBlendMode}
{
  // Get informations.
  const auto& ioData = MDyIOData::GetInstance();
  this->mPtrShaderInfo = ioData.GetPtrInformation<EDyResourceType::GLShader>(metaInfo.mShaderSpecifier);
  for (const auto& textureSpecifierName : metaInfo.mTextureNames)
  {
    if (textureSpecifierName.empty() == true) { break; }
    this->mPtrTextureInfoList.emplace_back(ioData.GetPtrInformation<EDyResourceType::Texture>(textureSpecifierName));
  }

  this->mBinderShaderInfo.TryRequireResource(metaInfo.mShaderSpecifier);
  MDY_ASSERT(this->mBinderShaderInfo.IsResourceExist() == true, "Unexpected error occurred.");
  for (const auto& textureSpecifierName : metaInfo.mTextureNames)
  {
    if (textureSpecifierName.empty() == true) { break; }
    DySafeUniquePtrEmplaceBack(this->mBinderTextureInfoList, textureSpecifierName);
  }
}

FDyMaterialInformation::~FDyMaterialInformation()
{
  MDY_NOT_IMPLEMENTED_ASSERT();
}

} /// ::dy namespace