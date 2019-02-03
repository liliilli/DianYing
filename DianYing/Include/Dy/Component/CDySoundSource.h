#ifndef GUARD_DY_COMPONENT_CDyAudioSource_H
#define GUARD_DY_COMPONENT_CDyAudioSource_H
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

#include <Dy/Element/Abstract/ADyGeneralBaseComponent.h>
#include <Dy/Component/Interface/IDyInitializeHelper.h>
#include <Dy/Meta/Information/PrefabMetaInformation.h>

#ifdef PlaySound
#undef PlaySound
#endif

//!
//! Forward declaration
//!

namespace dy
{
class FDySoundInstance;
} /// ::dy namespace

//!
//! Implementation
//!

namespace dy
{

/// @class CDySoundSource
/// @brief Sound source component. This can control sound more detail.
class CDySoundSource final : public ADyGeneralBaseComponent, public IDyInitializeHelper<PDySoundSourceComponentMetaInfo>
{
public:
  CDySoundSource(_MIN_ FDyActor& actorReference) : ADyGeneralBaseComponent(actorReference) {};
  virtual ~CDySoundSource() = default;
  
  MDY_ONLY_MOVEABLE_PROPERTIES_DEFAULT(CDySoundSource);
  MDY_SET_TYPEMATCH_FUNCTION(::dy::ADyGeneralBaseComponent, CDySoundSource);
  MDY_SET_CRC32_HASH_WITH_TYPE(CDySoundSource);

  /// @brief  Initialize with descriptor.
  /// @return If successful, return DY_SUCCESS or DY_FAILURE.
  MDY_NODISCARD EDySuccess Initialize(_MIN_ const PDySoundSourceComponentMetaInfo& descriptor) override final;
  /// @brief Release component.
  void Release() override final;

  /// @brief Play sound. If sound is not specified, do nothing but output log.
  void PlaySound();
  /// @brief Pause sound. If sound is not specified, already paused or stopped, do nothing.
  void PauseSound();
  /// @brief Stop sound. If sound is not specified, or stopped, do nothing.
  void StopSound();

  /// @brief Update component.
  void Update(_MIN_ TF32 dt) override;

  /// @brief Get information of component.
  std::string ToString() override final;

private:
  void TryActivateInstance() override final;
  void TryDeactivateInstance() override final;

  FDySoundInstance* mPtrInternalSoundInstance = nullptr;
};

} /// ::dy namespace

#endif /// GUARD_DY_COMPONENT_CDyAudioSource_H