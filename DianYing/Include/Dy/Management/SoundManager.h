#ifndef GUARD_DY_MANAGEMENT_SOUND_MANAGER_H
#define GUARD_DY_MANAGEMENT_SOUND_MANAGER_H
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

#include <Dy/Management/Interface/ISingletonCrtp.h>
#include <fmod.hpp>
#include <Dy/Element/Interface/IDyUpdatable.h>

namespace dy
{

///
/// @class MDySound
/// @brief Sound management class
///
class MDySound final : public IDySingleton<MDySound>, public IDyUpdatable
{
  MDY_SINGLETON_PROPERTIES(MDySound);
  MDY_SINGLETON_DERIVED(MDySound);
public:
  void Update(float dt) override final;

  ///
  /// @brief Check whether sound system is enabled or not.
  ///
  [[nodiscard]] FORCEINLINE bool IsEnabledSoundSystem() const noexcept
  {
    return this->sIssEnabledSoundSystem;
  }

  ///
  /// @brief Play sound element
  /// @TODO THIS FUNCTION IS TEMPORARY FUNCTION, SO WHEN IN PRODUCTION CODE HAVE TO PAUSE INDIVIDUAL 'SOUND CUE' INSTNACE AS PARAMETER.
  ///
  EDySuccess PlaySoundElement(const std::string& soundName) const noexcept;

  ///
  /// @brief Pause sound element
  /// @TODO THIS FUNCTION IS TEMPORARY FUNCTION, SO WHEN IN PRODUCTION CODE HAVE TO PAUSE INDIVIDUAL 'SOUND CUE' INSTNACE AS PARAMETER.
  ///
  EDySuccess PauseSoundElement(const std::string& soundName) const noexcept;

  ///
  /// @brief Stop sound element
  /// @TODO THIS FUNCTION IS TEMPORARY FUNCTION, SO WHEN IN PRODUCTION CODE HAVE TO PAUSE INDIVIDUAL 'SOUND CUE' INSTNACE AS PARAMETER.
  ///
  EDySuccess StopSoundElement(const std::string& soundName) const noexcept;

private:
  ///
  /// @brief
  ///
  [[nodiscard]]
  EDySuccess pfCreateSoundResource(const std::string& filePath, FMOD::Sound** soundResourcePtr);

  FMOD::System*       mSoundSystem            = nullptr;
  MDY_TRANSIENT TU32  mVersion                = MDY_INITIALIZE_DEFUINT;
  MDY_TRANSIENT TI32  mSoundDriverCount       = MDY_INITIALIZE_DEFINT;

  //TSoundChannelGroup s_channel_group;
  FMOD::ChannelGroup* sMasterChannel          = nullptr;
  FMOD::ChannelGroup* sEffectChannel          = nullptr;
  FMOD::ChannelGroup* sBackgroundChannel      = nullptr;

  bool                sIssEnabledSoundSystem  = false;

  friend class CDySoundResource_Deprecated;
};

} /// ::dy namespace

#endif /// GUARD_DY_MANAGEMENT_SOUND_MANAGER_H