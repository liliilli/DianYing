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

class CDyActorScript;
class CDyDirectionalLight;
class CDyCamera;
class CDyModelAnimator;
class CDyModelFilter;
class CDyModelRenderer;
class CDySoundSource;
class CDyPhysicsCollider;
class CDyPhysicsColliderSphere;
class CDyPhysicsColliderBox;
class CDyPhysicsColliderCapsule;
class CDySkybox;

enum class EDyComponentType 
{
  ActorScript,
  Transform,
  DirectionalLight,
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

template <EDyComponentType TType> 
struct TComponentBindingType 
{ 
  using Type = void; 
  TComponentBindingType() { static_assert(std::is_same_v<Type, void>, "Not supported type."); }
};

template <> struct TComponentBindingType<EDyComponentType::ActorScript> { using Type = CDyActorScript; };
template <> struct TComponentBindingType<EDyComponentType::Transform> { using Type = CDyTransform; };
template <> struct TComponentBindingType<EDyComponentType::DirectionalLight> { using Type = CDyDirectionalLight; };
template <> struct TComponentBindingType<EDyComponentType::Camera> { using Type = CDyCamera; };
template <> struct TComponentBindingType<EDyComponentType::ModelAnimator> { using Type = CDyModelAnimator; };
template <> struct TComponentBindingType<EDyComponentType::ModelFilter> { using Type = CDyModelFilter; };
template <> struct TComponentBindingType<EDyComponentType::ModelRenderer> { using Type = CDyModelRenderer; };
template <> struct TComponentBindingType<EDyComponentType::SoundSource> { using Type = CDySoundSource; };
template <> struct TComponentBindingType<EDyComponentType::Rigidbody> { using Type = CDyPhysicsRigidbody; };
template <> struct TComponentBindingType<EDyComponentType::ColliderSphere> { using Type = CDyPhysicsColliderSphere; };
template <> struct TComponentBindingType<EDyComponentType::ColliderBox> { using Type = CDyPhysicsColliderBox; };
template <> struct TComponentBindingType<EDyComponentType::ColliderCapsule> { using Type = CDyPhysicsColliderCapsule; };
template <> struct TComponentBindingType<EDyComponentType::Skybox> { using Type = CDySkybox; };

template <typename TType> 
struct TComponentUnbindingType;

template <> struct TComponentUnbindingType<CDyActorScript> { static constexpr EDyComponentType Value = EDyComponentType::ActorScript; };
template <> struct TComponentUnbindingType<CDyTransform> { static constexpr EDyComponentType Value = EDyComponentType::Transform; };
template <> struct TComponentUnbindingType<CDyDirectionalLight> { static constexpr EDyComponentType Value = EDyComponentType::DirectionalLight; };
template <> struct TComponentUnbindingType<CDyCamera> { static constexpr EDyComponentType Value = EDyComponentType::Camera; };
template <> struct TComponentUnbindingType<CDyModelAnimator> { static constexpr EDyComponentType Value = EDyComponentType::ModelAnimator; };
template <> struct TComponentUnbindingType<CDyModelFilter> { static constexpr EDyComponentType Value = EDyComponentType::ModelFilter; };
template <> struct TComponentUnbindingType<CDyModelRenderer> { static constexpr EDyComponentType Value = EDyComponentType::ModelRenderer; };
template <> struct TComponentUnbindingType<CDySoundSource> { static constexpr EDyComponentType Value = EDyComponentType::SoundSource; };
template <> struct TComponentUnbindingType<CDyPhysicsRigidbody> { static constexpr EDyComponentType Value = EDyComponentType::Rigidbody; };
template <> struct TComponentUnbindingType<CDyPhysicsColliderSphere> { static constexpr EDyComponentType Value = EDyComponentType::ColliderSphere; };
template <> struct TComponentUnbindingType<CDyPhysicsColliderBox> { static constexpr EDyComponentType Value = EDyComponentType::ColliderBox; };
template <> struct TComponentUnbindingType<CDyPhysicsColliderCapsule> { static constexpr EDyComponentType Value = EDyComponentType::ColliderCapsule; };
template <> struct TComponentUnbindingType<CDySkybox> { static constexpr EDyComponentType Value = EDyComponentType::Skybox; };

} /// ::dy namespace

#endif /// GUARD_DY_Core_Resource_Type_EDyComponentType_H