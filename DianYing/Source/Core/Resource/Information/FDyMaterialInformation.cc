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
#include <Dy/Core/Resource/Information/FDyMaterialInformation.h>
#include <Dy/Meta/Information/MetaInfoMaterial.h>
#include <Dy/Helper/System/Idioms.h>

namespace dy
{

FDyMaterialInformation::FDyMaterialInformation(_MIN_ const PDyMaterialInstanceMetaInfo& metaInfo) :
    mSpecifierName{metaInfo.mSpecifierName},
    mBlendMode{metaInfo.mBlendMode}
{
  // Bind informations RI.
  this->mBinderShaderInfo.TryRequireResource(metaInfo.mShaderSpecifier);
  MDY_ASSERT_MSG(this->mBinderShaderInfo.IsResourceExist() == true, "Unexpected error occurred.");
  for (const auto& bindingTextureItem : metaInfo.mTextureNames)
  {
    if (bindingTextureItem.mTextureSpecifier.empty() == true) { break; }
    SafeUniquePtrEmplaceBack(this->mBinderTextureInfoList, bindingTextureItem.mTextureSpecifier);
  }
}

} /// ::dy namespace