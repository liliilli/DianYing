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
#include <Dy/Core/Resource/Information/FInformationMesh.h>

#include <nlohmann/json.hpp>
#include <Dy/Core/Resource/Type/ModelMesh/DMeshCompressedBuffer.h>
#include <Dy/Helper/Library/HelperJson.h>
#include <future>
#include <Dy/Helper/Library/HelperFilesystem.h>

namespace dy
{

FInformationMesh::FInformationMesh(_MIN_ const PDyMeshInstanceMetaInfo& metaInfo) :
    mSpecifierName{metaInfo.mSpecifierName}
{
  // If Mesh meta info is from external, load mesh resources and store it.
  if (metaInfo.mSourceType == EDyResourceSource::External)
  {
    nlohmann::json jsonMeshAtlas;
    if (metaInfo.mIsCompressed == true)
    {
      const auto buffer = DyCreateMeshBufferInfoFromDyMeshFile(metaInfo.mExternalPath);
      MDY_NOT_IMPLEMENTED_ASSERT();
    }
    else
    { // dybMesh file.
      /* i(char) icount(unsigned_32bit) ...
       * v(char) vcount(unsigned_32bit) pos(float3) nrm(float3) uv0(float2) uv1(float2) tan(float3) bit(float3) bone(float4) wgh(float4)...
       */
      MDY_ASSERT_MSG_FORCE(IsFileExist(metaInfo.mExternalPath) == true, "Mesh file could not find.");

      std::FILE* fd = fopen(metaInfo.mExternalPath.c_str(), "rb");
      // Read index list.
      {
        unsigned numIndex;
        fread(&numIndex, sizeof(unsigned), 1, fd);
        this->mProperty.mIndiceBuffer.resize(numIndex);
        fread(this->mProperty.mIndiceBuffer.data(), sizeof(TU32), numIndex, fd);
      }
      // Read vertex list.
      {
        unsigned numVertex;
        fread(&numVertex, sizeof(unsigned), 1, fd);
        this->mProperty.mDefaultMeshBuffer.mVertexList.resize(numVertex);
        for (auto& vi : this->mProperty.mDefaultMeshBuffer.mVertexList)
        {
          fread(&vi.mPosition[0], sizeof(TF32), 3, fd);   // Position
          fread(&vi.mNormal[0], sizeof(TF32), 3, fd);     // Normal
          fread(&vi.mTexCoord0[0], sizeof(TF32), 2, fd);  // UV0 (u, v)
          fread(&vi.mTexCoord1[0], sizeof(TF32), 2, fd);  // UV1 (u, v)
          fread(&vi.mTangent[0], sizeof(TF32), 3, fd);    // Tangent 
          fread(&vi.mBitangent[0], sizeof(TF32), 3, fd);  // Bitangent
          fread(&vi.mBoneId[0], sizeof(TI32), 4, fd);     // BoneID 
          fread(&vi.mWeights[0], sizeof(TF32), 4, fd);    // Weights
        }
      }
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

const std::string& FInformationMesh::GetSpecifierName() const noexcept
{
  return this->mSpecifierName;
}

const DMeshData& FInformationMesh::GetMeshInformationList() const noexcept
{
  return this->mProperty;
}

} /// ::dy namespace