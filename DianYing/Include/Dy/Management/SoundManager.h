#ifndef GUARD_DY_MANAGEMENT_SOUND_MANAGER_H
#define GUARD_DY_MANAGEMENT_SOUND_MANAGER_H
///
/// MIT License
/// Copyright (c) 2018 Jongmin Yun
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
class MDySound final : public ISingleton<MDySound>, public IDyUpdatable
{
  MDY_SINGLETON_PROPERTIES(MDySound);
  MDY_SINGLETON_DERIVED(MDySound);
public:
  void Update(float dt) override final;

private:
  FMOD::System*       mSoundSystem            = nullptr;
  MDY_TRANSIENT TU32  mVersion                = MDY_NOT_INITIALIZED_0;
  MDY_TRANSIENT TI32  mSoundDriverCount       = MDY_NOT_INITIALIZED_M1;

  //TSoundChannelGroup s_channel_group;
  FMOD::ChannelGroup* sMasterChannel          = nullptr;
  FMOD::ChannelGroup* sEffectChannel          = nullptr;
  FMOD::ChannelGroup* sBackgroundChannel      = nullptr;

  bool                sIssEnabledSoundSystem  = false;
};

} /// ::dy namespace

#endif /// GUARD_DY_MANAGEMENT_SOUND_MANAGER_H