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
#include <Dy/Core/Resource/Information/FDyMeshInformation.h>

#include <nlohmann/json.hpp>
#include <Dy/Core/Resource/Type/ModelMesh/DDyMeshCompressedBuffer.h>
#include <Dy/Helper/Library/HelperJson.h>

namespace dy
{

FDyMeshInformation::FDyMeshInformation(_MIN_ const PDyMeshInstanceMetaInfo& metaInfo) :
    mSpecifierName{metaInfo.mSpecifierName}
{
  // If Mesh meta info is from external, load mesh resources and store it.
  if (metaInfo.mSourceType == EDyResourceSource::External)
  {
    nlohmann::json jsonMeshAtlas;
    if (metaInfo.mIsCompressed == true)
    {
      const auto buffer = DyCreateMeshBufferInfoFromDyMeshFile(metaInfo.mExternalPath);
    }
    else
    {
      const auto optMeshJson = DyGetJsonAtlasFromFile(metaInfo.mExternalPath);
      MDY_ASSERT_FORCE(optMeshJson.has_value() == true, "Failed to load raw mesh information file.");
      this->mProperty = optMeshJson.value().get<decltype(mProperty)>();
    }
  }
  else
  { // If not, just copy metaInfo.
    this->mProperty.mVAOBindingInfo = metaInfo.mVAOBindingInfo;
    this->mProperty.mMeshUsage      = metaInfo.mMeshUsage;
    this->mProperty.mIndiceBuffer   = metaInfo.mIndiceBuffer;
    this->mProperty.mDefaultMeshBuffer  = metaInfo.mDefaultMeshBuffer;
    this->mProperty.mCustomMeshBuffer   = metaInfo.mCustomMeshBuffer;
  }
}

const std::string& FDyMeshInformation::GetSpecifierName() const noexcept
{
  return this->mSpecifierName;
}

const DDyMeshData& FDyMeshInformation::GetMeshInformationList() const noexcept
{
  return this->mProperty;
}

} /// ::dy namespace