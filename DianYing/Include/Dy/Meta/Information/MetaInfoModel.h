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
#include <Dy/Helper/Type/Vector3.h>

namespace dy
{
  
///
/// @struct PDyModelInstanceMetaInfo
/// @brief Descriptor instance which saves information to create model meshes.
///
struct PDyModelInstanceMetaInfo final : public PDyCommonResourceMetaInfo
{
#ifdef false
  struct DFlags
  {
    bool mIsUsingSkeleton = false;
    bool mIsUsingMaterial = false;
  };
  struct DInitialTransform
  {
    DDyVector3 mPosition;
    DDyVector3 mRotation;
    DDyVector3 mScale;
  };
#endif
  using TBuiltinMeshSpecifierList = std::vector<std::string>;

  std::string               mSpecifierName            = MDY_INITIALIZE_EMPTYSTR;
  TBuiltinMeshSpecifierList mBuiltinMeshSpecifierList = {};
  bool                      mIsUsingBuiltinMesh       = false;

  std::string               mExternalPath       = MDY_INITIALIZE_EMPTYSTR;
#ifdef false
  std::string               mSkeletonSpecifier  = MDY_INITIALIZE_EMPTYSTR;
  DFlags                    mModelFlags {};
  DInitialTransform         mInitialTransform {};
#endif
};

/// @brief Serialization function
void to_json(_MINOUT_ nlohmann::json& j, _MIN_ const PDyModelInstanceMetaInfo& p);
/// @brief Deserialization function
void from_json(_MIN_ const nlohmann::json& j, _MINOUT_ PDyModelInstanceMetaInfo& p);

#ifdef false
/// @brief Serialization function
void to_json(_MINOUT_ nlohmann::json& j, _MIN_ const PDyModelInstanceMetaInfo::DFlags& p);
/// @brief Deserialization function
void from_json(_MIN_ const nlohmann::json& j, _MINOUT_ PDyModelInstanceMetaInfo::DFlags& p);

/// @brief Serialization function
void to_json(_MINOUT_ nlohmann::json& j, _MIN_ const PDyModelInstanceMetaInfo::DInitialTransform& p);
/// @brief Deserialization function
void from_json(_MIN_ const nlohmann::json& j, _MINOUT_ PDyModelInstanceMetaInfo::DInitialTransform& p);
#endif

} /// ::dy namespace

#endif /// GUARD_DY_META_INFORMATION_METAINFOMODEL_H