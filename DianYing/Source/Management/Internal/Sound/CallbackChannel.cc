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
#include <Dy/Management/Internal/Sound/CallbackChannel.h>
#include <Dy/Management/Type/Sound/ADySoundInstance.h>

namespace dy
{
class FDyInstantSound2D;
} /// ::dy namesapce

namespace dy
{

FMOD_RESULT F_CALLBACK __CallbackSoundChannel(
    FMOD_CHANNELCONTROL* chanControl, 
    FMOD_CHANNELCONTROL_TYPE controlType,
    FMOD_CHANNELCONTROL_CALLBACK_TYPE callbackType, void* commandData1, void* commandData2)
{
  if (controlType == FMOD_CHANNELCONTROL_CHANNEL
  &&  callbackType == FMOD_CHANNELCONTROL_CALLBACK_END)
  {
    auto* channel = reinterpret_cast<FMOD::Channel*>(chanControl);

    ADySoundInstance* ptrData = nullptr;
    // Get user data from channel, and cast to wrapping type to manipulate property.
    {
      void* ptrUserData = nullptr;
      channel->getUserData(&ptrUserData);
      if (ptrUserData != nullptr) { ptrData = reinterpret_cast<ADySoundInstance*>(ptrUserData); }
    }
    // Change status if sound is end.
    if (ptrData != nullptr) { ptrData->__SetStatus(EDySoundStatus::Stop); }
  }

  return FMOD_OK;
}

} /// ::dy namespace