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

#include <Dy/Component/Interface/IDyInitializeHelper.h>
#include <Dy/Core/Resource/Type/TDyInformationBinder.h>
#include <Dy/Element/Abstract/ADyGeneralBaseComponent.h>
#include <Dy/Meta/Information/ComponentMetaInformation.h>

//!
//! Forward declaration
//!

namespace dy
{
class FDyModelAnimScrapInformation;
} /// ::dy namespace

//!
//! Implementation
//!

namespace dy
{

/// @class CDyModelAnimator
/// @brief
class CDyModelAnimator final : public ADyGeneralBaseComponent, public IDyInitializeHelper<PDyModelAnimatorComponentMetaInfo>
{
public:
  CDyModelAnimator(FDyActor& actorReference);
  virtual ~CDyModelAnimator() = default;
  
  MDY_ONLY_MOVEABLE_PROPERTIES_DEFAULT(CDyModelAnimator);
  MDY_SET_TYPEMATCH_FUNCTION(::dy::ADyGeneralBaseComponent, CDyModelAnimator);
  MDY_SET_CRC32_HASH_WITH_TYPE(CDyModelAnimator);

  /// @brief  Initialize with descriptor.
  /// @param  descriptor
  /// @return If successful, return DY_SUCCESS or DY_FAILURE.
  MDY_NODISCARD EDySuccess Initialize(const PDyModelAnimatorComponentMetaInfo& descriptor) override final;

  /// @brief Release component.
  void Release() override final;

  /// @brief
  void Update(_MIN_ float dt) override;

  /// @brief
  std::string ToString() override final;

private:
  /// @brief
  void TryActivateInstance() override final;

  /// @brief
  void TryDeactivateInstance() override final;

  /// @enum EDyAnimatorStatus
  /// @brief Blending is not implemented yet.
  enum class EDyAnimatorStatus
  {
    Play,   // Animator is playing something single animation scrap.
    Stop,   // Animator is stopped, so any animator is not played, so last pose is fixed.
    Pause   // Animator is paused so any animator is not played yet but can be resumed.
  };

  /// @enum EDyAnimationScrapMode
  /// @brief 
  enum class EDyAnimationScrapMode
  {
    PlayOnce, // Played animation will be played once, If completed, animator status will be changed to Stop.
    Loop,     // Played animation will be played againly, when completed with blending first pose.
  };

  /// @struct DDyAnimatorStatus
  /// @brief
  struct DDyAnimatorStatus final
  {
    EDyAnimatorStatus     mStatus = EDyAnimatorStatus::Stop;
    EDyAnimationScrapMode mScrapMode = EDyAnimationScrapMode::PlayOnce;
    TF32                  mElapsedTime = 0.0f;
    FDyModelAnimScrapInformation* mPtrPresentAnimatorInfo = nullptr; 
  };

  /// @brief Status instance.
  DDyAnimatorStatus mStatus;
  /// @brief Valid model animation scrap reference handle.
  TDyLInformationBinderAnimScrap  mBinderAnimationScrap;
  /// @brief Valid skeleton reference handle.
  TDyLInformationBinderSkeleton   mBinderSkeleton;
};

} /// ::dy namespace

#endif /// GUARD_DY_COMPONENT_CDyModelAnimator_H