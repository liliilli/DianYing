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
#include <Dy/Management/PhysicsManager.h>

#include <queue>
#include <PxPhysicsAPI.h>
#include <PxFoundation.h>

#include <Dy/Element/Actor.h>
#include <Dy/Helper/Pointer.h>
#include <Dy/Helper/System/Idioms.h>
#include <Dy/Management/SettingManager.h>
#include <Dy/Component/CDyPhysicsRigidbody.h>
#include <Dy/Component/CDyPhysicsCollider.h>
#include <Dy/Management/Rendering/RenderingManager.h>
#include <Dy/Management/Type/Physics/DDyCollisionIssueItem.h>
#include <Dy/Management/Helper/PhysXErrorCallback.h>

//!
//! Test
//!

namespace
{

static const auto dyFlagTrigger = physx::PxPairFlag::eTRIGGER_DEFAULT;
static const auto dyFlagHit     = physx::PxPairFlag::eNOTIFY_TOUCH_FOUND | physx::PxPairFlag::eCONTACT_DEFAULT;

[[nodiscard]] std::pair<TU08, TU08> 
GetFilterResult(_MIN_ const physx::PxFilterData& data0, _MIN_ const physx::PxFilterData& data1)
{
  const auto lhsId = data0.word0 & 0x000000FF;
  const auto rhsId = data1.word0 & 0x000000FF;
  const auto lhsShift = lhsId % 16;
  const auto rhsShift = rhsId % 16;

  TU08 lhsFlag = 0;
  if (lhsId < 16)       { lhsFlag = (data0.word1 & (0b11 << (rhsShift * 2))) >> rhsShift * 2; }
  else if (lhsId < 32)  { lhsFlag = (data0.word2 & (0b11 << (rhsShift * 2))) >> rhsShift * 2; }
  else if (lhsId < 48)  { lhsFlag = (data0.word3 & (0b11 << (rhsShift * 2))) >> rhsShift * 2; }

  TU08 rhsFlag = 0;
  if (rhsId < 16)       { rhsFlag = (data1.word1 & (0b11 << (lhsShift * 2))) >> lhsShift * 2; }
  else if (rhsId < 32)  { rhsFlag = (data1.word2 & (0b11 << (lhsShift * 2))) >> lhsShift * 2; }
  else if (rhsId < 48)  { rhsFlag = (data1.word3 & (0b11 << (lhsShift * 2))) >> lhsShift * 2; }

  return {lhsFlag, rhsFlag};
}

/// @function DyFilterShader
/// @brief PhysX Filter shader customized setting function.
physx::PxFilterFlags DyFilterShader(
  _MIN_ physx::PxFilterObjectAttributes attributes0, _MIN_ physx::PxFilterData filterData0,
  _MIN_ physx::PxFilterObjectAttributes attributes1, _MIN_ physx::PxFilterData filterData1,
  _MIN_ physx::PxPairFlags& pairFlags, _MIN_ const void* constantBlock, _MIN_ physx::PxU32 constantBlockSize)
{
  // If first 24bit (rigidbody specifier id) is same, just neglect a pair of collider.
  if ((filterData0.word0 & 0xFFFFFF00) == (filterData1.word0 & 0xFFFFFF00))
  {
    return physx::PxFilterFlag::eKILL;
  }

  // Get information from filterData0, filterData1.
  auto [lhsFlag, rhsFlag] = GetFilterResult(filterData0, filterData1);

  // 00 (collision) 01 (overlap) 10 (ignore)
  // -- 00 01 10
  // 00 Co Ov Ig
  // 01 Ov Ov Ig
  // 10 Ig Ig Ig

	// generate contacts for all that were not filtered above
	pairFlags = physx::PxPairFlag::eCONTACT_DEFAULT;

  if (lhsFlag == rhsFlag)
  {
    if (lhsFlag == 0b00)      { pairFlags = dyFlagHit; }
    else if (lhsFlag == 0b01) { pairFlags = dyFlagTrigger; }
    else                      { return physx::PxFilterFlag::eKILL; }
  }
  else
  {
    if (lhsFlag < rhsFlag) { const auto temp = rhsFlag; rhsFlag = lhsFlag; lhsFlag = temp; }

    if (lhsFlag == 0b01)  { pairFlags = dyFlagTrigger; }
    else                  { return physx::PxFilterFlag::eKILL; }
  }

	return physx::PxFilterFlag::eDEFAULT;
}

} /// ::anonymous namespace

//!
//! Implementation
//!

namespace dy
{

class MDyPhysics::Impl final : 
    public IDyUpdatable, 
    public physx::PxDeletionListener, 
    public physx::PxSimulationEventCallback
{
public:
  Impl();
  virtual ~Impl();

  /// @brief Update render object transform.
  void UpdateRenderObjectTransform(TF32 iDt);
  /// @brief Update internal PxScene parameter (PxVisualizationParameter)
  void UpdateInternalPxSceneParameter();
  /// @brief Call callback function when not empty and satisfies given condition.
  /// After calling all function, callback list will be cleared.
  void CallCallbackIssueOnce();
  /// @brief Update physical simulation
  void Update(TF32 iDt) override final;

  /// @brief Init PhysX All resource related to scene.
  void InitScene();
  /// @brief Release PhysX All Resource related to scene.
  void ReleaseScene();

  /// @brief Get default setting instance.
  const DDySettingPhysics& GetDefaultSetting() const noexcept
  {
    return MDySetting::GetInstance().GetPhysicsSetting();
  }

  /// @brief Get default physics material instance reference.
  const physx::PxMaterial& GetDefaultPhysicsMaterial() const noexcept
  {
    return *this->mDefaultMaterial;
  }

  auto& MDY_PRIVATE(GetRefInternalSdk)()
  {
    MDY_ASSERT_FORCE(MDY_CHECK_ISNOTNULL(this->gPhysicx), "Internal PhysX PxPhysics must be initailized.");
    return *this->gPhysicx;
  }

  auto& MDY_PRIVATE(GetRefScene)()
  {
    MDY_ASSERT_FORCE(MDY_CHECK_ISNOTNULL(this->gScene), "Internal PhysX PxScene must be initailized.");
    return *this->gScene;
  }

  /// @brief Try enqueue debug line.
  void TryEnqueueDebugDrawCall();
  
  /// @brief Register activated rigidbody instance.
  void MDY_PRIVATE(RegisterRigidbody)(_MIN_ CDyPhysicsRigidbody& iRefRigidbody)
  {
    this->mActivatedRigidbodyList.emplace_back(DyMakeNotNull(&iRefRigidbody));

    auto ptrRigidbodyComponent = this->mActivatedRigidbodyList.back();

    MDY_ASSERT_FORCE(this->gScene != nullptr, "Physics scene must be valid.");
    this->gScene->addActor(ptrRigidbodyComponent->__GetRefInternalRigidbody());
  }

  /// @brief Unregister deactivated rigidbody instance.
  void MDY_PRIVATE(UnregisterRigidbody)(_MIN_ CDyPhysicsRigidbody& iRefRigidbody)
  {
    const auto it = std::find_if(
      MDY_BIND_BEGIN_END(this->mActivatedRigidbodyList),
      [ptrSource = &iRefRigidbody](const auto& ptrTarget) { return ptrTarget.Get() == ptrSource; }
    );

    MDY_ASSERT_FORCE(it != this->mActivatedRigidbodyList.end(), "Unexpected error occurred.");

    this->gScene->removeActor(it->Get()->__GetRefInternalRigidbody());
    DyFastErase(this->mActivatedRigidbodyList, it);
  }

private:
  /// @brief Override function callback. When object of physx is remove, this function will be called.
	void onRelease(
      MDY_NOTUSED const physx::PxBase* observed, 
      MDY_NOTUSED void* userData, 
      MDY_NOTUSED physx::PxDeletionEventFlag::Enum deletionEvent) override final;

  /// @brief Get PhyX error callback function instance.
  static physx::PxErrorCallback& MDY_PRIVATE(GetPhysXErrorCallback)()
  {
    static physx::PxDefaultErrorCallback gDefaultErrorCallback;
    return gDefaultErrorCallback;
  }

  /// @brief
  MDY_NODISCARD FORCEINLINE physx::PxSimulationFilterShader 
  MDY_PRIVATE(GetSampleFilterShader)()
  {
    return physx::PxDefaultSimulationFilterShader;
  }

  FDyPhysXErrorCallback         gCallback;

  physx::PxPvd*                 gPvd        = MDY_INITIALIZE_NULL;
  physx::PxFoundation*          gFoundation = MDY_INITIALIZE_NULL;
  physx::PxPhysics*             gPhysicx    = MDY_INITIALIZE_NULL; 
  physx::PxDefaultCpuDispatcher*gDispatcher = MDY_INITIALIZE_NULL;
  physx::PxScene*               gScene      = MDY_INITIALIZE_NULL;
  physx::PxMaterial*            mDefaultMaterial   = MDY_INITIALIZE_NULL;
  physx::PxCooking*             mCooking    = MDY_INITIALIZE_NULL;

  physx::PxReal                 stackZ      = 10.0f;
  physx::PxDefaultAllocator     defaultAllocatorCallback;

  /// @brief Activated ptr-rigidbody list.
  std::vector<NotNull<CDyPhysicsRigidbody*>> mActivatedRigidbodyList {};
  
  /// @brief Call this function instead of `pEnqueueCollisionIssue`.
  /// Enqueue collision issue item to `mCollisionCallbackIssueQueue`.
  /// In enqueueing collision issue item, does not check there is any callback item given type,
  /// and not-nullity of given collider & actor variable.
  void pTryEnqueueCollisionIssue(
      EDyCollisionCbType iHitType,
      physx::PxPairFlags iInternalFlag, 
      CDyPhysicsCollider* i0, 
      CDyPhysicsCollider* i1, 
      const FDyHitResult& iHitResult);
 
  /// @brief Collision callback issue queue. This list must be cleared before other-script update phase.
  std::queue<DDyCollisionIssueItem> mCollisionCallbackIssueQueue{};

  // Implements PxSimulationEventCallback

	void onContact(const physx::PxContactPairHeader& pairHeader, 
                 const physx::PxContactPair* pairs, 
                 physx::PxU32 nbPairs) override final;
 
	void onConstraintBreak(physx::PxConstraintInfo*, physx::PxU32) override final {}
	void onWake(physx::PxActor** , physx::PxU32) override final {}
	void onSleep(physx::PxActor** , physx::PxU32) override final {}
	void onAdvance(const physx::PxRigidBody*const*, const physx::PxTransform*, const physx::PxU32) override final {}
  
  // Garbage Bin
	void onTrigger(MDY_NOTUSED physx::PxTriggerPair* pairs, MDY_NOTUSED physx::PxU32 count) override final {  /* We do not use onTrigger on Dy. */ }
};

} /// ::dy namespace
#include <Dy/Management/Inline/MPhysicsImpl.inl>

namespace dy
{

EDySuccess MDyPhysics::pfInitialize()
{
  this->mPimpl = new (std::nothrow) Impl();
  return DY_SUCCESS;
}

EDySuccess MDyPhysics::pfRelease()
{
  delete this->mPimpl;
  return DY_SUCCESS;
}

void MDyPhysics::Update(TF32 dt) { this->mPimpl->Update(dt); }

void MDyPhysics::UpdateRenderObjectTransform(TF32 dt) { this->mPimpl->UpdateRenderObjectTransform(dt); }

void MDyPhysics::CallCallbackIssueOnce() { this->mPimpl->CallCallbackIssueOnce(); }

void MDyPhysics::InitScene() { this->mPimpl->InitScene(); }

void MDyPhysics::ReleaseScene() { this->mPimpl->ReleaseScene(); }

const DDySettingPhysics& MDyPhysics::GetDefaultSetting() const noexcept
{
  return this->mPimpl->GetDefaultSetting();
}

const physx::PxMaterial& MDyPhysics::GetDefaultPhysicsMaterial() const noexcept
{
  return this->mPimpl->GetDefaultPhysicsMaterial();
}

physx::PxPhysics& MDyPhysics::MDY_PRIVATE(GetRefInternalSdk)() { return this->mPimpl->__GetRefInternalSdk(); }

physx::PxScene& MDyPhysics::MDY_PRIVATE(GetRefScene)() { return this->mPimpl->__GetRefScene(); }

void MDyPhysics::UpdateInternalPxSceneParameter() { this->mPimpl->UpdateInternalPxSceneParameter(); }

void MDyPhysics::TryEnqueueDebugDrawCall() { this->mPimpl->TryEnqueueDebugDrawCall(); }

void MDyPhysics::MDY_PRIVATE(RegisterRigidbody)(_MIN_ CDyPhysicsRigidbody& iRefRigidbody)
{
  this->mPimpl->MDY_PRIVATE(RegisterRigidbody)(iRefRigidbody);
}

void MDyPhysics::MDY_PRIVATE(UnregisterRigidbody)(_MIN_ CDyPhysicsRigidbody& iRefRigidbody)
{
  this->mPimpl->MDY_PRIVATE(UnregisterRigidbody)(iRefRigidbody);
}

} /// ::dy namespace