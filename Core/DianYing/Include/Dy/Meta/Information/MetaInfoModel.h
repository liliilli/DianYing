#ifndef GUARD_DY_META_INFORMATION_METAINFOMODEL_H
#define GUARD_DY_META_INFORMATION_METAINFOMODEL_H
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

#include <nlohmann/json_fwd.hpp>
#include <Dy/Meta/Information/CommonResourceMetaInfo.h>
#include <Dy/Helper/Type/DVector3.h>

namespace dy
{
  
/// @struct PDyModelInstanceMetaInfo
/// @brief Descriptor instance which saves information to create model meshes.
struct PDyModelInstanceMetaInfo final : public PDyCommonResourceMetaInfo
{
  using TBuiltinMeshSpecifierList = std::vector<std::string>;
  struct DMesh final
  {
    std::string mMeshSpecifier      = MDY_INITIALIZE_EMPTYSTR;
    std::string mMaterialSpecifier  = MDY_INITIALIZE_EMPTYSTR;
    bool        mInstanced = false;

    DMesh(const std::string_view& mesh, const std::string_view& mat, bool iInstanced = false) 
      : mMeshSpecifier{(mesh)}, mMaterialSpecifier{(mat)}, mInstanced{iInstanced} {};
    DMesh(const char* mesh, const char* mat, bool iInstanced = false) 
      : mMeshSpecifier{mesh}, mMaterialSpecifier{mat}, mInstanced{iInstanced} {};
    DMesh(const std::string& mesh, const std::string& mat, bool iInstanced = false) 
      : mMeshSpecifier{mesh}, mMaterialSpecifier{mat}, mInstanced{iInstanced} {};
    DMesh() = default;
  };
  using TExternalMeshList = std::vector<DMesh>;

  struct DSkeleton final
  {
    bool        mIsUsingSkeleton    = false;
    std::string mSkeletonSpecifier  = MDY_INITIALIZE_EMPTYSTR;
  };

  struct DTransform final
  {
    DVec3 mPosition  = DVec3{0, 0, 0};
    DVec3 mRotation  = DVec3{0, 0, 0};
    DVec3 mScale     = DVec3{1, 1, 1};
  };

  std::string               mSpecifierName            = MDY_INITIALIZE_EMPTYSTR;
  //TBuiltinMeshSpecifierList mBuiltinMeshSpecifierList = {};
  //bool                      mIsUsingBuiltinMesh       = false;

  TExternalMeshList mMeshList = {};
  DSkeleton         mSkeleton;
  DTransform        mTransform;
};

/// @brief Serialization function
void to_json(_MINOUT_ nlohmann::json& j, _MIN_ const PDyModelInstanceMetaInfo& p);
/// @brief Deserialization function
void from_json(_MIN_ const nlohmann::json& j, _MINOUT_ PDyModelInstanceMetaInfo& p);

/// @brief Serialization function
void to_json(_MINOUT_ nlohmann::json& j, _MIN_ const PDyModelInstanceMetaInfo::DMesh& p);
/// @brief Deserialization function
void from_json(_MIN_ const nlohmann::json& j, _MINOUT_ PDyModelInstanceMetaInfo::DMesh& p);

/// @brief Serialization function
void to_json(_MINOUT_ nlohmann::json& j, _MIN_ const PDyModelInstanceMetaInfo::DSkeleton& p);
/// @brief Deserialization function
void from_json(_MIN_ const nlohmann::json& j, _MINOUT_ PDyModelInstanceMetaInfo::DSkeleton& p);

/// @brief Serialization function
void to_json(_MINOUT_ nlohmann::json& j, _MIN_ const PDyModelInstanceMetaInfo::DTransform& p);
/// @brief Deserialization function
void from_json(_MIN_ const nlohmann::json& j, _MINOUT_ PDyModelInstanceMetaInfo::DTransform& p);

} /// ::dy namespace

#endif /// GUARD_DY_META_INFORMATION_METAINFOMODEL_H