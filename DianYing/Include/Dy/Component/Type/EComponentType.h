#ifndef GUARD_DY_Core_Resource_Type_EDyComponentType_H
#define GUARD_DY_Core_Resource_Type_EDyComponentType_H

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

namespace dy
{

class CActorScript;
class CLightDirectional;
class CLightPoint;
class CLightSpot;
class CCamera;
class CModelAnimator;
class CModelFilter;
class CModelRenderer;
class CDySoundSource;
class CBasePhysicsCollider;
class CPhysicsColliderSphere;
class CPhysicsColliderBox;
class CyPhysicsColliderCapsule;
class CDySkybox;
class CDyTransform;

enum class EComponentType 
{
  ActorScript,
  Transform,
  DirectionalLight,
  PointLight,
  SpotLight,
  Camera,
  ModelAnimator,
  ModelFilter,
  ModelRenderer,
  SoundSource,
  Rigidbody,
  ColliderSphere,
  ColliderBox,
  ColliderCapsule,
  Skybox,
};

template <EComponentType TType> 
struct TComponentBindingType 
{ 
  using Type = void; 
  TComponentBindingType() { static_assert(std::is_same_v<Type, void>, "Not supported type."); }
};

template <> struct TComponentBindingType<EComponentType::ActorScript> { using Type = CActorScript; };
template <> struct TComponentBindingType<EComponentType::Transform> { using Type = CDyTransform; };
template <> struct TComponentBindingType<EComponentType::DirectionalLight> { using Type = CLightDirectional; };
template <> struct TComponentBindingType<EComponentType::PointLight> { using Type = CLightPoint; };
template <> struct TComponentBindingType<EComponentType::SpotLight> { using Type = CLightSpot; };
template <> struct TComponentBindingType<EComponentType::Camera> { using Type = CCamera; };
template <> struct TComponentBindingType<EComponentType::ModelAnimator> { using Type = CModelAnimator; };
template <> struct TComponentBindingType<EComponentType::ModelFilter> { using Type = CModelFilter; };
template <> struct TComponentBindingType<EComponentType::ModelRenderer> { using Type = CModelRenderer; };
template <> struct TComponentBindingType<EComponentType::SoundSource> { using Type = CDySoundSource; };
template <> struct TComponentBindingType<EComponentType::Rigidbody> { using Type = CDyPhysicsRigidbody; };
template <> struct TComponentBindingType<EComponentType::ColliderSphere> { using Type = CPhysicsColliderSphere; };
template <> struct TComponentBindingType<EComponentType::ColliderBox> { using Type = CPhysicsColliderBox; };
template <> struct TComponentBindingType<EComponentType::ColliderCapsule> { using Type = CyPhysicsColliderCapsule; };
template <> struct TComponentBindingType<EComponentType::Skybox> { using Type = CDySkybox; };

template <typename TType> 
struct TComponentUnbindingType;

template <> struct TComponentUnbindingType<CActorScript> { static constexpr EComponentType Value = EComponentType::ActorScript; };
template <> struct TComponentUnbindingType<CDyTransform> { static constexpr EComponentType Value = EComponentType::Transform; };
template <> struct TComponentUnbindingType<CLightDirectional> { static constexpr EComponentType Value = EComponentType::DirectionalLight; };
template <> struct TComponentUnbindingType<CLightPoint> { static constexpr EComponentType Value = EComponentType::PointLight; };
template <> struct TComponentUnbindingType<CLightSpot> { static constexpr EComponentType Value = EComponentType::SpotLight; };
template <> struct TComponentUnbindingType<CCamera> { static constexpr EComponentType Value = EComponentType::Camera; };
template <> struct TComponentUnbindingType<CModelAnimator> { static constexpr EComponentType Value = EComponentType::ModelAnimator; };
template <> struct TComponentUnbindingType<CModelFilter> { static constexpr EComponentType Value = EComponentType::ModelFilter; };
template <> struct TComponentUnbindingType<CModelRenderer> { static constexpr EComponentType Value = EComponentType::ModelRenderer; };
template <> struct TComponentUnbindingType<CDySoundSource> { static constexpr EComponentType Value = EComponentType::SoundSource; };
template <> struct TComponentUnbindingType<CDyPhysicsRigidbody> { static constexpr EComponentType Value = EComponentType::Rigidbody; };
template <> struct TComponentUnbindingType<CPhysicsColliderSphere> { static constexpr EComponentType Value = EComponentType::ColliderSphere; };
template <> struct TComponentUnbindingType<CPhysicsColliderBox> { static constexpr EComponentType Value = EComponentType::ColliderBox; };
template <> struct TComponentUnbindingType<CyPhysicsColliderCapsule> { static constexpr EComponentType Value = EComponentType::ColliderCapsule; };
template <> struct TComponentUnbindingType<CDySkybox> { static constexpr EComponentType Value = EComponentType::Skybox; };

} /// ::dy namespace

#endif /// GUARD_DY_Core_Resource_Type_EDyComponentType_H