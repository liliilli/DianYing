#ifndef GAURD_DY_META_INFORMATION_PHYSICS_DColliderMetaInfo_H
#define GAURD_DY_META_INFORMATION_PHYSICS_DColliderMetaInfo_H
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

#include <Dy/Helper/Type/DClamp.h>
#include <Dy/Helper/Type/DVector3.h>

namespace dy
{

/// @struct PDySphereColliderMetaInfo
/// @brief Sphere collider meta information.
/// @reference https://docs.nvidia.com/gameworks/content/gameworkslibrary/physx/guide/Manual/Geometry.html
struct PDySphereColliderMetaInfo final
{
  /// A PxSphereGeometry is specified by one attribute, its radius, and is centered at the origin.
  DClamp<TF32, 0, 1'000'000> mRadius = 0;
};

void to_json  (_MINOUT_ nlohmann::json& j, _MIN_ const PDySphereColliderMetaInfo& p);
void from_json(_MIN_ const nlohmann::json& j, _MINOUT_ PDySphereColliderMetaInfo& p);

/// @struct PDyCapsulesColliderMetaInfo
/// @brief Sphere collider meta information.
/// A PxCapsuleGeometry is centered at the origin. 
/// It is specified by a radius and a half-height value by which its axis extends along the positive and negative Y-axis.
/// @reference https://docs.nvidia.com/gameworks/content/gameworkslibrary/physx/guide/Manual/Geometry.html
struct PDyCapsulesColliderMetaInfo final
{
  // We have to retate quater-sphere to Z-axis because default capsule is forwarding to X-axis.
  DClamp<TF32, 0, 1'000'000> mHalfHeight = 0;
  DClamp<TF32, 0, 1'000'000> mRadius = 0;
};

void to_json  (_MINOUT_ nlohmann::json& j, _MIN_ const PDyCapsulesColliderMetaInfo& p);
void from_json(_MIN_ const nlohmann::json& j, _MINOUT_ PDyCapsulesColliderMetaInfo& p);

/// @struct PDyBoxColliderMetaInfo
/// @brief Box collider meta information.
/// A PxBoxGeometry has three attributes, the three extents halved: 
/// It is specified by half-height values by which its axis extends along the positive and negative Y-axis.
/// @reference https://docs.nvidia.com/gameworks/content/gameworkslibrary/physx/guide/Manual/Geometry.html
struct PDyBoxColliderMetaInfo final
{
  // We have to retate quater-sphere to Z-axis because default capsule is forwarding to X-axis.
  DVector3 mHalfExtent{};
};

void to_json  (_MINOUT_ nlohmann::json& j, _MIN_ const PDyBoxColliderMetaInfo& p);
void from_json(_MIN_ const nlohmann::json& j, _MINOUT_ PDyBoxColliderMetaInfo& p);

} /// ::dy namespace

#endif /// GAURD_DY_META_INFORMATION_PHYSICS_DColliderMetaInfo_H