#ifndef GUARD_DY_COMPONENT_CDyModelAnimator_H
#define GUARD_DY_COMPONENT_CDyModelAnimator_H
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

#include <Dy/Component/Interface/IInitializeHelper.h>
#include <Dy/Core/Resource/Type/TInformationBinder.h>
#include <Dy/Element/Abstract/AGeneralBaseComponent.h>
#include <Dy/Meta/Information/ComponentMetaInformation.h>
#include <Dy/Helper/Type/DMatrix4x4.h>

//!
//! Forward declaration
//!

namespace dy
{
class FInformationModelAnimScrap;
} /// ::dy namespace

//!
//! Implementation
//!

namespace dy
{

/// @class CModelAnimator
/// @brief Model Animator component.
class CModelAnimator final : public AGeneralBaseComponent, public IInitializeHelper<PDyModelAnimatorComponentMetaInfo>
{
public:
  CModelAnimator(FActor& actorReference);
  virtual ~CModelAnimator() = default;
  
  MDY_ONLY_MOVEABLE_PROPERTIES_DEFAULT(CModelAnimator);
  MDY_SET_TYPEMATCH_FUNCTION(::dy::AGeneralBaseComponent, CModelAnimator);
  MDY_SET_CRC32_HASH_WITH_TYPE(CModelAnimator);

  /// @brief  Initialize with descriptor.
  /// @return If successful, return DY_SUCCESS or DY_FAILURE.
  MDY_NODISCARD EDySuccess Initialize(const PDyModelAnimatorComponentMetaInfo& descriptor) override final;

  /// @brief Release component.
  void Release() override final;

  /// @brief
  void Update(TF32 dt) override;

  /// @brief
  std::string ToString() override final;

  /// @brief Get final transform list.
  MDY_NODISCARD const std::vector<DMat4>& GetFinalTransformList() const noexcept;

private:
  /// @brief
  void TryActivateInstance() override final;
  /// @brief
  void TryDeactivateInstance() override final;

  /// @brief Update final transform of given animation. This function must be called when `Play`.
  void TryUpdateFinalTransform(TI32 iIdSkelNode, const DMat4& iParentTransform, bool iIsLooped);

  /// @enum EAnimatorStatus
  /// @brief Blending is not implemented yet.
  enum class EAnimatorStatus
  {
    Play,   // Animator is playing something single animation scrap.
    Stop,   // Animator is stopped, so any animator is not played, so last pose is fixed.
    Pause,  // Animator is paused so any animator is not played yet but can be resumed.
    Init,   // Animator is initiated. (Default-value)
  };

  /// @enum EAnimationScrapMode
  /// @brief 
  enum class EAnimationScrapMode
  {
    PlayOnce, // Played animation will be played once, If completed, animator status will be changed to Stop.
    Loop,     // Played animation will be played againly, when completed with blending first pose.
  };

  /// @struct DAnimatorStatus
  /// @brief
  struct DAnimatorStatus final
  {
    EAnimatorStatus     mStatus = EAnimatorStatus::Init;
    EAnimationScrapMode mScrapMode = EAnimationScrapMode::PlayOnce;
    TF32                mElapsedTime = 0.0f;
    const FInformationModelAnimScrap* mPtrPresentAnimatorInfo = nullptr; 
    
    /// @brief
    std::vector<DMat4> mFinalTransformList;
  };

  /// @brief Status instance.
  DAnimatorStatus mStatus;
  /// @brief Valid model animation scrap reference handle.
  TDyInformationBinderAnimScrap mBinderAnimationScrap;
  /// @brief Valid skeleton reference handle.
  TDyInformationBinderSkeleton  mBinderSkeleton;
};

} /// ::dy namespace

#endif /// GUARD_DY_COMPONENT_CDyModelAnimator_H