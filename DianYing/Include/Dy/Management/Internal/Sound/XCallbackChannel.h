#ifndef GUARD_DY_MANAGEMENT_INTERNAL_SOUND_CallbackChannel_H
#define GUARD_DY_MANAGEMENT_INTERNAL_SOUND_CallbackChannel_H
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

#include <fmod.hpp>

namespace dy
{

FMOD_RESULT F_CALLBACK __CallbackSoundChannel(FMOD_CHANNELCONTROL* chanControl, 
                                  FMOD_CHANNELCONTROL_TYPE controlType, 
                                  FMOD_CHANNELCONTROL_CALLBACK_TYPE callbackType, 
                                  void* commandData1, 
                                  void* commandData2);

} /// ::dy namespace

#endif /// GUARD_DY_MANAGEMENT_INTERNAL_SOUND_CallbackChannel_H